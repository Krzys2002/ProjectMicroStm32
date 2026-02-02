/**
  ******************************************************************************
  * @file    motor_control.c
  * @brief   Implementation of the motor control loop and PID controller.
  ******************************************************************************
  */

#define ARM_MATH_CM7
#include "arm_math.h"
#include "motor_control.h"
#include "control_state.h"
#include "pid_sample_rb.h"
#include "pwm.h"

/* Private variables ---------------------------------------------------------*/
extern TIM_HandleTypeDef htim1; /**< Timer handle for PWM output */
extern TIM_HandleTypeDef htim2; /**< Timer handle for encoder input */

static arm_pid_instance_f32 PID;       /**< PID controller instance from CMSIS-DSP */
static uint32_t last_hw_cnt = 0;       /**< Last encoder count to calculate delta */
static float current_pos_deg = 0.0f;   /**< Current estimated motor position in degrees */
static float last_out = 0.0f;          /**< Last controller output */
float out = 0;                         /**< Current controller output (public for debug) */
float last_set_p = 0.0f;               /**< Last setpoint value to detect changes */

/** 
  * @brief PWM handle for motor control 
  */
PWM_Handle_TypeDef hand1 = PWM_INIT_HANDLE(&htim1, TIM_CHANNEL_2);

/**
  * @brief  Initializes the PID instance, ring buffer, and starts PWM peripherals.
  * @retval None
  */
void MotorControl_Init(void)
{
    float Ts = 1.0f / PID_FREQUENCY_HZ;

    last_hw_cnt = __HAL_TIM_GET_COUNTER(&htim2);

    /* CMSIS-DSP PID Configuration */
    PID.Kp = 0.15f;
    PID.Ki = 0.0f * Ts;
    PID.Kd = 0.015f / Ts;

    arm_pid_init_f32(&PID, 1);

    pidrb_init();

    /* Start PWM Generation */
    PWM_Init(&hand1);
}

/**
  * @brief  Execution step for the motor control loop.
  *         Calculates position from encoder, computes PID output, and updates PWM.
  * @note   Must be called at a fixed frequency (typically from TIM7 ISR).
  * @retval None
  */
void MotorControl_Step(void)
{
	if(last_set_p != control_get_setpoint_deg())
	{
		current_pos_deg = 0.0f;
		last_set_p = control_get_setpoint_deg();
	}
    /* Update position from encoder counts */
    uint32_t current_hw_cnt = __HAL_TIM_GET_COUNTER(&htim2);
    uint32_t hw_delta = current_hw_cnt - last_hw_cnt;
    last_hw_cnt = current_hw_cnt;

    /* Determine direction based on control history */
    //int32_t signed_delta = (last_out >= 0) ? (int32_t)hw_delta : -(int32_t)hw_delta;
    int32_t signed_delta = (int32_t)hw_delta;
    float step_deg = ((float)signed_delta / PULSES_PER_REV) * 360.0f;
    current_pos_deg += step_deg;

    /* Compute control effort */
    float error = control_get_setpoint_deg() - current_pos_deg;
    out = arm_pid_f32(&PID, error);

    float out_pwm = out;
    /* Output clamping */
    if (out > 50.0f) out_pwm = 50.f;
    else if (out < -50.0f) out_pwm = -50.0f;

    if(out < 15.0f && out > 1.0f) out_pwm = 20.0f;
    else if(out > -15.0f && out < -1.0f) out_pwm = -20.0f;

    if(out < 1.0f && out > -1.0f) out_pwm = 0;

    /* Actuator update */
    if (out_pwm >= 0) {
        HAL_GPIO_WritePin(MOTOR_DIR_GPIO_Port, MOTOR_DIR_Pin, GPIO_PIN_SET);
        PWM_WriteDuty(&hand1, out_pwm);
    } else {
        HAL_GPIO_WritePin(MOTOR_DIR_GPIO_Port, MOTOR_DIR_Pin, GPIO_PIN_RESET);
        PWM_WriteDuty(&hand1, -out_pwm);
    }

    last_out = out;

    /* Log telemetry data */
    pid_sample_t s = {
        .t_ms = HAL_GetTick(),
        .pos_deg = current_pos_deg,
        .spd_deg_s = step_deg * PID_FREQUENCY_HZ,
        .sp_deg = control_get_setpoint_deg(),
        .out = out
    };
    pidrb_push_isr(&s);
}
