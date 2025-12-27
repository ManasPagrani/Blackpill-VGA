# Blackpill-VGA
Fully DMA-driven monochrome VGA signal generator on STM32F411

## Overview

This repository contains a **800X600 72Hz 1bit** VGA signal generator for **STM32F411**, implemented without interrupts.
Video timing, pixel streaming, and buffer updates are handled using **timers, SPI, and DMA chaining**.

---


## Framebuffer Interface

```c
uint16_t* vga_setup(void);
```

`vga_setup()`:

* vga_setup() returns a pointer to a row-dominant framebuffer representing an 800 × 600 monochrome display, stored as an array of **30,000 uint16_t elements**.
**Each element encodes 16 horizontal pixels**, with the MSB corresponding to the left-most pixel on the displayed element.

---

## Requirements

* STM32F411
* STM32 HAL / CMSIS
* External resistor or level shifting for VGA output

---

## Pin Assignment

* PA7 - DATA
* PA11 - HSYNC
* PA15 - VSYNC
* GND - Ground


## Build & Usage

1. Add the source files to an STM32 project.
2. Ensure system and peripheral clocks are running at 100MHz preferably sourced from an external crystal.
3. Call `vga_setup()` once from `main()`
4. Write pixel data directly to the returned framebuffer


---


## Notes

* This project is intended as a **low-level embedded systems exercise**
* Not designed for portability
* Timing values are tuned for the specific STM32 configuration

---
