/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Control
 * @{
 ******************************************************************************
 */

#ifndef CONTROL_CLOCKEDINITIALIZABLE_H_
#define CONTROL_CLOCKEDINITIALIZABLE_H_

class ClockedInitializable {
public:
    virtual ~ClockedInitializable() = default;

    virtual void startInitializing() = 0;
    virtual bool tickInit() = 0;
};

#endif /* CONTROL_CLOCKEDINITIALIZABLE_H_ */
/** @} */
