#include "sd_card.h"
#define SD_CS_LOW()   (PB_ODR &= ~(1 << 5))
#define SD_CS_HIGH()  (PB_ODR |=  (1 << 5))

static uint8_t SD_SPI(uint8_t data)
{
    return SPI_TransmitReceiveByte(data);
}

SD_Status_t SD_SendCmd(SD_Handle_t *sd, uint8_t cmd, uint32_t arg)
{
    SD_CS_LOW();

    SD_SPI(cmd);
    SD_SPI(arg >> 24);
    SD_SPI(arg >> 16);
    SD_SPI(arg >> 8);
    SD_SPI(arg);
    SD_SPI(0x95); // CRC для CMD0

    for (uint8_t i = 0; i < SD_RESPONSE_SIZE; i++) {
        sd->response[i] = SD_SPI(0xFF);
        if ((sd->response[i] & 0x80) == 0) {
            SD_CS_HIGH();
            return SD_OK;
        }
    }

    SD_CS_HIGH();
    sd->error = SD_BAD_RESPONSE;
    return SD_BAD_RESPONSE;
}

SD_Status_t SD_Init(SD_Handle_t *sd)
{
    sd->initialized = 0;
    sd->error = SD_OK;

    SD_CS_HIGH();
    for (uint8_t i = 0; i < 10; i++) {
        SD_SPI(0xFF);
    }

    if (SD_SendCmd(sd, 0x40, 0) != SD_OK) {
        return SD_ERROR;
    }

    if (sd->response[0] != 0x01) {
        sd->error = SD_BAD_RESPONSE;
        return SD_BAD_RESPONSE;
    }

    sd->initialized = 1;
    return SD_OK;
}

SD_Status_t SD_ReadBlock(SD_Handle_t *sd, uint32_t block)
{
    if (!sd->initialized || sd->data == 0) {
        return SD_NOT_INITIALIZED;
    }

    if (SD_SendCmd(sd, 0x51, block * 512) != SD_OK) {
        return SD_ERROR;
    }

    uint8_t token;
    do {
        token = SD_SPI(0xFF);
    } while (token == 0xFF);

    if (token != 0xFE) {
        sd->error = SD_BAD_RESPONSE;
        return SD_BAD_RESPONSE;
    }

    for (uint16_t i = 0; i < SD_BLOCK_SIZE; i++) {
        sd->data[i] = SD_SPI(0xFF);
    }

    SD_SPI(0xFF);
    SD_SPI(0xFF);

    return SD_OK;
}
