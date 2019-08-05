/*
 * controller.c
 *
 *  Created on: 2019/08/02
 *      Author: tomok
 */


#include "controller.h"



static void PSControllerSelect(void);
static void PSControllerDeselect(void);
static void PSControllerWait(void);

void PSControllerGetData(uint16_t *button){
	uint8_t txdata[10] = {0x01,0x42};
	uint8_t rxdata[10] = {};

	CONTROLLER_SPI_HANDLER.Init.FirstBit = SPI_FIRSTBIT_LSB;
	HAL_SPI_Init(&CONTROLLER_SPI_HANDLER);

	PSControllerSelect();
	PSControllerWait();
	for(uint8_t i =0;i<3;i++){
		HAL_SPI_TransmitReceive(&CONTROLLER_SPI_HANDLER,&txdata[i],&rxdata[i],1,0xFFFF);
		PSControllerWait();

	}
	PSControllerDeselect();

	rxdata[3] = ~rxdata[3];
	rxdata[4] = ~rxdata[4];
	*button = ((rxdata[3]<<8) + rxdata[4]);
}

static void PSControllerSelect(void){
	HAL_GPIO_WritePin(CONTROLLER_CS_PORT,CONTROLLER_CS_PIN,GPIO_PIN_SET);
}

static void PSControllerDeselect(void){
	HAL_GPIO_WritePin(CONTROLLER_CS_PORT,CONTROLLER_CS_PIN,GPIO_PIN_RESET);
}

static void PSControllerWait(void){
	volatile uint32_t i;
	for(i=0;i<30;i++);
}
