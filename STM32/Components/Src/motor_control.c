/**
  ******************************************************************************
  * @file    motor_control.c
  * @brief   Motor control loop and PID implementation
  ******************************************************************************
  */

#define ARM_MATH_CM7
#include "arm_math.h"
#include "motor_control.h"
#include "control_state.h"
#include "pid_sample_rb.h"

/* Private variables ---------------------------------------------------------*/
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;

static arm_pid_instance_f32 PID;
static uint32_t last_hw_cnt = 0;
static float current_pos_deg = 0.0f;
static float last_out = 0.0f;

/**
  * @brief  Initializes the PID instance and starts PWM peripherals.
  * @retval None
  */
void MotorControl_Init(void)
{
    float Ts = 1.0f / PID_FREQUENCY_HZ;

    last_hw_cnt = __HAL_TIM_GET_COUNTER(&htim2);

    /* CMSIS-DSP PID Configuration */
    PID.Kp = 0.65f;
    PID.Ki = 0.15f * Ts;
    PID.Kd = 0.02f / Ts;

    arm_pid_init_f32(&PID, 1);

    pidrb_init();

    /* Start PWM Generation */
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
}

/**
  * @brief  Execution step for the motor control loop.
  * Calculates position and updates actuators.
  * @note   Called from TIM7 IRQ Handler at 1kHz.
  * @retval None
  */
void MotorControl_Step(void)
{
    /* Update position from encoder counts */
    uint32_t current_hw_cnt = __HAL_TIM_GET_COUNTER(&htim2);
    uint32_t hw_delta = current_hw_cnt - last_hw_cnt;
    last_hw_cnt = current_hw_cnt;

    /* Determine direction based on control history */
    int32_t signed_delta = (last_out >= 0) ? (int32_t)hw_delta : -(int32_t)hw_delta;
    float step_deg = ((float)signed_delta / PULSES_PER_REV) * 360.0f;
    current_pos_deg += step_deg;

    /* Compute control effort */
    float error = control_get_setpoint_deg() - current_pos_deg;
    float out = arm_pid_f32(&PID, error);

    /* Output clamping */
    if (out > MAX_OUT) out = MAX_OUT;
    else if (out < -MAX_OUT) out = -MAX_OUT;

    /* Actuator update */
    if (out >= 0) {
        HAL_GPIO_WritePin(MOTOR_DIR_GPIO_Port, MOTOR_DIR_Pin, GPIO_PIN_SET);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (uint32_t)out);
    } else {
        HAL_GPIO_WritePin(MOTOR_DIR_GPIO_Port, MOTOR_DIR_Pin, GPIO_PIN_RESET);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (uint32_t)fabsf(out));
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
