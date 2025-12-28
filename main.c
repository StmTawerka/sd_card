#include "main.h"
#include "spi.h"
#include "sd_card.h"

static uint8_t sd_buffer[512];

int main(void)
{
    SD_Handle_t sd;

    CLK_CKDIVR = 0x00;

    PB_DDR |= (1 << 5);
    PB_CR1 |= (1 << 5);
    PB_ODR |= (1 << 5);

    SPI_Init();

    sd.data = sd_buffer;

    /* Заполняем данные */
    for (uint16_t i = 0; i < 512; i++) {
        sd_buffer[i] = i & 0xFF;
    }

    SD_Init(&sd);
    SD_WriteBlock(&sd, 1);
    SD_ReadBlock(&sd, 1);

    while (1);
}
