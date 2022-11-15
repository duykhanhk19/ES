    #include <stdio.h>
    #include "freertos/FreeRTOS.h"
    #include "freertos/FreeRTOSConfig.h"
    #include "freertos/task.h"

    volatile uint32_t ulIdleCycleCount = 0UL;
    volatile uint32_t task1Counter = 0UL;
    volatile uint32_t task2Counter = 0UL;
    volatile uint32_t task3Counter = 0UL;

    void vApplicationTickHook( void ){}

    void vApplicationIdleHook( void )
    {
            ulIdleCycleCount++;
    }

    static inline void printStatus(){
        printf("--- Task 1:\t%d\n", task1Counter);
        printf("--- Task 2:\t%d\n", task2Counter);
        printf("--- Task 3:\t%d\n", task3Counter);
        printf("--- Task Idle:\t%d\n", ulIdleCycleCount);
        printf("--- Core: \t%d\n",xPortGetCoreID());
    }

    void task_1(void *pvParameter)
    {
        printf("----------Task 1 start----------\n");
        printStatus();
        printf("+++ 1 \n");
        while (1)
        {
            task1Counter++;
            if(!configUSE_PREEMPTION && !configUSE_TIME_SLICING){
                if(task1Counter > 1000000){
                    printf("----------task 1 end----------\n");
                    vTaskDelete(NULL);
                }
            }
        }
    }

    void task_2(void *pvParameter)
    {
        printf("----------Task 2 start----------\n");
        printStatus();
        printf("+++ 2 \n");
        while (1)
        {
            task2Counter++;
            if(!configUSE_PREEMPTION && !configUSE_TIME_SLICING){
                if(task2Counter > 1000000){
                    printf("----------task 2 end----------\n");
                    vTaskDelete(NULL);
                }
            }
        }
    }

    void task_3(void *pvParameter)
    {
        printf("----------Task 3 start----------\n");
        printStatus();
        printf("+++ 3 \n");
        while(1){
            task3Counter++;
            if(task3Counter >= 1000000){
                printf("----------task 3 end----------\n");
                vTaskDelete(NULL);
            }
        }
    }

    void task_monitor(void *pvParameter)
    {
        int counter = 0;
        while (1)
        {
            counter++;
            if(!(counter % 100000)){
                printStatus();
            }
        }
    }

    void app_main()
    {
        printf("Configuration: \n");
        printf("configUSE_PREEMPTION : %d \n",configUSE_PREEMPTION);
        printf("configUSE_TIME_SLICING : %d \n",configUSE_TIME_SLICING);
        printf("----------------------------------------------------\n");
        if(configUSE_PREEMPTION && configUSE_TIME_SLICING){
            printf("\tPrioritized Pre-emptive Scheduling with Time Slicing\n");
        }
        else if(configUSE_PREEMPTION && !configUSE_TIME_SLICING){
            printf("\tPrioritized Pre-emptive Scheduling without Time Slicing\n");
        }
        else if(!configUSE_PREEMPTION && !configUSE_TIME_SLICING){
            printf("\tCo-operative Scheduling\n");
        }
        printf("----------------------------------------------------\n");

        xTaskCreatePinnedToCore(&task_monitor,"task_monitor",1024*5,NULL,3,NULL,1);
        xTaskCreatePinnedToCore(&task_1,"task 1",1024*5,NULL,0,NULL,0);
        xTaskCreatePinnedToCore(&task_2,"task 2",1024*5,NULL,0,NULL,0);
        vTaskDelay(pdMS_TO_TICKS(500));
        xTaskCreatePinnedToCore(&task_3,"task 3",1024*5,NULL,5,NULL,0);
    }


