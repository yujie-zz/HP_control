/*!
 * @file valve_control.c
 *
 * @brief 阀门控制模块实现 - 换向阀、旁通阀、风冷器控制
 *
 */

#include "valve_control.h"
#include "sensor.h"
#include "gpio_drv.h"
#include "pwm_common.h"
#include "pwm_output.h"
#include "osif.h"
#include <string.h>

/* ==========================================  Variables  =========================================== */

static valve_control_data_t g_valve_control_data;

/* ==========================================  Functions  =========================================== */

void ValveControl_Init(void)
{
    memset(&g_valve_control_data, 0, sizeof(valve_control_data_t));
    g_valve_control_data.directional_valve_state = VALVE_STATE_OFF;
    g_valve_control_data.bypass_valve_state = VALVE_STATE_OFF;
    g_valve_control_data.cooler_state = VALVE_STATE_OFF;
    g_valve_control_data.bypass_valve_duty = 0.0f;
    g_valve_control_data.valve_mode = DIRECTIONAL_VALVE_MODE_AUTO;
    // auto_adjustment_enabled 字段已移除
    g_valve_control_data.last_update_time = OSIF_GetMilliseconds();
    
}

void ValveControl_SetDirectionalValve(bool enable)
{
    printf("[VALVE] Directional Valve: %s (PB4)\r\n", enable ? "ON" : "OFF");
    if (enable) {
        GPIO_DRV_SetPins(GPIOB, 1U << DIRECTIONAL_VALVE_PIN);
        g_valve_control_data.directional_valve_state = VALVE_STATE_ON;
        printf("[VALVE] GPIO Set: PB4 = HIGH\r\n");
    } else {
        GPIO_DRV_ClearPins(GPIOB, 1U << DIRECTIONAL_VALVE_PIN);
        g_valve_control_data.directional_valve_state = VALVE_STATE_OFF;
        printf("[VALVE] GPIO Set: PB4 = LOW\r\n");
    }
    g_valve_control_data.last_update_time = OSIF_GetMilliseconds();
}

valve_state_t ValveControl_GetDirectionalValveState(void)
{
    return g_valve_control_data.directional_valve_state;
}

void ValveControl_SetBypassValve(float duty)
{
    printf("[VALVE] Bypass Valve: %.2f%% (PWM0_CH2)\r\n", duty);
    if (duty < BYPASS_VALVE_MIN_DUTY) duty = BYPASS_VALVE_MIN_DUTY;
    if (duty > BYPASS_VALVE_MAX_DUTY) duty = BYPASS_VALVE_MAX_DUTY;
    
    // 使用PWM通道计数值来设置占空比
    uint16_t max_count = PWM_DRV_GetMaxCountValue(0); // 使用PWM实例0
    uint16_t count_value = (uint16_t)(max_count * duty / 100.0f);
    printf("[VALVE] PWM Set: MaxCount=%u, CountValue=%u\r\n", max_count, count_value);
    PWM_DRV_SetChannelCountValue(0, PWM_CH_2, count_value);  // 使用PWM_CH_2 (PC2)
    g_valve_control_data.bypass_valve_duty = duty;
    g_valve_control_data.bypass_valve_state = (duty > 0.0f) ? VALVE_STATE_ON : VALVE_STATE_OFF;
    g_valve_control_data.last_update_time = OSIF_GetMilliseconds();
}

float ValveControl_GetBypassValveDuty(void)
{
    return g_valve_control_data.bypass_valve_duty;
}

valve_state_t ValveControl_GetBypassValveState(void)
{
    return g_valve_control_data.bypass_valve_state;
}

void ValveControl_SetCooler(bool enable)
{
    printf("[VALVE] Cooler: %s (PE8)\r\n", enable ? "ON" : "OFF");
    if (enable) {
        GPIO_DRV_SetPins(GPIOE, 1U << COOLER_CONTROL_PIN);
        g_valve_control_data.cooler_state = VALVE_STATE_ON;
        printf("[VALVE] GPIO Set: PE8 = HIGH\r\n");
    } else {
        GPIO_DRV_ClearPins(GPIOE, 1U << COOLER_CONTROL_PIN);
        g_valve_control_data.cooler_state = VALVE_STATE_OFF;
        printf("[VALVE] GPIO Set: PE8 = LOW\r\n");
    }
    g_valve_control_data.last_update_time = OSIF_GetMilliseconds();
}

valve_state_t ValveControl_GetCoolerState(void)
{
    return g_valve_control_data.cooler_state;
}

void ValveControl_SetDirectionalValveMode(directional_valve_mode_t mode)
{
    g_valve_control_data.valve_mode = mode;
}

directional_valve_mode_t ValveControl_GetDirectionalValveMode(void)
{
    return g_valve_control_data.valve_mode;
}





void ValveControl_ResetDirectionalValveControl(void)
{
    memset(&g_valve_control_data.stats, 0, sizeof(directional_valve_stats_t));
    g_valve_control_data.stats.min_pressure_valley = 999.0f;
}

directional_valve_stats_t ValveControl_GetDirectionalValveStats(void)
{
    return g_valve_control_data.stats;
}

void ValveControl_ResetDirectionalValveStats(void)
{
    memset(&g_valve_control_data.stats, 0, sizeof(directional_valve_stats_t));
    g_valve_control_data.stats.min_pressure_valley = 999.0f;
}

// 已删除以下空函数 - 平台无关化后不再需要：
// - ValveControl_BypassValveSmartControl() - 智能控制已移至PC端
// - ValveControl_EnableBypassAutoAdjustment() - 自动调节已移至PC端
// - ValveControl_DisableBypassAutoAdjustment() - 自动调节已移至PC端
// - ValveControl_SetBypassAdjustmentParams() - 参数设置已移至PC端
// - ValveControl_Update() - 更新逻辑已移至PC端

const valve_control_data_t* ValveControl_GetData(void)
{
    return &g_valve_control_data;
}

void ValveControl_PrintStatus(void)
{
    // 阀门控制状态检查（无打印）
}

bool ValveControl_CheckHardwareStatus(void)
{
    // 硬件状态检查 - 平台无关化后的简化实现
    // 检查执行器是否响应正常
    
    // 基础检查：所有状态是否正常
    bool hardware_ok = true;
    
    // 检查1：PWM模块状态（旁通阀）
    // 简化实现：假设PWM正常工作
    // 实际项目中可以添加PWM状态寄存器检查
    
    // 检查2：GPIO状态（换向阀、冷却器）
    // 简化实现：假设GPIO正常响应
    // 实际项目中可以添加GPIO回读验证
    
    // 检查3：硬件错误计数
    if (g_valve_control_data.error_count > 10) {
        hardware_ok = false;
    }
    
    return hardware_ok;
}
