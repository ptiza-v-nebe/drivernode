/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Serial
 * @{
 ******************************************************************************
 */

#ifndef SERIAL_MESSAGESENDER_H_
#define SERIAL_MESSAGESENDER_H_

#include <cstdint>

class MessageSender {
public:
    virtual ~MessageSender() = default;
    virtual void send(const uint8_t* buffer, const int size) = 0;
};

#endif /* SERIAL_MESSAGESENDER_H_ */
/** @} */
