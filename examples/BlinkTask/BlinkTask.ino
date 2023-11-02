#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>

static xSemaphoreHandle sem;

static void Thread1(void* arg)
{
    while (true) {


        // Wait for signal from thread 2.
        xSemaphoreTake(sem, portMAX_DELAY);

        digitalWrite(LED_BUILTIN, LOW);
    }
}

static void Thread2(void* arg)
{
    while (true) {

        digitalWrite(LED_BUILTIN, HIGH);

        vTaskDelay((200L * configTICK_RATE_HZ) / 1000L);

        xSemaphoreGive(sem);

        vTaskDelay((200L * configTICK_RATE_HZ) / 1000L);
    }
}


void setup()
{
    Serial.begin(115200);

    pinMode(LED_BUILTIN, OUTPUT);

    // initialize semaphore
    sem = xSemaphoreCreateCounting(1, 0);

    // create task at priority two
    auto s1 = xTaskCreate(Thread1, NULL, configMINIMAL_STACK_SIZE, NULL, 2, NULL);

    // create task at priority one
    auto s2 = xTaskCreate(Thread2, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    // check for creation errors
    if (sem== NULL || s1 != pdPASS || s2 != pdPASS ) {
        Serial.println("Creation problem");
        while(1);
    }

    Serial.println("Starting the scheduler !");

    // start scheduler
    vTaskStartScheduler();
    Serial.println("Insufficient RAM");
    while(1);
}

void loop()
{
}
