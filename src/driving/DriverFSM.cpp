/*
 * DriverFSM.cpp
 *
 *  Created on: Apr 10, 2018
 *      Author: max
 */

#include <cmath>
#include "driving/DriverFSM.h"
#include "driving/DriverBaseState.h"
#include "driving/DriverStates.h"
#include "constants.h"
#include "hal/Motor.h"
#include "position/PositionManager.h"
#include "position/calibration_parameters.h"
#include "util/util.h"
#include "serial/messages/StatusMessage.h"

static constexpr float MINIMUM_DELTA_SPEED_FOR_STUCK = 0.3f;

DriverFSM::DriverFSM(Motor& motorLeft, Motor& motorRight, PositionManager& pm,
        MessageDispatcher& md, bool& backwardVision) :
        currentState(nullptr), leftMotor(motorLeft), rightMotor(motorRight), leftWheelControl(
                CONTROLLER_SYSTEM_LEFT_WHEEL_KP,
                CONTROLLER_SYSTEM_LEFT_WHEEL_DELAYTIME,
                CONTROLLER_SAMPLING_TIME), rightWheelControl(
                CONTROLLER_SYSTEM_RIGHT_WHEEL_KP,
                CONTROLLER_SYSTEM_RIGHT_WHEEL_DELAYTIME,
                CONTROLLER_SAMPLING_TIME), pm(pm), md(md), referencePosition(
                pm.getPosition()), currentDistance(0.0f), rampDistance(0.0f), distanceError(
                0.0f), referenceAngle(pm.getHeading()), angleError(0.0), startAngle(
                0.0), rampAngle(0.0), leftMotorVelocity(0.0f), rightMotorVelocity(
                0.0f), driveSpeed(DriveSpeed::SLOW), driveDirection(
                DriveDirection::FORWARD), driveAccuracy(DriveAccuracy::LOW), referenceSpeedLeft(
                0.0f), referenceSpeedRight(0.0f), speed(0.2), direction(1), targetRadius(
                0.25), turningAngle(false), backwardVision(backwardVision) {
    currentState = new Idle(*this);
}

void DriverFSM::tick() {
    currentState->doAction();
}

void DriverFSM::updateControl() {
    float angleGain = 1.2;

    // recalculate referenceAngl
    if (!turningAngle) {
        Vector targetVector(pm.xum * 0.001, pm.yum * 0.001);
        referenceAngle =
                (referencePosition.asVectorFromOrigin() - targetVector).getPolarAngle();
    }

    if (direction < 0) { // if driving backward
        referenceAngle = referenceAngle - 180_deg;
    }

    angleError = referenceAngle - pm.getHeading();

    if (fabs(angleError.getAngleInDegreesAround(0.0)) > 10.0) {
        angleGain = /*0.28*/0.3;
    } else {
        if (turningAngle) {
            angleGain = /*0.28*/0.3;
            distanceError = 0;
        } else {
            currentDistance = referencePosition.distanceTo(pm.getPosition())
                    * 0.001f;

            if (fabs(rampDistance) > 0) {
                rampDistance = rampDistance - speed * CONTROLLER_SAMPLING_TIME;
                rampDistance = std::max(rampDistance, 0.0f);
            }

            distanceError = -(rampDistance - currentDistance) * direction;
        }
    }

    referenceSpeedLeft = 1.75 * distanceError
            - (TRACK_WIDTH_UM * 0.000002) * angleGain
                    * angleError.getAngleInRadianAround(0.0);
    referenceSpeedRight = 1.75 * distanceError
            + (TRACK_WIDTH_UM * 0.000002) * angleGain
                    * angleError.getAngleInRadianAround(0.0);

    leftMotorVelocity = leftWheelControl.update(
            referenceSpeedLeft - pm.leftWheelVelocity);
    rightMotorVelocity = rightWheelControl.update(
            referenceSpeedRight - pm.rightWheelVelocity);


	leftMotorVelocity = clamp<float>(leftMotorVelocity, -1.5, 1.5);
	rightMotorVelocity = clamp<float>(rightMotorVelocity, -1.5, 1.5);

    leftMotor.setSpeed(leftMotorVelocity * MOTORCONSTANT);
    rightMotor.setSpeed(rightMotorVelocity * MOTORCONSTANT);

    n++; // counting tick up
    //printf("%f %f %f %f %f\r\n",n*CONTROLLER_SAMPLING_TIME,angleError.getAngleInRadianAround(0.0), rampDistance, currentDistance, pm.getHeading().getAngleInRadianAround(0.0));
    printf("%f %f %f %f\r\n", n * CONTROLLER_SAMPLING_TIME, rampDistance,
            currentDistance, distanceError);
}

void DriverFSM::resetControl() {
    leftWheelControl.reset();
    rightWheelControl.reset();
    referenceSpeedLeft = 0;
    referenceSpeedRight = 0;
    leftMotorVelocity = 0;
    rightMotorVelocity = 0;
}

void DriverFSM::enableMotors() {
    leftMotor.enable();
    rightMotor.enable();
}

void DriverFSM::disableMotors() {
    leftMotor.disableAndStop();
    rightMotor.disableAndStop();
}

void DriverFSM::stopMotors() {
    leftMotor.stop();
    rightMotor.stop();
}

bool DriverFSM::referencePositionReached() {
    float distance = pm.getPosition().distanceTo(referencePosition);

    if (turningAngle) {
        if (fabs(angleError.getAngleInDegreesAround(0.0)) < 1.0) {
            return true;
        } else {
            return false;
        }
    } else {
        if ((distance < targetRadius
                && fabs(angleError.getAngleInDegreesAround(0.0)) < 1.0)) { //||
            //(1 == pointOnWhichSideOfLine(referencePosition.asVectorFromOrigin()-Vector(startAngle,3.5),Vector(startAngle+90_deg,1), pm.getPosition()))) {
            return true;
        } else {
            return false;
        }
    }

}

void DriverFSM::calculateDistance() {
    turningAngle = false;
    rampDistance = referencePosition.distanceTo(pm.getPosition()) * 0.001f
            - distanceError;
    rampDistance = std::max(rampDistance, 0.0f);

    angleError = 0;
    Vector targetVector(pm.xum * 0.001, pm.yum * 0.001);
    referenceAngle =
            (referencePosition.asVectorFromOrigin() - targetVector).getPolarAngle();

    if (direction < 0) { // if driving backward
        referenceAngle = referenceAngle - 180_deg;
    }

    startAngle = referenceAngle - pm.getHeading();
}

void DriverFSM::calculateAngle() {
    turningAngle = true;
    // calculate angleError
    angleError = referenceAngle - pm.getHeading();

    // set distance control to zero
    distanceError = 0;
    rampDistance = 0;
}

bool DriverFSM::isAccuracyHigh() {
    if (driveAccuracy == DriveAccuracy::HIGH) {
        return true;
    }
    return false;
}

bool DriverFSM::isRobotStuck() {
    /*float forwardVelocity = (leftMotorVelocity + rightMotorVelocity) * 0.5;

     bool forwardStuck = fabs(forwardVelocity - pm.getForwardVelocity())
     > std::max(speed + 0.5f * speed, MINIMUM_DELTA_SPEED_FOR_STUCK);

     return forwardStuck;*/

    if (turningAngle || (leftMotorVelocity < 0 && rightMotorVelocity > 0)
            || (leftMotorVelocity > 0 && rightMotorVelocity < 0)) {
        return false;
    } else {

        bool leftStuck = fabs((leftMotorVelocity - pm.getLeftWheelVelocity()))
                > std::max(speed, MINIMUM_DELTA_SPEED_FOR_STUCK);
        bool rightStuck = fabs(
                (rightMotorVelocity - pm.getRightWheelVelocity()))
                > std::max(speed, MINIMUM_DELTA_SPEED_FOR_STUCK);

        return leftStuck && rightStuck;
    }
}

bool DriverFSM::isEnemyBehindRobot() {
    if (driveDirection == DriveDirection::BACKWARD) {
        return backwardVision;
    }
    return false;
}

void DriverFSM::sendFinishedMessage() {
    md.sendMessage(StatusMessage(Status::DRIVER_FINISHED));
}

void DriverFSM::sendStuckMessage() {
    md.sendMessage(StatusMessage(Status::STUCK));
}

DriverFSM::~DriverFSM() {
    delete currentState;
}

void DriverFSM::newPosition() {
    currentState->newPosition();
}

void DriverFSM::newAngle() {
    currentState->newAngle();
}

void DriverFSM::stop() {
    currentState->stop();
}

void DriverFSM::setReferencePosition(Position position) {
    this->referencePosition = position;
}

void DriverFSM::setReferenceAngle(Angle angle) {
    this->referenceAngle = angle;
}

void DriverFSM::setDriveSpeed(DriveSpeed speed) {
    if (speed == DriveSpeed::FAST) {
        this->speed = 0.8; // meters per second
    } else {
        this->speed = 0.2; // meters per second
    }
    this->driveSpeed = speed;
    this->speed = 0.2;
}

void DriverFSM::setDriveDirection(DriveDirection direction) {
    if (direction == DriveDirection::BACKWARD) {
        this->direction = -1;
    } else {
        this->direction = 1;
    }
    this->driveDirection = direction;
}

void DriverFSM::setDriveAccuracy(DriveAccuracy accuracy) {
    if (accuracy == DriveAccuracy::HIGH) {
        targetRadius = 4; //mm
    } else {
        targetRadius = 80; //mm
    }
    this->driveAccuracy = accuracy;
}

int DriverFSM::pointOnWhichSideOfLine(Vector ro, Vector rd, Position p) {
    return signum(rd.y * (p.x - ro.x) - rd.x * (p.y - ro.y));
}
