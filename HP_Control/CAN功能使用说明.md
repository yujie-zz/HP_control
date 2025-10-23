# 高压项目CAN功能使用说明

## 概述

本文档详细说明了高压项目代码中CAN功能的使用方法。该CAN功能基于bldc项目的成功实现，并针对高压控制器的特定需求进行了优化，集成了gcu.dbc文件支持。

## 主要特性

- ✅ **250Kbps波特率** - 标准CAN通信（默认配置）
- ✅ **PE4/PE5引脚** - 标准CAN接口配置
- ✅ **PE10备用通道** - 配置为ALT5功能
- ✅ **DBC文件集成** - 支持gcu.dbc消息格式
- ✅ **高压控制器专用** - 针对旁通阀、换向阀等设备优化
- ✅ **自动周期性发送** - 5ms周期发送状态信息（提高响应速度）
- ✅ **任务调度集成** - 已集成到任务调度器中
- ✅ **启动开关集成** - 支持启动开关使能控制
- ✅ **时序冲突解决** - 5ms周期避免与阀门控制冲突
- ✅ **独立CAN传输** - CAN信号传输不受启动开关状态影响

## 硬件配置

### 引脚定义
```c
#define CAN_RX_PIN      4U    // PE4 - CAN接收
#define CAN_TX_PIN      5U    // PE5 - CAN发送  
#define CAN_STB_PIN     10U   // PE10 - 备用通道(ALT5)
```

### 连接方式
- PE4 → CAN收发器RX
- PE5 → CAN收发器TX
- PE10 → 备用通道(ALT5功能)

## 软件集成

### 1. 头文件包含
```c
#include "can_config.h"
#include "gcu.h"  // DBC生成的头文件
```

### 2. 初始化CAN模块
```c
int main(void)
{
    // 系统初始化...
    
    // 初始化CAN模块
    if (!CAN_Config_Init())
    {
        printf("CAN初始化失败!\n");
        return -1;
    }
    
    // 主循环...
}
```

### 3. 在主循环中调用任务函数
```c
// 在5ms任务中自动调用
void Task_CAN_5ms(void)
{
    // 获取传感器和控制系统数据
    hpc_status_data_t status;
    status.oil_pressure = Sensor_GetOilPressure();
    status.gas_pressure = Sensor_GetLNGPressure();
    status.oil_temperature = Sensor_GetOilTemperature();
    status.gas_temperature = Sensor_GetLNGTemperature();
    status.bypass_ratio = ValveControl_GetBypassValveDuty();
    status.reversal_valve_hz = (uint8_t)HydraulicControl_GetReversingFrequency();
    status.reversal_valve_st = (ValveControl_GetDirectionalValveState() == VALVE_STATE_ON);
    status.reserve_debug1 = 0x00;
    
    // 更新CAN层状态并发送
    CAN_Config_UpdateHPCStatus(&status);
    CAN_Config_SendHPCStatus();
}
```

## 数据结构和API

### 高压控制器状态数据结构
```c
typedef struct {
    float bypass_ratio;             // 旁通阀开度 (0-127%)
    bool reversal_valve_st;         // 换向阀开关状态
    float gas_pressure;             // 高压缓冲罐压力 (0-51.1 MPa)
    uint8_t reversal_valve_hz;      // 换向阀频率 (0-127 Hz)
    float gas_temperature;          // 高压缓冲罐温度 (-80~329.5°C)
    float oil_pressure;             // 油压 (0-25.5 MPa)
    float oil_temperature;          // 油温 (-80~329.5°C)
    uint8_t reserve_debug1;         // 保留字段
} hpc_status_data_t;
```

### 主要API函数

#### 状态数据管理
```c
// 更新高压控制器状态数据
void CAN_Config_UpdateHPCStatus(const hpc_status_data_t *statusData);

// 获取当前高压控制器状态数据
void CAN_Config_GetHPCStatus(hpc_status_data_t *statusData);
```

#### 消息发送
```c
// 手动发送高压控制器状态消息
bool CAN_Config_SendHPCStatus();

// 发送自定义CAN消息
bool CAN_Config_SendMessage(uint32_t id, const uint8_t *data, uint8_t length, bool isExtended);

// 发送初始状态消息（系统启动时）
void CAN_Config_SendInitialStatus(void);
```

#### 消息接收处理
```c
// 处理接收到的CAN消息（DBC解码）
void CAN_Config_ProcessReceivedMessage(const can_msg_info_t *msg);

// 安装接收消息处理器
void CAN_Config_InstallRxHandler(can_msg_handler_t handler);
```

## 使用示例

### 示例1: 更新并发送状态数据
```c
// 更新状态数据
hpc_status_data_t statusData = {
    .bypass_ratio = 50.0f,           // 50%开度
    .reversal_valve_st = true,       // 换向阀开启
    .gas_pressure = 25.5f,           // 25.5 MPa
    .reversal_valve_hz = 60,         // 60 Hz
    .gas_temperature = 25.0f,        // 25°C
    .oil_pressure = 15.0f,           // 15 MPa
    .oil_temperature = 80.0f,        // 80°C
    .reserve_debug1 = 0
};

// 更新CAN模块中的状态数据
CAN_Config_UpdateHPCStatus(&statusData);

// 手动发送状态消息（可选，通常由周期性任务自动发送）
CAN_Config_SendHPCStatus();
```

### 示例2: 安装消息接收处理器
```c
// 定义消息接收处理函数
void my_can_rx_handler(const can_msg_info_t *msg)
{
    // 处理接收到的消息
    CAN_Config_ProcessReceivedMessage(msg);
}

// 安装接收处理器
CAN_Config_InstallRxHandler(my_can_rx_handler);
```

### 示例3: 获取CAN统计信息
```c
uint32_t rxCount, txCount, errorCount;
CAN_Config_GetStats(&rxCount, &txCount, &errorCount);

printf("CAN统计: 接收=%lu, 发送=%lu, 错误=%lu\n", 
       rxCount, txCount, errorCount);
```

## 消息格式

### GCU Debug消息 (ID: 0x180FF16C)
根据gcu.dbc文件定义，该消息包含以下信号：

| 信号名称 | 位置 | 长度 | 范围 | 单位 | 说明 |
|---------|------|------|------|------|------|
| bypass_ratio | 0:7 | 7位 | 0-127 | % | 旁通阀开度 |
| reversal_valve_st | 7:7 | 1位 | 0-1 | - | 换向阀状态 |
| LNG_pressure | 8:16 | 9位 | 0-511 | 0.1MPa | 气压(0-51.1MPa) |
| reversal_valve_hz | 17:23 | 7位 | 0-127 | Hz | 换向阀频率(0-80Hz) |
| LNG_temperature | 24:35 | 12位 | 0-4095 | 0.1°C | 气压温度(-80~90°C) |
| oil_pressure | 36:43 | 8位 | 0-255 | MPa | 油压(0-20MPa) |
| oil_temperature | 44:55 | 12位 | 0-4095 | 0.1°C | 油温(-80~90°C) |
| reserve_debug1 | 56:63 | 8位 | 0-255 | - | 保留字段 |

## 配置参数

### CAN通信参数
```c
// 250Kbps配置 (默认使用index 3)
#define CAN_BITRATE_250K_SEG1       34U     // 位段1
#define CAN_BITRATE_250K_SEG2       11U     // 位段2  
#define CAN_BITRATE_250K_SJW        11U     // 同步跳转宽度
#define CAN_BITRATE_250K_PRESC      4U      // 预分频器
```

### 消息发送参数
```c
#define GCU_DEBUG_MSG_CYCLE_MS     100U    // 消息周期 (100ms)
#define GCU_DEBUG_MSG_LENGTH       8U      // 消息长度 (8字节)
```

## 任务调度集成

### 5ms周期优势
- **提高响应速度**: 从100ms提升到5ms，CAN消息更新频率提高20倍
- **解决时序冲突**: 避免与5ms传感器采集任务的时序冲突
- **实时性更好**: 旁通阀和换向阀状态变化能更快反映到CAN消息中
- **数据一致性**: 与传感器数据采集同步，确保状态数据的一致性
- **独立传输**: CAN信号传输不受启动开关状态影响，确保监控系统始终能接收到状态信息

### 自动任务调用
CAN功能已集成到任务调度器中，无需手动调用：

```c
// 在5ms任务中自动执行
void Task1_5ms(void)
{
    // 传感器数据采集
    Monitor_UpdateAll();
    
    /* 处理CAN通信任务 - 5ms周期，提高响应速度 */
    extern void Task_CAN_5ms(void);
    Task_CAN_5ms();
}
```

### 任务频率
- **CAN状态发送**: 5ms周期
- **CAN消息处理**: 5ms周期
- **传感器数据更新**: 5ms周期
- **换向阀控制**: 50ms周期
- **系统状态监控**: 1000ms周期

## 系统集成

### 启动流程集成
```c
// 在main.c的SystemInit()函数中
static void SystemInit(void)
{
    // ... 其他初始化 ...
    
    // 初始化CAN通信模块
    CAN_Config_Init();
    
    // 立即发送第一条CAN状态消息，不等待任务调度
    CAN_Config_SendInitialStatus();
}
```

### 传感器数据集成
```c
// 在Task_CAN_5ms()中获取实时传感器数据
status.oil_pressure = Sensor_GetOilPressure();
status.gas_pressure = Sensor_GetLNGPressure();
status.oil_temperature = Sensor_GetOilTemperature();
status.gas_temperature = Sensor_GetLNGTemperature();
```

### 控制状态集成
```c
// 获取阀门控制状态
status.bypass_ratio = ValveControl_GetBypassValveDuty();
status.reversal_valve_st = (ValveControl_GetDirectionalValveState() == VALVE_STATE_ON);
status.reversal_valve_hz = (uint8_t)HydraulicControl_GetReversingFrequency();
```

### 启动开关集成
```c
// CAN功能不受启动开关控制，始终发送信号
extern bool g_systemEnabled;

void Task_CAN_5ms(void)
{
    // CAN消息始终发送，不受启动开关状态影响
    // 但阀门状态会根据启动开关状态调整：
    // - 系统使能时：发送实际阀门状态
    // - 系统未使能时：发送安全状态（阀门关闭）
    
    // ... CAN消息发送逻辑 ...
}
```

## 注意事项

1. **初始化顺序**: 确保在调用CAN功能前先初始化系统时钟和GPIO
2. **自动任务**: CAN功能已集成到任务调度器，无需手动调用`CAN_Config_Task()`
3. **DBC文件**: 确保gcu.h和gcu.c文件已正确生成并包含在项目中
4. **引脚复用**: PE4/PE5已配置为CAN功能，PE10为备用通道
5. **错误处理**: 检查所有CAN函数的返回值，确保操作成功
6. **波特率**: 当前默认使用250Kbps，可通过修改`s_canBitrate`数组索引调整
7. **系统使能**: 启动开关激活时系统控制功能工作，但CAN信号始终传输
8. **任务周期**: 使用5ms周期（`Task_CAN_5ms()`），提高响应速度，避免时序冲突
9. **独立CAN传输**: CAN信号传输不受启动开关状态影响，确保监控系统始终能接收到状态信息

## 故障排除

### 常见问题

1. **CAN初始化失败**
   - 检查时钟配置
   - 验证GPIO引脚配置
   - 确认CAN驱动是否正确加载

2. **消息发送失败**
   - 检查CAN总线是否空闲
   - 验证消息ID和长度
   - 确认收发器连接状态

3. **接收不到消息**
   - 检查过滤器配置
   - 验证中断是否使能
   - 确认消息ID匹配

4. **DBC解码错误**
   - 检查gcu.h文件是否正确包含
   - 验证消息格式是否匹配
   - 确认数据长度正确

5. **任务调度问题**
   - 确认任务调度器已正确初始化
   - 检查5ms任务是否正常执行
   - 验证CAN任务函数是否被调用

6. **系统启动问题**
   - 检查启动开关状态
   - 确认系统使能标志
   - 验证初始化顺序

7. **启动开关集成问题**
   - 检查PC17引脚配置
   - 确认启动开关逻辑（低电平激活）
   - 验证系统使能状态传递

## 版本历史

- **v1.0** - 初始版本，基于bldc项目CAN功能
- **v1.1** - 添加DBC文件集成支持
- **v1.2** - 优化高压控制器特定功能
- **v1.3** - 完善错误处理和调试信息
- **v1.4** - 集成任务调度器，实现自动100ms周期发送
- **v1.5** - 集成系统启动控制，支持启动开关使能
- **v1.6** - 完善任务调度集成，支持多频率任务执行
- **v1.7** - 优化为5ms周期，解决时序冲突，提高响应速度
- **v1.8** - CAN信号传输独立化，不受启动开关状态影响 