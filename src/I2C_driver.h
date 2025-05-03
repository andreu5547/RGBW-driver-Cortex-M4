#ifndef I2CHW_H
#define I2CHW_H
#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************
* INCLUDES
******************************************************************************/
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
/******************************************************************************
* PUBLIC TYPES
******************************************************************************/
typedef enum
{
    I2CHW_SUCCESS,
    I2CHW_ERR_INVALID_PARAMS,
    I2CHW_ERR_TIMEOUT,
    I2CHW_ERR_NACK,
    I2CHW_ERR_HW,
} i2chw_error_t;

typedef enum
{
    I2CHW_BUS_I2C0,
    I2CHW_BUS_I2C1,
    I2CHW_BUS_I2C2,
    I2CHW_BUS_I2C3,
} i2chw_bus_t;

typedef enum
{
    I2CHW_SLAVE_MODE,
    I2CHW_MASTER_MODE,
} i2chw_dir_mode_t;

typedef enum
{
    I2CHW_ADDR_WIDTH_7BIT,
    I2CHW_ADDR_WIDTH_8BIT,
    I2CHW_ADDR_WIDTH_10BIT,
} i2chw_addr_t;

typedef enum
{
    I2CHW_10_KHZ,
    I2CHW_100_KHZ,
    I2CHW_400_KHZ,
    I2CHW_1000_KHZ,
} i2chw_bus_freq_t;

typedef struct
{
    i2chw_bus_t bus_num; // I2C bus number
    uint16_t dev_addr; // I2C device address on I2C bus
    i2chw_addr_t addr_width; // I2C device address width
} i2chw_dev_t;

typedef struct
{
    i2chw_bus_freq_t bus_freq; // I2C frequency
    i2chw_dir_mode_t dir_mode; // I2C slave/master mode
} i2chw_cfg_t;

/******************************************************************************
* PUBLIC FUNCTION PROTOTYPES
******************************************************************************/

/**
* @brief Initialize I2C bus. Shall be called after I2CHW_Configure
*
* @param bus_num - number of I2C bus
* @return hal_error_t - initialization result
* I2CHW_SUCCESS - the bus was initialized successfully
* I2CHW_ERR_INVALID_PARAMS - invalid parameters are given of the bus was not configured before
* I2CHW_ERR_HW - the bus was not initialized because of HW issues
*
*/

/**
* @brief инициализирует шину I2C. Должен вызываться после I2CHW_Configure
*
* @param bus_num - номер шины I2C
* @return hal_error_t - результат инициализации
* I2CHW_SUCCESS - шина была успешно инициализирована
* I2C HW_ERR_INVALID_PARAMS - указаны недопустимые параметры шины, которая не была настроена ранее.
* I2C HW_ERR_HW - шина не была инициализирована из-за проблем с HW
*
*/
i2chw_error_t I2CHW_Init(const i2chw_bus_t bus_num);

/**
* @brief Configure I2C bus before initialization
*
* @param bus_num - number of I2C bus
* @param p_config - pointer to I2C configuration
* @return i2chw_error_t - configuration result
* I2CHW_SUCCESS - the bus was configured successfully
* I2CHW_ERR_INVALID_PARAMS - invalid parameters are given
*/

/**
* @brief Конфигурация шины I2C перед инициализацией
*
* @param bus_num - номер шины I2C
* @param p_config - указатель на конфигурацию I2C
* @return i2chw_error_t - результат настройки
* I2CHW_SUCCESS - шина была успешно настроена
* I2CHW_ERR_INVALID_PARAMS - указаны недопустимые параметры
*/
i2chw_error_t I2CHW_Configure(const i2chw_bus_t bus_num,
const i2chw_cfg_t* p_config);

/**
* @brief Transmit data to device in master mode synchronously
*
* @param p_dev - pointer to I2C device configuration
* @param p_tx_data - pointer to data to be transmitted
* @param tx_data_len - length of transmitted data
* @return i2chw_error_t - result of transmission
* I2CHW_SUCCESS - all data has been transmitted
* I2CHW_ERR_INVALID_PARAMS - invalid parameters are given
* I2CHW_ERR_TIMEOUT - transmission was not finished because of execution timeout
* I2CHW_ERR_NACK - the device did't set ACK bit
* I2CHW_ERR_HW - the bus has hardware issues
*/

/**
* @brief синхронно передает данные на устройство в master режиме
*
* @param p_dev - указатель на конфигурацию устройства I2C
* @param p_tx_data - указатель на данные, которые должны быть переданы
* @param tx_data_len - длина передаваемых данных
* @return i2chw_error_t - результат передачи
* I2CHW_SUCCESS - все данные переданы.
* I2CHW_ERR_INVALID_PARAMS - указаны недопустимые параметры
* I2CHW_ERR_TIMEOUT - передача не была завершена из-за тайм-аута выполнения
* I2CHW_ERR_NACK - устройство не установило бит подтверждения
* I2CHW_ERR_HW - у шины проблемы с оборудованием
*/
i2chw_error_t I2CHW_WriteSync(const i2chw_dev_t* p_dev,
const uint8_t* p_tx_data,
const uint16_t tx_data_len);

/**
* @brief Transmit data to I2C device and then receive data from the device in master mode synchronously
*
* @param p_dev - pointer to I2C device configuration
* @param p_tx_data - pointer to data to be transmitted
* @param tx_data_len - length of transmitted data
* @param p_rx_data - pointer to buffer to put received data
* @param rx_data_len - length of received data
* @return i2chw_error_t - result of exchange
* I2CHW_SUCCESS - all data has been transmitted and received
* I2CHW_ERR_INVALID_PARAMS - invalid parameters are given
* I2CHW_ERR_TIMEOUT - exchange was not finished because of execution timeout
* I2CHW_ERR_NACK - the device did't set ACK bit
* I2CHW_ERR_HW - the bus has hardware issues
*
*/

/**
* @brief Передает данные на устройство I2C, а затем синхронно принимает данные с устройства в master режиме
*
* @param p_dev - указатель на конфигурацию устройства I2C
* @param p_tx_data - указатель на передаваемые данные
* @param tx_data_len - длина передаваемых данных
* @param p_rx_data - указатель на буфер для размещения полученных данных
* @param rx_data_len - длина полученных данных
* @return i2chw_error_t - результат обмена
* I2CHW_SUCCESS - все данные переданы и получены
* I2CHW_ERR_INVALID_PARAMS - указаны недопустимые параметры.
* I2CHW_ERR_TIMEOUT - обмен не был завершен из-за истечения времени ожидания выполнения
* I2CHW_ERR_NACK - устройство не установило бит подтверждения
* I2CHW_ERR_HW - у шины проблемы с оборудованием
*
*/
i2chw_error_t I2CHW_WriteReadSync(const i2chw_dev_t* p_dev,
const uint8_t* p_tx_data,
const uint16_t tx_data_len,
uint8_t* p_rx_data,
const uint16_t rx_data_len);

/**
* @brief Receive data from device in master mode synchronously
*
* @param p_dev - pointer to I2C device configuration
* @param p_rx_data - pointer to buffer to put received data
* @param rx_data_len - length of received data
* @return i2chw_error_t - result of receiving
* I2CHW_SUCCESS - all data has received
* I2CHW_ERR_INVALID_PARAMS - invalid parameters are given
* I2CHW_ERR_TIMEOUT - exchange was not finished because of execution timeout
* I2CHW_ERR_NACK - the device did't set ACK bit
* I2CHW_ERR_HW - the bus has hardware issues
*/

/**
* @brief синхронно получает данные от устройства в master режиме
*
* @param p_dev - указатель на конфигурацию устройства I2C
* @param p_rx_data - указатель на буфер для размещения полученных данных
* @param rx_data_len - длина полученных данных
* @return i2chw_error_t - результат получения
* I2CHW_SUCCESS - все данные получены.
* I2CHW_ERR_INVALID_PARAMS - указаны недопустимые параметры
* I2CHW_ERR_TIMEOUT - обмен не был завершен из-за тайм-аута выполнения
* I2CHW_ERR_NACK - устройство не установило бит подтверждения
* I2CHW_ERR_HW - у шины проблемы с оборудованием
*/
i2chw_error_t I2CHW_ReadSync(const i2chw_dev_t* p_dev,
uint8_t* p_rx_data,
const uint16_t rx_data_len);

#ifdef __cplusplus
}
#endif
#endif // I2CHW_H