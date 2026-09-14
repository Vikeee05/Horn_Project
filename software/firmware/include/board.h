#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>

void board_init(void);
void board_led_init(void);
void board_led_on(void);
void board_led_off(void);
void board_led_toggle(void);
void board_delay_ms(uint32_t milliseconds);

#endif
