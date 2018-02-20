/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */


#ifndef HAL_ACTOR_H_
#define HAL_ACTOR_H_

class Actor {
public:
    virtual ~Actor() = default;

    /**
     * Enable the actor, allowing movement / output
     */
    virtual void enable() = 0;

    /**
     * Stop the actor and disable it, preventing movement/output until it is enabled again
     */
    virtual void disableAndStop() = 0;

};

#endif /* HAL_ACTOR_H_ */
/** @} */
