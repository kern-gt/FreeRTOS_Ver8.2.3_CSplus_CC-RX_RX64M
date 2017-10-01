/**~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
＊　ファイル名： main_apl.c
＊　責務　　　： ユーザmain関数の定義
＊　作成日　　： 2017/10/1
＊　作成者　　： kern-gt

＊　コンパイラ：CC-RX

＊　備考：
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~**/
/**----------------------------------------------------------------------------
<<利用ファイルのヘッダ>>
-----------------------------------------------------------------------------**/
/*#include "r_cg_macrodriver.h"*/
/*#include "r_cg_userdefine.h"*/
#include <stdio.h>
#include "iodefine.h"

/* Kernel includes. */
#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/task.h"
#include "FreeRTOS/queue.h"

/**----------------------------------------------------------------------------
<<自ファイルのヘッダ>>
-----------------------------------------------------------------------------**/
#include "main_apl.h"

/***タスク定義*******************************************************************/
void vTask1(void *pvParameters)
{
	while(1) {
		//PORTD.DR.BIT.B1 = ~PORTD.DR.BIT.B1;
		vTaskDelay(1000/portTICK_PERIOD_MS);
	}
}

void vTask2(void *pvParameters)
{
	while(1) {
		//PORTD.DR.BIT.B2 = ~PORTD.DR.BIT.B2;
		vTaskDelay(2000/portTICK_PERIOD_MS);
	}
}

/***configASSERT()定義*******************************************************************/
void vAssertCalled( void )
{
	volatile unsigned long ul = 0;

	taskENTER_CRITICAL();
	{
		/* Use the debugger to set ul to a non-zero value in order to step out
		of this function to determine why it was called. */
		while( ul == 0 )
		{
			portNOP();
		}
	}
	taskEXIT_CRITICAL();
}

/***カーネルタイマ初期設定*******************************************************************/
/*
	カーネルタイマ（OSタイマ）にはCMT0を使用している。
	スケジューラ起動時にこのコールバック関数が呼ばれタイマの周期設定後、タイマを起動する。
	また、FreeRTOS/portable/port.cにOStickハンドラとしてCMT0の割込みハンドラを定義しているので、
	コード生成ツールを使用する場合はCMT0を使用しないようにする。
	以下の関数はデモ用のサンプルからそのまま利用している。
	
	 The RX port uses this callback function to configure its tick interrupt.
	This allows the application to choose the tick interrupt source. 
*/
void vApplicationSetupTimerInterrupt( void )
{
	const uint32_t ulEnableRegisterWrite = 0xA50BUL, ulDisableRegisterWrite = 0xA500UL;

	/* Disable register write protection. */
	SYSTEM.PRCR.WORD = ulEnableRegisterWrite;

	/* Enable compare match timer 0. */
	MSTP( CMT0 ) = 0;

	/* Interrupt on compare match. */
	CMT0.CMCR.BIT.CMIE = 1;

	/* Set the compare match value. */
	CMT0.CMCOR = ( unsigned short ) ( ( ( configPERIPHERAL_CLOCK_HZ / configTICK_RATE_HZ ) -1 ) / 8 );

	/* Divide the PCLK by 8. */
	CMT0.CMCR.BIT.CKS = 0;

	/* Enable the interrupt... */
	_IEN( _CMT0_CMI0 ) = 1;

	/* ...and set its priority to the application defined kernel priority. */
	_IPR( _CMT0_CMI0 ) = configKERNEL_INTERRUPT_PRIORITY;

	/* Start the timer. */
	CMT.CMSTR0.BIT.STR0 = 1;

    /* Reneable register protection. */
    SYSTEM.PRCR.WORD = ulDisableRegisterWrite;
}
/*-----------------------------------------------------------*/

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
＊　関数名　： MainApplication
＊　機能　　： ユーザmain関数
＊　引数　　： 
＊　戻り値　： 
＊　備考　　： 呼び出し元は生成コードのr_cg_main.c/void main(void);
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~**/
void MainApplication(void)
{
	/*タスク生成*/
	xTaskCreate(vTask1,"Task1",100,NULL,1,NULL);
	xTaskCreate(vTask2,"Task2",100,NULL,1,NULL);
	
	/* タスクスケジューラ起動*/
	vTaskStartScheduler();

	/*ここには戻ってこないはず*/
	while(1);
}