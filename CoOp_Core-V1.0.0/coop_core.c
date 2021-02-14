/*
------------------------------------------------------------------------------
~ File   : coop_core.c
~ Author : Majid Derhambakhsh
~ Version: V1.0.0
~ Created: 02/14/2021 06:00:00 AM
~ Brief  :
~ Support:
		   E-Mail : Majid.do16@gmail.com (subject : Embedded Library Support)

		   Github : https://github.com/Majid-Derhambakhsh
------------------------------------------------------------------------------
~ Description:

~ Attention  :

~ Changes    :
------------------------------------------------------------------------------
*/

#include "coop_core.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static cTask_TypeDef *TaskList[_COOP_NMB_OF_TASK];

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static struct cTaskControl_T
{
	
	uint8_t          TaskCounter;
	
	volatile uint8_t NMBOfRegisteredTasks;
	volatile uint8_t TickCounter;
	
}TaskControl;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* :::::::::::::::::: Initialize :::::::::::::::::: */
uint8_t Task_RegisterNew(cTask_TypeDef *uTask, cTaskFunc_TypeDef uFunc)
{
	
	if (TaskControl.NMBOfRegisteredTasks == _COOP_NMB_OF_TASK)
	{
		return 0;
	}
	else
	{
		/* Set the task parameters */
		TaskList[TaskControl.NMBOfRegisteredTasks] = uTask;
		
		TaskList[TaskControl.NMBOfRegisteredTasks]->Instances = TaskControl.NMBOfRegisteredTasks;
		TaskList[TaskControl.NMBOfRegisteredTasks]->Delay     = 0;
		TaskList[TaskControl.NMBOfRegisteredTasks]->State     = cTaskRunning;
		TaskList[TaskControl.NMBOfRegisteredTasks]->RunStep   = 0;
		TaskList[TaskControl.NMBOfRegisteredTasks]->Function  = uFunc;
		
		/* ....................... */
		TaskControl.NMBOfRegisteredTasks++;
		
		return 1;
	}
	
}

/* :::::::::::::::: Get Parameter ::::::::::::::::: */
uint8_t Task_GetStepParam(uint8_t *paramIn)
{
	return *paramIn;
}

/* :::::::::::::::: Set Parameter ::::::::::::::::: */
void Task_SetNextStepParam(uint8_t nexStep)
{
	TaskList[TaskControl.TaskCounter]->RunStep = nexStep;
}

/* :::::::::::::::::: Task Timing ::::::::::::::::: */
void Task_IncTick(void)
{
	
	for (TaskControl.TickCounter = 0; TaskControl.TickCounter < TaskControl.NMBOfRegisteredTasks; TaskControl.TickCounter++)
	{
		
		if (TaskList[TaskControl.TickCounter]->State == cTaskWait)
		{
			
			if (TaskList[TaskControl.TickCounter]->Delay)
			{
				TaskList[TaskControl.TickCounter]->Delay--;
			}
			
		}
		
	}
	
}

void Task_Delay(uint16_t tim, uint8_t step)
{
	/* Set task parameters */
	if (TaskList[TaskControl.TaskCounter]->State == cTaskRunning)
	{
		
		TaskList[TaskControl.TaskCounter]->Delay   = tim;
		TaskList[TaskControl.TaskCounter]->State   = cTaskWait;
		TaskList[TaskControl.TaskCounter]->RunStep = step;
		
	}
	
}

/* ::::::::::::::::: Task Control ::::::::::::::::: */
/* .... Task Resume .... */
void Task_Resume(cTask_TypeDef *uTask)
{
	uTask->State = cTaskRunning;
}
void Task_ResumeAll(void)
{
	
	uint8_t tCounter = 0;
	
	/* ~~~~~~~~~~ Suspend tasks ~~~~~~~~~~ */
	for (; tCounter < TaskControl.NMBOfRegisteredTasks; tCounter++)
	{
		TaskList[tCounter]->State = cTaskRunning;
	}
	
}

/* .... Task Suspend .... */
void Task_Suspend(cTask_TypeDef *uTask)
{
	uTask->State = cTaskSuspended;
}
void Task_SuspendAll(void)
{
	
	uint8_t tCounter = 0;
	
	/* ~~~~~~~~~~ Suspend tasks ~~~~~~~~~~ */
	for (; tCounter < TaskControl.NMBOfRegisteredTasks; tCounter++)
	{
		TaskList[tCounter]->State = cTaskSuspended;
	}
	
}

/* .... Task Delete .... */
void Task_Delete(cTask_TypeDef *uTask)
{
	
	uint8_t tCounter = uTask->Instances;
	
	/* ~~~~~~~~~~ Check availability ~~~~~~~~~~ */
	if (uTask->State != cTaskDeleted)
	{
		
		/* ~~~~~~~~~~ Delete task ~~~~~~~~~~ */
		for (; tCounter < (TaskControl.NMBOfRegisteredTasks - 1); tCounter++)
		{
			TaskList[tCounter] = TaskList[tCounter + 1];
			
			/* .... Set new instances .... */
			TaskList[tCounter]->Instances = tCounter;
			
		}
		
		TaskControl.NMBOfRegisteredTasks--;
		
		/* Set new state */
		uTask->State = cTaskDeleted;
		
	}
	
}
void Task_DeleteAll(void)
{
	TaskControl.NMBOfRegisteredTasks = 0;
}

/* ::::::::::::::::::: Task Run ::::::::::::::::::: */
void Task_RunCoOpSystem(void)
{
	
	while (1)
	{
		
		for (TaskControl.TaskCounter = 0; TaskControl.TaskCounter < TaskControl.NMBOfRegisteredTasks; TaskControl.TaskCounter++)
		{
			
			switch (TaskList[TaskControl.TaskCounter]->State)
			{
				
				case cTaskRunning:
				{
					TaskList[TaskControl.TaskCounter]->Function(&TaskList[TaskControl.TaskCounter]->RunStep);
				}
				break;
				case cTaskWait:
				{
					
					if (TaskList[TaskControl.TaskCounter]->Delay == 0)
					{
						
						TaskList[TaskControl.TaskCounter]->State = cTaskRunning;
						TaskList[TaskControl.TaskCounter]->Function(&TaskList[TaskControl.TaskCounter]->RunStep);
						
					}
					
				}
				break;
				case cTaskSuspended:
				break;
				case cTaskDeleted:
				break;
				
			}
			
		}
		
	}
	
}

