#include "bluetooth_msg.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARG_LIMIT      5

const char *delimiter = ",";

void bluetooth_msg_solve_frame(char frame[]){
	char *token = strtok(frame, delimiter);
	
	int msgCode = 0;
	float args[ARG_LIMIT];
	uint8_t arg_idx = 0;
	
	while(token != NULL){
		args[arg_idx++] = atof(token);
		token = strtok(NULL, delimiter);
		if(arg_idx >= ARG_LIMIT){
			break;
		}
	}
	msgCode = (int)args[0];
	//根据信息码，综合响应
	switch (msgCode){
		case 121:{
			led_test(args[1]);
			break;
		}
		default:break;
	}
}

void led_test(float flag){
	if((int)flag == 0){
		HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
	}
	if((int)flag == 1){
		HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);
	}
}