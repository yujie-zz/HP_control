/**
 * @file config.h
 * @brief 统一配置管理模块 - 整合所有配置参数
 * 
 * 功能模块：
 * - 应用版本和状态管理
 * - 系统配置参数
 * - 调试和安全配置
 * - 传感器配置参数
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include <stdbool.h>
#include "common_types.h"

/* ============================================  Define  ============================================ */

// 调试配置
#define DEBUG_ENABLED              1   // 调试功能使能 (0: 禁用, 1: 启用)
#define DEBUG_SWITCH_STATUS        1   // 启动开关状态调试 (0: 禁用, 1: 启用)
#define DEBUG_SENSOR_DATA          1   // 传感器数据调试 (0: 禁用, 1: 启用)
// 已删除：DEBUG_HYDRAULIC_CONTROL - 液压控制模块已删除

// 条件编译宏
#if DEBUG_ENABLED
    #define DEBUG_PRINT(fmt, ...)  printf(fmt, ##__VA_ARGS__)
#else
    #define DEBUG_PRINT(fmt, ...)  ((void)0)
#endif

#if DEBUG_SWITCH_STATUS
    #define DEBUG_SWITCH(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
    #define DEBUG_SWITCH(fmt, ...) ((void)0)
#endif

#if DEBUG_SENSOR_DATA
    #define DEBUG_SENSOR(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
    #define DEBUG_SENSOR(fmt, ...) ((void)0)
#endif

// 已删除：DEBUG_HYDRAULIC 宏 - 液压控制模块已删除

// 系统配置
// 使用common_types.h中的统一定义
#define TASK_SCHEDULER_ENABLED    1           // 任务调度器使能

// 传感器配置
// 使用common_types.h中的统一定义


// 硬件配置参数
#define DEBUG_UART              UART3
#define PRESSURE_SENSOR_ADC     ADC1
#define PRESSURE_CONTROL_PWM    PWM0

// PWM配置参数
#define PWM_CHANNEL_2    2U    // PC2_PWM0_CH2
// 使用common_types.h中的统一定义

// 日志输出宏定义
#define LOG_INFO(module, format, ...)    printf("[%s] " format "\r\n", module, ##__VA_ARGS__)

/* ===========================================  Typedef  ============================================ */

// 应用状态枚举和系统配置结构体已移动到common_types.h中统一管理

/* ==========================================  Functions  =========================================== */

/**
 * @brief 初始化应用配置
 */
void Config_InitApp(void);

/**
 * @brief 初始化配置模块
 */
void Config_Init(void);

/**
 * @brief 获取应用版本信息
 * @param major 主版本号
 * @param minor 次版本号
 * @param patch 修订版本号
 */
void Config_GetVersion(uint8_t* major, uint8_t* minor, uint8_t* patch);

/**
 * @brief 设置系统配置
 * @param config 系统配置结构体指针
 */
void Config_SetSystemConfig(const system_config_t* config);

/**
 * @brief 验证系统配置
 * @return true: 配置有效, false: 配置无效
 */
bool Config_ValidateSystemConfig(void);

/**
 * @brief 检查调试功能是否启用
 * @return true: 启用, false: 禁用
 */
bool Config_IsDebugEnabled(void);

/**
 * @brief 设置调试功能
 * @param enable true: 启用, false: 禁用
 */
void Config_SetDebugEnabled(bool enable);

#endif 
