
/* Hardware specific includes. */
#include "../iodefine.h"

/* Kernel includes. */
#include "../FreeRTOS.h"
#include "../task.h"
#include "../queue.h"



/* If configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h, then this
function will be called if pvPortMalloc() returns NULL because it has exhausted
the available FreeRTOS heap space.  See http://www.freertos.org/a00111.html. */
void vApplicationMallocFailedHook( void )
{
	for( ;; );
}
/*-----------------------------------------------------------*/

/* If configCHECK_FOR_STACK_OVERFLOW is set to either 1 or 2 in
FreeRTOSConfig.h, then this function will be called if a task overflows its
stack space.  See
http://www.freertos.org/Stacks-and-stack-overflow-checking.html. */
void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	for( ;; );
}
/*-----------------------------------------------------------*/

/* If configUSE_IDLE_HOOK is set to 1 in FreeRTOSConfig.h, then this function
will be called on each iteration of the idle task.  See
http://www.freertos.org/a00016.html */
void vApplicationIdleHook( void )
{
	/* If this is being executed then the kernel has been started.  Start the high
	frequency timer test as described at the top of this file.  This is only
	included in the optimised build configuration - otherwise it takes up too much
	CPU time and can disrupt other tests. */
}
/*-----------------------------------------------------------*/

#if 0
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

/* The RX port uses this callback function to configure its tick interrupt.
This allows the application to choose the tick interrupt source. */
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
#endif
/*-----------------------------------------------------------*/