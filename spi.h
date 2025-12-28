#ifndef SPI_H
#define SPI_H
#include <main.h>
//==========регистр CR1
#define CPHA 						0x01 
#define CPOL						0x02
#define MSTR 						0x04
#define f_MASTER_2 			0x0 
#define f_MASTER_4			0x8
#define f_MASTER_8			0x10
#define f_MASTER_16			0x18
#define f_MASTER_32			0x20
#define f_MASTER_64			0x28
#define f_MASTER_128		0x30
#define f_MASTER_256		0x38
#define SPE_BIT 				0x40
#define LSBFIRST 				0x80
//=========регистр CR2
#define SSI 						0x01
#define SSM 						0x02
#define RXONLY 					0x04
#define CRCNEXT 				0x10
#define CRCEN 					0x20
#define BDOE 						0x40
#define BDM 						0x80
//=========статусный регистр
#define RXNE						0x01
#define TXE							0x02 
#define BUSY						0x80 
//======== CMD
#define CMD0	

void SPI_init(void);		
void transmit (char a);	
void transmit_array (unsigned char *ptr, unsigned int len);
void receive_R1 (char* b);
void receive_R2(char *r1);
void receive_R7 (char* ar);
void receive_R3 (char* ar);
void receive_token(char *token);
void receive_512byte(uint8_t *data);
void receive_byte(uint8_t *data, uint8_t len);
void power_on (void);
void send_CMD0 (void);
void send_CMD8 (void);
void send_CMD55 (void);
void send_ACMD41 (void);
void send_CMD58(void);
// uint8_t send_CMD17_get_response(uint32_t address);
void SD_ReadOCR(char *ocr);
void Init_CS(void);
void SD_CARD_Init_1GB(void);
void CARD_Write(uint8_t *data, uint32_t adress);
void CARD_Read(uint8_t *data, uint32_t adress);
void send_CMD24(uint32_t address);
void send_CMD17(uint32_t address);
 void send_CMD13(void);
#endif
