/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */


#ifndef HAL_INTERUPTS_H_
#define HAL_INTERUPTS_H_

constexpr int UART_PREMPTION_PRIO = 1; ///< Preemption priority for the UART interrupt (lower is higher)
constexpr int UART_SUB_PRIO = 3; ///< Subpriority for the UART interrupt (lower is higher)

constexpr int ENCODERS_PREEMPTION_PRIORITY = 1; ///< Preemption priority for the Encoder interrupt(s) (lower is higher)
constexpr int ENCODERS_SUB_PRIORITY = 2; ///< Subpriority for the Encoder interrupt(s) (lower is higher)

constexpr int GAME_TIMER_PREEMPTION_PRIORITY = 0; ///< Preemption priority for the game timer interrupt (lower is higher)
constexpr int GAME_TIMER_SUB_PRIORITY = 2; ///< Subpriority for the game timer interrupt (lower is higher)


#endif /* HAL_INTERUPTS_H_ */
/** @} */
