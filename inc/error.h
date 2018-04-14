/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 ******************************************************************************
 */


#ifndef ERROR_H_
#define ERROR_H_

#include "config.h"

#ifdef HUMAN_MODE
#ifdef CONTINUE_ON_ERROR
#define STOP_PROGRAM()
#else
#define STOP_PROGRAM() \
    printf("\nStopping program...please press RESET\r\n"); \
    while(1) {}
#endif
/**
 * Report an error and stop program
 */
#define ERROR(...) \
{\
    printf("\r\n==================================================\r\n"); \
    printf("            A FATAL ERROR HAS OCCURRED!\r\n"); \
    printf("==================================================\r\n"); \
    auto file = __FILE__; \
    printf("%s:%d (%s)\r\n", (file + 7), __LINE__, __FUNCTION__); \
    printf(__VA_ARGS__); \
    printf("\r\n"); \
    STOP_PROGRAM()\
}

#define TRY(x) \
    if(int result = (x)) { \
        ERROR("%s failed with result %d", #x, result);\
    }

#else
#define ERROR(...)
#define TRY(x) x
#endif





#endif /* ERROR_H_ */
