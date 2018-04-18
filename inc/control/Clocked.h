/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Control
 * @{
 ******************************************************************************
 */

#ifndef CONTROL_CLOCKED_H_
#define CONTROL_CLOCKED_H_

class Clocked {
public:
    virtual ~Clocked() = default;

    virtual void tick() = 0;
};

#endif /* CONTROL_CLOCKED_H_ */
/** @} */
