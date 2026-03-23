/*
 * stm32_vga.c
 *
 *  Created on: Dec 26, 2025
 *      Author: manas
 */

#include "stm32_vga.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include <stdlib.h>

uint16_t* vga_setup(void)
{
	uint16_t* buffer=malloc(30050*sizeof(uint16_t));
	  	for(int i=30000;i<30050;i++){
	  				buffer[i]=0;
	  	}
	  	for(int i=0;i<30000;i++){
	  		  				buffer[i]=0xffff;
	  		  	}




	  	uint16_t** mbuffer = malloc(666 * sizeof(uint16_t*));
	  	uint16_t* tx_buf=malloc(65*sizeof(uint16_t));

	  	for(int i=0;i<65;i++){
	  	  				tx_buf[i]=0;
	  	  	}



	  	for(int i=0;i<22;i++){
	  		mbuffer[i]=buffer+(600*50);
	  	}
	  	for(int i=0;i<600;i++){
	  		mbuffer[i+22]=buffer+(i*50);
	  	}
	  	for(int i=622;i<666;i++){
	  		mbuffer[i]=buffer+(600*50);
	  	}
	  	uint32_t* config= malloc(3*sizeof(uint32_t));
	  	 config[0]=25; // NDTR
	  	 config[1]=(3<<16)|(2<<13)|(2<<11)|(1<<10)|(1<<9)|(2<<6)|(1); //CR buf to tx_buf DMA2-S7
	  	 config[2]=(1<<22)|(15<<24);//flag clear buf to tx_buf DMA flag clear


	    RCC->APB2ENR|=1|(1<<12);//TIM1 clock enable + SPI1 clock enable
	    RCC->APB1ENR|=1;//TIM2 clock enable


	    RCC->AHB1ENR|=1|(1<<22); //GPIO A clock enable + DMA2 clock enable


	    GPIOA->OSPEEDR|=(3<<22)|(3<<30)|(3<<14); // Puts Hsync and Vsync AND SPI1 pins in high-speed mode
	    GPIOA->MODER|=(2<<22)|(2<<30)|(2<<14); // selects AF mode for above
	    GPIOA->AFR[1]|=(1<<12)|(1<<28);// selects tim1 & 2 ch4,1 as AF
	    GPIOA->AFR[0]|=(5<<28); //SPI1 MOSI in AFR mode

	    SPI1->CR1|=(1<<15)|(1<<14)|(1<<11)|(1<<2);// SPI config:50Mbits,Tx only, 16bit frame
	    SPI1->CR2|=(1<<1)|(1<<2);

	    // SPI DMA configuration:Circular mode,16*65=1040 pixels
	    DMA2_Stream3->CR=(3<<25)|(3<<16)|(1<<13)|(1<<11)|(1<<10)|(1<<6)|(1<<8);
	    DMA2_Stream3->NDTR=65;
	    DMA2_Stream3->PAR=&(SPI1->DR);
	    DMA2_Stream3->FCR|=(1<<2)|3;
	    DMA2_Stream3->M0AR= tx_buf;

	    //tx-buf DMA config (MAR update)(updates memory address after each line).


	    DMA2_Stream1->CR|=(6<<25)|(3<<16)|(2<<13)|(2<<11)|(1<<10)|(1<<8)|(1<<6);
	    DMA2_Stream1->NDTR=666;
	    DMA2_Stream1->PAR=&(DMA2_Stream7->PAR);
	    DMA2_Stream1->M0AR=mbuffer ;
	    DMA2_Stream1->FCR|=3|(1<<2);

	    //tx-buf DMA config ( NDTR update)(updates NDTR after each line)

	    DMA2_Stream2->CR|=(6<<25)|(3<<16)|(2<<13)|(2<<11)|(1<<10)|(1<<8)|(1<<6);
	    DMA2_Stream2->NDTR=1;
	    DMA2_Stream2->PAR=&(DMA2_Stream7->NDTR);
	    DMA2_Stream2->M0AR= config;
	    DMA2_Stream2->FCR|=3|(1<<2);

	    //tx-buf DMA config (EN bit update) (re-enables tx_buf DMA stream for each line)

	    DMA2_Stream6->CR|=(6<<25)|(3<<16)|(2<<13)|(2<<11)|(1<<10)|(1<<8)|(1<<6);
	    DMA2_Stream6->NDTR=1;
	    DMA2_Stream6->PAR=&(DMA2_Stream7->CR);
	    DMA2_Stream6->M0AR= config+1;
	    DMA2_Stream6->FCR|=3|(1<<2);

	    //tx-buf DMA config (flag clear) (clears tx_buf DMA stream flags before each line)

	    DMA2_Stream4->CR|=(6<<25)|(3<<16)|(2<<13)|(2<<11)|(1<<10)|(1<<8)|(1<<6);
	    DMA2_Stream4->NDTR=1;
	    DMA2_Stream4->PAR=&(DMA2->HIFCR);
	    DMA2_Stream4->M0AR= config+2;
	    DMA2_Stream4->FCR|=3|(1<<2);

	    //tx_bux DMA init - MEM2MEM
	    DMA2_Stream7->CR|=(3<<16)|(2<<13)|(2<<11)|(1<<10)|(1<<9)|(2<<6);
	    DMA2_Stream7->NDTR=25;
	    DMA2_Stream7->PAR=mbuffer;
	    DMA2_Stream7->FCR|=3|(1<<2);
	    DMA2_Stream7->M0AR=tx_buf+1;


	    TIM1->CR2|=(1<<4); // SETS CEN as TRIG;

	    // H-SYNC signal using PWM mode on TIM1
	    TIM1->CCMR2|=(6<<12);
	    TIM1->CCER|=(1<<12);
	    TIM1->DIER|=(1<<9)|(1<<10)|(1<<11)|(1<<12);

	    TIM1->CNT= 0xffff-540;
	    TIM1->ARR=2079;

	    // Config DMA triggering times
	    TIM1->CCR1=1850;
	    TIM1->CCR2=1855;
	    TIM1->CCR3=1900;


	    TIM1->CCR4=1840;
	    TIM1->BDTR|=(1<<15);


	    //V-SYNC signal using PWM mode on TIM2 started with TIM1.

	    TIM2->CR2|=(1<<7);
	    TIM2->SMCR|=6;
	    TIM2->CCMR1|=(6<<4)|(6<<12);
	    TIM2->CCMR2|=(6<<12);
	    TIM2->CCER|=1|(1<<4)|(1<<12);

	    TIM2->ARR=665;
	    TIM2->CCR4=66;
	    TIM2->CCR1=660;
	    TIM2->CCR2=22;
	    TIM2->PSC=2079;



	    HAL_Delay(10);
	    // Config DMAs enabled
	    DMA2_Stream1->CR|=1;
	    DMA2_Stream2->CR|=1;
	    DMA2_Stream6->CR|=1;
	    DMA2_Stream4->CR|=1;

	    HAL_Delay(10);
	    //H-SYNC AND V-SYNC signals enabled by TIM1
	    TIM1->CR1|=1;
	    //SPI DMA enabled at position
	    SPI1->CR1|=(1<<6);
	    HAL_Delay(20);
	    while((TIM1->CNT)<99){;}
	    while((TIM1->CNT)>99){;}
	    while((TIM1->CNT)<99){;}

	    DMA2_Stream3->CR|=1;

	    HAL_Delay(10);




  return buffer;
  }


