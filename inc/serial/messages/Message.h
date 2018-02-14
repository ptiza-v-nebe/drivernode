/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup SerialMessages
 * @{
 ******************************************************************************
 */

#ifndef SERIAL_MESSAGES_MESSAGE_H_
#define SERIAL_MESSAGES_MESSAGE_H_

#include <cstdint>

class Message {
public:
	virtual ~Message() = default;

	virtual int serialize(uint8_t* buffer, int size) = 0;
	virtual void print() = 0;
};

#endif /* SERIAL_MESSAGES_MESSAGE_H_ */
/** @} */
