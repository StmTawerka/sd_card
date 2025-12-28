#include <spi.h>
#include <stdint.h>

volatile char r1 = 0;
volatile char r_cmd55 = 0;
volatile char r_acmd41 = 0;
volatile uint8_t response_cmd17 = 0;
volatile uint8_t response_smd24 = 0;
volatile char token[1] = {0};
volatile char r7[5] = {0};
volatile char r3[5] = {0}; 
volatile int i = 0;
volatile uint8_t data_test[2] = {0};

volatile char complete_operation[32] = {0};
/* ================= SPI INIT ================= */
void SPI_init(void)
{
    SPI_CR1 &= ~(0x38);          
    SPI_CR1 |= f_MASTER_128;     

    SPI_CR1 &= ~CPOL;
    SPI_CR1 &= ~CPHA;
    SPI_CR1 &= ~LSBFIRST;

    SPI_CR2 |= SSM;
    SPI_CR2 |= SSI;

    SPI_CR1 |= MSTR;
    SPI_CR1 |= SPE_BIT;

    SPI_CR2 &= ~BDM;
    SPI_CR2 &= ~RXONLY;
}
/* ================= SPI BASIC ================= */
void transmit(char a)
{
    while (!(SPI_SR & TXE));
    SPI_DR = a;
    while (!(SPI_SR & RXNE));
    (void)SPI_DR;
}
void transmit_array(unsigned char *ptr, unsigned int len)
{
		unsigned int i;

    for (i = 0; i < len; i++)
    {
        while (!(SPI_SR & TXE));
        SPI_DR = ptr[i];
        while (!(SPI_SR & RXNE));
        (void)SPI_DR;
    }
}

/* ================= SD RESPONSES ================= */
void receive_R1(char *r1)
{
    unsigned int i;
    for (i = 0; i < 8; i++)
    {
        while (!(SPI_SR & TXE));
        SPI_DR = 0xFF;

        while (!(SPI_SR & RXNE));
        *r1 = SPI_DR;

        if (!(*r1 & 0x80))
            return;
    }
}
void receive_R2(char *r2)
{
    unsigned int i;
    for (i = 0; i < 2; i++)
    {
        while (!(SPI_SR & TXE));
        SPI_DR = 0xFF;
        while (!(SPI_SR & RXNE));
        r2[i] = SPI_DR;
    }
}
void receive_R7(char *r7)
{
    unsigned int i;

    receive_R1(&r7[0]);   // R1
    for (i = 1; i < 5; i++)
    {
        while (!(SPI_SR & TXE));
        SPI_DR = 0xFF;
        while (!(SPI_SR & RXNE));
        r7[i] = SPI_DR;
    }
}
void receive_R3(char *r3)
{
    unsigned int i;
		receive_R1(&r3[0]);
    for (i = 1; i < 5; i++)
    {
        while (!(SPI_SR & TXE));
        SPI_DR = 0xFF;
        while (!(SPI_SR & RXNE));
        r3[i] = SPI_DR;
    }
}
void receive_token(char *token)
{
        while (!(SPI_SR & TXE));
        SPI_DR = 0xFF;
        while (!(SPI_SR & RXNE));
        token[0] = SPI_DR;
}
void receive_512byte(uint8_t *data)
{
    unsigned int i;
    for (i = 0; i < 512; i++)
    {
				(void)SPI_DR;
        while (!(SPI_SR & TXE));
        SPI_DR = 0xFF;
        while (!(SPI_SR & RXNE));
        data[i] = SPI_DR;
    }
}
void receive_byte(uint8_t *data, uint8_t len)
{
    unsigned int i;
    for (i = 0; i < len; i++)
    {
				(void)SPI_DR;
        while (!(SPI_SR & TXE));
        SPI_DR = 0xFF;
        while (!(SPI_SR & RXNE));
        data[i] = SPI_DR;
    }
}
/* ================= SD COMMANDS ================= */
void power_on(void)
{
    char i;
    for (i = 0; i < 10; i++)     
        transmit(0xFF);
}
void send_CMD0(void)
{
    unsigned char CMD_0[6] = {64, 0, 0, 0, 0,  149};
    transmit_array(CMD_0, 6);
}
void send_CMD8(void)
{
    unsigned char CMD_8[6] = {72, 0, 0, 1, 0xAA, 135};
    transmit_array(CMD_8, 6);
}
void send_CMD55(void)
{
    unsigned char CMD55[6] = {0x77, 0, 0, 0, 0, 0x01};
    transmit_array(CMD55, 6);
}
void send_ACMD41(void)
{
    unsigned char ACMD41[6] = {0x69, 0x40, 0, 0, 0, 0x01};
    transmit_array(ACMD41, 6);
}
void send_CMD58(void)
{
    unsigned char CMD_58[6] = {122, 0, 0, 0, 0,117};
    transmit_array(CMD_58, 6);
}
/* ================= SD-CARD INIT ================= */
void SD_CARD_Init_1GB(void){
	    do {// CMD0
        PE_ODR &= ~(1 << 5);
        send_CMD0();
        receive_R1(&r1);
        PE_ODR |= (1 << 5);
        transmit(0xFF);
    } while (r1 != 0x01);
      do {// CMD8
				PE_ODR &= ~(1 << 5);
				send_CMD8();
				receive_R7(r7);
				PE_ODR |= (1 << 5);
				transmit(0xFF);
		} while (r7[4] != 0xAA);
    do {// CMD55
        PE_ODR &= ~(1 << 5);
        send_CMD55();
        receive_R1(&r_cmd55); 
        PE_ODR |= (1 << 5);
        transmit(0xFF);
        // ACMD41
        PE_ODR &= ~(1 << 5);
        send_ACMD41();
        receive_R1(&r_acmd41);
        PE_ODR |= (1 << 5);
        transmit(0xFF);
    } while (r_acmd41 != 0x00);
		do {// CMD58
				PE_ODR &= ~(1 << 5);
				send_CMD58();
				receive_R3(r3);
				PE_ODR |= (1 << 5);
				transmit(0xFF);
		} while (r3[0] != 0x00);
		// sd-card initialized. CCS = 0.
}
 void send_CMD13(void){ //������� ������
		uint8_t CMD_13[6] = {0};
    CMD_13[0] = 0x4D;  
    CMD_13[1] = 0;
    CMD_13[2] = 0;
    CMD_13[3] = 0;
    CMD_13[4] = 0;
    CMD_13[5] = 0xFF;  
    transmit_array(CMD_13, 6);
	}
void send_CMD17(uint32_t address){ //������� ������
		uint8_t CMD_17[6] = {0};
    CMD_17[0] = 0x51;  
    CMD_17[1] = (address >> 24) & 0xFF;
    CMD_17[2] = (address >> 16) & 0xFF;
    CMD_17[3] = (address >> 8) & 0xFF;
    CMD_17[4] = address & 0xFF;
    CMD_17[5] = 0xFF;  
    transmit_array(CMD_17, 6);
	}
void send_CMD24(uint32_t address) {//������� ������
		uint8_t CMD_24[6] = {0};
    CMD_24[0] = 0x58;  
    CMD_24[1] = (address >> 24) & 0xFF;
    CMD_24[2] = (address >> 16) & 0xFF;
    CMD_24[3] = (address >> 8) & 0xFF;
    CMD_24[4] = address & 0xFF;
    CMD_24[5] = 0xFF;  
		transmit_array(CMD_24, 6);
}
void CARD_Read(uint8_t *data, uint32_t adress){//������� ������
	do{
		PE_ODR &= ~(1 << 5);
		send_CMD17(adress);
		do{
		receive_R1(&response_cmd17);}
	  while(response_cmd17 != 0x00);
		do{
	  receive_token(token);}
	  while(token[0] != 0xFE);
		receive_512byte(data);
		PE_ODR |= (1 << 5);}
	while(complete_operation[0] != 0x00 && complete_operation[1] != 0x00);
}
void CARD_Write(uint8_t *data, uint32_t adress){//������� ������
	do{
		PE_ODR &= ~(1 << 5);
		send_CMD24(adress);
		do{
			receive_R1(&response_smd24);}
		while(response_smd24 != 0x00);
		transmit(0xFE);
		transmit_array(data, 512);
		receive_byte(data_test,2);//���������� ���� ������ data_response � busy
		send_CMD13();
		receive_R2(complete_operation);
		PE_ODR |= (1 << 5);}
	while(complete_operation[0] != 0x00 && complete_operation[1] != 0x00);
}
void Init_CS(void){
	    PE_DDR |= (1 << 5);
    PE_CR1 |= (1 << 5);
    PE_CR2 |= (1 << 5);
    PE_ODR |= (1 << 5); 
	}
uint8_t SPI_TransmitReceiveByte(uint8_t data)
{
    SPI1_DR = data;
    while (!(SPI1_SR & SPI_SR_RXNE));
    return SPI1_DR;
}
