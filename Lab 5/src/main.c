#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

TimerHandle_t timer1, timer2;

void timerCallbackFunction( TimerHandle_t xTimer ){
    TickType_t xTimeNow;
    uint32_t executionCount;

    executionCount = ( uint32_t ) pvTimerGetTimerID( xTimer );
	executionCount++;
	vTimerSetTimerID( xTimer, ( void * ) executionCount );

	// Obtain the current tick count
	xTimeNow = xTaskGetTickCount();

	if( xTimer == timer1 )
	{
		printf("%d: ahihi\n", xTimeNow);
        if( executionCount == 10 )
		{
			xTimerStop( xTimer, 0 );
		}
	}
	else if( xTimer == timer2)
	{
		printf("%d: ihaha\n", xTimeNow);
        if( executionCount == 5 )
		{
			xTimerStop( xTimer, 0 );
		}
	}
}

void app_main(void)
{
    timer1 = xTimerCreate( "Timer 1", pdMS_TO_TICKS( 2000UL ), pdTRUE, 0, timerCallbackFunction );
    timer2 = xTimerCreate( "Timer 2", pdMS_TO_TICKS( 3000UL ), pdTRUE, 0, timerCallbackFunction ); 

    if(timer1 != NULL && timer2 != NULL){
        xTimerStart( timer1, 0 );
        xTimerStart( timer2, 0 );
    }
}