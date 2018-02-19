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
    const uint8_t type; ///< the message type
public:
    Message(const uint8_t type) : type(type) {}
	virtual ~Message() = default;

	/**
	 * Serialize the message into an array of bytes.
	 *
	 * @param buffer pointer to the memory where the serialized data is to be put
	 * @param size   the size of the buffer
	 *
	 * @return used size
	 * @retval -1 not enough space
	 */
	virtual int serialize(uint8_t* buffer, int size) const = 0;

	/**
	 * Print a human readable representation of the message using {@code printf}
	 */
	virtual void print() const = 0;

	uint8_t getType() const { return type; }
};

#endif /* SERIAL_MESSAGES_MESSAGE_H_ */
/** @} */
