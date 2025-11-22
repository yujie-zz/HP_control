/**
 * @file gcu_control_dbc.c
 * @brief Auto-generated from gcu_hp_control_04.dbc
 * @note This file was automatically generated. Do not modify manually.
 * @date 2025-01-27
 */

#include "gcu_control_dbc.h"
#include <string.h>

/* ========================================================================
 * Bit Manipulation Helper Functions
 * ======================================================================== */

static inline uint8_t pack_left_shift_u8(
    uint8_t value,
    uint8_t shift,
    uint8_t mask)
{
    return (uint8_t)((uint8_t)(value << shift) & mask);
}

static inline uint8_t pack_left_shift_u16(
    uint16_t value,
    uint8_t shift,
    uint8_t mask)
{
    return (uint8_t)((uint8_t)(value << shift) & mask);
}

static inline uint8_t pack_right_shift_u8(
    uint8_t value,
    uint8_t shift,
    uint8_t mask)
{
    return (uint8_t)((uint8_t)(value >> shift) & mask);
}

static inline uint8_t pack_right_shift_u16(
    uint16_t value,
    uint8_t shift,
    uint8_t mask)
{
    return (uint8_t)((uint8_t)(value >> shift) & mask);
}

static inline uint8_t unpack_left_shift_u8(
    uint8_t value,
    uint8_t shift,
    uint8_t mask)
{
    return (uint8_t)((uint8_t)(value & mask) << shift);
}

static inline uint16_t unpack_left_shift_u16(
    uint8_t value,
    uint8_t shift,
    uint8_t mask)
{
    return (uint16_t)((uint16_t)(value & mask) << shift);
}

static inline uint8_t unpack_right_shift_u8(
    uint8_t value,
    uint8_t shift,
    uint8_t mask)
{
    return (uint8_t)((uint8_t)(value & mask) >> shift);
}

static inline uint16_t unpack_right_shift_u16(
    uint8_t value,
    uint8_t shift,
    uint8_t mask)
{
    return (uint16_t)((uint16_t)(value & mask) >> shift);
}

/* ========================================================================
 * Initialization Functions
 * ======================================================================== */

int device_status_init(device_status_t *msg_p)
{
    if (msg_p == NULL) {
        return -1;
    }
    
    memset(msg_p, 0, sizeof(device_status_t));
    
    return 0;
}

int tsmaster_control_init(tsmaster_control_t *msg_p)
{
    if (msg_p == NULL) {
        return -1;
    }
    
    memset(msg_p, 0, sizeof(tsmaster_control_t));
    
    return 0;
}

int tsmaster_control2_init(tsmaster_control2_t *msg_p)
{
    if (msg_p == NULL) {
        return -1;
    }
    
    memset(msg_p, 0, sizeof(tsmaster_control2_t));
    
    return 0;
}

/* ========================================================================
 * device_status Message Pack/Unpack (ID: 0x980FF16C)
 * ======================================================================== */

int device_status_pack(
    uint8_t *dst_p,
    const device_status_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-EINVAL);
    }
    
    if (dst_p == NULL || src_p == NULL) {
        return (-EINVAL);
    }
    
    memset(&dst_p[0], 0, 8);
    
    /* bypass_ratio: bit 0, 8 bits */
    dst_p[0] |= pack_left_shift_u8(src_p->bypass_ratio, 0u, 0xffu);
    
    /* oil_pressure: bit 8, 8 bits */
    dst_p[1] |= pack_left_shift_u8(src_p->oil_pressure, 0u, 0xffu);
    
    /* oil_temperature: bit 16, 8 bits */
    dst_p[2] |= pack_left_shift_u8(src_p->oil_temperature, 0u, 0xffu);
    
    /* LNG_temperature: bit 24, 8 bits */
    dst_p[3] |= pack_left_shift_u8(src_p->LNG_temperature, 0u, 0xffu);
    
    /* LNG_pressure: bit 32, 8 bits */
    dst_p[4] |= pack_left_shift_u8(src_p->LNG_pressure, 0u, 0xffu);
    
    /* rev_freq: bit 40, 10 bits */
    dst_p[5] |= pack_left_shift_u16(src_p->rev_freq, 0u, 0xffu);
    dst_p[6] |= pack_right_shift_u16(src_p->rev_freq, 8u, 0x03u);
    
    /* rev_enable: bit 50, 1 bit */
    dst_p[6] |= pack_left_shift_u8(src_p->rev_enable, 2u, 0x04u);
    
    /* cooler_enable: bit 51, 1 bit */
    dst_p[6] |= pack_left_shift_u8(src_p->cooler_enable, 3u, 0x08u);
    
    /* bit 52-63 (剩余12位) 未使用，填充为0 */
    // 字节7的bit 4-7 保持为0
    dst_p[7] = 0;
    
    return (8);
}

int device_status_unpack(
    device_status_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-EINVAL);
    }
    
    if (dst_p == NULL || src_p == NULL) {
        return (-EINVAL);
    }
    
    /* bypass_ratio: bit 0, 8 bits */
    dst_p->bypass_ratio = unpack_right_shift_u8(src_p[0], 0u, 0xffu);
    
    /* oil_pressure: bit 8, 8 bits */
    dst_p->oil_pressure = unpack_right_shift_u8(src_p[1], 0u, 0xffu);
    
    /* oil_temperature: bit 16, 8 bits */
    dst_p->oil_temperature = unpack_right_shift_u8(src_p[2], 0u, 0xffu);
    
    /* LNG_temperature: bit 24, 8 bits */
    dst_p->LNG_temperature = unpack_right_shift_u8(src_p[3], 0u, 0xffu);
    
    /* LNG_pressure: bit 32, 8 bits */
    dst_p->LNG_pressure = unpack_right_shift_u8(src_p[4], 0u, 0xffu);
    
    /* rev_freq: bit 40, 10 bits */
    dst_p->rev_freq = unpack_right_shift_u16(src_p[5], 0u, 0xffu);
    dst_p->rev_freq |= unpack_left_shift_u16(src_p[6], 8u, 0x03u);
    
    /* rev_enable: bit 50, 1 bit */
    dst_p->rev_enable = unpack_right_shift_u8(src_p[6], 2u, 0x04u);
    
    /* cooler_enable: bit 51, 1 bit */
    dst_p->cooler_enable = unpack_right_shift_u8(src_p[6], 3u, 0x08u);
    
    /* bit 52-63 (剩余12位) 未使用，忽略 */
    
    return (0);
}

/* ========================================================================
 * tsmaster_control Message Pack/Unpack (ID: 0x98080100)
 * ======================================================================== */

int tsmaster_control_pack(
    uint8_t *dst_p,
    const tsmaster_control_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-EINVAL);
    }
    
    if (dst_p == NULL || src_p == NULL) {
        return (-EINVAL);
    }
    
    memset(&dst_p[0], 0, 8);
    
    /* set_bypass_initial_decline_time: bit 0, 10 bits */
    dst_p[0] |= pack_left_shift_u16(src_p->set_bypass_initial_decline_time, 0u, 0xffu);
    dst_p[1] |= pack_right_shift_u16(src_p->set_bypass_initial_decline_time, 8u, 0x03u);
    
    /* set_rev_start_oilP_max: bit 10, 7 bits */
    dst_p[1] |= pack_left_shift_u8(src_p->set_rev_start_oilP_max, 2u, 0xfcu);
    dst_p[2] |= pack_right_shift_u8(src_p->set_rev_start_oilP_max, 6u, 0x01u);
    
    /* set_rev_start_oilP_min: bit 17, 7 bits */
    dst_p[2] |= pack_left_shift_u8(src_p->set_rev_start_oilP_min, 1u, 0xfeu);
    dst_p[3] |= pack_right_shift_u8(src_p->set_rev_start_oilP_min, 7u, 0x01u);
    
    /* set_first_fix_freq_time_on: bit 24, 10 bits */
    dst_p[3] |= pack_left_shift_u16(src_p->set_first_fix_freq_time_on, 0u, 0xffu);
    dst_p[4] |= pack_right_shift_u16(src_p->set_first_fix_freq_time_on, 8u, 0x03u);
    
    /* set_first_fix_freq_time_off: bit 34, 10 bits */
    dst_p[4] |= pack_left_shift_u16(src_p->set_first_fix_freq_time_off, 2u, 0xfcu);
    dst_p[5] |= pack_right_shift_u16(src_p->set_first_fix_freq_time_off, 6u, 0x0fu);
    
    /* set_second_rev_oilP_max: bit 44, 9 bits */
    dst_p[5] |= pack_left_shift_u16(src_p->set_second_rev_oilP_max, 4u, 0xf0u);
    dst_p[6] |= pack_right_shift_u16(src_p->set_second_rev_oilP_max, 4u, 0x1fu);
    
    /* set_second_rev_oilP_min: bit 53, 9 bits */
    dst_p[6] |= pack_left_shift_u16(src_p->set_second_rev_oilP_min, 5u, 0xe0u);
    dst_p[7] |= pack_right_shift_u16(src_p->set_second_rev_oilP_min, 3u, 0x1fu);
    
    return (8);
}

int tsmaster_control_unpack(
    tsmaster_control_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-EINVAL);
    }
    
    if (dst_p == NULL || src_p == NULL) {
        return (-EINVAL);
    }
    
    /* set_bypass_initial_decline_time: bit 0, 10 bits */
    dst_p->set_bypass_initial_decline_time = unpack_right_shift_u16(src_p[0], 0u, 0xffu);
    dst_p->set_bypass_initial_decline_time |= unpack_left_shift_u16(src_p[1], 8u, 0x03u);
    
    /* set_rev_start_oilP_max: bit 10, 7 bits */
    dst_p->set_rev_start_oilP_max = unpack_right_shift_u8(src_p[1], 2u, 0xfcu);
    dst_p->set_rev_start_oilP_max |= unpack_left_shift_u8(src_p[2], 6u, 0x01u);
    
    /* set_rev_start_oilP_min: bit 17, 7 bits */
    dst_p->set_rev_start_oilP_min = unpack_right_shift_u8(src_p[2], 1u, 0xfeu);
    dst_p->set_rev_start_oilP_min |= unpack_left_shift_u8(src_p[3], 7u, 0x01u);
    
    /* set_first_fix_freq_time_on: bit 24, 10 bits */
    dst_p->set_first_fix_freq_time_on = unpack_right_shift_u16(src_p[3], 0u, 0xffu);
    dst_p->set_first_fix_freq_time_on |= unpack_left_shift_u16(src_p[4], 8u, 0x03u);
    
    /* set_first_fix_freq_time_off: bit 34, 10 bits */
    dst_p->set_first_fix_freq_time_off = unpack_right_shift_u16(src_p[4], 2u, 0xfcu);
    dst_p->set_first_fix_freq_time_off |= unpack_left_shift_u16(src_p[5], 6u, 0x0fu);
    
    /* set_second_rev_oilP_max: bit 44, 9 bits */
    dst_p->set_second_rev_oilP_max = unpack_right_shift_u16(src_p[5], 4u, 0xf0u);
    dst_p->set_second_rev_oilP_max |= unpack_left_shift_u16(src_p[6], 4u, 0x1fu);
    
    /* set_second_rev_oilP_min: bit 53, 9 bits */
    dst_p->set_second_rev_oilP_min = unpack_right_shift_u16(src_p[6], 5u, 0xe0u);
    dst_p->set_second_rev_oilP_min |= unpack_left_shift_u16(src_p[7], 3u, 0x1fu);
    
    return (0);
}

/* ========================================================================
 * tsmaster_control2 Message Pack/Unpack (ID: 0x98080108)
 * ======================================================================== */

int tsmaster_control2_pack(
    uint8_t *dst_p,
    const tsmaster_control2_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-EINVAL);
    }
    
    if (dst_p == NULL || src_p == NULL) {
        return (-EINVAL);
    }
    
    memset(&dst_p[0], 0, 8);
    
    /* set_rev_compel_time_on: bit 0, 10 bits */
    dst_p[0] |= pack_left_shift_u16(src_p->set_rev_compel_time_on, 0u, 0xffu);
    dst_p[1] |= pack_right_shift_u16(src_p->set_rev_compel_time_on, 8u, 0x03u);
    
    /* set_rev_compel_time_off: bit 10, 10 bits */
    dst_p[1] |= pack_left_shift_u16(src_p->set_rev_compel_time_off, 2u, 0xfcu);
    dst_p[2] |= pack_right_shift_u16(src_p->set_rev_compel_time_off, 6u, 0x0fu);
    
    /* system_enable: bit 20, 1 bit */
    dst_p[2] |= pack_left_shift_u8(src_p->system_enable, 4u, 0x10u);
    
    /* set_cooler_temperature_on: bit 21, 7 bits */
    dst_p[2] |= pack_left_shift_u8(src_p->set_cooler_temperature_on, 5u, 0xe0u);
    dst_p[3] |= pack_right_shift_u8(src_p->set_cooler_temperature_on, 3u, 0x03u);
    
    /* set_cooler_temperature_off: bit 28, 7 bits */
    dst_p[3] |= pack_left_shift_u8(src_p->set_cooler_temperature_off, 2u, 0xfcu);
    dst_p[4] |= pack_right_shift_u8(src_p->set_cooler_temperature_off, 6u, 0x01u);
    
    /* set_bypass_ratio: bit 35, 10 bits */
    dst_p[4] |= pack_left_shift_u16(src_p->set_bypass_ratio, 3u, 0xf8u);
    dst_p[5] |= pack_right_shift_u16(src_p->set_bypass_ratio, 5u, 0x07u);
    
    /* set_second_on_overtime: bit 45, 9 bits */
    dst_p[5] |= pack_left_shift_u16(src_p->set_second_on_overtime, 5u, 0xe0u);
    dst_p[6] |= pack_right_shift_u16(src_p->set_second_on_overtime, 3u, 0x3fu);
    
    /* set_second_off_overtime: bit 54, 9 bits */
    dst_p[6] |= pack_left_shift_u16(src_p->set_second_off_overtime, 6u, 0xc0u);
    dst_p[7] |= pack_right_shift_u16(src_p->set_second_off_overtime, 2u, 0x7fu);
    
    // 注意：bit 63 (剩余1位) 未使用，保持为0
    
    return (8);
}

int tsmaster_control2_unpack(
    tsmaster_control2_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-EINVAL);
    }
    
    if (dst_p == NULL || src_p == NULL) {
        return (-EINVAL);
    }
    
    /* set_rev_compel_time_on: bit 0, 10 bits */
    dst_p->set_rev_compel_time_on = unpack_right_shift_u16(src_p[0], 0u, 0xffu);
    dst_p->set_rev_compel_time_on |= unpack_left_shift_u16(src_p[1], 8u, 0x03u);
    
    /* set_rev_compel_time_off: bit 10, 10 bits */
    dst_p->set_rev_compel_time_off = unpack_right_shift_u16(src_p[1], 2u, 0xfcu);
    dst_p->set_rev_compel_time_off |= unpack_left_shift_u16(src_p[2], 6u, 0x0fu);
    
    /* system_enable: bit 20, 1 bit */
    dst_p->system_enable = unpack_right_shift_u8(src_p[2], 4u, 0x10u);
    
    /* set_cooler_temperature_on: bit 21, 7 bits */
    dst_p->set_cooler_temperature_on = unpack_right_shift_u8(src_p[2], 5u, 0xe0u);
    dst_p->set_cooler_temperature_on |= unpack_left_shift_u8(src_p[3], 3u, 0x03u);
    
    /* set_cooler_temperature_off: bit 28, 7 bits */
    dst_p->set_cooler_temperature_off = unpack_right_shift_u8(src_p[3], 2u, 0xfcu);
    dst_p->set_cooler_temperature_off |= unpack_left_shift_u8(src_p[4], 6u, 0x01u);
    
    /* set_bypass_ratio: bit 35, 10 bits */
    dst_p->set_bypass_ratio = unpack_right_shift_u16(src_p[4], 3u, 0xf8u);
    dst_p->set_bypass_ratio |= unpack_left_shift_u16(src_p[5], 5u, 0x07u);
    
    /* set_second_on_overtime: bit 45, 9 bits */
    dst_p->set_second_on_overtime = unpack_right_shift_u16(src_p[5], 5u, 0xe0u);
    dst_p->set_second_on_overtime |= unpack_left_shift_u16(src_p[6], 3u, 0x3fu);
    
    /* set_second_off_overtime: bit 54, 9 bits */
    dst_p->set_second_off_overtime = unpack_right_shift_u16(src_p[6], 6u, 0xc0u);
    dst_p->set_second_off_overtime |= unpack_left_shift_u16(src_p[7], 2u, 0x7fu);
    
    // 注意：bit 63 (剩余1位) 未使用，忽略这个位
    
    return (0);
}

/* ========================================================================
 * device_status Encode/Decode Functions
 * ======================================================================== */

uint8_t device_status_bypass_ratio_encode(double value)
{
    // DBC定义范围: 0..50%, 自动限制到有效范围
    if (value < 0.0) value = 0.0;
    if (value > 50.0) value = 50.0;
    return (uint8_t)(value / 0.2);
}

double device_status_bypass_ratio_decode(uint8_t value)
{
    return ((double)value * 0.2);
}

bool device_status_bypass_ratio_is_in_range(uint8_t value)
{
    return (value <= 250u);
}

uint8_t device_status_oil_pressure_encode(double value)
{
    // DBC定义范围: 0..40 MPa, 自动限制到有效范围
    if (value < 0.0) value = 0.0;
    if (value > 40.0) value = 40.0;
    return (uint8_t)(value / 0.15625);
}

double device_status_oil_pressure_decode(uint8_t value)
{
    return ((double)value * 0.15625);
}

bool device_status_oil_pressure_is_in_range(uint8_t value)
{
    return (value <= 256u);
}

uint8_t device_status_oil_temperature_encode(double value)
{
    // DBC定义范围: -40..90°C, Scale: 0.5098, Offset: -40
    // 编码公式: raw = (physical - offset) / scale = (value + 40) / 0.5098
    if (value < -40.0) value = -40.0;
    if (value > 90.0) value = 90.0;
    return (uint8_t)((value + 40.0) / 0.5098);
}

double device_status_oil_temperature_decode(uint8_t value)
{
    // 解码公式: physical = raw * scale + offset = value * 0.5098 - 40
    return ((double)value * 0.5098 - 40.0);
}

bool device_status_oil_temperature_is_in_range(uint8_t value)
{
    return (value <= 255u);
}

uint8_t device_status_LNG_temperature_encode(double value)
{
    // DBC定义范围: -40..90°C, Scale: 0.5098, Offset: -40
    // 编码公式: raw = (physical - offset) / scale = (value + 40) / 0.5098
    if (value < -40.0) value = -40.0;
    if (value > 90.0) value = 90.0;
    return (uint8_t)((value + 40.0) / 0.5098);
}

double device_status_LNG_temperature_decode(uint8_t value)
{
    // 解码公式: physical = raw * scale + offset = value * 0.5098 - 40
    return ((double)value * 0.5098 - 40.0);
}

bool device_status_LNG_temperature_is_in_range(uint8_t value)
{
    return (value <= 255u);
}

uint8_t device_status_LNG_pressure_encode(double value)
{
    // DBC定义范围: 0..40 MPa, 自动限制到有效范围
    if (value < 0.0) value = 0.0;
    if (value > 40.0) value = 40.0;
    return (uint8_t)(value / 0.15625);
}

double device_status_LNG_pressure_decode(uint8_t value)
{
    return ((double)value * 0.15625);
}

bool device_status_LNG_pressure_is_in_range(uint8_t value)
{
    return (value <= 256u);
}

uint16_t device_status_rev_freq_encode(double value)
{
    // DBC定义范围: 0..1000 min/次, 自动限制到有效范围
    if (value < 0.0) value = 0.0;
    if (value > 1000.0) value = 1000.0;
    return (uint16_t)(value);
}

double device_status_rev_freq_decode(uint16_t value)
{
    return ((double)value);
}

bool device_status_rev_freq_is_in_range(uint16_t value)
{
    return (value <= 1000u);
}

uint8_t device_status_rev_enable_encode(double value)
{
    return (uint8_t)(value);
}

double device_status_rev_enable_decode(uint8_t value)
{
    return ((double)value);
}

bool device_status_rev_enable_is_in_range(uint8_t value)
{
    return (value <= 1u);
}

uint8_t device_status_cooler_enable_encode(double value)
{
    return (uint8_t)(value);
}

double device_status_cooler_enable_decode(uint8_t value)
{
    return ((double)value);
}

bool device_status_cooler_enable_is_in_range(uint8_t value)
{
    return (value <= 1u);
}

uint8_t gcu_debug1_reversal_valve_st_encode(double value)
{
    return (uint8_t)(value);
}

double gcu_debug1_reversal_valve_st_decode(uint8_t value)
{
    return ((double)value);
}

bool gcu_debug1_reversal_valve_st_is_in_range(uint8_t value)
{
    return (value <= 1u);
}

uint16_t gcu_debug1_LNG_pressure_encode(double value)
{
    return (uint16_t)(value / 0.1);
}

double gcu_debug1_LNG_pressure_decode(uint16_t value)
{
    return ((double)value * 0.1);
}

bool gcu_debug1_LNG_pressure_is_in_range(uint16_t value)
{
    return (value <= 511u);
}

uint8_t gcu_debug1_reversal_valve_hz_encode(double value)
{
    return (uint8_t)(value);
}

double gcu_debug1_reversal_valve_hz_decode(uint8_t value)
{
    return ((double)value);
}

bool gcu_debug1_reversal_valve_hz_is_in_range(uint8_t value)
{
    return (value <= 127u);
}

uint16_t gcu_debug1_LNG_temperature_encode(double value)
{
    return (uint16_t)((value - -80.0) / 0.1);
}

double gcu_debug1_LNG_temperature_decode(uint16_t value)
{
    return (((double)value * 0.1) + -80.0);
}

bool gcu_debug1_LNG_temperature_is_in_range(uint16_t value)
{
    return (value <= 4095u);
}

uint8_t gcu_debug1_oil_pressure_encode(double value)
{
    return (uint8_t)(value / 0.1);
}

double gcu_debug1_oil_pressure_decode(uint8_t value)
{
    return ((double)value * 0.1);
}

bool gcu_debug1_oil_pressure_is_in_range(uint8_t value)
{
    (void)value;
    return (true);
}

uint16_t gcu_debug1_oil_temperature_encode(double value)
{
    return (uint16_t)((value - -80.0) / 0.1);
}

double gcu_debug1_oil_temperature_decode(uint16_t value)
{
    return (((double)value * 0.1) + -80.0);
}

bool gcu_debug1_oil_temperature_is_in_range(uint16_t value)
{
    return (value <= 4095u);
}

uint8_t gcu_debug1_reserve_debug1_encode(double value)
{
    return (uint8_t)(value);
}

double gcu_debug1_reserve_debug1_decode(uint8_t value)
{
    return ((double)value);
}

bool gcu_debug1_reserve_debug1_is_in_range(uint8_t value)
{
    (void)value;
    return (true);
}

/* ========================================================================
 * gcu_control Encode/Decode Functions
 * ======================================================================== */

uint8_t gcu_control_ctrl_reversal_valve_enable_encode(double value)
{
    return (uint8_t)(value);
}

double gcu_control_ctrl_reversal_valve_enable_decode(uint8_t value)
{
    return ((double)value);
}

bool gcu_control_ctrl_reversal_valve_enable_is_in_range(uint8_t value)
{
    return (value <= 1u);
}

uint16_t gcu_control_ctrl_bypass_valve_duty_encode(double value)
{
    return (uint16_t)(value / 0.1);
}

double gcu_control_ctrl_bypass_valve_duty_decode(uint16_t value)
{
    return ((double)value * 0.1);
}

bool gcu_control_ctrl_bypass_valve_duty_is_in_range(uint16_t value)
{
    return (value <= 1000u);
}

uint8_t gcu_control_ctrl_reversal_valve_freq_encode(double value)
{
    return (uint8_t)(value);
}

double gcu_control_ctrl_reversal_valve_freq_decode(uint8_t value)
{
    return ((double)value);
}

bool gcu_control_ctrl_reversal_valve_freq_is_in_range(uint8_t value)
{
    return (value <= 100u);
}

uint8_t gcu_control_ctrl_cooler_enable_encode(double value)
{
    return (uint8_t)(value);
}

double gcu_control_ctrl_cooler_enable_decode(uint8_t value)
{
    return ((double)value);
}

bool gcu_control_ctrl_cooler_enable_is_in_range(uint8_t value)
{
    return (value <= 1u);
}

uint8_t gcu_control_ctrl_system_enable_encode(double value)
{
    return (uint8_t)(value);
}

double gcu_control_ctrl_system_enable_decode(uint8_t value)
{
    return ((double)value);
}

bool gcu_control_ctrl_system_enable_is_in_range(uint8_t value)
{
    return (value <= 1u);
}

uint32_t gcu_control_ctrl_reserved_encode(double value)
{
    return (uint32_t)(value);
}

double gcu_control_ctrl_reserved_decode(uint32_t value)
{
    return ((double)value);
}

bool gcu_control_ctrl_reserved_is_in_range(uint32_t value)
{
    return (value <= 1073741823u);  /* 30 bits: 2^30 - 1 */
}

/* ========================================================================
 * tsmaster_control Encode/Decode Functions
 * ======================================================================== */

uint16_t tsmaster_control_set_bypass_initial_decline_time_encode(double value)
{
    return (uint16_t)(value / 0.1);
}

double tsmaster_control_set_bypass_initial_decline_time_decode(uint16_t value)
{
    return ((double)value * 0.1);
}

bool tsmaster_control_set_bypass_initial_decline_time_is_in_range(uint16_t value)
{
    return (value <= 1000u);
}


uint8_t tsmaster_control_set_rev_start_oilP_max_encode(double value)
{
    return (uint8_t)(value / 0.1);
}

double tsmaster_control_set_rev_start_oilP_max_decode(uint8_t value)
{
    return ((double)value * 0.1);
}

bool tsmaster_control_set_rev_start_oilP_max_is_in_range(uint8_t value)
{
    return (value <= 100u);
}

uint8_t tsmaster_control_set_rev_start_oilP_min_encode(double value)
{
    return (uint8_t)(value / 0.1);
}

double tsmaster_control_set_rev_start_oilP_min_decode(uint8_t value)
{
    return ((double)value * 0.1);
}

bool tsmaster_control_set_rev_start_oilP_min_is_in_range(uint8_t value)
{
    return (value <= 100u);
}

uint16_t tsmaster_control_set_first_fix_freq_time_on_encode(double value)
{
    return (uint16_t)(value / 0.1);
}

double tsmaster_control_set_first_fix_freq_time_on_decode(uint16_t value)
{
    return ((double)value * 0.1);
}

bool tsmaster_control_set_first_fix_freq_time_on_is_in_range(uint16_t value)
{
    return (value <= 1000u);
}

uint16_t tsmaster_control_set_first_fix_freq_time_off_encode(double value)
{
    return (uint16_t)(value / 0.1);
}

double tsmaster_control_set_first_fix_freq_time_off_decode(uint16_t value)
{
    return ((double)value * 0.1);
}

bool tsmaster_control_set_first_fix_freq_time_off_is_in_range(uint16_t value)
{
    return (value <= 1000u);
}

uint16_t tsmaster_control_set_second_rev_oilP_max_encode(double value)
{
    return (uint16_t)(value / 0.1);
}

double tsmaster_control_set_second_rev_oilP_max_decode(uint16_t value)
{
    return ((double)value * 0.1);
}

bool tsmaster_control_set_second_rev_oilP_max_is_in_range(uint16_t value)
{
    return (value <= 400u);
}

uint16_t tsmaster_control_set_second_rev_oilP_min_encode(double value)
{
    return (uint16_t)(value / 0.1);
}

double tsmaster_control_set_second_rev_oilP_min_decode(uint16_t value)
{
    return ((double)value * 0.1);
}

bool tsmaster_control_set_second_rev_oilP_min_is_in_range(uint16_t value)
{
    return (value <= 400u);
}

/* ========================================================================
 * tsmaster_control2 Encode/Decode Functions
 * ======================================================================== */

uint16_t tsmaster_control2_set_rev_compel_time_on_encode(double value)
{
    return (uint16_t)(value / 0.1);
}

double tsmaster_control2_set_rev_compel_time_on_decode(uint16_t value)
{
    return ((double)value * 0.1);
}

bool tsmaster_control2_set_rev_compel_time_on_is_in_range(uint16_t value)
{
    return (value <= 1000u);
}

uint16_t tsmaster_control2_set_rev_compel_time_off_encode(double value)
{
    return (uint16_t)(value / 0.1);
}

double tsmaster_control2_set_rev_compel_time_off_decode(uint16_t value)
{
    return ((double)value * 0.1);
}

bool tsmaster_control2_set_rev_compel_time_off_is_in_range(uint16_t value)
{
    return (value <= 1000u);
}

uint8_t tsmaster_control2_system_enable_encode(double value)
{
    return (uint8_t)(value);
}

double tsmaster_control2_system_enable_decode(uint8_t value)
{
    return ((double)value);
}

bool tsmaster_control2_system_enable_is_in_range(uint8_t value)
{
    return (value <= 1u);
}

uint8_t tsmaster_control2_set_cooler_temperature_on_encode(double value)
{
    return (uint8_t)(value);
}

double tsmaster_control2_set_cooler_temperature_on_decode(uint8_t value)
{
    return ((double)value);
}

bool tsmaster_control2_set_cooler_temperature_on_is_in_range(uint8_t value)
{
    return (value <= 100u);
}

uint8_t tsmaster_control2_set_cooler_temperature_off_encode(double value)
{
    return (uint8_t)(value);
}

double tsmaster_control2_set_cooler_temperature_off_decode(uint8_t value)
{
    return ((double)value);
}

bool tsmaster_control2_set_cooler_temperature_off_is_in_range(uint8_t value)
{
    return (value <= 100u);
}

uint16_t tsmaster_control2_set_bypass_ratio_encode(double value)
{
    // DBC定义范围: 0..50%, 自动限制到有效范围
    if (value < 0.0) value = 0.0;
    if (value > 50.0) value = 50.0;
    return (uint16_t)(value / 0.1);
}

double tsmaster_control2_set_bypass_ratio_decode(uint16_t value)
{
    return ((double)value * 0.1);
}

bool tsmaster_control2_set_bypass_ratio_is_in_range(uint16_t value)
{
    return (value <= 500u);
}

uint16_t tsmaster_control2_set_second_on_overtime_encode(double value)
{
    // DBC定义范围: 0..100 s, 自动限制到有效范围
    if (value < 0.0) value = 0.0;
    if (value > 100.0) value = 100.0;
    return (uint16_t)(value / 0.1);
}

double tsmaster_control2_set_second_on_overtime_decode(uint16_t value)
{
    return ((double)value * 0.1);
}

bool tsmaster_control2_set_second_on_overtime_is_in_range(uint16_t value)
{
    return (value <= 1000u);
}

uint16_t tsmaster_control2_set_second_off_overtime_encode(double value)
{
    // DBC定义范围: 0..100 s, 自动限制到有效范围
    if (value < 0.0) value = 0.0;
    if (value > 100.0) value = 100.0;
    return (uint16_t)(value / 0.1);
}

double tsmaster_control2_set_second_off_overtime_decode(uint16_t value)
{
    return ((double)value * 0.1);
}

bool tsmaster_control2_set_second_off_overtime_is_in_range(uint16_t value)
{
    return (value <= 1000u);
}
