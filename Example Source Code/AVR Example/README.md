# CoOp-Core Example
The example of Co Operative Multitasking

> #### Download Library : [CoOp-Core](https://github.com/Majid-Derhambakhsh/CoOp-Core)  

- #### Type : Embedded Software

- #### MCU  : ATMEGA16A

- #### IDE  : Atmel Studio

- #### Program Language : C

- #### Required Library/Driver :  [CoOp-Core](https://github.com/Majid-Derhambakhsh/CoOp-Core)  

- #### Description : this example has four Task
  - Task1: Toggle PIN A.0 with 1000ms delay
  - Task2: Toggle PIN A.1 with 500ms delay
  - Task3: Write High/Low on PIN A.2 with 50ms delay
  - Task4: Suspend the Task1 & Task4 after 20s

- #### Code :
```c++  
#include <avr/io.h>
#include <avr/interrupt.h>

#include "User Library/coop_core.h"

/* ..... User Defines ..... */
#define GPIO_WriteHigh(GPIO, PIN) ((GPIO) |= (1 << PIN))
#define GPIO_WriteLow(GPIO, PIN)  ((GPIO) &= ~(1 << PIN))
#define GPIO_Toggle(GPIO, PIN)    ((GPIO) ^= (1 << PIN))

/* ..... User Tasks ..... */
cTask_TypeDef Task1;
cTask_TypeDef Task2;
cTask_TypeDef Task3;
cTask_TypeDef Task4;

/* ..... Prototypes ..... */
void Task1Func(uint8_t *argument);
void Task2Func(uint8_t *argument);
void Task3Func(uint8_t *argument);
void Task4Func(uint8_t *argument);

void GPIO_Init(void);
void SysTickTimer_Init(void);

int main(void)
{
	
	/* HW Init */
	__asm__ ("sei"); // Enable Global Interrupt
	
	GPIO_Init();
	SysTickTimer_Init();
	
	/* Register User Tasks */
	Task_RegisterNew(&Task1, Task1Func);
	Task_RegisterNew(&Task2, Task2Func);
	Task_RegisterNew(&Task3, Task3Func);
	Task_RegisterNew(&Task4, Task4Func);
	
	/* Run CoOp System */
	Task_RunCoOpSystem();
	
	while (1) 
	{
	}
}

/* ..... User Tasks Code ..... */
void Task1Func(uint8_t *argument)
{
	GPIO_Toggle(PORTA, 0);
	Task_Delay(1000, 0); // 1000ms Delay with step 0
}

void Task2Func(uint8_t *argument)
{
	GPIO_Toggle(PORTA, 1);
	Task_Delay(500, 0); // 500ms Delay with step 0
}

void Task3Func(uint8_t *argument)
{
	switch (*argument)
	{
		case 0: // Step 0 is default
		{
			GPIO_WriteHigh(PORTA, 2);
			Task_Delay(50, 1); // Wait 50ms and goto step 1 (case 1)
		}
		break;
		case 1: // Step 1
		{
			GPIO_WriteLow(PORTA, 2);
			Task_Delay(50, 0); // Wait 50ms and goto step 0 (case 0)
		}
		break;
		
		default:
		break;
	}
	
}

void Task4Func(uint8_t *argument)
{

	switch (*argument)
	{
		case 0: // Step 0 is default
		{
			Task_Delay(20000, 1); // Wait 20s and goto step 1 (case 1)
		}
		break;
		case 1: // Step 1
		{
			Task_Suspend(&Task1); // Suspend Task1
			Task_Suspend(&Task4); // Suspend Task4
		}
		break;
		
		default:
		break;
	}

}

/* ..... GPIO initialize ..... */
void GPIO_Init(void)
{
	
	DDRA = 0x07; // Set PA.0 ~ PA.2 As Digital Output
	
}

/* ..... Timer initialize ..... */
void SysTickTimer_Init(void)
{
	
	/* Set counter value for 1ms:
		
		F_TIMER    = (CPU_CLK / TIMER_PRE)
		COUNT_TIME = (1 / F_TIMER)
		OVF_TIME   = COUNT_TIME * (Counter_Period - 1)
		
		F_TIMER    = (1MHz / 8) = 125,000Hz
		COUNT_TIME = (1 / 125,000) = 0.000008
		OVF_TIME   = 1ms = (0.001 / 0.000008) - 1 = 124
		
		COUNTER_PERIOD = 255 - 124 = 131
		
	*/
	
	TCNT0 = 131;  // Set Counter Period
	TIMSK = 0x01; // Enable OVF Interrupt
	TCCR0 = 0x02; // Set PRE to 8
	
}

/* ..... Timer 0 overflow interrupt ..... */
ISR(TIMER0_OVF_vect)
{
	TCNT0 = 131;  // Set Counter Period
	
	Task_IncTick();
	
}
   
``` 

#### Developer: Majid Derhambakhsh

