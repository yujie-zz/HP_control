/* Copyright Statement:
 *
 * This software/firmware and related documentation ("AutoChips Software") are
 * protected under relevant copyright laws. The information contained herein is
 * confidential and proprietary to AutoChips Inc. and/or its licensors. Without
 * the prior written permission of AutoChips inc. and/or its licensors, any
 * reproduction, modification, use or disclosure of AutoChips Software, and
 * information contained herein, in whole or in part, shall be strictly
 * prohibited.
 *
 * AutoChips Inc. (C) 2020. All rights reserved.
 */

/*!
 * @file can_config.h
 *
 * @brief CAN配置头文件 - 高压控制器专用
 *
 */

#ifndef __CAN_CONFIG_H
#define __CAN_CONFIG_H

/* ===========================================  Includes  =========================================== */
#include "common_types.h"  /* 包含公共类型和宏定义 */
#include "gcu_control_dbc.h"  /* 包含新的DBC头文件 */

/* 前向声明以避免与can_drv.h的冲突 */
struct can_msg_info_s;
typedef struct can_msg_info_s can_msg_info_t;

/* 包含CAN驱动类型 */
#include "can_drv.h"

/* ============================================  Define  ============================================ */

/* CAN实例配置 */
#define CAN_INSTANCE                0U              /* CAN0实例 */
#define CAN_INSTANCE_MAX_HPC        1U              /* 高压控制器最大CAN实例数 */

/* CAN引脚配置 - 根据硬件接口详细说明.md */
#define CAN_RX_PORT                 PORTE           /* CAN RX端口 */
#define CAN_RX_PIN                  4U              /* PE4 - CAN RX */
#define CAN_TX_PORT                 PORTE           /* CAN TX端口 */
#define CAN_TX_PIN                  5U              /* PE5 - CAN TX */
#define CAN_STB_PORT                PORTE           /* CAN STB端口 */
#define CAN_STB_PIN                 10U             /* PE10 - CAN STB */


/* 500Kbps配置（默认） */
#define CAN_BITRATE_500K_SEG1       34U             /* 位段1 */
#define CAN_BITRATE_500K_SEG2       11U             /* 位段2 */
#define CAN_BITRATE_500K_SJW        11U             /* 同步跳转宽度 */
#define CAN_BITRATE_500K_PRESC      4U              /* 预分频器 */

/* 高压控制器专用配置 */
#define GCU_DEBUG_MSG_ID           0x980FF16CU     /* 来自DBC的GCU调试消息ID */
#define GCU_DEBUG_MSG_CYCLE_MS     10U             /* 消息周期时间（毫秒） */
#define GCU_DEBUG_MSG_LENGTH       8U              /* 消息长度（字节） */

/* 不同频率的CAN消息配置 */
#define CAN_HIGH_FREQ_MS           5U              /* 高频消息周期：5ms */
#define CAN_LOW_FREQ_MS            50U             /* 低频消息周期：50ms */

/* ==================== CAN信号透传消息ID定义 ==================== */
/* 根据CAN信号透出实施方案添加的消息ID */
#define CAN_MSG_SENSOR_FAST_ID      0x18FF1001U  /* 快速传感器数据 */
#define CAN_MSG_SENSOR_SLOW_ID      0x18FF1002U  /* 慢速传感器数据 */
#define CAN_MSG_SYSTEM_DIAG_ID      0x18FF1003U  /* 系统诊断数据 */
#define CAN_MSG_ACTUATOR_CMD_ID     0x18FF2001U  /* 执行器控制命令 */
#define CAN_MSG_PARAM_SET_ID        0x18FF2002U  /* 参数设置命令 */
#define CAN_MSG_PC_CONTROL_CMD_ID   0x18FF2003U  /* PC端控制算法结果命令 */

/* 消息周期定义 */
#define CAN_MSG_SENSOR_FAST_PERIOD_MS   5U    /* 5ms周期 */
#define CAN_MSG_SENSOR_SLOW_PERIOD_MS   50U   /* 50ms周期 */
#define CAN_MSG_SYSTEM_DIAG_PERIOD_MS   100U  /* 100ms周期 */

/* ===========================================  Typedef  ============================================ */

/*!
 * @brief CAN消息处理器回调类型
 */
typedef void (*can_msg_handler_t)(const can_msg_info_t *msg);

/*!
 * @brief CAN接收回调类型 - 用于平台无关化
 * @param msg_id CAN消息ID
 * @param data 数据指针
 * @param length 数据长度
 */
typedef void (*can_rx_callback_t)(uint32_t msg_id, const uint8_t* data, uint8_t length);

/*!
 * @brief CAN节点统计信息
 */
typedef struct {
    uint32_t recvCount;                              /* 接收消息计数 */
    uint32_t sendCount;                              /* 发送消息计数 */
} can_cur_node_t;

/*!
 * @brief 高压控制器状态数据结构
 */
typedef struct {
    float bypass_ratio;             /* 旁通阀开度 (0-50%) */
    bool reversal_valve_st;         /* 换向阀开关状态 */
    float gas_pressure;             /* 高压缓冲罐压力 (0-51.1 MPa) */
    uint8_t reversal_valve_per_min; /* 换向阀频率 (0-200 次/分钟) */
    float gas_temperature;          /* 高压缓冲罐温度 (-80~90°C) */
    float oil_pressure;             /* 油压 (0-20 MPa) */
    float oil_temperature;          /* 油温 (-80~90°C) */
    uint8_t reserve_debug1;         /* 保留字段 */
} hpc_status_data_t;

/*!
 * @brief CAN透传传感器数据结构
 * 根据CAN信号透出实施方案定义
 */
typedef struct {
    float oil_pressure;        /* 油压 (MPa) */
    float lng_pressure;        /* LNG压力 (MPa) */
    float oil_temperature;     /* 油温 (°C) */
    float lng_temperature;     /* LNG温度 (°C) */
    bool directional_valve_state;  /* 换向阀状态 */
    uint8_t directional_valve_freq;  /* 换向阀频率 */
    float bypass_valve_duty;   /* 旁通阀开度 (%) */
    bool cooler_state;         /* 风冷器状态 */
    bool system_enabled;       /* 系统使能 */
    bool fault_active;         /* 故障激活 */
} can_sensor_data_t;

/*!
 * @brief CAN透传执行器控制结构
 */
typedef struct {
    bool directional_valve_enable;  /* 换向阀使能 */
    float bypass_valve_duty;        /* 旁通阀开度 */
    bool cooler_enable;             /* 风冷器使能 */
    bool system_enable;             /* 系统使能 */
} can_actuator_cmd_t;

/*!
 * @brief CAN配置结构体
 */
typedef struct {
    bool initialized;                   /*!< CAN初始化状态 */
    can_msg_handler_t rxHandler;        /*!< 接收消息处理器 */
    can_msg_handler_t txHandler;        /*!< 发送完成处理器 */
    uint32_t errorCount;                /*!< 错误计数器 */
    uint32_t rxCount;                   /*!< 接收消息计数器 */
    uint32_t txCount;                   /*!< 发送消息计数器 */
    hpc_status_data_t statusData;       /*!< 高压控制器状态数据 */
    uint32_t cycleCounter;              /*!< 周期性任务循环计数器 */
    uint32_t lastTxTime;                /*!< 最后发送时间 */
    uint8_t currentBitrateIndex;        /*!< 当前波特率索引 */
    uint32_t highFreqCounter;           /*!< 高频消息计数器 */
    uint32_t lowFreqCounter;            /*!< 低频消息计数器 */
} can_app_config_t;

/* ====================================  Functions declaration  ===================================== */

/*!
 * @brief 使用500Kbps配置初始化CAN模块
 * @retval true: 成功, false: 失败
 */
bool CAN_Config_Init(void);

/*!
 * @brief 反初始化CAN模块
 * @retval true: 成功, false: 失败
 */
bool CAN_Config_Deinit(void);

/*!
 * @brief 发送CAN消息
 * @param[in] id: CAN标识符
 * @param[in] data: 消息数据
 * @param[in] length: 数据长度 (0-8字节)
 * @param[in] isExtended: true为扩展ID, false为标准ID
 * @retval true: 成功, false: 失败
 */
bool CAN_Config_SendMessage(uint32_t id, const uint8_t *data, uint8_t length, bool isExtended);

/*!
 * @brief 接收CAN消息（非阻塞）
 * @param[out] msg: 接收到的消息信息
 * @retval true: 接收到消息, false: 无消息
 */
bool CAN_Config_ReceiveMessage(const can_msg_info_t *msg);

/*!
 * @brief 注册CAN接收回调函数 - 用于平台无关化
 * @param[in] callback: 回调函数指针
 */
void CAN_Config_RegisterRxCallback(can_rx_callback_t callback);

/*!
 * @brief 设置CAN待机模式
 * @param[in] enable: true启用待机, false禁用待机
 * @retval true: 成功, false: 失败
 */
bool CAN_Config_SetStandby(bool enable);

/*!
 * @brief 安装接收消息处理器
 * @param[in] handler: 消息处理器回调
 */
void CAN_Config_InstallRxHandler(can_msg_handler_t handler);

/*!
 * @brief 安装发送完成处理器
 * @param[in] handler: 发送完成回调
 */
void CAN_Config_InstallTxHandler(can_msg_handler_t handler);

/*!
 * @brief 获取CAN统计信息
 * @param[out] rxCount: 接收消息计数
 * @param[out] txCount: 发送消息计数
 * @param[out] errorCount: 错误计数
 */
void CAN_Config_GetStats(uint32_t *rxCount, uint32_t *txCount, uint32_t *errorCount);

/*!
 * @brief CAN周期性任务（应在主循环中调用）
 */
void CAN_Config_Task(void);

/*!
 * @brief 检查CAN收发器状态和STB引脚状态
 * @param[out] stbPinState: STB引脚状态 (true=高电平/待机, false=低电平/正常)
 * @param[out] canControllerStandby: CAN控制器待机状态
 * @param[out] canBusIdle: CAN总线空闲状态
 * @retval true: 检查成功, false: CAN未初始化
 */
bool CAN_Config_CheckTransceiverStatus(bool *stbPinState, bool *canControllerStandby, bool *canBusIdle);

/*!
 * @brief 重置CAN控制器（用于错误恢复）
 * @retval true: 成功, false: 失败
 */
bool CAN_Config_ResetController(void);

/* ====================================  高压控制器专用函数  ===================================== */

/*!
 * @brief 更新高压控制器状态数据
 * @param[in] statusData: 新的状态数据
 */
void CAN_Config_UpdateHPCStatus(const hpc_status_data_t *statusData);

/*!
 * @brief 获取当前高压控制器状态数据
 * @param[out] statusData: 当前状态数据
 */
void CAN_Config_GetHPCStatus(hpc_status_data_t *statusData);

/*!
 * @brief 发送高压控制器状态消息（使用DBC格式）
 * @retval true: 成功, false: 失败
 */
bool CAN_Config_SendHPCStatus(void);

/*!
 * @brief 发送初始CAN状态消息
 * 在系统初始化完成后立即调用，不等待任务调度
 */
void CAN_Config_SendInitialStatus(void);

/*!
 * @brief CAN消息发送任务 - 5ms周期
 * 提高CAN消息响应速度，解决时序冲突问题
 */
void Task_CAN_5ms(void);

/*!
 * @brief 高频CAN消息发送任务 - 5ms周期
 * 传输油压、换向阀开关信号、换向阀频率
 */
void Task_CAN_HighFreq_5ms(void);

/*!
 * @brief 低频CAN消息发送任务 - 50ms周期
 * 传输LNG压力、油温、LNG温度、旁通阀信号
 */
void Task_CAN_LowFreq_50ms(void);

/*!
 * @brief 处理接收到的CAN消息（DBC解码）
 * @param[in] msg: 接收到的CAN消息
 */
void CAN_Config_ProcessReceivedMessage(const can_msg_info_t *msg);

/* ====================================  高级CAN功能  ===================================== */

/*!
 * @brief 获取指定波特率的配置参数
 * @param[in] index: 波特率索引 (0-6)
 * @return 波特率配置结构体
 */
can_time_segment_t CAN_Config_GetBitrate(uint8_t index);

/*!
 * @brief 动态切换CAN波特率
 * @param[in] bitrateIndex: 新的波特率索引
 * @retval true: 成功, false: 失败
 */
bool CAN_Config_ChangeBitrate(uint8_t bitrateIndex);

/*!
 * @brief 获取当前波特率索引
 * @return 当前波特率索引
 */
uint8_t CAN_Config_GetCurrentBitrateIndex(void);

/*!
 * @brief 配置CAN过滤器
 * @param[in] filterIndex: 过滤器索引
 * @param[in] code: 过滤器代码
 * @param[in] mask: 过滤器掩码
 * @param[in] idType: ID类型
 * @retval true: 成功, false: 失败
 */
bool CAN_Config_SetFilter(uint8_t filterIndex, uint32_t code, uint32_t mask, uint8_t idType);

/*!
 * @brief 测试CAN发送功能
 * @param[in] instance: CAN实例
 * @param[in] type: 发送缓冲区类型
 * @retval 0: 发送成功, 1: 发送失败
 */
int32_t CAN_Config_SendTest(uint8_t instance, uint8_t type);

/*!
 * @brief 获取CAN节点统计信息
 * @param[in] instance: CAN实例
 * @param[out] nodeInfo: 节点统计信息
 * @retval true: 成功, false: 失败
 */
bool CAN_Config_GetNodeStats(uint8_t instance, can_cur_node_t *nodeInfo);

/*!
 * @brief 重置CAN统计信息
 * @param[in] instance: CAN实例
 */
void CAN_Config_ResetStats(uint8_t instance);

/*!
 * @brief 设置CAN时间戳功能
 * @param[in] enable: 是否启用时间戳
 * @param[in] clockSrc: 时间戳时钟源
 * @param[in] clockDiv: 时间戳时钟分频
 * @retval true: 成功, false: 失败
 */
bool CAN_Config_SetTimeStamp(bool enable, uint8_t clockSrc, uint8_t clockDiv);

/*!
 * @brief 获取CAN错误状态
 * @param[out] errorWarningLimit: 错误警告限制
 * @param[out] rxErrorCount: 接收错误计数
 * @param[out] txErrorCount: 发送错误计数
 * @retval true: 成功, false: 失败
 */
bool CAN_Config_GetErrorStatus(uint8_t *errorWarningLimit, uint8_t *rxErrorCount, uint8_t *txErrorCount);

/* ==================== CAN信号透传函数声明 ==================== */
/* 根据CAN信号透出实施方案添加的透传函数 */

/*!
 * @brief 初始化CAN透传模块
 */
void CANTransparent_Init(void);

/*!
 * @brief 发送快速传感器数据（5ms周期）
 * @param[in] data 传感器数据
 */
void CANTransparent_SendSensorFast(const can_sensor_data_t* data);

/*!
 * @brief 发送慢速传感器数据（50ms周期）
 * @param[in] data 传感器数据
 */
void CANTransparent_SendSensorSlow(const can_sensor_data_t* data);

/*!
 * @brief 发送系统诊断数据（100ms周期）
 */
void CANTransparent_SendSystemDiag(void);

/*!
 * @brief 处理执行器控制命令
 * @param[in] data 命令数据
 * @param[in] length 数据长度
 */
void CANTransparent_ProcessActuatorCmd(const uint8_t* data, uint8_t length);

/*!
 * @brief 处理参数设置命令
 * @param[in] data 参数数据
 * @param[in] length 数据长度
 */
void CANTransparent_ProcessParamSet(const uint8_t* data, uint8_t length);

/*!
 * @brief 处理PC端控制算法结果命令
 * @param[in] data 控制命令数据
 * @param[in] length 数据长度
 */
void CANTransparent_ProcessPCControlCmd(const uint8_t* data, uint8_t length);

/*!
 * @brief CAN透传5ms周期任务
 */
void CANTransparent_Task_5ms(void);

/*!
 * @brief CAN透传50ms周期任务
 */
void CANTransparent_Task_50ms(void);

/*!
 * @brief CAN透传100ms周期任务
 */
void CANTransparent_Task_100ms(void);

#endif /* __CAN_CONFIG_H */

/* =============================================  EOF  ============================================== */
