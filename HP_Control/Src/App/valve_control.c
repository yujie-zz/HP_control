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
#include "ckgen_drv.h"
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
    
    // 初始化PWM0模块用于旁通阀控制
    pwm_simply_config_t pwm_config;
    memset(&pwm_config, 0, sizeof(pwm_simply_config_t));
    
    // 配置PWM参数
    pwm_config.allChCombineMode = PWM_INDEPENDENT_MODE;
    pwm_config.countMode = PWM_UP_COUNT;
    pwm_config.levelMode = PWM_HIGH_TRUE;
    pwm_config.clkSource = PWM_CLK_SOURCE_SYSTEM;
    pwm_config.clkPsc = 1U;
    pwm_config.initValue = 0U;
    pwm_config.maxValue = 10000U;
    pwm_config.modDitherValue = 0U;
    pwm_config.oddPolarity = PWM_OUTPUT_POLARITY_ACTIVE_HIGH;
    pwm_config.evenPolarity = PWM_OUTPUT_POLARITY_ACTIVE_HIGH;
    pwm_config.oddInitLevel = PWM_LOW_LEVEL;
    pwm_config.evenInitLevel = PWM_LOW_LEVEL;
    pwm_config.complementEn = false;
    pwm_config.initChOutputEn = true;  // 使能初始通道输出
    pwm_config.deadtime = 0U;
    pwm_config.deadtimePsc = PWM_DEADTIME_DIVID_1;
    pwm_config.interruptEn = false;
    pwm_config.overflowEventEn = false;
    pwm_config.underflowEventEn = false;
    pwm_config.overflowInterrupEn = false;
    pwm_config.overflowCallback = NULL;
    pwm_config.channelCallback = NULL;
    
    // 初始化PWM0
    PWM_DRV_SimplyInit(0, &pwm_config);
    
    // 配置PC2为PWM0_CH2功能
    GPIO_DRV_SetMuxModeSel(PORTC, 2U, PORT_MUX_ALT2);
    
    // 配置PWM0通道2为独立模式
    pwm_independent_ch_config_t ch0_config = {
        .channel = PWM_CH_2,
        .chValue = 0U,  // 初始占空比为0
        .chDitherValue = 0U,
        .levelMode = PWM_HIGH_TRUE,  // 高电平有效
        .polarity = PWM_OUTPUT_POLARITY_ACTIVE_HIGH,
        .interruptEn = false,
        .initLevel = PWM_LOW_LEVEL,  // 初始输出低电平
        .matchTriggerEn = false,
        .chEventDmaEn = false
    };
    
    pwm_modulation_config_t mod_config = {
        .countMode = PWM_UP_COUNT,
        .independentChannelNum = 1U,
        .combineChannelNum = 0U,
        .triggerRatio = 0U,
        .independentChConfig = &ch0_config,
        .combineChConfig = NULL,
        .initChOutputEn = false,  // 先禁用输出，配置完成后再启用
        .initTriggerEn = false,
        .maxTriggerEn = false,
        .centerAlignDutyType = PWM_DUTY_MODE_0
    };
    
    // 设置PWM为调制模式（这会设置通道计数值为chValue=0）
    PWM_DRV_SetPWMMode(0, &mod_config);
    
    // 确保通道计数值为0（占空比0%）
    PWM_DRV_SetChannelCountValue(0, PWM_CH_2, 0);
    
    // 启动PWM全局时间基准（这是必需的，否则PWM计数器不会运行）
    PWM_DRV_SetGlobalTimeBase(0, true);
    PWM_DRV_SetGlobalTimeBaseOutput(0, true);
    
    // 等待PWM稳定（确保计数器开始运行）
    for(volatile int i = 0; i < 1000; i++);
    
    // 重要：初始化时不启用通道输出，防止上电即开启
    // 只有在收到CAN命令设置占空比后才启用输出
    // 掩蔽输出：true = 掩蔽(关闭输出)，false = 取消掩蔽(开启输出)
    PWM_DRV_SetChannelOutputMask(0, PWM_CH_2, true);
    
    // 验证设置：读取并打印实际设置的计数值
    uint32_t actual_count = PWM_DRV_GetChannelCountValue(0, PWM_CH_2);
    printf("[VALVE INIT] PWM0 initialized for bypass valve (PWM_CH_2), output DISABLED, CountValue: %lu\r\n", actual_count);
}

void ValveControl_SetDirectionalValve(bool enable)
{
    // 只在状态改变时打印
    bool state_changed = (enable && g_valve_control_data.directional_valve_state != VALVE_STATE_ON) ||
                         (!enable && g_valve_control_data.directional_valve_state != VALVE_STATE_OFF);
    
    if (state_changed) {
        printf("[VALVE] Directional Valve: %s (PB4)\r\n", enable ? "ON" : "OFF");
    }
    
    if (enable) {
        GPIO_DRV_SetPins(GPIOB, 1U << DIRECTIONAL_VALVE_PIN);
        g_valve_control_data.directional_valve_state = VALVE_STATE_ON;
        if (state_changed) {
            printf("[VALVE] GPIO Set: PB4 = HIGH\r\n");
        }
    } else {
        GPIO_DRV_ClearPins(GPIOB, 1U << DIRECTIONAL_VALVE_PIN);
        g_valve_control_data.directional_valve_state = VALVE_STATE_OFF;
        if (state_changed) {
            printf("[VALVE] GPIO Set: PB4 = LOW\r\n");
        }
    }
    g_valve_control_data.last_update_time = OSIF_GetMilliseconds();
}

valve_state_t ValveControl_GetDirectionalValveState(void)
{
    return g_valve_control_data.directional_valve_state;
}

void ValveControl_SetBypassValve(float duty)
{
    if (duty < BYPASS_VALVE_MIN_DUTY) duty = BYPASS_VALVE_MIN_DUTY;
    if (duty > BYPASS_VALVE_MAX_DUTY) duty = BYPASS_VALVE_MAX_DUTY;
    
    // 只在占空比变化超过1%时打印（避免微小变化导致的频繁打印）
    float duty_change = (duty > g_valve_control_data.bypass_valve_duty) ? 
                        (duty - g_valve_control_data.bypass_valve_duty) : 
                        (g_valve_control_data.bypass_valve_duty - duty);
    bool duty_changed = (duty_change > 1.0f);
    
    if (duty_changed) {
        printf("[VALVE] Bypass Valve: %.2f%% (PWM0_CH2)\r\n", duty);
    }
    
    // 使用PWM通道计数值来设置占空比
    uint16_t max_count = PWM_DRV_GetMaxCountValue(0); // 使用PWM实例0
    uint16_t count_value = (uint16_t)(max_count * duty / 100.0f);
    if (duty_changed) {
        printf("[VALVE] PWM Set: MaxCount=%u, CountValue=%u\r\n", max_count, count_value);
    }
    
    // 设置PWM通道计数值
    PWM_DRV_SetChannelCountValue(0, PWM_CH_2, count_value);
    
    // 根据占空比控制通道输出：占空比>0时启用输出，占空比=0时禁用输出
    if (duty > 0.0f) {
        // 占空比>0：启用PWM输出（取消掩蔽）
        PWM_DRV_SetChannelOutputMask(0, PWM_CH_2, false);
    } else {
        // 占空比=0：禁用PWM输出（掩蔽）
        PWM_DRV_SetChannelOutputMask(0, PWM_CH_2, true);
    }
    
    // 验证设置是否生效（调试用）
    uint32_t verify_count = PWM_DRV_GetChannelCountValue(0, PWM_CH_2);
    if (duty_changed && verify_count != count_value) {
        printf("[VALVE WARNING] CountValue mismatch! Set=%u, Read=%lu\r\n", count_value, verify_count);
    }
    
    // 确保PWM全局时间基准已启动（每次设置时检查）
    // 注意：如果PWM未启动，输出将不会改变
    
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
    // 只在状态改变时打印
    bool state_changed = (enable && g_valve_control_data.cooler_state != VALVE_STATE_ON) ||
                         (!enable && g_valve_control_data.cooler_state != VALVE_STATE_OFF);
    
    if (state_changed) {
        printf("[VALVE] Cooler: %s (PE8)\r\n", enable ? "ON" : "OFF");
    }
    
    if (enable) {
        GPIO_DRV_SetPins(GPIOE, 1U << COOLER_CONTROL_PIN);
        g_valve_control_data.cooler_state = VALVE_STATE_ON;
        if (state_changed) {
            printf("[VALVE] GPIO Set: PE8 = HIGH\r\n");
        }
    } else {
        GPIO_DRV_ClearPins(GPIOE, 1U << COOLER_CONTROL_PIN);
        g_valve_control_data.cooler_state = VALVE_STATE_OFF;
        if (state_changed) {
            printf("[VALVE] GPIO Set: PE8 = LOW\r\n");
        }
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
