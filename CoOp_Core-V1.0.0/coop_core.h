/*
------------------------------------------------------------------------------
~ File   : coop_core.h
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

#ifndef __COOP_CORE_H_
#define __COOP_CORE_H_

/*----------------------------------------------------------*/
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Include ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include <stdint.h>
#include "coop_core_conf.h"

/* ------------------------------------------------------------------ */

#ifdef __CODEVISIONAVR__  /* Check compiler */

#pragma warn_unref_func- /* Disable 'unused function' warning */

/* ------------------------------------------------------------------ */

#elif defined(__GNUC__) && !defined(USE_HAL_DRIVER)  /* Check compiler */

#pragma GCC diagnostic ignored "-Wunused-function" /* Disable 'unused function' warning */

/* ------------------------------------------------------------------ */

#elif defined(USE_HAL_DRIVER)  /* Check driver */

	#if defined ( __ICCARM__ ) /* ICCARM Compiler */

	#pragma diag_suppress=Pe177   /* Disable 'unused function' warning */

	#elif defined   (  __GNUC__  ) /* GNU Compiler */

	#pragma diag_suppress 177     /* Disable 'unused function' warning */

	#endif /* __ICCARM__ */

/* ------------------------------------------------------------------ */

#endif /* __CODEVISIONAVR__ */

/* ------------------------------------------------------------------ */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ------------------------ Library ------------------------ */
#define _COOP_CORE_LIBRARY_VERSION 0.0.0

/* ------------------------ Timing ------------------------- */
/* :::::::::: u second :::::::::: */

/* :::::::::: m second :::::::::: */

/* ------------------------ Public ------------------------- */

/* ---------------------- By compiler ---------------------- */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
typedef void(*cTaskFunc_TypeDef)(uint8_t *argument);

typedef enum 
{
	
	cTaskRunning   = 0x00,
	cTaskWait      = 0x01,
	cTaskSuspended = 0x02,
	cTaskDeleted   = 0x03
	
}cTaskState;

typedef struct 
{
	
	uint8_t               Instances;
	uint8_t               RunStep;
	uint16_t              Delay;
	
	cTaskFunc_TypeDef     Function;
	volatile cTaskState   State;
	
}cTask_TypeDef;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* :::::::::::::::::: Initialize :::::::::::::::::: */
uint8_t Task_RegisterNew(cTask_TypeDef *uTask, cTaskFunc_TypeDef uFunc);

/* :::::::::::::::: Get Parameter ::::::::::::::::: */
uint8_t Task_GetStepParam(uint8_t *paramIn);

/* :::::::::::::::: Set Parameter ::::::::::::::::: */
void Task_SetNextStepParam(uint8_t nexStep);

/* :::::::::::::::::: Task Timing ::::::::::::::::: */
void Task_IncTick(void);
void Task_Delay(uint16_t tim, uint8_t step);

/* ::::::::::::::::: Task Control ::::::::::::::::: */
void Task_Resume(cTask_TypeDef *uTask);
void Task_ResumeAll(void);

/* .... Task Suspend .... */
void Task_Suspend(cTask_TypeDef *uTask);
void Task_SuspendAll(void);

/* .... Task Delete .... */
void Task_Delete(cTask_TypeDef *uTask);
void Task_DeleteAll(void);

/* ::::::::::::::::::: Task Run ::::::::::::::::::: */
void Task_RunCoOpSystem(void);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#endif /* __COOP_CORE_H_ */
