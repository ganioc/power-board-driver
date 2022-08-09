/**
  **************************************************************************
  * @file     at32f421_int.c
  * @version  v2.0.6
  * @date     2022-05-20
  * @brief    main interrupt service routines.
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */

/* includes ------------------------------------------------------------------*/
#include "at32f421_int.h"
#include "at32f421_board.h"
#include "led.h"

/** @addtogroup AT32F421_periph_template
  * @{
  */

/** @addtogroup 421_LED_toggle
  * @{
  */

extern uint8_t usart1_rx_buffer[];
extern uint16_t usart1_rx_counter;
//extern uint16_t dma_trans_complete_flag;
//extern uint16_t dma_in_working;

/**
  * @brief  this function handles nmi exception.
  * @param  none
  * @retval none
  */
void NMI_Handler(void)
{
}

/**
  * @brief  this function handles hard fault exception.
  * @param  none
  * @retval none
  */
void HardFault_Handler(void)
{
  /* go to infinite loop when hard fault exception occurs */
  while(1)
  {
  }
}

/**
  * @brief  this function handles memory manage exception.
  * @param  none
  * @retval none
  */
void MemManage_Handler(void)
{
  /* go to infinite loop when memory manage exception occurs */
  while(1)
  {
  }
}

/**
  * @brief  this function handles bus fault exception.
  * @param  none
  * @retval none
  */
void BusFault_Handler(void)
{
  /* go to infinite loop when bus fault exception occurs */
  while(1)
  {
  }
}

/**
  * @brief  this function handles usage fault exception.
  * @param  none
  * @retval none
  */
void UsageFault_Handler(void)
{
  /* go to infinite loop when usage fault exception occurs */
  while(1)
  {
  }
}

/**
  * @brief  this function handles svcall exception.
  * @param  none
  * @retval none
  */
void SVC_Handler(void)
{
}

/**
  * @brief  this function handles debug monitor exception.
  * @param  none
  * @retval none
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  this function handles pendsv_handler exception.
  * @param  none
  * @retval none
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  this function handles systick handler.
  * @param  none
  * @retval none
  */
void SysTick_Handler(void)
{

}

/**
  * @}
  */
/**
  * @brief  this function handles usart1 handler.
  * @param  none
  * @retval none
  */
void USART1_IRQHandler(void)
{


  if(usart_flag_get(USART1, USART_RDBF_FLAG) != RESET)
  {
    /* read one byte from the receive data register */
    usart1_rx_buffer[usart1_rx_counter++] = usart_data_receive(USART1);

    if(usart1_rx_counter == USART1_RX_BUFFER_SIZE)
    {
      /* disable the usart1 receive interrupt */
      // usart_interrupt_enable(USART1, USART_RDBF_INT, FALSE);
    	usart1_rx_counter=0;
    }
    at32_led_toggle(LED2);

  }

//  if(usart_flag_get(USART1, USART_TDBE_FLAG) != RESET)
//  {
//    /* write one byte to the transmit data register */
//    usart_data_transmit(USART1, usart1_tx_buffer[usart1_tx_counter++]);
//
//    if(usart1_tx_counter == usart1_tx_buffer_size)
//    {
//      /* disable the usart1 transmit interrupt */
//      usart_interrupt_enable(USART1, USART_TDBE_INT, FALSE);
//    }
//  }
}

/**
  * @brief  this function handles dma1_channel1 handler.
  * @param  none
  * @retval none
  */
void DMA1_Channel1_IRQHandler(void)
{

}

void TMR1_BRK_OVF_TRG_HALL_IRQHandler(void) {
	if (tmr_flag_get(TMR1, TMR_OVF_FLAG) != RESET) {
		/* add user code... */

//		at32_led_toggle(LED3);
		led_red_shine();
//		led_blue_shine();
//		led_green_shine();

		tmr_flag_clear(TMR1, TMR_OVF_FLAG);

	}
}
/**
  * @}
  */
