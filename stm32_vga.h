/*
 * stm32_vga.h
 *
 *  Created on: Dec 26, 2025
 *      Author: manas
 */

#ifndef SRC_STM32_VGA_H_
#define SRC_STM32_VGA_H_


#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize hardware peripherals and start VGA signal generation.
 *
 * This function configures GPIO, timers (TIM1, TIM2), SPI1, and multiple
 * DMA2 streams to generate a VGA-compatible video signal without using
 * CPU interrupts during active video output.
 *
 * After successful initialization, the VGA pipeline runs autonomously
 * using chained DMA transfers and hardware timers.
 *
 * @return
 * Pointer to the frame buffer used for pixel data.
 *
 * Memory notes:
 * - The returned buffer is dynamically allocated and must remain valid
 *   for the lifetime of VGA operation.
 * - The buffer is accessed directly by DMA; it must not be freed or
 *   relocated while VGA output is active.
 *
 * Usage notes:
 * - This function must be called once during system initialization.
 * - The system clock must already be configured to the expected frequency(100 MHz)
 *   and should be sourced from an external crystal/ceramic oscillator for best stability .
 * -
 *
 * @warning
 * - This function performs direct register access and assumes exclusive
 *   ownership of the configured peripherals.
 * - Misconfiguration may result in unstable video output or undefined behavior.
 */

uint16_t *vga_setup(void);

#ifdef __cplusplus
}
#endif

#endif /* SRC_STM32_VGA_H_ */
