#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/queue.h"
#include <stdlib.h>

typedef struct {
	uint8_t Id;
	int32_t Data;
} Data_t;

QueueHandle_t xQueueTask1, xQueueTask2;

static void vTaskReception(void * pvParameters) {
	BaseType_t xTaskStatus;
	const TickType_t xTicksToWait = pdMS_TO_TICKS(100UL);
   Data_t *requestPtr = (Data_t*) malloc(sizeof(Data_t));;
	while(1) {
      requestPtr->Id = rand() % 3 + 1;  
      requestPtr->Data = rand() % 10 + 10;
      printf("Reception receive new request: ID: %d - Data: %d\r\n", requestPtr->Id, requestPtr->Data);

      if (requestPtr->Id == 1) xTaskStatus = xQueueSendToBack(xQueueTask1, requestPtr, xTicksToWait);
      else if (requestPtr->Id == 2) xTaskStatus = xQueueSendToBack(xQueueTask2, requestPtr, xTicksToWait);
      else{
         xTaskStatus = pdFAIL;
         printf("Reception cannot execute this request!\r\n");
      }

		if(xTaskStatus == pdPASS) {
			printf("Reception have executed this request!\r\n");
		}
      vTaskDelay(pdMS_TO_TICKS(100));
	} 
}

static void vTask1(void * pvParameters) {

	Data_t requestReceived;
	BaseType_t xTaskStatus;
	while(1) {
		xTaskStatus = xQueueReceive(xQueueTask1, &requestReceived, 0);
		// Check if received task success or not
		if(xTaskStatus == pdPASS) {
			printf("--- From Task 1, received request: Id = %d, Data = %d\r\n", requestReceived.Id, requestReceived.Data);
		}
	}
}

static void vTask2(void * pvParameters) {

	Data_t requestReceived;
	BaseType_t xTaskStatus;
	while(1) {
		xTaskStatus = xQueueReceive(xQueueTask2, &requestReceived, 0);
		// Check if received task success or not
		if(xTaskStatus == pdPASS) {
			printf("--- From Task 2, received request: Id = %d, Data = %d\r\n", requestReceived.Id, requestReceived.Data);
		}
	}
}

void app_main(void) {

	// Create queue
	xQueueTask1 = xQueueCreate(5, sizeof(Data_t));
   xQueueTask2 = xQueueCreate(5, sizeof(Data_t));

	// Check queue create success or not
	if(xQueueTask1 != NULL && xQueueTask2 != NULL) {

		xTaskCreate(vTaskReception, "Reception task", 1024*5, NULL, 2, NULL);
		xTaskCreate(vTask1, "Task 1", 1024*5, NULL, 1, NULL);
		xTaskCreate(vTask2, "Task 2", 1024*5, NULL, 1, NULL);
	}
}