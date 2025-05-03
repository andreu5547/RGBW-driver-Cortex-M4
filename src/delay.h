#ifndef DELAY_H
#define DELAY_H
#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/
#include <stdint.h>

/******************************************************************************
* PUBLIC FUNCTION PROTOTYPES
******************************************************************************/

/**
* @brief Задержка в мкс
*
* @param delay_us - задержка в мкс
* @return void
*/
void delay_us(uint32_t delay_us);

#ifdef __cplusplus
}
#endif
#endif // DELAY_H