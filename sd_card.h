#ifndef SD_CARD_H
#define SD_CARD_H

#include "spi.h"

/* Статусы */
typedef enum {
    SD_OK = 0,
    SD_ERROR,
    SD_NOT_INITIALIZED,
    SD_BAD_RESPONSE
} SD_Status_t;

/* Размеры */
#define SD_BLOCK_SIZE     512
#define SD_RESPONSE_SIZE  6

/* Контекст SD-карты */
typedef struct {
    uint8_t initialized;                    // карта инициализирована?
    uint8_t response[SD_RESPONSE_SIZE];     // ответы SD
    SD_Status_t error;                      // код ошибки
    uint8_t *data;                          // указатель на буфер данных
} SD_Handle_t;

/* API */
SD_Status_t SD_Init(SD_Handle_t *sd);
SD_Status_t SD_SendCmd(SD_Handle_t *sd, uint8_t cmd, uint32_t arg);
SD_Status_t SD_ReadBlock(SD_Handle_t *sd, uint32_t block);

#endif /* SD_CARD_H */
