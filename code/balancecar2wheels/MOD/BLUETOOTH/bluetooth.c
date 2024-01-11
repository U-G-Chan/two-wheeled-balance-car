#include "bluetooth.h"
#include "usart.h"
#include "bluetooth_msg.h"


#define END_CHAR      '>'

#define RXBUFFERSIZE  128
#define TXBUFFERSIZE  128
#define FMBUFFERSIZE  64


uint8_t bluetooth_buffer_rx[RXBUFFERSIZE];
uint8_t bluetooth_buffer_tx[TXBUFFERSIZE];
uint8_t bluetooth_buffer_fm[FMBUFFERSIZE];
uint8_t bluetooth_rx_idx  = 0,bluetooth_tx_idx  = 0,bluetooth_fm_idx=0;
uint8_t bluetooth_rx_data = 0,bluetooth_tx_data = 0,bluetooth_fm_data=0;

uint8_t frame_receive_flag = 0;



/**
���bluetooth_buffer_rx[]
*/
void bluetooth_rx_buf_clear(){
	bluetooth_rx_idx = 0;  // ���ý�������
	memset(bluetooth_buffer_rx, 0, strlen((char *)bluetooth_buffer_rx));
}

/**
���bluetooth_buffer_tx[]
*/
void bluetooth_tx_buf_clear(){
	bluetooth_tx_idx = 0;  // ���÷�������
	memset(bluetooth_buffer_tx, 0, strlen((char *)bluetooth_buffer_rx));
}
/**
bluetooth_buffer_fm[]
*/
void bluetooth_fm_buf_clear(){
	bluetooth_fm_idx = 0;  // ������Ϣ֡
	memset(bluetooth_buffer_fm, 0, strlen((char *)bluetooth_buffer_fm));
}


/**
����ָ����������
*/
void bluetooth_transmit(uint8_t * pData, uint16_t size){
	HAL_UART_Transmit(&HUART_BLUETOOTH, pData, size, 1000);
}

/**
����ָ����������
*/
void bluetooth_receive(uint8_t * pData, uint16_t size){
	HAL_UART_Receive(&HUART_BLUETOOTH, pData, size, 1000);
}


/**
�жϽ�������,���ˢ��
*/
void bluetooth_rx_buf_it(uint8_t* rx_data){
		if (bluetooth_rx_idx < RXBUFFERSIZE)
	{
			bluetooth_buffer_rx[bluetooth_rx_idx++] = *rx_data;  // �����յ���������ӵ�������
	}
	if(bluetooth_rx_idx >= TXBUFFERSIZE)
	{
		//show_message("buf={%s}\r\n",bluetooth_buffer_rx);
		//show_message("[bluetooth]clear buf_rx\r\n");
		bluetooth_rx_buf_clear();
	}
	HAL_UART_Receive_IT(&HUART_BLUETOOTH,rx_data, 1);  // �������������ж�
}


/**
�жϽ�����Ϣ֡������
*/
void bluetooth_fm_buf_it(uint8_t* fm_data){
	//printf("%c\r\n",*fm_data);
	if(bluetooth_fm_idx < FMBUFFERSIZE){
		if(*fm_data == '{' && frame_receive_flag < 2){
			frame_receive_flag++;
			HAL_UART_Receive_IT(&HUART_BLUETOOTH,fm_data,1);
			return;
		}
		if(frame_receive_flag == 2){
			bluetooth_buffer_fm[bluetooth_fm_idx++] = *fm_data;
		}
		if(*fm_data == '}' && frame_receive_flag >= 2){
			frame_receive_flag++;
			if(frame_receive_flag >= 4){
				bluetooth_msg_solve_frame((char*)bluetooth_buffer_fm);
				bluetooth_fm_buf_clear();
				frame_receive_flag = 0;
			}
		}
		
	}
	if(bluetooth_fm_idx >= FMBUFFERSIZE)
	{
		bluetooth_fm_buf_clear();
	}
	HAL_UART_Receive_IT(&HUART_BLUETOOTH,fm_data,1);
}



