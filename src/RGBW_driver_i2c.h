#ifndef RGBW_DRIVER_I2C_H
#define RGBW_DRIVER_I2C_H
#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/
#include "I2C_driver.h"
#include "delay.h"

/******************************************************************************
* DEFINES
******************************************************************************/

// Service
#define RGBW_DELAY_ResetChip_us  200

// Addr
#define RGBW_I2C_ADDR            0x30
#define RGBW_I2C_ADDR_WIDTH      I2CHW_ADDR_WIDTH_7BIT
#define RGBW_I2C_FREQ            I2CHW_100_KHZ         // Max. val 400 kHz

// Reg
#define RGBW_REG_Res_Ctrl        0x00
#define RGBW_REG_Chan_En         0x04
#define RGBW_REG_Iout_D1         0x06
#define RGBW_REG_Iout_D2         0x07
#define RGBW_REG_Iout_D3         0x08
#define RGBW_REG_Iout_D4         0x09

// Command
#define RGBW_CMD_ResetChip       0b111
#define RGBW_CMD_DevAlwaysOn     0b11100
#define RGBW_CMD_DevAlwaysOn_msk 0b11111
#define RGBW_CMD_CHx_en          0b01
#define RGBW_CMD_CHx_en_msk      0b11
#define RGBW_CMD_CHall_en        0b01010101

/******************************************************************************
* PUBLIC TYPES
******************************************************************************/

/*
* Тип данных ошибок управления RGBW микросхемой
*/
typedef enum
{
    RGBW_SUCCESS,
    RGBW_ERR_INVALID_PARAMS,
    RGBW_ERR_TIMEOUT,
    RGBW_ERR_NACK,
    RGBW_ERR_HW,
    RGBW_ERR_DEV_NOT_INIT,
} rgbw_error_t;

/*
* Тип данных каналов управления RGBW светодиодами
*/
typedef enum
{
    RGBW_CHANNEL_R,
    RGBW_CHANNEL_G,
    RGBW_CHANNEL_B,
    RGBW_CHANNEL_W,
} rgbw_channel_t;

/*
* Тип данных конфигурации микросхемы дуправления RGBW
*/
typedef struct
{
    i2chw_dev_t dev;
    bool init;
} rgbw_dev_t;

/******************************************************************************
* PUBLIC FUNCTION PROTOTYPES
******************************************************************************/

/**
* @brief Инициализация RGBW микросхемы по шине I2C
*
* @param rgbw_dev - указатель на конфигурацию микросхемы
* @param bus_num - номер шины I2C
* @param do_i2c_init - инициализировать ли шину i2c
* @return rgbw_error_t - результат настройки
* RGBW_SUCCESS - все данные переданы на микросхему
* RGBW_ERR_INVALID_PARAMS - указаны недопустимые параметры передачи
* RGBW_ERR_TIMEOUT - передача не была завершена из-за тайм-аута
* RGBW_ERR_NACK - устройство не установило бит подтверждения
* RGBW_ERR_HW - у шины I2C проблемы с оборудованием
* RGBW_ERR_DEV_NOT_INIT - устройство не инициализировано
*/
rgbw_error_t rgbw_dev_init(rgbw_dev_t* rgbw_dev, i2chw_bus_t bus_num, bool do_i2c_init);

/**
* @brief Управление яркостью каналов RGBW микросхемы по шине I2C
*
* @param rgbw_dev - указатель на конфигурацию микросхемы
* @param chx - номер канала RGBW
* @param brightness - яркость канала от 0 до 255
* @return rgbw_error_t - результат настройки
* RGBW_SUCCESS - все данные переданы на микросхему
* RGBW_ERR_INVALID_PARAMS - указаны недопустимые параметры передачи
* RGBW_ERR_TIMEOUT - передача не была завершена из-за тайм-аута
* RGBW_ERR_NACK - устройство не установило бит подтверждения
* RGBW_ERR_HW - у шины I2C проблемы с оборудованием
* RGBW_ERR_DEV_NOT_INIT - устройство не инициализировано
*/
rgbw_error_t rgbw_ch_set_brightness(rgbw_dev_t* rgbw_dev, rgbw_channel_t chx, uint8_t brightness);

/**
* @brief Изменение состояния канала CHx RGBW микросхемы по шине I2C
*
* @param rgbw_dev - указатель на конфигурацию микросхемы
* @param chx - номер канала RGBW
* @param enable - Enable 1 / Disable 0
* @return rgbw_error_t - результат включения канала x
* RGBW_SUCCESS - все данные переданы на микросхему
* RGBW_ERR_INVALID_PARAMS - указаны недопустимые параметры передачи
* RGBW_ERR_TIMEOUT - передача не была завершена из-за тайм-аута
* RGBW_ERR_NACK - устройство не установило бит подтверждения
* RGBW_ERR_HW - у шины I2C проблемы с оборудованием
* RGBW_ERR_DEV_NOT_INIT - устройство не инициализировано
*/
rgbw_error_t rgbw_CHx_en(rgbw_dev_t* rgbw_dev, rgbw_channel_t chx, bool enable);

/**
* @brief Изменение состояния всех каналов RGBW микросхемы по шине I2C
*
* @param rgbw_dev - указатель на конфигурацию микросхемы
* @param enable - Enable 1 / Disable 0
* @return rgbw_error_t - результат включения каналов
* RGBW_SUCCESS - все данные переданы на микросхему
* RGBW_ERR_INVALID_PARAMS - указаны недопустимые параметры передачи
* RGBW_ERR_TIMEOUT - передача не была завершена из-за тайм-аута
* RGBW_ERR_NACK - устройство не установило бит подтверждения
* RGBW_ERR_HW - у шины I2C проблемы с оборудованием
* RGBW_ERR_DEV_NOT_INIT - устройство не инициализировано
*/
rgbw_error_t rgbw_CHall_en(rgbw_dev_t* rgbw_dev, bool enable);

/**
* @brief Установка цвета светодиодов в градациях RGB при применении единого RGB светодиода для RGBW микросхемы по шине I2C
*
* @param rgbw_dev - указатель на конфигурацию микросхемы
* @param r - яркость канала R от 0 до 255
* @param g - яркость канала G от 0 до 255
* @param b - яркость канала B от 0 до 255
* @return rgbw_error_t - результат включения каналов
* RGBW_SUCCESS - все данные переданы на микросхему
* RGBW_ERR_INVALID_PARAMS - указаны недопустимые параметры передачи
* RGBW_ERR_TIMEOUT - передача не была завершена из-за тайм-аута
* RGBW_ERR_NACK - устройство не установило бит подтверждения
* RGBW_ERR_HW - у шины I2C проблемы с оборудованием
* RGBW_ERR_DEV_NOT_INIT - устройство не инициализировано
*/
rgbw_error_t rgbw_RGB_set_brightness(rgbw_dev_t* rgbw_dev, uint8_t r, uint8_t g, uint8_t b);

/**
* @brief Установка цвета светодиодов в градациях RGB при применении единого RGB светодиода для RGBW микросхемы по шине I2C
*
* @param rgbw_dev - указатель на конфигурацию микросхемы
* @param r - яркость канала R от 0 до 255
* @param g - яркость канала G от 0 до 255
* @param b - яркость канала B от 0 до 255
* @param w - яркость канала W от 0 до 255
* @return rgbw_error_t - результат включения каналов
* RGBW_SUCCESS - все данные переданы на микросхему
* RGBW_ERR_INVALID_PARAMS - указаны недопустимые параметры передачи
* RGBW_ERR_TIMEOUT - передача не была завершена из-за тайм-аута
* RGBW_ERR_NACK - устройство не установило бит подтверждения
* RGBW_ERR_HW - у шины I2C проблемы с оборудованием
* RGBW_ERR_DEV_NOT_INIT - устройство не инициализировано
*/
rgbw_error_t rgbw_RGBW_set_brightness(rgbw_dev_t* rgbw_dev, uint8_t r, uint8_t g, uint8_t b, uint8_t w);

/******************************************************************************
* STATIC FUNCTION PROTOTYPES
******************************************************************************/
/**
* @brief Проверка валидности конфигурации
*
* @param rgbw_dev - указатель на конфигурацию микросхемы
* @return rgbw_error_t - результат проверки
* RGBW_SUCCESS - все данные переданы на микросхему
* RGBW_ERR_INVALID_PARAMS - указаны недопустимые параметры передачи
* RGBW_ERR_TIMEOUT - передача не была завершена из-за тайм-аута
* RGBW_ERR_NACK - устройство не установило бит подтверждения
* RGBW_ERR_HW - у шины I2C проблемы с оборудованием
* RGBW_ERR_DEV_NOT_INIT - устройство не инициализировано
*/
static rgbw_error_t rgbw_dev_check_errors(rgbw_dev_t* rgbw_dev);

/**
* @brief Перезагрузка RGBW микросхемы по шине I2C
*
* @param rgbw_dev - указатель на конфигурацию микросхемы
* @return rgbw_error_t - результат перезагрузки
* RGBW_SUCCESS - все данные переданы на микросхему
* RGBW_ERR_INVALID_PARAMS - указаны недопустимые параметры передачи
* RGBW_ERR_TIMEOUT - передача не была завершена из-за тайм-аута
* RGBW_ERR_NACK - устройство не установило бит подтверждения
* RGBW_ERR_HW - у шины I2C проблемы с оборудованием
* RGBW_ERR_DEV_NOT_INIT - устройство не инициализировано
*/
static rgbw_error_t rgbw_dev_reset(rgbw_dev_t* rgbw_dev);

/**
* @brief Включение RGBW микросхемы по шине I2C (все линии выкл, микросхема готова к работе, микросхема всегда вкл)
*
* @param rgbw_dev - указатель на конфигурацию микросхемы
* @return rgbw_error_t - результат включения
* RGBW_SUCCESS - все данные переданы на микросхему
* RGBW_ERR_INVALID_PARAMS - указаны недопустимые параметры передачи
* RGBW_ERR_TIMEOUT - передача не была завершена из-за тайм-аута
* RGBW_ERR_NACK - устройство не установило бит подтверждения
* RGBW_ERR_HW - у шины I2C проблемы с оборудованием
* RGBW_ERR_DEV_NOT_INIT - устройство не инициализировано
*/
static rgbw_error_t rgbw_dev_on(rgbw_dev_t* rgbw_dev);

/**
* @brief Конвертация уровней яркости из 0-255 в 0-191 из-за особеннойстей микросхемы см. даташит
*
* @param val - значение для конвертации
* @return uint8_t - результат конвертации
*/
static uint8_t convert_brightness_255_to_191(uint8_t val);

#ifdef __cplusplus
}
#endif
#endif // RGBW_DRIVER_I2C_H