/*

 * junk.c
 *
 *  Created on: Jun 19, 2014
 *      Author: Callen Fisher



void initI2CJustinsSensor(void);
void i2cJustinRegisterSetup(void);
void initSPISCcard(void);
void spiSDCardRegisterSetup(void);

  //initI2CJustinsSensor();
  //i2cJustinRegisterSetup();
  //initSPISCcard();
  //spiSDCardRegisterSetup();






///////////////////////////////////////////////////////////////////////////
//////////////JUSTINS SENSOR///////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
void initI2CJustinsSensor(void)
{
	I2C_InitTypeDef  I2C_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	 Enable I2C and GPIO clocks
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	 Configure I2C pins: SCL and SDA
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	 I2C configuration
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 100000;

	 Apply I2C configuration after enabling it
	I2C_Init(I2C1, &I2C_InitStructure);

	 I2C Peripheral Enable
	I2C_Cmd(I2C1, ENABLE);
}
void i2cJustinRegisterSetup(void)
{
	//register address
	I2C_GenerateSTART(I2C1,ENABLE);
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C1,,I2C_Direction_Transmitter);//slave address
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_SendData(I2C1,);//write address
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_SendData(I2C1,);//data
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_GenerateSTOP(I2C1,ENABLE);
}
///////////////////////////////////////////////////////////////////////////
////////////////SD card////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
void initSPISCcard(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_SPI1,ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;

	 Pack the struct
	GPIO_InitStruct.GPIO_Pin =GPIO_Pin_5 | GPIO_Pin_6| GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode =GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	//Init CS-Pin
	GPIO_InitStruct.GPIO_Pin =GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Mode =GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_SetBits(GPIOA, GPIO_Pin_4);  //Set CS high

	SPI_InitTypeDef  SPI_InitStructure;

	SPI_I2S_DeInit(SPI1);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b ;//
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//?
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//?
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;

	SPI_Init(SPI1, &SPI_InitStructure);

	SPI_Cmd(SPI1, ENABLE);
}
void spiSDCardRegisterSetup(void)
{

}*/
