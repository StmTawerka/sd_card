#include "main.h"
#include "spi.h"
#include "stdint.h"
volatile uint8_t receive_1data[512] = {0};
volatile uint8_t receive_data[512] = {0};
volatile uint8_t transmit_data[512] = {0};
/* test variables */
int main(void)
{
/*	int cnt=0;
		for(cnt=0;cnt<512;cnt++){
		transmit_data[cnt] = 512-cnt;}
*/
    /* Pin CS init 	*/
    Init_CS();  
		/*   SPI init		*/
		SPI_init();
		/* sd-card init */
    power_on();          
		SD_CARD_Init_1GB();
		/* write/read data */
		CARD_Read(receive_1data, 	0x00010000);
	  CARD_Write(transmit_data, 0x00010000);
		CARD_Read(receive_data, 	0x00010000);
		while (1);
}
