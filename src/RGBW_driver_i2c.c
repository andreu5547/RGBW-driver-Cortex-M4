#include "RGBW_driver_i2c.h"

rgbw_error_t rgbw_dev_init(rgbw_dev_t* rgbw_dev, i2chw_bus_t bus_num, bool do_i2c_init) {
    rgbw_error_t res;

    rgbw_dev->dev.addr_width = RGBW_I2C_ADDR_WIDTH;
    rgbw_dev->dev.dev_addr = RGBW_I2C_ADDR;
    rgbw_dev->dev.bus_num = bus_num;

    if (do_i2c_init) { // Если нужно, иницивлизируем i2c
        i2chw_cfg_t i2c_cnfg = {
            .bus_freq = RGBW_I2C_FREQ,
            .dir_mode = I2CHW_MASTER_MODE
        };
    
        res = I2CHW_Configure(rgbw_dev->dev.bus_num, &i2c_cnfg);
        if (res != RGBW_SUCCESS) return res;
        res = I2CHW_Init(rgbw_dev->dev.bus_num);
        if (res != RGBW_SUCCESS) return res;
    }

    res = rgbw_dev_reset(rgbw_dev); // После включения необходим Reset Complete Chip

    if (res != RGBW_ERR_NACK) return res; // При вызове Reset Complete Chip последний байт следует без подтверждения

    res = rgbw_dev_on(rgbw_dev); // Включаем микросхему
    rgbw_dev->init = 1;
    return res;
}

static rgbw_error_t rgbw_dev_check_errors(const rgbw_dev_t* rgbw_dev) {
    if (rgbw_dev == NULL) return RGBW_ERR_INVALID_PARAMS;
    if (rgbw_dev->init != 1) return RGBW_ERR_DEV_NOT_INIT;
    return RGBW_SUCCESS;
}

static rgbw_error_t rgbw_dev_on(rgbw_dev_t* rgbw_dev) {
    rgbw_error_t res = rgbw_dev_check_errors(rgbw_dev);
    if (res != RGBW_SUCCESS) return res;

    uint8_t data[2] = {RGBW_REG_Res_Ctrl, 0};
    res = I2CHW_WriteReadSync(&rgbw_dev->dev, data, 1, data + 1, 1); // Считываем значение регистра

    if (res != RGBW_SUCCESS) return res;

    data[1] = (data[1] & ~(RGBW_CMD_DevAlwaysOn_msk)) | RGBW_CMD_DevAlwaysOn; // Модифицируем
    res = I2CHW_WriteSync(&rgbw_dev->dev, data, 2); // Записываем новое значение
    return res;
}

static rgbw_error_t rgbw_dev_reset(rgbw_dev_t* rgbw_dev) {
    rgbw_error_t res = rgbw_dev_check_errors(rgbw_dev);
    if (res != RGBW_SUCCESS) return res;

    const uint8_t data_tx[2] = {RGBW_REG_Res_Ctrl, RGBW_CMD_ResetChip};
    i2chw_error_t res = I2CHW_WriteSync(&rgbw_dev->dev, data_tx, 2);
    delay_us(RGBW_DELAY_ResetChip_us); // После Reset Complete Chip нужно подождать перед следующей командой
    return res;
}

rgbw_error_t rgbw_CHx_en(rgbw_dev_t* rgbw_dev, rgbw_channel_t chx, bool enable) {
    rgbw_error_t res = rgbw_dev_check_errors(rgbw_dev);
    if (res != RGBW_SUCCESS) return res;

    uint8_t data[2] = {RGBW_REG_Chan_En, 0};
    res = I2CHW_WriteReadSync(&rgbw_dev->dev, data, 1, data + 1, 1); // Считываем значение регистра

    if (res != RGBW_SUCCESS) return res;

    uint8_t val = enable ? (RGBW_CMD_CHx_en << chx) : 0;
    data[1] = (data[1] & ~(RGBW_CMD_CHx_en_msk << chx)) | val; // Модифицируем
    res = I2CHW_WriteSync(&rgbw_dev->dev, data, 2); // Записываем новое значение
    return res;
}

rgbw_error_t rgbw_CHall_en(rgbw_dev_t* rgbw_dev, bool enable) {
    rgbw_error_t res = rgbw_dev_check_errors(rgbw_dev);
    if (res != RGBW_SUCCESS) return res;

    uint8_t val = enable ? RGBW_CMD_CHall_en : 0;
    const uint8_t data[2] = {RGBW_REG_Chan_En, val};
    res = I2CHW_WriteSync(&rgbw_dev->dev, data, 2);
    return res;
}

static uint8_t convert_brightness_255_to_191(uint8_t val) {
    return (uint16_t)val * 191 / 255;
}

rgbw_error_t rgbw_ch_set_brightness(rgbw_dev_t* rgbw_dev, rgbw_channel_t chx, uint8_t brightness) {
    rgbw_error_t res = rgbw_dev_check_errors(rgbw_dev);
    if (res != RGBW_SUCCESS) return res;

    if (brightness == 0) { // Еслия яркость 0, то нужно выключить канал
        res = rgbw_CHx_en(rgbw_dev, chx, 0);
    } else { // Если не 0, то записать переведенное значение яркости в регистр и включить канал 
        const uint8_t data[2] = {RGBW_REG_Iout_D1 + chx, convert_brightness_255_to_191(brightness)};
        res = I2CHW_WriteSync(&rgbw_dev->dev, data, 2);

        if (res != RGBW_SUCCESS) return res;

        res = rgbw_CHx_en(rgbw_dev, chx, 1);
    }
    return res;
}

rgbw_error_t rgbw_RGB_set_brightness(rgbw_dev_t* rgbw_dev, uint8_t r, uint8_t g, uint8_t b) {
    rgbw_error_t res = rgbw_dev_check_errors(rgbw_dev);
    if (res != RGBW_SUCCESS) return res;

    res = rgbw_ch_set_brightness(rgbw_dev, RGBW_CHANNEL_R, r);
    if (res != RGBW_SUCCESS) return res;
    res = rgbw_ch_set_brightness(rgbw_dev, RGBW_CHANNEL_G, g);
    if (res != RGBW_SUCCESS) return res;
    res = rgbw_ch_set_brightness(rgbw_dev, RGBW_CHANNEL_B, b);
    return res;
}

rgbw_error_t rgbw_RGBW_set_brightness(rgbw_dev_t* rgbw_dev, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
    rgbw_error_t res = rgbw_dev_check_errors(rgbw_dev);
    if (res != RGBW_SUCCESS) return res;

    res = rgbw_RGB_set_brightness(rgbw_dev, r, g, b);
    if (res != RGBW_SUCCESS) return res;
    res = rgbw_ch_set_brightness(rgbw_dev, RGBW_CHANNEL_W, w);
    return res;
}