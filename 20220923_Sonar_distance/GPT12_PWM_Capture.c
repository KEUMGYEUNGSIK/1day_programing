/**********************************************************************************************************************
 * \file GPT12_PWM_Capture.c
 * \copyright Copyright (C) Infineon Technologies AG 2019
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG or its licensees. If and as long as no such terms of use
 * are agreed, use of this file is subject to following:
 *
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization obtaining a copy of the software and
 * accompanying documentation covered by this license (the "Software") to use, reproduce, display, distribute, execute,
 * and transmit the Software, and to prepare derivative works of the Software, and to permit third-parties to whom the
 * Software is furnished to do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including the above license grant, this restriction
 * and the following disclaimer, must be included in all copies of the Software, in whole or in part, and all
 * derivative works of the Software, unless such copies or derivative works are solely in the form of
 * machine-executable object code generated by a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *********************************************************************************************************************/

/*********************************************************************************************************************/
/*-----------------------------------------------------Includes------------------------------------------------------*/
/*********************************************************************************************************************/
#include "Ifx_Types.h"
#include "IfxGpt12.h"
#include "Bsp.h"

/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/
#define ISR_PRIORITY_GPT12_T2_INT   10              /* Define the GPT12 Timer 2 interrupt priority                  */
#define ISR_PRIORITY_GPT12_T3_INT   11              /* Define the GPT12 Timer 2 interrupt priority                  */
#define MAX_VAL_16_BIT              0xFFFF          /* Used for calculation of timer value with overflows           */
#define TRIG_PIN                    &MODULE_P02,3
#define ECHO_PIN                    &MODULE_P00,7

/*********************************************************************************************************************/
/*-------------------------------------------------Global variables--------------------------------------------------*/
/*********************************************************************************************************************/
float32 g_generatedPwmFreq_Hz   = 50.0; /* Global variable for frequency of generated PWM signal                    */
float32 g_measuredPwmFreq_Hz    = 0.0;  /* Global variable for frequency calculation of PWM signal                  */
uint32  g_cntOverflow           = 0;    /* Global counter variable for the timer overflow between two edges         */
uint32  g_cntOverflow_rising    = 0;
uint32  g_cntOverflow_falling   = 0;
uint32  g_previousCntVal        = 0;    /* Global variable which stores the timer value of the previous interrupt   */
uint32 currentCntVal_rising     = 0;
uint32 currentCntVal_falling    = 0;
/*********************************************************************************************************************/
/*---------------------------------------------Function Implementations----------------------------------------------*/
/*********************************************************************************************************************/
/* Macro to define the Interrupt Service Routine */
IFX_INTERRUPT(GPT12_T2_Int0_Handler, 0, ISR_PRIORITY_GPT12_T2_INT);
extern uint32 finalCntVal_main;
extern float32 T3_frequency_main;
extern uint32 g_cntOverflow_main;
extern uint32 eleps_cnt_val;
extern float32 Sonar_Distance;
float32 HIGH_TIME = 0;
int type_edge = 1;
/* Interrupt Service Routine of timer T2, gets triggered by rising edge on input pin of timer T2 */
void GPT12_T2_Int0_Handler(void)
{
    uint32 currentCntVal = IfxGpt12_T2_getTimerValue(&MODULE_GPT120); /* Get timer value of timer T2 */
    uint32 finalCntVal = 0; /* Variable to calculate final timer counter value */


    if(g_cntOverflow == 0)
        {
            /* If no overflow detected */
            finalCntVal = currentCntVal - g_previousCntVal; /* Total counter value calculation */
        }
        else
        {
            /* One or more overflows detected */
            /* Add to the current counter value, the amount of counter ticks which passed before the first overflow,
             * plus 65525 (0xFFFF) for each additional overflow since the previous rising edge.
             */
            finalCntVal = (uint32)(currentCntVal + (MAX_VAL_16_BIT - g_previousCntVal) + ((g_cntOverflow - 1) * MAX_VAL_16_BIT));
        }

        /* Calculation of the PWM frequency by dividing the frequency of timer T3 through the final total counter value */
        T3_frequency_main = IfxGpt12_T3_getFrequency(&MODULE_GPT120);
        g_measuredPwmFreq_Hz = IfxGpt12_T3_getFrequency(&MODULE_GPT120) / finalCntVal;

        finalCntVal_main = finalCntVal;
        g_previousCntVal = currentCntVal;    /* Set g_previousCntVal to currentCntVal for the next calculation */
        g_cntOverflow_main = g_cntOverflow;
        //g_cntOverflow = 0; /* Reset overflow flag */

        if(type_edge ==1 ){
            IfxGpt12_T2_setCaptureInputMode(&MODULE_GPT120, IfxGpt12_CaptureInputMode_fallingEdgeTxIN);
            type_edge =0;
            g_cntOverflow = 0;
            g_cntOverflow_rising = g_cntOverflow;
            currentCntVal_rising = currentCntVal;
        }
        else
        {
            IfxGpt12_T2_setCaptureInputMode(&MODULE_GPT120, IfxGpt12_CaptureInputMode_risingEdgeTxIN);
            type_edge = 1;
            g_cntOverflow_falling = g_cntOverflow;
            currentCntVal_falling = currentCntVal;
            eleps_cnt_val = (uint32)(currentCntVal_falling + (MAX_VAL_16_BIT-currentCntVal_rising) + (g_cntOverflow_falling-g_cntOverflow_rising-1)*MAX_VAL_16_BIT);
            HIGH_TIME = (float32)eleps_cnt_val * 40.0;
            Sonar_Distance = (340.0 * HIGH_TIME ) / (2.0 * 1000000.0);
        }
}

/* Macro to define the Interrupt Service Routine. */
IFX_INTERRUPT(GPT12_T3_Int0_Handler, 0, ISR_PRIORITY_GPT12_T3_INT);

/* Interrupt Service Routine of timer T3, gets triggered after T3 timer overflow */
void GPT12_T3_Int0_Handler(void)
{
    g_cntOverflow++; /* Increase overflow counter */
}

/* This function initializes timer T2 and T3 of the block GPT1 of module GPT12 to capture PWM signals. */
void init_GPT12_module(void)
{
    /* Enable GPT12 module */
    IfxGpt12_enableModule(&MODULE_GPT120);
    /* Select 4 as prescaler for the GPT1 module -> fastest clock frequency for best accuracy */
    IfxGpt12_setGpt1BlockPrescaler(&MODULE_GPT120, IfxGpt12_Gpt1BlockPrescaler_4);
    /* Set core timer T3 in timer mode */
    IfxGpt12_T3_setMode(&MODULE_GPT120, IfxGpt12_Mode_timer);
    /* Set auxiliary timer T2 in capture mode */
    IfxGpt12_T2_setMode(&MODULE_GPT120, IfxGpt12_Mode_capture);
    /* Select input pin A of timer T2 which is P00.7 (the James Bond pin) */
    IfxGpt12_T2_setInput(&MODULE_GPT120, IfxGpt12_Input_A);
    /* Select rising edge as capture event */
    IfxGpt12_T2_setCaptureInputMode(&MODULE_GPT120, IfxGpt12_CaptureInputMode_risingEdgeTxIN);

    /* Service request configuration */
    /* Get source pointer of timer T2, initialize and enable */
    volatile Ifx_SRC_SRCR *src = IfxGpt12_T2_getSrc(&MODULE_GPT120);
    IfxSrc_init(src, IfxSrc_Tos_cpu0, ISR_PRIORITY_GPT12_T2_INT);
    IfxSrc_enable(src);
    /* Get source pointer of timer T3, initialize and enable */
    src = IfxGpt12_T3_getSrc(&MODULE_GPT120);
    IfxSrc_init(src, IfxSrc_Tos_cpu0, ISR_PRIORITY_GPT12_T3_INT);
    IfxSrc_enable(src);

    /* Initialize PWM_PIN port pin */
    IfxPort_setPinMode(TRIG_PIN, IfxPort_Mode_outputPushPullGeneral);

    /* Start timer T3*/
    IfxGpt12_T3_run(&MODULE_GPT120, IfxGpt12_TimerRun_start);
}

/* Generation of simple PWM signal by toggling a pin (frequency can be changed during runtime) */
void trigger_signal(void)
{
    /* Calculate the total time between two rising edges for the specific frequency */
    sint32 targetWaitTime_us_1ms = IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, 1);
    sint32 targetWaitTime_us_99ms = IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, 99);
    /* Set the pin high to trigger an interrupt */
    IfxPort_setPinState(TRIG_PIN, IfxPort_State_high);
    /* Wait for an amount of CPU ticks that represent the calculated microseconds considering the duty cycle */
    wait( targetWaitTime_us_1ms);
    /* Set pin state to low */
    IfxPort_setPinState(TRIG_PIN, IfxPort_State_low);
    /* Wait for an amount of CPU ticks that represent the calculated microseconds considering the duty cycle */

    wait(targetWaitTime_us_99ms);

}
