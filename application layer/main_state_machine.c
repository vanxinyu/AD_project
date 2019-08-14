#include "main_state_machine.h"
#include "usart.h"
#include "string.h"
#include "freertos.h"
#include "BY_log.h"

QueueHandle_t FSM_Simple_Queue;

typedef enum{
	STATE_INIT = 0,
	STATE_SELECTING,
	STATE_CONTINUE,
} State_t;

typedef int (* ESP_prompt_cb_t)(uint8_t temp);
typedef struct{
	const char* head;
	uint8_t isContinue;
	const ESP_prompt_cb_t cb;
}ESP_prompt_head_item_t;

static int Callback_WRITE(uint8_t temp);
static int Callback_READ(uint8_t temp);


static const ESP_prompt_head_item_t ESP_prompt_head[] = 
{
	{"AT+WRITE",                       0, Callback_WRITE},
	{"AT+READ",                        0, Callback_READ},
};
static const int ESP_promt_head_size = sizeof( ESP_prompt_head ) / sizeof( ESP_prompt_head[0] );

typedef struct{
	uint8_t size;
	uint8_t data[ESP_promt_head_size];
} ESP_record_t;

static void FSM_simple_state_notify(FSM_simple_state_type_t type)
{
	if(uxQueueMessagesWaiting(FSM_Simple_Queue)==FSM_Simple_Queue_LEN)
	{
		xQueueReset(FSM_Simple_Queue);
		log_warn("FSM_Simple_Queue is full, so clear it!!!");
	}

	FSM_simple_t FSM_simple_state;
	FSM_simple_state.type = type;

	BaseType_t err;
	err = xQueueSend(FSM_Simple_Queue, &FSM_simple_state, 0);
	if(err != pdPASS)
	{
		log_warn("unexpectedly fail to send FSM_Simple_Queue");
	}
}

static int Callback_WRITE(uint8_t temp)
{
	FSM_simple_state_notify(FSM_OK);

	return ABORT;
}
static int Callback_READ(uint8_t temp)
{
	FSM_simple_state_notify(FSM_ERROR);

	return ABORT;
}
void USART_FSM_Init(void)
{
	FSM_Simple_Queue = xQueueCreate(FSM_Simple_Queue_LEN, sizeof(FSM_simple_t));
}
void USART_StateMachine(uint8_t temp)
{
	static State_t state = STATE_INIT;
	static int char_index = 0;
	static ESP_record_t record = {0};

	int str_index;
	int record_index;

	int record_count = 0;

	switch(state)
	{
		case STATE_INIT:
		{
			for(str_index=0; str_index<ESP_promt_head_size; str_index++)
			{
				if(ESP_prompt_head[str_index].head[char_index] == temp) record.data[record_count++] = str_index; // matched
			}
			record.size = record_count;
			goto HADNLE_RECORD;
		}

		case STATE_SELECTING:
		{
			for(record_index=0; record_index<record.size; record_index++)
			{
				str_index = record.data[record_index];
				if(ESP_prompt_head[str_index].head[char_index] == temp) record.data[record_count++] = str_index; // matched
			}
			record.size = record_count;
			goto HADNLE_RECORD;
		}

		case STATE_CONTINUE:
		{
			if(ESP_prompt_head[record.data[0]].cb(temp) == CONTINUE)
			{
				char_index++; // continue
				return;
			}
			goto BACK_TO_INIT; // abort, finish
		}
	}

HADNLE_RECORD:
	if(record.size==0) // get no match
	{
		goto BACK_TO_INIT;
	}
	else if((record.size==1) && (ESP_prompt_head[record.data[0]].head[char_index+1] == '\0')) // get 1 match
	{
		if(ESP_prompt_head[record.data[0]].isContinue)
		{
			char_index++;
			state = STATE_CONTINUE;
			return;
		}
		else
		{
			if(ESP_prompt_head[record.data[0]].cb == NULL)
				log_warn("the callback is NULL");
			else
				ESP_prompt_head[record.data[0]].cb(temp);

			goto BACK_TO_INIT;
		}
	}
	else // get several matches
	{
		char_index++;
		state = STATE_SELECTING; // need to compare the next character
		return;
	}

BACK_TO_INIT:
	char_index = 0;
	state = STATE_INIT;
}
