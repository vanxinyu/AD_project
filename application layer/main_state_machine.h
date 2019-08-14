#ifndef _MAIN_STATE_MACHINE_H_
#define _MAIN_STATE_MACHINE_H_
#include "cmsis_os.h"

#define ABORT     0
#define CONTINUE  1

typedef enum {
	FSM_UNKNOW,
	FSM_OK,
	FSM_ERROR,
	FSM_RECV,
} FSM_simple_state_type_t;

typedef struct{
	FSM_simple_state_type_t type;
} FSM_simple_t;


#define FSM_Simple_Queue_LEN  2

extern QueueHandle_t FSM_Simple_Queue;
extern QueueHandle_t IPD_Queue;
extern QueueHandle_t IPD_buffer_Queue;

void USART_FSM_Init(void);
void USART_StateMachine(uint8_t temp);

#endif
