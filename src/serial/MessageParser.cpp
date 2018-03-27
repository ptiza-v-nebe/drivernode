/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Serial
 * @{
 ******************************************************************************
 */
#include <serial/MessageParser.h>
#include "serial/Serializer.h"
#include "serial/messages/all.h"

#include "config.h"

/**
 * Parse an ASCII message into the serialized version of a message object.
 *
 * @param msg        the ASCII message
 * @param size       the size of the ASCII message
 * @param buffer     the buffer for the serialized message object
 * @param buffersize the size of the buffer for the serialized message object
 *
 * @return number of bytes used
 * @retval -1 invalid ASCII message
 */
int MessageParser::parseMessage(const uint8_t* msg, const int size,
        uint8_t* buffer, const int buffersize) {

    if (size >= 1) {
        switch (msg[0]) {
            case 'q':
                return parseStopMessage(buffer, buffersize);
            case 'd':
                return parseDriveMessage(msg, size, buffer, buffersize);
            case 't':
                return parseTurnMessage(msg, size, buffer, buffersize);
            case 's':
                return parseSetSpeedMessage(msg, size, buffer, buffersize);
            case 'r':
                return parseResetMessage(msg, size, buffer, buffersize);
            default:
                return -1;
        }
    } else {
        return -1;
    }
}

/**
 * Parse an ASCII stop message into the serialized version of the StopMessage object.
 *
 * @param buffer     the buffer for the serialized message object
 * @param buffersize the size of the buffer for the serialized message object
 *
 * @return number of bytes used
 * @retval -1 invalid ASCII message
 */
int MessageParser::parseStopMessage(uint8_t* buffer, const int buffersize) {
    StopMessage sm;
    print(sm);
    return Serializer::serialize(sm, buffer, buffersize);
}

/**
 * Parse an ASCII message regarding driving into the serialized version of the proper message object.
 *
 * @param msg        the ASCII message
 * @param size       the size of the ASCII message
 * @param buffer     the buffer for the serialized message object
 * @param buffersize the size of the buffer for the serialized message object
 *
 * @return number of bytes used
 * @retval -1 invalid ASCII message
 */
int MessageParser::parseDriveMessage(const uint8_t* msg, const int size,
        uint8_t* buffer, const int buffersize) {
    if (size >= 2) {
        switch (msg[1]) {
            case 'f': //df
            {
                SimpleDriveMessage sdm(DriveSpeed::SLOW,
                        DriveDirection::FORWARD);
                print(sdm);
                return Serializer::serialize(sdm, buffer, buffersize);
            }
            case 'b': //db
            {
                SimpleDriveMessage sdm(DriveSpeed::SLOW,
                        DriveDirection::BACKWARD);
                print(sdm);
                return Serializer::serialize(sdm, buffer, buffersize);
            }
            case 'p': {
                uint16_t x, y;
                if (sscanf(reinterpret_cast<const char*>(msg + 2), "%hu %hu",
                        &x, &y) == 2) {
                    // successfully parsed two unsigned ints
                    ControlledDriveMessage cdm(DriveSpeed::SLOW, {x, y});
                    print(cdm);
                    return Serializer::serialize(cdm, buffer, buffersize);
                } else {
                    return -1;
                }
            }
            default:
                return -1;
        }
    } else {
        return -1;
    }
}

/**
 * Parse an ASCII message regarding turning into the serialized version of the proper message object.
 *
 * @param msg        the ASCII message
 * @param size       the size of the ASCII message
 * @param buffer     the buffer for the serialized message object
 * @param buffersize the size of the buffer for the serialized message object
 *
 * @return number of bytes used
 * @retval -1 invalid ASCII message
 */
int MessageParser::parseTurnMessage(const uint8_t* msg, const int size,
        uint8_t* buffer, const int buffersize) {
    if (size >= 3) {
        switch (msg[1]) {
            case 'c':
                switch (msg[2]) {
                    case 'c': // tcc
                    {
                        SimpleTurnMessage stm(TurnSpeed::SLOW,
                                TurnDirection::CCW);
                        print(stm);
                        return Serializer::serialize(stm, buffer, buffersize);
                    }
                    case 'w': //tcw
                    {
                        SimpleTurnMessage stm(TurnSpeed::SLOW,
                                TurnDirection::CW);
                        print(stm);
                        return Serializer::serialize(stm, buffer, buffersize);
                    }
                    default:
                        return -1;
                }
            case ' ': {
                float degrees;
                if (sscanf(reinterpret_cast<const char*>(msg + 2), "%f",
                        &degrees) == 1) {
                    // successfully parsed float
                    ControlledTurnMessage ctm(TurnSpeed::SLOW,
                            Angle::getFromDegrees(degrees));
                    print(ctm);
                    return Serializer::serialize(ctm, buffer, buffersize);
                } else {
                    return -1;
                }
            }
            default:
                return -1;
        }
    } else {
        return -1;
    }
}

/**
 * Parse an ASCII message regarding speed into the serialized version of the SetSpeedMessage object.
 *
 * @param msg        the ASCII message
 * @param size       the size of the ASCII message
 * @param buffer     the buffer for the serialized message object
 * @param buffersize the size of the buffer for the serialized message object
 *
 * @return number of bytes used
 * @retval -1 invalid ASCII message
 */
int MessageParser::parseSetSpeedMessage(const uint8_t* msg, const int,
        uint8_t* buffer, const int buffersize) {
    int left, right;
    int argCount = sscanf(reinterpret_cast<const char*>(msg + 1), "%d %d",
            &left, &right);
    if (argCount == 1) {
        // successfully parsed one int
        SetSpeedMessage ssm(left);
        print(ssm);
        return Serializer::serialize(ssm, buffer, buffersize);

    } else if (argCount == 2) {
        // successfully parsed two ints
        SetSpeedMessage ssm(left, right);
        print(ssm);
        return Serializer::serialize(ssm, buffer, buffersize);
    } else {
        return -1;
    }
}

/**
 * Parse an ASCII message regarding odometry reset into the serialized version of the ResetOdometryMessage object.
 *
 * @param msg        the ASCII message
 * @param size       the size of the ASCII message
 * @param buffer     the buffer for the serialized message object
 * @param buffersize the size of the buffer for the serialized message object
 *
 * @return number of bytes used
 * @retval -1 invalid ASCII message
 */
int MessageParser::parseResetMessage(const uint8_t* msg, const int,
        uint8_t* buffer, const int buffersize) {
    uint16_t x, y;
    float degrees;
    if (sscanf(reinterpret_cast<const char*>(msg + 2), "%hu %hu %f", &x, &y, &degrees) == 3) {
        // successfully parsed float
        ResetOdometryMessage rom({x, y}, Angle::getFromDegrees(degrees));
        print(rom);
        return Serializer::serialize(rom, buffer, buffersize);
    } else {
        return -1;
    }
}

/**
 * Print the message that was parsed before serializing it.
 *
 * @param message the message to be printed
 */
#ifdef DEBUG_PARSER
void MessageParser::print(const Message& message) {
    printf("Received ");
    message.print();
    printf("\r\n");
#else
void MessageParser::print(const Message&) {
#endif
}
/** @} */