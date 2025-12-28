#include "main.h"
#include "spi.h"
#include "sd_card.h"

static uint8_t sd_buffer[512];

int main(void)
{
    SD_Handle_t sd;

    CLK_CKDIVR = 0x00; // fCPU = 16 MHz

    /* CS как выход */
    PB_DDR |= (1 << 4);
    PB_CR1 |= (1 << 4);
    PB_ODR |= (1 << 4);

    SPI_Init();

    sd.data = sd_buffer;

    SD_Init(&sd);
    SD_ReadBlock(&sd, 0);

    while (1) {
        /* основной цикл */
    }
}
