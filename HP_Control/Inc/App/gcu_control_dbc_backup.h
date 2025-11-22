/**
 * @file gcu_control_dbc.h
 * @brief Auto-generated from gcu_control.dbc
 * @note This file was automatically generated. Do not modify manually.
 */

#ifndef GCU_CONTROL_DBC_H
#define GCU_CONTROL_DBC_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef EINVAL
#    define EINVAL 22
#endif

/* ========================================================================
 * CAN Message IDs
 * ======================================================================== */
#define CAN_MSG_GCU_DEBUG1_ID       0x98000E6CU  /* 2551181676 */
#define CAN_MSG_GCU_CONTROL_ID      0x98000000U  /* 2550661376 */

/* Message cycle times (ms) */
#define CAN_MSG_GCU_DEBUG1_CYCLE    10U
#define CAN_MSG_GCU_CONTROL_CYCLE   100U

/* ========================================================================
 * Signal Structures
 * ======================================================================== */

/**
 * @brief gcu_debug1 message signals (GCU -> TSMaster)
 * @note Message ID: 0x98000E6C, DLC: 8, Cycle: 10ms
 * All signal values are as on the CAN bus (raw values).
 */
typedef struct {
    /**
     * 旁通阀开度比例
     * Range: 0..50 (0..50 %)
     * Scale: 1
     * Offset: 0
     */
    uint8_t bypass_ratio;
    
    /**
     * 换向阀开关状态
     * Range: 0..1 (0..1 -)
     * Scale: 1
     * Offset: 0
     */
    uint8_t reversal_valve_st;
    
    /**
     * 气压缓冲罐压力
     * Range: 0..511 (0..40 MPa)
     * Scale: 0.1
     * Offset: 0
     */
    uint16_t LNG_pressure;
    
    /**
     * 换向阀频率
     * Range: 0..127 (0..100 Hz)
     * Scale: 1
     * Offset: 0
     */
    uint8_t reversal_valve_hz;
    
    /**
     * 气压缓冲罐温度
     * Range: 0..4095 (-80..90 °C)
     * Scale: 0.1
     * Offset: -80
     */
    uint16_t LNG_temperature;
    
    /**
     * 油压力
     * Range: 0..255 (0..20 MPa)
     * Scale: 0.1
     * Offset: 0
     */
    uint8_t oil_pressure;
    
    /**
     * 油温
     * Range: 0..4095 (-80..90 °C)
     * Scale: 0.1
     * Offset: -80
     */
    uint16_t oil_temperature;
    
    /**
     * 预留
     * Range: 0..255 (0..255 -)
     * Scale: 1
     * Offset: 0
     */
    uint8_t reserve_debug1;
} gcu_debug1_t;

/**
 * @brief gcu_control message signals (TSMaster -> GCU)
 * @note Message ID: 0x18100000, DLC: 8, Cycle: 100ms
 * All signal values are as on the CAN bus (raw values).
 */
typedef struct {
    /**
     * 换向阀使能控制
     * Range: 0..1 (0=OFF, 1=ON)
     * Scale: 1
     * Offset: 0
     */
    uint8_t ctrl_reversal_valve_enable;
    
    /**
     * 旁通阀占空比控制
     * Range: 0..1000 (0..100 %)
     * Scale: 0.1
     * Offset: 0
     */
    uint16_t ctrl_bypass_valve_duty;
    
    /**
     * 换向阀频率控制
     * Range: 0..100 (0..100 Hz)
     * Scale: 1
     * Offset: 0
     */
    uint8_t ctrl_reversal_valve_freq;
    
    /**
     * 冷却器使能控制
     * Range: 0..1 (0=OFF, 1=ON)
     * Scale: 1
     * Offset: 0
     */
    uint8_t ctrl_cooler_enable;
    
    /**
     * 系统使能控制
     * Range: 0..1 (0=Disable, 1=Enable)
     * Scale: 1
     * Offset: 0
     */
    uint8_t ctrl_system_enable;
    
    /**
     * 保留字段
     * Range: 0..1073741823 (30 bits)
     * Scale: 1
     * Offset: 0
     */
    uint32_t ctrl_reserved;
} gcu_control_t;

/* ========================================================================
 * Signal Value Definitions
 * ======================================================================== */

/* ctrl_reversal_valve_enable */
#define CTRL_REVERSAL_VALVE_OFF     0U
#define CTRL_REVERSAL_VALVE_ON      1U

/* ctrl_cooler_enable */
#define CTRL_COOLER_OFF             0U
#define CTRL_COOLER_ON              1U

/* ctrl_system_enable */
#define CTRL_SYSTEM_DISABLE         0U
#define CTRL_SYSTEM_ENABLE          1U

/* ========================================================================
 * Function Prototypes
 * ======================================================================== */

/* ========================================================================
 * Initialization Functions
 * ======================================================================== */

/**
 * @brief Initialize gcu_debug1 structure with default values
 * @param[in] msg_p Message to init
 * @return zero(0) on success or (-1) in case of nullptr argument
 */
int gcu_debug1_init(gcu_debug1_t *msg_p);

/**
 * @brief Initialize gcu_control structure with default values
 * @param[in] msg_p Message to init
 * @return zero(0) on success or (-1) in case of nullptr argument
 */
int gcu_control_init(gcu_control_t *msg_p);

/* ========================================================================
 * Pack/Unpack Functions
 * ======================================================================== */

/**
 * @brief Pack gcu_debug1 message
 * @param[out] dst_p Buffer to pack the message into
 * @param[in] src_p Data to pack
 * @param[in] size Size of dst_p
 * @return Size of packed data, or negative error code
 */
int gcu_debug1_pack(
    uint8_t *dst_p,
    const gcu_debug1_t *src_p,
    size_t size);

/**
 * @brief Unpack gcu_debug1 message
 * @param[out] dst_p Object to unpack the message into
 * @param[in] src_p Message to unpack
 * @param[in] size Size of src_p
 * @return zero(0) or negative error code
 */
int gcu_debug1_unpack(
    gcu_debug1_t *dst_p,
    const uint8_t *src_p,
    size_t size);

/**
 * @brief Pack gcu_control message
 * @param[out] dst_p Buffer to pack the message into
 * @param[in] src_p Data to pack
 * @param[in] size Size of dst_p
 * @return Size of packed data, or negative error code
 */
int gcu_control_pack(
    uint8_t *dst_p,
    const gcu_control_t *src_p,
    size_t size);

/**
 * @brief Unpack gcu_control message
 * @param[out] dst_p Object to unpack the message into
 * @param[in] src_p Message to unpack
 * @param[in] size Size of src_p
 * @return zero(0) or negative error code
 */
int gcu_control_unpack(
    gcu_control_t *dst_p,
    const uint8_t *src_p,
    size_t size);

/* ========================================================================
 * gcu_debug1 Encode/Decode Functions
 * ======================================================================== */

uint8_t gcu_debug1_bypass_ratio_encode(double value);
double gcu_debug1_bypass_ratio_decode(uint8_t value);
bool gcu_debug1_bypass_ratio_is_in_range(uint8_t value);

uint8_t gcu_debug1_reversal_valve_st_encode(double value);
double gcu_debug1_reversal_valve_st_decode(uint8_t value);
bool gcu_debug1_reversal_valve_st_is_in_range(uint8_t value);

uint16_t gcu_debug1_LNG_pressure_encode(double value);
double gcu_debug1_LNG_pressure_decode(uint16_t value);
bool gcu_debug1_LNG_pressure_is_in_range(uint16_t value);

uint8_t gcu_debug1_reversal_valve_hz_encode(double value);
double gcu_debug1_reversal_valve_hz_decode(uint8_t value);
bool gcu_debug1_reversal_valve_hz_is_in_range(uint8_t value);

uint16_t gcu_debug1_LNG_temperature_encode(double value);
double gcu_debug1_LNG_temperature_decode(uint16_t value);
bool gcu_debug1_LNG_temperature_is_in_range(uint16_t value);

uint8_t gcu_debug1_oil_pressure_encode(double value);
double gcu_debug1_oil_pressure_decode(uint8_t value);
bool gcu_debug1_oil_pressure_is_in_range(uint8_t value);

uint16_t gcu_debug1_oil_temperature_encode(double value);
double gcu_debug1_oil_temperature_decode(uint16_t value);
bool gcu_debug1_oil_temperature_is_in_range(uint16_t value);

uint8_t gcu_debug1_reserve_debug1_encode(double value);
double gcu_debug1_reserve_debug1_decode(uint8_t value);
bool gcu_debug1_reserve_debug1_is_in_range(uint8_t value);

/* ========================================================================
 * gcu_control Encode/Decode Functions
 * ======================================================================== */

uint8_t gcu_control_ctrl_reversal_valve_enable_encode(double value);
double gcu_control_ctrl_reversal_valve_enable_decode(uint8_t value);
bool gcu_control_ctrl_reversal_valve_enable_is_in_range(uint8_t value);

uint16_t gcu_control_ctrl_bypass_valve_duty_encode(double value);
double gcu_control_ctrl_bypass_valve_duty_decode(uint16_t value);
bool gcu_control_ctrl_bypass_valve_duty_is_in_range(uint16_t value);

uint8_t gcu_control_ctrl_reversal_valve_freq_encode(double value);
double gcu_control_ctrl_reversal_valve_freq_decode(uint8_t value);
bool gcu_control_ctrl_reversal_valve_freq_is_in_range(uint8_t value);

uint8_t gcu_control_ctrl_cooler_enable_encode(double value);
double gcu_control_ctrl_cooler_enable_decode(uint8_t value);
bool gcu_control_ctrl_cooler_enable_is_in_range(uint8_t value);

uint8_t gcu_control_ctrl_system_enable_encode(double value);
double gcu_control_ctrl_system_enable_decode(uint8_t value);
bool gcu_control_ctrl_system_enable_is_in_range(uint8_t value);

uint32_t gcu_control_ctrl_reserved_encode(double value);
double gcu_control_ctrl_reserved_decode(uint32_t value);
bool gcu_control_ctrl_reserved_is_in_range(uint32_t value);

#ifdef __cplusplus
}
#endif

#endif /* GCU_CONTROL_DBC_H */

