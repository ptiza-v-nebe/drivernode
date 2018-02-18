/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup SerialMessages
 * @{
 ******************************************************************************
 */


#ifndef SERIAL_MESSAGES_DRIVE_TYPES_H_
#define SERIAL_MESSAGES_DRIVE_TYPES_H_

#include "util/enum_to_string.h"

/*
 * Enums used in the driving related messages
 */

#define DRIVE_SPEED_VALS(m) \
    m(SLOW) \
    m(FAST)

#define DRIVE_DIRECTION_VALS(m) \
    m(FORWARD) \
    m(BACKWARD)

#define TURN_SPEED_VALS(m) \
    m(SLOW) \
    m(FAST)

#define TURN_DIRECTION_VALS(m) \
    m(CW) \
    m(CCW)

SMARTENUM_DEFINE_ENUM(DriveSpeed, DRIVE_SPEED_VALS)
SMARTENUM_DEFINE_ENUM(DriveDirection, DRIVE_DIRECTION_VALS)
SMARTENUM_DEFINE_ENUM(TurnSpeed, TURN_SPEED_VALS)
SMARTENUM_DEFINE_ENUM(TurnDirection, TURN_DIRECTION_VALS)

#endif /* SERIAL_MESSAGES_DRIVE_TYPES_H_ */
/** @} */
