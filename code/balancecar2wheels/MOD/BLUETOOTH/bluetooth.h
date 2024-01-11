#ifndef __BLUETOOTH_H__
#define __BLUETOOTH_H__

#include "stm32f1xx_hal.h"

#define HUART_BLUETOOTH          huart2

void bluetooth_rx_buf_clear();

void bluetooth_tx_buf_clear();  

void bluetooth_fm_buf_clear();

void bluetooth_transmit(uint8_t * pData, uint16_t size);

void bluetooth_receive(uint8_t * pData, uint16_t size);

void bluetooth_rx_buf_it(uint8_t* rx_data);

void bluetooth_fm_buf_it(uint8_t* rx_data);


#endif