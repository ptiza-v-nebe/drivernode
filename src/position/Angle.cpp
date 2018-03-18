/** ****************************************************************
 * @file
 * \addtogroup Position
 * @{
 ******************************************************************
 */

#include <cmath>
#include "position/Angle.h"

/**
 * full circle in radian
 */
static constexpr float RADIAN_IN_CIRCLE = 2 * PI;

/**
 * full circle in degrees
 */
static constexpr float DEGREES_IN_CIRCLE = 360;

/**
 * Constructs an Angle with a value of 0.
 */
Angle::Angle() :
        radian(0) {
}

/**
 * Constructs an Angle from radian.
 *
 * @param radian the angle
 */
Angle::Angle(float radian) {
    setRadian(radian);
}

/**
 * Constructs and returns an Angle from a value in degrees.
 *
 * @param degrees the value for constructing an Angle with.
 * @return an Angle object equivalent to the specified angle in degrees.
 */
Angle Angle::getFromDegrees(float degrees) {
    return Angle(degrees / DEGREES_IN_CIRCLE * RADIAN_IN_CIRCLE);
}

/**
 * Inverts the Angle
 *
 * @return the inverted angle
 */
Angle Angle::operator -() const {
    return Angle(-radian);
}

/**
 * Sets the internal value and ensures that it's always between 0 and 2 PI.
 *
 * @param rad the value to set
 */
void Angle::setRadian(float rad) {
    while (rad >= RADIAN_IN_CIRCLE) {
        rad -= RADIAN_IN_CIRCLE;
    }
    while (rad < 0) {
        rad += RADIAN_IN_CIRCLE;
    }
    this->radian = rad;
}

/**
 * Returns the value of the Angle in degrees.
 *
 * @return the value in degrees
 */
float Angle::getAngleInDegrees() const {
    return radian / RADIAN_IN_CIRCLE * DEGREES_IN_CIRCLE;
}
/**
 * Returns the value of the Angle in degrees, shifted to be in a symmetric 360° interval around the specified center angle.
 *
 * @example Center = 180° ---> Angle is between 0° and 360°
 * @example Center = 0°   ---> Angle is between -180° and 180°
 * @example Center = 200° ---> Angle is between 20° and 380°
 *
 * @param degreesCenter the center of the symmetric angle interval
 * @return the angle within the specified interval
 */
float Angle::getAngleInDegreesAround(float degreesCenter) const {
    float degrees = getAngleInDegrees();

    // shift into correct interval (half circle either side)
    while (degrees < (degreesCenter - (DEGREES_IN_CIRCLE / 2))) {
        degrees += DEGREES_IN_CIRCLE;
    }
    while (degrees > (degreesCenter + (DEGREES_IN_CIRCLE / 2))) {
        degrees -= DEGREES_IN_CIRCLE;
    }

    return degrees;
}

/**
 * Returns the value of the Angle in radiant.
 *
 * @return the value in radian
 */
float Angle::getAngleInRadian() const {
    return radian;
}

/**
 * Returns the value of the Angle in radian, shifted to be in a symmetric 2 PI interval around the specified center angle.
 *
 * @example Center = PI ---> Angle is between 0 and 2 PI
 * @example Center = 0   ---> Angle is between -PI and PI
 * @example Center = PI + PI/6 ---> Angle is between PI/6 and 2 PI + PI/6
 *
 * @param radianCenter the center of the symmetric angle interval
 * @return the angle within the specified interval
 */
float Angle::getAngleInRadianAround(float radianCenter) const {
    float radian = getAngleInRadian();

    // shift into correct interval (half circle either side)
    while (radian < (radianCenter - (RADIAN_IN_CIRCLE / 2))) {
        radian += RADIAN_IN_CIRCLE;
    }
    while (radian > (radianCenter + (RADIAN_IN_CIRCLE / 2))) {
        radian -= RADIAN_IN_CIRCLE;
    }

    return radian;
}

// Addition
void Angle::add(const Angle& other) {
    setRadian(radian + other.radian);
}

Angle operator+(const Angle& left, const Angle& right) {
    Angle sum = left;
    sum.add(right);
    return sum;
}

Angle operator+(float left, const Angle& right) {
    return Angle(left) + right;
}

Angle operator+(const Angle& left, float right) {
    return left + Angle(right);
}

// Subtraction
void Angle::subtract(const Angle& other) {
    setRadian(radian - other.radian);
}

Angle operator-(const Angle& left, const Angle& right) {
    Angle sum = left;
    sum.subtract(right);
    return sum;
}
Angle operator-(float left, const Angle& right) {
    return Angle(left) - right;
}

Angle operator-(const Angle& left, float right) {
    return left - Angle(right);
}

Angle Angle::operator=(float radian) {
    setRadian(radian);
    return *this;
}

/**
 * Calculates and returns the sin of the angle.
 *
 * @return the sine
 */
float Angle::sin() const {
    return std::sin(radian);
}

/**
 * Calculates and returns the cosine of the angle.
 *
 * @return the cosine
 */
float Angle::cos() const {
    return std::cos(radian);
}

/**
 * User defined literal for Angle in degrees
 *
 * @param deg the magnitude in degrees
 *
 * @return a corresponding Angle object
 */
Angle operator ""_deg(long double deg) {
    return Angle::getFromDegrees(deg);
}

/**
 * User defined literal for Angle in degrees
 *
 * @param deg the magnitude in degrees
 *
 * @return a corresponding Angle object
 */
Angle operator ""_deg(unsigned long long deg) {
    return Angle::getFromDegrees(deg);
}
/** @} */
