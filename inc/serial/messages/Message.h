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
private:
    const uint8_t type;
public:
    Message(const uint8_t type) : type(type) {}
	virtual ~Message() = default;

	virtual int serialize(uint8_t* buffer, int size) = 0;
	virtual void print() = 0;
	uint8_t getType() { return type; }
};

#endif /* SERIAL_MESSAGES_MESSAGE_H_ */
/** @} */
