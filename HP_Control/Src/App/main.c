/*!
* @file main.c
*
* @brief 高压液压控制系统主程序 - 平台无关化版本
*
* 版本历史：
* - v8.0 (2025-10-22): 平台无关化 - 只负责数据采集和执行，控制算法移至PC端
* - v7.3 (2025-10-15): 代码深度清理
*/

/* ===========================================  Includes  =========================================== */
#include "config.h"
#include "clock_config.h"
#include "gpio_drv.h"
#include "pwm_common.h"
#include "pwm_output.h"
#include "sensor.h"
#include "valve_control.h"
#include "fault_diagnosis.h"
#include "can_config.h"
#include "gcu_control_dbc.h"
#include "optimized_task_scheduler.h"
#include "debugout_ac7840x.h"
#include "osif.h"
#include <stdio.h>
#include <string.h>

/* ============================================  Define  ============================================ */
// PC17启动开关相关宏定义
#define START_SWITCH_GPIO_PORT   GPIOC
#define START_SWITCH_PIN        17U
#define START_SWITCH_ON_LEVEL   0U  // 低电平为开
// 如需强制将启动开关视为"开启"状态，置1
#define FORCE_START_SWITCH_ON   0

// PC命令超时保护
#define PC_CMD_TIMEOUT_MS  1000  // 1秒超时

/* ==========================================  Variables  =========================================== */
// 全局变量
bool g_systemEnabled = false;
static uint32_t last_pc_cmd_time = 0;  // PC命令接收时间
static uint32_t g_can_rx_message_count = 0;  // CAN接收消息计数

// PC控制参数（从CAN接收）
static uint8_t g_reversal_valve_freq = 0;  // 换向阀频率（Hz）
static uint8_t g_control_mode = 0;         // 控制模式

/* ====================================  Functions declaration  ===================================== */
static void SystemHardwareInit(void);
static void SystemInit(void);
bool IsStartupSwitchActive(void);

// 任务声明
void Task_10ms_SendSensorData(void);
void Task_50ms_SafetyCheck(void);
void Task_1000ms_CANStatusMonitor(void);
void Task_2000ms_SensorDataMonitor(void);
void Task_100ms_RealTimeCANMonitor(void);
void Task_1ms_CANMessageProcess(void);

// CAN接收回调
void CAN_RxCallback(uint32_t msg_id, const uint8_t* data, uint8_t length);

// 访问函数（供其他模块使用）
uint8_t GetReversalValveFreq(void);
uint8_t GetControlMode(void);

/* ======================================  Functions define  ======================================== */

/*!
 * @brief 统一系统初始化函数
 */
static void SystemInit(void)
{
    // 硬件初始化
    SystemHardwareInit();
    
    // 核心模块初始化
    Sensor_Init();            // 传感器模块初始化
    ValveControl_Init();      // 阀门控制初始化
    FaultDiagnosis_Init();    // 故障诊断初始化
    
    // CAN通信模块初始化 - 添加调试信息
    printf("[INIT] Initializing CAN module...\r\n");
    bool can_init_result = CAN_Config_Init();
    if (can_init_result) {
        printf("[INIT] CAN module initialized successfully\r\n");
    } else {
        printf("[INIT] ERROR: CAN module initialization failed!\r\n");
    }
    
    // 初始化优化任务调度器
    OptimizedTaskScheduler_Init();
    
    // 初始为未使能，待接收CAN系统使能后再开启
    g_systemEnabled = false;

    // 上电默认关闭风冷器
    ValveControl_SetCooler(false);
    
    // 上电默认关闭换向阀
    ValveControl_SetDirectionalValve(false);
    
    // 上电默认关闭旁通阀
    ValveControl_SetBypassValve(0.0f);
    
    // 系统初始化完成确认
    printf("[INIT] System initialization completed\r\n");
    printf("[INIT] Default state: Cooler=OFF, DirectionalValve=OFF, BypassValve=0%%\r\n");
}

/*!
* @brief 主函数 - 平台无关化版本
* 
* 功能说明：
* - 只负责传感器数据采集和通过CAN发送给PC
* - 接收PC的CAN控制命令并执行
* - 保留基础硬件安全保护
* - 所有控制算法在PC端运行
*/
int main(void)
{
    // 立即初始化串口进行测试
    CKGEN_DRV_Enable(CLK_UART1, true);
    CKGEN_DRV_SoftReset(SRST_UART1, true);
    CKGEN_DRV_SoftReset(SRST_UART1, false);
    InitDebug();
    
    // 立即测试串口输出
    
    SystemInit();  // 统一初始化
    
    // 系统上电状态确认打印
    printf("\r\n================= GCU System Startup =================\r\n");
    printf("Project: High-Pressure Controller (Platform-Independent)\r\n");
    printf("MCU: AC7840x, SYSCLK: %u MHz\r\n", (unsigned)(SYSTEM_CLOCK_FREQ_HZ/1000000U));
    printf("CAN0: 500 Kbps, Pins RX=PE4 TX=PE5 STB=PE10\r\n");
    printf("DEBUG: UART1@115200, TX=PC9 RX=PC8\r\n");
    printf("IO: DirValve PB4, Bypass PWM0_CH2@PC2, Cooler PE8, StartSw PC17\r\n");
    printf("System Status: INITIALIZED\r\n");
    printf("Sensor Data: Sending to PC every 10ms\r\n");
    printf("Safety Check: Running every 50ms\r\n");
    printf("CAN Monitor: Status check every 1000ms\r\n");
    printf("Sensor Monitor: Data display every 2000ms\r\n");
    printf("Control Commands: Receiving from PC via CAN\r\n");
    printf("========================================================\r\n");
    
    /* 注册CAN接收回调 */
    CAN_Config_RegisterRxCallback(CAN_RxCallback);
    
    /* 配置任务调度器 - 只保留必要任务 */
    // 任务1: 10ms - 发送传感器数据给PC
    OptimizedTaskScheduler_AddTask(Task_10ms_SendSensorData, 10, TASK_PRIORITY_HIGH);
    
    // 任务2: 50ms - 安全保护检查
    OptimizedTaskScheduler_AddTask(Task_50ms_SafetyCheck, 50, TASK_PRIORITY_CRITICAL);
    
    // 任务3: 1000ms - CAN通信状态监控
    OptimizedTaskScheduler_AddTask(Task_1000ms_CANStatusMonitor, 1000, TASK_PRIORITY_LOW);
    
    // 任务4: 2000ms - 传感器数据监控
    OptimizedTaskScheduler_AddTask(Task_2000ms_SensorDataMonitor, 2000, TASK_PRIORITY_LOW);
    
    // 任务5: 100ms - 实时CAN信号监控
    OptimizedTaskScheduler_AddTask(Task_100ms_RealTimeCANMonitor, 100, TASK_PRIORITY_NORMAL);
    
    // 任务6: 1ms - CAN消息处理（关键任务！）
    OptimizedTaskScheduler_AddTask(Task_1ms_CANMessageProcess, 1, TASK_PRIORITY_CRITICAL);
    
    /* 启动任务调度器 */
    OptimizedTaskScheduler_Start();
    
    // 任务调度器启动确认
    printf("[SYSTEM] Task scheduler started successfully\r\n");
    printf("[SYSTEM] Entering main loop...\r\n");
    
    // 启动后立即显示一次状态
    printf("\r\n=== Initial System Status ===\r\n");
    printf("System is running with 6 tasks:\r\n");
    printf("- Task 1: Sensor data (10ms)\r\n");
    printf("- Task 2: Safety check (50ms)\r\n");
    printf("- Task 3: CAN monitor (1000ms)\r\n");
    printf("- Task 4: Sensor monitor (2000ms)\r\n");
    printf("- Task 5: Real-time CAN monitor (100ms)\r\n");
    printf("- Task 6: CAN message process (1ms) - CRITICAL!\r\n");
    printf("First status reports will appear in 5-6 seconds...\r\n");
    printf("================================\r\n");
    
    // 添加调试信息：检查任务是否被正确添加
    printf("[DEBUG] Checking task scheduler status...\r\n");
    printf("[DEBUG] Scheduler running: %s\r\n", OptimizedTaskScheduler_IsRunning() ? "YES" : "NO");
    
    // 立即执行一次监控任务来测试
    printf("[DEBUG] Executing immediate test...\r\n");
    Task_1000ms_CANStatusMonitor();
    Task_2000ms_SensorDataMonitor();
    
    // 移除启动自测PWM，避免比例阀上电即开启；如需生产测试请单独编译开关
    
    // CAN发送测试
    printf("\r\n=== CAN TEST MESSAGE ===\r\n");
    printf("Sending test message...\r\n");
    uint8_t test_data[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    printf("ID: 0x123 (Standard)\r\n");
    printf("Length: 8 bytes\r\n");
    printf("Data: [");
    for (int i = 0; i < 8; i++) {
        printf("%02X", test_data[i]);
        if (i < 7) printf(" ");
    }
    printf("]\r\n");
    printf("Hex: ");
    for (int i = 0; i < 8; i++) {
        printf("%02X ", test_data[i]);
    }
    printf("\r\n");
    
    bool test_result = CAN_Config_SendMessage(0x123, test_data, 8, false);
    printf("Test Send Result: %s\r\n", test_result ? "SUCCESS" : "FAILED");
    printf("========================\r\n");
    
    /* 主循环 */
    printf("[SYSTEM] Entering main loop...\r\n");
    
    // CAN错误检测和恢复计数器
    static uint32_t can_error_count = 0;
    static uint32_t last_can_error_check = 0;
    
    while (1) {
        // 执行任务调度
    OptimizedTaskScheduler_MainLoop();
        
        // 每5000ms检查一次CAN错误状态（减少检查频率）
        static uint32_t tick_counter = 0;
        tick_counter++;
        if (tick_counter - last_can_error_check >= 5000) {
            last_can_error_check = tick_counter;
            
            // 检查CAN错误计数
            uint32_t rx_count, tx_count, error_count;
            CAN_Config_GetStats(&rx_count, &tx_count, &error_count);
            
            // 如果错误过多，尝试重置CAN控制器
            if (error_count > 1000) {
                can_error_count++;
                printf("[CAN ERROR] High error count detected: %lu, attempting reset #%lu\r\n", 
                       error_count, can_error_count);
                
                if (CAN_Config_ResetController()) {
                    printf("[CAN RECOVERY] Controller reset successful\r\n");
                    can_error_count = 0;  // 重置计数器
                } else {
                    printf("[CAN RECOVERY] Controller reset failed\r\n");
                }
            }
        }
    }
    
    // 主循环是无限循环，永远不会到达这里
    // return 0;  // 已删除：不可达代码
}

/* ========================================================================
 * 任务1：发送传感器数据（10ms周期）
 * ======================================================================== */
void Task_10ms_SendSensorData(void)
{
    gcu_debug1_t msg;
    uint8_t can_data[8];
    
    /* 1. 更新传感器数据 */
    Sensor_UpdateMonitor();
    
    /* 2. 填充CAN消息（使用encode函数转换物理值→原始值） */
    msg.oil_temperature = gcu_debug1_oil_temperature_encode(Sensor_GetOilTemperature());
    msg.LNG_temperature = gcu_debug1_LNG_temperature_encode(Sensor_GetLNGTemperature());
    msg.oil_pressure = gcu_debug1_oil_pressure_encode(Sensor_GetOilPressure());
    msg.LNG_pressure = gcu_debug1_LNG_pressure_encode(Sensor_GetLNGPressure());
    msg.bypass_ratio = gcu_debug1_bypass_ratio_encode(ValveControl_GetBypassValveDuty());
    msg.reversal_valve_st = (ValveControl_GetDirectionalValveState() == VALVE_STATE_ON) ? 1 : 0;
    msg.reversal_valve_hz = 0;  // 根据实际硬件填充
    msg.reserve_debug1 = 0;
    
    /* 3. 打包并发送 */
    if (gcu_debug1_pack(can_data, &msg, sizeof(can_data)) > 0) {
        static uint32_t send_count = 0;
        static uint32_t send_fail_count = 0;
        
        // 简化发送信息显示（每1000次显示一次）
        if (++send_count % 1000 == 0) {
            printf("[CAN TX] Count: #%lu\r\n", send_count);
        }
        
        bool send_result = CAN_Config_SendMessage(CAN_MSG_GCU_DEBUG1_ID, can_data, 8, true);
        
        // 只在失败时显示详细信息
        if (!send_result) {
            send_fail_count++;
            if (send_fail_count % 100 == 0) {
                printf("[CAN TX FAIL] Count: #%lu, Failures: %lu\r\n", send_count, send_fail_count);
            }
        }
    }
}

/* ========================================================================
 * 任务2：安全保护检查（50ms周期）
 * ======================================================================== */
void Task_50ms_SafetyCheck(void)
{
    uint32_t current_time = OSIF_GetMilliseconds();
    
    /* 1. 超压保护（>45MPa） */
    if (Sensor_GetOilPressure() > 45.0f) {
        ValveControl_SetBypassValve(100.0f);  // 全开旁通阀
        ValveControl_SetDirectionalValve(false);
    }
    
    /* 2. 超温保护（>120°C） */
    if (Sensor_GetOilTemperature() > 120.0f) {
        ValveControl_SetCooler(true);  // 强制开启冷却
    }
    
    /* 3. 传感器故障保护 */
    if (!Sensor_CheckDataValidity()) {
        ValveControl_SetBypassValve(0.0f);
        ValveControl_SetDirectionalValve(false);
        ValveControl_SetCooler(false);
    }
    
    /* 4. PC命令超时保护（1秒无命令） */
    if (last_pc_cmd_time > 0 && (current_time - last_pc_cmd_time) > PC_CMD_TIMEOUT_MS) {
        ValveControl_SetBypassValve(0.0f);
        ValveControl_SetDirectionalValve(false);
        ValveControl_SetCooler(false);
        last_pc_cmd_time = 0;  // 重置以避免重复打印
    }
    
    /* 5. 硬件故障检查 */
    if (!ValveControl_CheckHardwareStatus()) {
        ValveControl_SetBypassValve(0.0f);
        ValveControl_SetDirectionalValve(false);
    }
}

/* ========================================================================
 * CAN接收回调：处理PC控制命令
 * ======================================================================== */
void CAN_RxCallback(uint32_t msg_id, const uint8_t* data, uint8_t length)
{
    // 增加接收消息计数
    g_can_rx_message_count++;
    
    // 添加调试输出确认回调函数被调用
    printf("[CAN RX CALLBACK] Called! ID: 0x%08X, Length: %d\r\n", msg_id, length);
    
    // 只显示控制消息的调试信息
    if (msg_id == 0x18080100) {
        printf("[CAN RX] Control message received: ID=0x%08X, Len=%d\r\n", msg_id, length);
    }
    
    // 临时修复：处理接收到的控制消息ID
    if ((msg_id == CAN_MSG_GCU_CONTROL_ID || msg_id == 0x18080100) && length == 8) {
        gcu_control_t ctrl_msg;
        
        /* 解包CAN消息 */
        if (gcu_control_unpack(&ctrl_msg, data, length) == 0) {
            
            /* 更新接收时间 */
            last_pc_cmd_time = OSIF_GetMilliseconds();
            
            printf("\r\n=== CAN CONTROL COMMAND RECEIVED ===\r\n");
            printf("Reversal Valve Enable: %u\r\n", ctrl_msg.ctrl_reversal_valve_enable);
            printf("Cooler Enable: %u\r\n", ctrl_msg.ctrl_cooler_enable);
            
            /* 执行控制命令 - 处理所有6个控制信号 */
            
            // 1. 换向阀使能控制
            bool reversal_enable = (ctrl_msg.ctrl_reversal_valve_enable == 1);
            ValveControl_SetDirectionalValve(reversal_enable);
            
            // 2. 旁通阀占空比控制（使用decode转换原始值→物理值）
            double bypass_duty = gcu_control_ctrl_bypass_valve_duty_decode(ctrl_msg.ctrl_bypass_valve_duty);
            ValveControl_SetBypassValve((float)bypass_duty);
            
            // 3. 换向阀频率控制
            g_reversal_valve_freq = ctrl_msg.ctrl_reversal_valve_freq;
            
            // 4. 冷却器使能控制
            bool cooler_enable = (ctrl_msg.ctrl_cooler_enable == 1);
            ValveControl_SetCooler(cooler_enable);
            
            // 5. 系统使能控制
            if (ctrl_msg.ctrl_system_enable == 0) {
                // 系统禁用，安全关闭所有执行器
                ValveControl_SetBypassValve(0.0f);
                ValveControl_SetDirectionalValve(false);
                ValveControl_SetCooler(false);
                g_systemEnabled = false;
            } else {
                g_systemEnabled = true;
            }
            
            // 6. 控制模式（使用ctrl_reserved的低8位）
            g_control_mode = (uint8_t)(ctrl_msg.ctrl_reserved & 0xFFU);
            
            printf("=== CONTROL COMMAND EXECUTED ===\r\n");
            
            // 每次接收后处理控制量（无打印）
            
            // 检查换向阀实际状态
            valve_state_t dir_valve_state = ValveControl_GetDirectionalValveState();

            // 处理调试信息（可选，实际部署时可注释）
            static uint32_t msg_count = 0;
            if (++msg_count % 100 == 0) {  // 每100条消息处理一次
                // 处理PC命令（无打印）
            }
        }
    }
}

/*!
 * @brief 硬件初始化
 */
static void SystemHardwareInit(void)
{
    ClockConfig_Init();                    // 时钟配置
    CKGEN_DRV_Enable(CLK_GPIO, true);     // GPIO时钟使能
    
    // 初始化PWM模块 - 旁通阀控制需要PWM0
    CKGEN_DRV_Enable(CLK_PWM0, true);     // PWM0时钟使能
    CKGEN_DRV_SoftReset(SRST_PWM0, true); // PWM0软复位
    CKGEN_DRV_SoftReset(SRST_PWM0, false); // PWM0软复位完成
    
    // 配置PWM0模块 - 旁通阀PWM控制
    pwm_simply_config_t pwm_config;
    
    // 初始化配置结构体
    memset(&pwm_config, 0, sizeof(pwm_simply_config_t));
    
    // 设置基本配置
    pwm_config.allChCombineMode = PWM_INDEPENDENT_MODE;
    pwm_config.countMode = PWM_UP_COUNT;
    pwm_config.levelMode = PWM_LOW_TRUE;
    pwm_config.clkSource = PWM_CLK_SOURCE_SYSTEM;
    pwm_config.clkPsc = 1U;
    pwm_config.initValue = 0U;
    pwm_config.maxValue = 10000U;  // PWM周期值
    pwm_config.oddPolarity = PWM_OUTPUT_POLARITY_ACTIVE_HIGH;
    pwm_config.evenPolarity = PWM_OUTPUT_POLARITY_ACTIVE_HIGH;
    pwm_config.oddInitLevel = PWM_LOW_LEVEL;
    pwm_config.evenInitLevel = PWM_LOW_LEVEL;
    pwm_config.initChOutputEn = (1U << PWM_CH_2);  // 使能通道2输出
    pwm_config.deadtimePsc = PWM_DEADTIME_DIVID_1;
    
    // 初始化PWM0模块
    PWM_DRV_SimplyInit(0, &pwm_config);
    
    // 配置PC2为PWM0_CH2功能
    GPIO_DRV_SetMuxModeSel(PORTC, 2U, PORT_MUX_ALT2);  // PC2_PWM0_CH2
    
    printf("[INIT] PWM0 module initialized for bypass valve control\r\n");
    
    // 确保调试串口(UART1)时钟就绪（InitDebug内部也会开启，这里冗余确保上电早期可用）
    CKGEN_DRV_Enable(CLK_UART1, true);
    CKGEN_DRV_SoftReset(SRST_UART1, true);
    CKGEN_DRV_SoftReset(SRST_UART1, false);
    InitDebug();
    
    // 等待串口稳定
    for(volatile int i = 0; i < 100000; i++);
    
    // 系统启动信息（无打印）
    
    // 初始化PC17启动开关GPIO
    gpio_settings_config_t start_switch_gpio_config = {
        .base = PORTC,
        .pinPortIdx = 17U,
        .pullConfig = PORT_INTERNAL_PULL_UP_ENABLED,
        .driveSelect = PORT_LOW_DRIVE_STRENGTH,
        .mux = PORT_MUX_AS_GPIO,
        .pinLock = false,
        .intConfig = PORT_DMA_INT_DISABLED,
        .clearIntFlag = false,
        .digitalFilter = false,
        .gpioBase = GPIOC,
        .direction = GPIO_INPUT_DIRECTION,
        .initValue = 0U
    };
    GPIO_DRV_Init(1U, &start_switch_gpio_config);

    // 初始化PE8风冷器控制为GPIO输出
    gpio_settings_config_t cooler_gpio_config = {
        .base = PORTE,
        .pinPortIdx = 8U,
        .pullConfig = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PORT_LOW_DRIVE_STRENGTH,
        .mux = PORT_MUX_AS_GPIO,
        .pinLock = false,
        .intConfig = PORT_DMA_INT_DISABLED,
        .clearIntFlag = false,
        .digitalFilter = false,
        .gpioBase = GPIOE,
        .direction = GPIO_OUTPUT_DIRECTION,
        .initValue = 0U
    };
    GPIO_DRV_Init(1U, &cooler_gpio_config);

    // 默认保持风冷器关闭，由CAN指令控制开启

    // 初始化PB4换向阀控制为GPIO输出（确保方向正确）
    gpio_settings_config_t dir_valve_gpio_config = {
        .base = PORTB,
        .pinPortIdx = 4U,
        .pullConfig = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PORT_LOW_DRIVE_STRENGTH,
        .mux = PORT_MUX_AS_GPIO,
        .pinLock = false,
        .intConfig = PORT_DMA_INT_DISABLED,
        .clearIntFlag = false,
        .digitalFilter = false,
        .gpioBase = GPIOB,
        .direction = GPIO_OUTPUT_DIRECTION,
        .initValue = 0U
    };
    GPIO_DRV_Init(1U, &dir_valve_gpio_config);
}

/*!
 * @brief 检查启动开关是否激活
 * @return true: 开关激活, false: 开关关闭
 */
bool IsStartupSwitchActive(void)
{
    gpio_channel_type_t switch_status = GPIO_DRV_ReadPins(START_SWITCH_GPIO_PORT);
    uint32_t startup_switch = (switch_status & (1U << START_SWITCH_PIN)) ? 0U : 1U;
    
    static uint32_t last_startup_switch = 2U;
    if (last_startup_switch != startup_switch) {
        last_startup_switch = startup_switch;
    }
    
    #if FORCE_START_SWITCH_ON
    return true;
    #else
    return (startup_switch == 1U);
    #endif
}

/*!
 * @brief 最小错误处理实现
 */
void ErrorHandler_Process(void)
{
    /* no-op */
}

/*!
 * @brief 获取换向阀频率（从PC接收的参数）
 * @return 换向阀频率（Hz）
 */
uint8_t GetReversalValveFreq(void)
{
    return g_reversal_valve_freq;
}

/*!
 * @brief 获取控制模式（从PC接收的参数）
 * @return 控制模式
 *         0 = 自动模式
 *         1 = 手动模式
 *         2 = 测试模式
 *         其他 = 保留
 */
uint8_t GetControlMode(void)
{
    return g_control_mode;
}

/* ========================================================================
 * 任务3：CAN通信状态监控（1000ms周期）
 * ======================================================================== */
void Task_1000ms_CANStatusMonitor(void)
{
    static uint32_t last_rx_count = 0;
    static uint32_t last_tx_count = 0;
    static uint32_t last_error_count = 0;
    static uint32_t monitor_count = 0;
    
    uint32_t current_rx_count, current_tx_count, current_error_count;
    
    // 获取CAN统计信息
    CAN_Config_GetStats(&current_rx_count, &current_tx_count, &current_error_count);
    
    // 计算增量
    uint32_t rx_delta = current_rx_count - last_rx_count;
    uint32_t tx_delta = current_tx_count - last_tx_count;
    uint32_t error_delta = current_error_count - last_error_count;
    
    // 更新上次计数
    last_rx_count = current_rx_count;
    last_tx_count = current_tx_count;
    last_error_count = current_error_count;
    
    // 每10次监控（10秒）显示状态
    if (++monitor_count % 10 == 0) {
        printf("[CAN] RX:%lu TX:%lu Err:%lu (10s)\r\n", rx_delta, tx_delta, error_delta);
    }
    
    // 每50次监控（50秒）打印一次详细状态
    if (monitor_count % 50 == 0) {
        printf("\r\n=== CAN Communication Status ===\r\n");
        printf("Total RX: %lu, TX: %lu, Errors: %lu\r\n", 
               current_rx_count, current_tx_count, current_error_count);
        printf("Rate (10s): RX=%lu, TX=%lu, Errors=%lu\r\n", 
               rx_delta, tx_delta, error_delta);
        printf("===============================\r\n");
    }
    
    // 错误检测和报警
    if (error_delta > 0) {
        printf("[CAN] Warning: %lu errors detected in last 1s\r\n", error_delta);
    }
    
    // 通信超时检测
    static uint32_t no_rx_count = 0;
    if (rx_delta == 0 && current_rx_count > 0) {
        if (++no_rx_count >= 5) {  // 5秒无接收
            printf("[CAN] Warning: No RX messages for %lu seconds\r\n", no_rx_count);
        }
    } else {
        no_rx_count = 0;  // 重置计数器
    }
}

/* ========================================================================
 * 任务4：传感器数据监控（2000ms周期）
 * ======================================================================== */
void Task_2000ms_SensorDataMonitor(void)
{
    static uint32_t monitor_count = 0;
    
    // 更新传感器数据
    Sensor_UpdateMonitor();
    
    // 获取当前传感器读数
    float oil_temp = Sensor_GetOilTemperature();
    float lng_temp = Sensor_GetLNGTemperature();
    float oil_pressure = Sensor_GetOilPressure();
    float lng_pressure = Sensor_GetLNGPressure();
    
    // 获取阀门状态
    valve_state_t dir_valve_state = ValveControl_GetDirectionalValveState();
    valve_state_t cooler_state = ValveControl_GetCoolerState();
    float bypass_duty = ValveControl_GetBypassValveDuty();
    
    // 每10次监控（20秒）显示简单状态
    if (++monitor_count % 10 == 0) {
        printf("[SENSOR] Oil:%.1f°C/%.1fMPa LNG:%.1f°C/%.1fMPa\r\n", 
               oil_temp, oil_pressure, lng_temp, lng_pressure);
    }
    
    // 每20次监控（40秒）打印一次详细数据
    if (monitor_count % 20 == 0) {
        printf("\r\n=== Sensor Data Monitor ===\r\n");
        printf("Oil Temperature: %.2f°C\r\n", oil_temp);
        printf("LNG Temperature: %.2f°C\r\n", lng_temp);
        printf("Oil Pressure: %.2f MPa\r\n", oil_pressure);
        printf("LNG Pressure: %.2f MPa\r\n", lng_pressure);
        printf("Directional Valve: %s\r\n", (dir_valve_state == VALVE_STATE_ON) ? "ON" : "OFF");
        printf("Cooler: %s\r\n", (cooler_state == VALVE_STATE_ON) ? "ON" : "OFF");
        printf("Bypass Valve: %.1f%%\r\n", bypass_duty);
        printf("System Enabled: %s\r\n", g_systemEnabled ? "YES" : "NO");
        printf("Control Mode: %u\r\n", g_control_mode);
        printf("Reversal Freq: %u Hz\r\n", g_reversal_valve_freq);
        printf("===========================\r\n");
    }
    
    // 传感器数据有效性检查
    if (!Sensor_CheckDataValidity()) {
        printf("[SENSOR] Warning: Sensor data validity check failed\r\n");
    }
    
    // 超限报警
    if (oil_pressure > 40.0f) {
        printf("[SENSOR] Warning: Oil pressure high (%.2f MPa)\r\n", oil_pressure);
    }
    if (oil_temp > 100.0f) {
        printf("[SENSOR] Warning: Oil temperature high (%.2f°C)\r\n", oil_temp);
    }
    if (lng_temp > 80.0f) {
        printf("[SENSOR] Warning: LNG temperature high (%.2f°C)\r\n", lng_temp);
    }
}

/* ========================================================================
 * 任务5：实时CAN信号监控（100ms周期）
 * ======================================================================== */
void Task_100ms_RealTimeCANMonitor(void)
{
    static uint32_t last_rx_count = 0;
    static uint32_t last_tx_count = 0;
    static uint32_t monitor_count = 0;
    
    uint32_t current_rx_count, current_tx_count, current_error_count;
    
    // 获取CAN统计信息
    CAN_Config_GetStats(&current_rx_count, &current_tx_count, &current_error_count);
    
    // 计算增量
    uint32_t rx_delta = current_rx_count - last_rx_count;
    uint32_t tx_delta = current_tx_count - last_tx_count;
    
    // 更新上次计数
    last_rx_count = current_rx_count;
    last_tx_count = current_tx_count;
    
    // 每100次监控（10秒）显示实时CAN活动
    if (++monitor_count % 100 == 0) {
        printf("[CAN] RX: %lu, TX: %lu, Rate: %lu/%lu msg/s\r\n", 
               current_rx_count, current_tx_count, rx_delta, tx_delta);
    }
}

/* ========================================================================
 * 任务6：CAN消息处理（1ms周期 - 关键任务！）
 * ======================================================================== */
void Task_1ms_CANMessageProcess(void)
{
    // 调用CAN配置模块的消息处理任务
    // 这个函数负责：
    // 1. 处理接收到的CAN消息
    // 2. 调用接收回调函数
    // 3. 更新CAN统计信息
    CAN_Config_Task();
}

/* =============================================  EOF  ============================================== */
