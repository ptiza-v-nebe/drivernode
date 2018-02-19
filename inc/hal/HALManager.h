/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#ifndef HAL_HALMANAGER_H_
#define HAL_HALMANAGER_H_

class Encoder;

class HALManager {
public:
    virtual ~HALManager() = default;

    virtual Encoder& getLeftEncoder() = 0;
    virtual Encoder& getRightEncoder() = 0;
};

#endif /* HAL_HALMANAGER_H_ */
/** @} */
