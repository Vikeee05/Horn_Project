#include <stdint.h>
#include <sys/types.h>
#include "board.h"

#define RCC_BASE        0x40023800UL
#define GPIOA_BASE      0x40020000UL
#define USART2_BASE     0x40004400UL

#define REG32(address)  (*(volatile uint32_t *)(address))

#define RCC_AHB1ENR     REG32(RCC_BASE + 0x30UL)
#define RCC_APB1ENR     REG32(RCC_BASE + 0x40UL)
#define GPIOA_MODER     REG32(GPIOA_BASE + 0x00UL)
#define GPIOA_AFRL      REG32(GPIOA_BASE + 0x20UL)
#define USART2_SR       REG32(USART2_BASE + 0x00UL)
#define USART2_DR       REG32(USART2_BASE + 0x04UL)
#define USART2_BRR      REG32(USART2_BASE + 0x08UL)
#define USART2_CR1      REG32(USART2_BASE + 0x0CUL)

#define USART_SR_TXE    (1UL << 7)
#define USART_CR1_TE    (1UL << 3)
#define USART_CR1_RE    (1UL << 2)
#define USART_CR1_UE    (1UL << 13)

void board_init(void)
{
    /* Enable GPIOA and USART2 while running from the 16 MHz HSI clock. */
    RCC_AHB1ENR |= (1UL << 0);
    RCC_APB1ENR |= (1UL << 17);

    GPIOA_MODER &= ~((3UL << 4) | (3UL << 6));
    GPIOA_MODER |= (2UL << 4) | (2UL << 6);
    GPIOA_AFRL &= ~((0xFUL << 8) | (0xFUL << 12));
    GPIOA_AFRL |= (7UL << 8) | (7UL << 12);

    USART2_BRR = 139UL; /* 16,000,000 Hz / 115,200 baud, rounded. */
    USART2_CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

int _write(int file, const char *buffer, int length)
{
    int index;
    (void)file;

    for (index = 0; index < length; ++index) {
        while ((USART2_SR & USART_SR_TXE) == 0U) {
        }
        USART2_DR = (uint32_t)buffer[index];
    }

    return length;
}
