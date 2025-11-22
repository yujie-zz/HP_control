/**
 * @file gcu_control_dbc.c
 * @brief Auto-generated from gcu_control.dbc
 * @note This file was automatically generated. Do not modify manually.
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

static inline uint8_t pack_left_shift_u32(
    uint32_t value,
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

static inline uint8_t pack_right_shift_u32(
    uint32_t value,
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

static inline uint32_t unpack_left_shift_u32(
    uint8_t value,
    uint8_t shift,
    uint8_t mask)
{
    return (uint32_t)((uint32_t)(value & mask) << shift);
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

static inline uint32_t unpack_right_shift_u32(
    uint8_t value,
    uint8_t shift,
    uint8_t mask)
{
    return (uint32_t)((uint32_t)(value & mask) >> shift);
}

/* ========================================================================
 * Initialization Functions
 * ======================================================================== */

int gcu_debug1_init(gcu_debug1_t *msg_p)
{
    if (msg_p == NULL) {
        return -1;
    }
    
    memset(msg_p, 0, sizeof(gcu_debug1_t));
    
    return 0;
}

int gcu_control_init(gcu_control_t *msg_p)
{
    if (msg_p == NULL) {
        return -1;
    }
    
    memset(msg_p, 0, sizeof(gcu_control_t));
    
    /* Set default values */
    msg_p->ctrl_reversal_valve_enable = CTRL_REVERSAL_VALVE_OFF;
    msg_p->ctrl_bypass_valve_duty = 0;
    msg_p->ctrl_reversal_valve_freq = 0;
    msg_p->ctrl_cooler_enable = CTRL_COOLER_OFF;
    msg_p->ctrl_system_enable = CTRL_SYSTEM_DISABLE;
    msg_p->ctrl_reserved = 0;
    
    return 0;
}

/* ========================================================================
 * gcu_debug1 Message Pack/Unpack (ID: 0x980FF16C)
 * ======================================================================== */

int gcu_debug1_pack(
    uint8_t *dst_p,
    const gcu_debug1_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-EINVAL);
    }
    
    if (dst_p == NULL || src_p == NULL) {
        return (-EINVAL);
    }
    
    memset(&dst_p[0], 0, 8);
    
    /* bypass_ratio: bit 0, 7 bits */
    dst_p[0] |= pack_left_shift_u8(src_p->bypass_ratio, 0u, 0x7fu);
    
    /* reversal_valve_st: bit 7, 1 bit */
    dst_p[0] |= pack_left_shift_u8(src_p->reversal_valve_st, 7u, 0x80u);
    
    /* LNG_pressure: bit 8, 9 bits */
    dst_p[1] |= pack_left_shift_u16(src_p->LNG_pressure, 0u, 0xffu);
    dst_p[2] |= pack_right_shift_u16(src_p->LNG_pressure, 8u, 0x01u);
    
    /* reversal_valve_hz: bit 17, 7 bits */
    dst_p[2] |= pack_left_shift_u8(src_p->reversal_valve_hz, 1u, 0xfeu);
    
    /* LNG_temperature: bit 24, 12 bits */
    dst_p[3] |= pack_left_shift_u16(src_p->LNG_temperature, 0u, 0xffu);
    dst_p[4] |= pack_right_shift_u16(src_p->LNG_temperature, 8u, 0x0fu);
    
    /* oil_pressure: bit 36, 8 bits */
    dst_p[4] |= pack_left_shift_u8(src_p->oil_pressure, 4u, 0xf0u);
    dst_p[5] |= pack_right_shift_u8(src_p->oil_pressure, 4u, 0x0fu);
    
    /* oil_temperature: bit 44, 12 bits */
    dst_p[5] |= pack_left_shift_u16(src_p->oil_temperature, 4u, 0xf0u);
    dst_p[6] |= pack_right_shift_u16(src_p->oil_temperature, 4u, 0xffu);
    
    /* reserve_debug1: bit 56, 8 bits */
    dst_p[7] |= pack_left_shift_u8(src_p->reserve_debug1, 0u, 0xffu);
    
    return (8);
}

int gcu_debug1_unpack(
    gcu_debug1_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-EINVAL);
    }
    
    if (dst_p == NULL || src_p == NULL) {
        return (-EINVAL);
    }
    
    /* bypass_ratio: bit 0, 7 bits */
    dst_p->bypass_ratio = unpack_right_shift_u8(src_p[0], 0u, 0x7fu);
    
    /* reversal_valve_st: bit 7, 1 bit */
    dst_p->reversal_valve_st = unpack_right_shift_u8(src_p[0], 7u, 0x80u);
    
    /* LNG_pressure: bit 8, 9 bits */
    dst_p->LNG_pressure = unpack_right_shift_u16(src_p[1], 0u, 0xffu);
    dst_p->LNG_pressure |= unpack_left_shift_u16(src_p[2], 8u, 0x01u);
    
    /* reversal_valve_hz: bit 17, 7 bits */
    dst_p->reversal_valve_hz = unpack_right_shift_u8(src_p[2], 1u, 0xfeu);
    
    /* LNG_temperature: bit 24, 12 bits */
    dst_p->LNG_temperature = unpack_right_shift_u16(src_p[3], 0u, 0xffu);
    dst_p->LNG_temperature |= unpack_left_shift_u16(src_p[4], 8u, 0x0fu);
    
    /* oil_pressure: bit 36, 8 bits */
    dst_p->oil_pressure = unpack_right_shift_u8(src_p[4], 4u, 0xf0u);
    dst_p->oil_pressure |= unpack_left_shift_u8(src_p[5], 4u, 0x0fu);
    
    /* oil_temperature: bit 44, 12 bits */
    dst_p->oil_temperature = unpack_right_shift_u16(src_p[5], 4u, 0xf0u);
    dst_p->oil_temperature |= unpack_left_shift_u16(src_p[6], 4u, 0xffu);
    
    /* reserve_debug1: bit 56, 8 bits */
    dst_p->reserve_debug1 = unpack_right_shift_u8(src_p[7], 0u, 0xffu);
    
    return (0);
}

/* ========================================================================
 * gcu_control Message Pack/Unpack (ID: 0x98080100)
 * ======================================================================== */

int gcu_control_pack(
    uint8_t *dst_p,
    const gcu_control_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-EINVAL);
    }
    
    if (dst_p == NULL || src_p == NULL) {
        return (-EINVAL);
    }
    
    memset(&dst_p[0], 0, 8);
    
    /* ctrl_reversal_valve_enable: bit 0, 1 bit */
    dst_p[0] |= pack_left_shift_u8(src_p->ctrl_reversal_valve_enable, 0u, 0x01u);
    
    /* ctrl_bypass_valve_duty: bit 8, 16 bits */
    dst_p[1] |= pack_left_shift_u16(src_p->ctrl_bypass_valve_duty, 0u, 0xffu);
    dst_p[2] |= pack_right_shift_u16(src_p->ctrl_bypass_valve_duty, 8u, 0xffu);
    
    /* ctrl_reversal_valve_freq: bit 24, 8 bits */
    dst_p[3] |= pack_left_shift_u8(src_p->ctrl_reversal_valve_freq, 0u, 0xffu);
    
    /* ctrl_cooler_enable: bit 32, 1 bit */
    dst_p[4] |= pack_left_shift_u8(src_p->ctrl_cooler_enable, 0u, 0x01u);
    
    /* ctrl_system_enable: bit 33, 1 bit */
    dst_p[4] |= pack_left_shift_u8(src_p->ctrl_system_enable, 1u, 0x02u);
    
    /* ctrl_reserved: bit 34, 30 bits */
    dst_p[4] |= pack_left_shift_u32(src_p->ctrl_reserved, 2u, 0xfcu);
    dst_p[5] |= pack_right_shift_u32(src_p->ctrl_reserved, 6u, 0xffu);
    dst_p[6] |= pack_right_shift_u32(src_p->ctrl_reserved, 14u, 0xffu);
    dst_p[7] |= pack_right_shift_u32(src_p->ctrl_reserved, 22u, 0xffu);
    
    return (8);
}

int gcu_control_unpack(
    gcu_control_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-EINVAL);
    }
    
    if (dst_p == NULL || src_p == NULL) {
        return (-EINVAL);
    }
    
    /* ctrl_reversal_valve_enable: bit 0, 1 bit */
    dst_p->ctrl_reversal_valve_enable = unpack_right_shift_u8(src_p[0], 0u, 0x01u);
    
    /* ctrl_bypass_valve_duty: bit 8, 16 bits */
    dst_p->ctrl_bypass_valve_duty = unpack_right_shift_u16(src_p[1], 0u, 0xffu);
    dst_p->ctrl_bypass_valve_duty |= unpack_left_shift_u16(src_p[2], 8u, 0xffu);
    
    /* ctrl_reversal_valve_freq: bit 24, 8 bits */
    dst_p->ctrl_reversal_valve_freq = unpack_right_shift_u8(src_p[3], 0u, 0xffu);
    
    /* ctrl_cooler_enable: bit 32, 1 bit */
    dst_p->ctrl_cooler_enable = unpack_right_shift_u8(src_p[4], 0u, 0x01u);
    
    /* ctrl_system_enable: bit 33, 1 bit */
    dst_p->ctrl_system_enable = unpack_right_shift_u8(src_p[4], 1u, 0x02u);
    
    /* ctrl_reserved: bit 34, 30 bits */
    dst_p->ctrl_reserved = unpack_right_shift_u32(src_p[4], 2u, 0xfcu);
    dst_p->ctrl_reserved |= unpack_left_shift_u32(src_p[5], 6u, 0xffu);
    dst_p->ctrl_reserved |= unpack_left_shift_u32(src_p[6], 14u, 0xffu);
    dst_p->ctrl_reserved |= unpack_left_shift_u32(src_p[7], 22u, 0xffu);
    
    return (0);
}

/* ========================================================================
 * gcu_debug1 Encode/Decode Functions
 * ======================================================================== */

uint8_t gcu_debug1_bypass_ratio_encode(double value)
{
    return (uint8_t)(value);
}

double gcu_debug1_bypass_ratio_decode(uint8_t value)
{
    return ((double)value);
}

bool gcu_debug1_bypass_ratio_is_in_range(uint8_t value)
{
    return (value <= 127u);
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
