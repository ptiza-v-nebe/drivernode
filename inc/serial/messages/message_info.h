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

constexpr int MAX_PAYLOAD = 0xF;
constexpr int MAX_TYPE = 0xF;

static constexpr uint8_t MSG_TYPE_MASK = 0xF0;
static constexpr int MSG_TYPE_OFFSET = 4;

constexpr uint8_t PAYLOAD_SIZE_MASK = 0x0F;

#endif /* SERIAL_MESSAGES_MESSAGE_INFO_H_ */
/** @} */
