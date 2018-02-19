/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup SerialMessages
 * @{
 ******************************************************************************
 */


#ifndef SERIAL_MESSAGES_MESSAGE_INFO_H_
#define SERIAL_MESSAGES_MESSAGE_INFO_H_

/**
 * Maximum number of payload bytes
 */
constexpr int MAX_PAYLOAD = 0xF;

/**
 * Maximum value for the type
 */
constexpr int MAX_TYPE = 0xF;

/**
 * bitmask of the message type field (in the header)
 */
static constexpr uint8_t MSG_TYPE_MASK = 0xF0;

/**
 * offset of the message type field (in the header)
 */
static constexpr int MSG_TYPE_OFFSET = 4;

/**
 * bitmask of the payload size field (in the header)
 */
constexpr uint8_t PAYLOAD_SIZE_MASK = 0x0F;

/**
 * how many message types are possible
 */
constexpr int MESSAGE_TYPE_COUNT = MAX_TYPE + 1;

#endif /* SERIAL_MESSAGES_MESSAGE_INFO_H_ */
/** @} */
