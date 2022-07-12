/**
  ******************************************************************************
  * @file      startup_stm32f0xx.s
  * @author    MCD Application Team
  * @version   V1.3.1
  * @date      17-January-2014
  * @brief     STM32F030 Devices vector table for RIDE7 toolchain.
  *            This module performs:
  *                - Set the initial SP
  *                - Set the initial PC == Reset_Handler,
  *                - Set the vector table entries with the exceptions ISR address
  *                - Configure the system clock 
  *                - Branches to main in the C library (which eventually
  *                  calls main()).
  *            After Reset the Cortex-M0 processor is in Thread mode,
  *            priority is Privileged, and the Stack is set to Main.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

  .syntax unified
  .cpu cortex-m0
  .fpu softvfp
  .thumb

.global g_pfnVectors
.global Default_Handler

/* start address for the initialization values of the .data section.
defined in linker script */
.word _sidata
/* start address for the .data section. defined in linker script */
.word _sdata
/* end address for the .data section. defined in linker script */
.word _edata
/* start address for the .bss section. defined in linker script */
.word _sbss
/* end address for the .bss section. defined in linker script */
.word _ebss

.equ  BootRAM, 0xF108F85F
/**
 * @brief  This is the code that gets called when the processor first
 *          starts execution following a reset event. Only the absolutely
 *          necessary set is performed, after which the application
 *          supplied main() routine is called.
 * @param  None
 * @retval : None
*/

  .section .text.Reset_Handler
  .weak Reset_Handler
  .type Reset_Handler, %function
Reset_Handler:
  ldr   r0, =_estack
  mov   sp, r0          /* set stack pointer */

/* Copy the data segment initializers from flash to SRAM */
  movs r1, #0
  b LoopCopyDataInit

CopyDataInit:
  ldr r3, =_sidata
  ldr r3, [r3, r1]
  str r3, [r0, r1]
  adds r1, r1, #4

LoopCopyDataInit:
  ldr r0, =_sdata
  ldr r3, =_edata
  adds r2, r0, r1
  cmp r2, r3
  bcc CopyDataInit
  ldr r2, =_sbss
  b LoopFillZerobss
/* Zero fill the bss segment. */
FillZerobss:
  movs r3, #0
  str  r3, [r2]
  adds r2, r2, #4


LoopFillZerobss:
  ldr r3, = _ebss
  cmp r2, r3
  bcc FillZerobss

/* Call the clock system intitialization function.*/
    bl  SystemInit
    
/* Call the application's entry point.*/
  bl main
  
LoopForever:
    b LoopForever


.size Reset_Handler, .-Reset_Handler

/**
 * @brief  This is the code that gets called when the processor receives an
 *         unexpected interrupt.  This simply enters an infinite loop, preserving
 *         the system state for examination by a debugger.
 *
 * @param  None
 * @retval : None
*/
    .section .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
  b Infinite_Loop
  .size Default_Handler, .-Default_Handler
/******************************************************************************
*
* The minimal vector table for a Cortex M0.  Note that the proper constructs
* must be placed on this to ensure that it ends up at physical address
* 0x0000.0000.
*
******************************************************************************/
   .section .isr_vector,"a",%progbits
  .type g_pfnVectors, %object
  .size g_pfnVectors, .-g_pfnVectors


g_pfnVectors:
  .word _estack
  .word Reset_Handler

  .word NMI_Handler
  .word HardFault_Handler
  .word 0
  .word 0
  .word 0
  .word 0
  .word 0
  .word 0
  .word 0
  .word SVC_Handler
  .word 0
  .word 0
  .word PendSV_Handler
  .word SysTick_Handler


  .word WWDG_IRQHandler
  .word 0  
  .word RTC_IRQHandler
  .word FLASH_IRQHandler
  .word RCC_IRQHandler
  .word EXTI0_1_IRQHandler
  .word EXTI2_3_IRQHandler
  .word EXTI4_15_IRQHandler
  .word 0  
  .word DMA1_Channel1_IRQHandler
  .word DMA1_Channel2_3_IRQHandler
  .word DMA1_Channel4_5_IRQHandler
  .word ADC1_IRQHandler 
  .word TIM1_BRK_UP_TRG_COM_IRQHandler
  .word TIM1_CC_IRQHandler
  .word 0  
  .word TIM3_IRQHandler
  .word 0   
  .word 0  
  .word TIM14_IRQHandler
  .word TIM15_IRQHandler
  .word TIM16_IRQHandler
  .word TIM17_IRQHandler
  .word I2C1_IRQHandler
  .word I2C2_IRQHandler
  .word SPI1_IRQHandler
  .word SPI2_IRQHandler
  .word USART1_IRQHandler
  .word USART2_IRQHandler
  .word 0
  .word 0
  .word 0
  .word BootRAM          /* @0x108. This is for boot in RAM mode for 
                            STM32F0xx devices. */
 

/*******************************************************************************
*
* Provide weak aliases for each Exception handler to the Default_Handler.
* As they are weak aliases, any function with the same name will override
* this definition.
*
*******************************************************************************/

  .weak NMI_Handler
  .thumb_set NMI_Handler,Default_Handler

  .weak HardFault_Handler
  .thumb_set HardFault_Handler,Default_Handler

  .weak SVC_Handler
  .thumb_set SVC_Handler,Default_Handler

  .weak PendSV_Handler
  .thumb_set PendSV_Handler,Default_Handler

  .weak SysTick_Handler
  .thumb_set SysTick_Handler,Default_Handler

  .weak WWDG_IRQHandler
  .thumb_set WWDG_IRQHandler,Default_Handler
 
  .weak RTC_IRQHandler
  .thumb_set RTC_IRQHandler,Default_Handler
  
  .weak FLASH_IRQHandler
  .thumb_set FLASH_IRQHandler,Default_Handler
  
  .weak RCC_IRQHandler
  .thumb_set RCC_IRQHandler,Default_Handler
  
  .weak EXTI0_1_IRQHandler
  .thumb_set EXTI0_1_IRQHandler,Default_Handler
  
  .weak EXTI2_3_IRQHandler
  .thumb_set EXTI2_3_IRQHandler,Default_Handler
  
  .weak EXTI4_15_IRQHandler
  .thumb_set EXTI4_15_IRQHandler,Default_Handler
  
  .weak DMA1_Channel1_IRQHandler
  .thumb_set DMA1_Channel1_IRQHandler,Default_Handler
  
  .weak DMA1_Channel2_3_IRQHandler
  .thumb_set DMA1_Channel2_3_IRQHandler,Default_Handler
  
  .weak DMA1_Channel4_5_IRQHandler
  .thumb_set DMA1_Channel4_5_IRQHandler,Default_Handler
  
  .weak ADC1_IRQHandler
  .thumb_set ADC1_IRQHandler,Default_Handler
   
  .weak TIM1_BRK_UP_TRG_COM_IRQHandler
  .thumb_set TIM1_BRK_UP_TRG_COM_IRQHandler,Default_Handler
  
  .weak TIM1_CC_IRQHandler
  .thumb_set TIM1_CC_IRQHandler,Default_Handler
    
  .weak TIM3_IRQHandler
  .thumb_set TIM3_IRQHandler,Default_Handler
    
  .weak TIM14_IRQHandler
  .thumb_set TIM14_IRQHandler,Default_Handler
  
  .weak TIM15_IRQHandler
  .thumb_set TIM15_IRQHandler,Default_Handler
  
  .weak TIM16_IRQHandler
  .thumb_set TIM16_IRQHandler,Default_Handler
  
  .weak TIM17_IRQHandler
  .thumb_set TIM17_IRQHandler,Default_Handler
  
  .weak I2C1_IRQHandler
  .thumb_set I2C1_IRQHandler,Default_Handler
  
  .weak I2C2_IRQHandler
  .thumb_set I2C2_IRQHandler,Default_Handler
  
  .weak SPI1_IRQHandler
  .thumb_set SPI1_IRQHandler,Default_Handler
  
  .weak SPI2_IRQHandler
  .thumb_set SPI2_IRQHandler,Default_Handler
  
  .weak USART1_IRQHandler
  .thumb_set USART1_IRQHandler,Default_Handler
  
  .weak USART2_IRQHandler
  .thumb_set USART2_IRQHandler,Default_Handler
  

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

