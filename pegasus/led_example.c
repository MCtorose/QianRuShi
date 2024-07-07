
#include <stdio.h>
#include <unistd.h>

#include "ohos_init.h"
#include "cmsis_os2.h"
#include "iot_gpio.h"

#define LED_INTERVAL_TIME_US 300000
#define LED_TASK_STACK_SIZE 512
#define LED_TASK_PRIO 25
#define LED_TEST_GPIO 9 // for hispark_pegasus

static long long g_iState = 0;

enum LedState {
   LED_OFF=0, 
   LED_ON = 1, 
    LED_SPARK,
};

enum LedState g_ledState = LED_SPARK;

static void *LedTask(const char *arg)
{
    (void)arg;
    while (1) {
        switch (g_ledState) {
            case LED_ON:
                IoTGpioSetOutputVal(LED_TEST_GPIO, 1);
                usleep(LED_INTERVAL_TIME_US);//delay
                g_iState++;
                break;
            case LED_OFF:
                IoTGpioSetOutputVal(LED_TEST_GPIO, 0);//0是不亮
                usleep(LED_INTERVAL_TIME_US);
                g_iState++;
                break;
            case LED_SPARK:
                IoTGpioSetOutputVal(LED_TEST_GPIO, 1);
                usleep(LED_INTERVAL_TIME_US);
                IoTGpioSetOutputVal(LED_TEST_GPIO, 0);
                usleep(LED_INTERVAL_TIME_US);
                g_iState++;
                break;
            default:
                usleep(LED_INTERVAL_TIME_US);
                break;
        }
        if (g_iState == 15) {
            g_iState = 0;
            break;
        }
    }
    return NULL;
}

static void LedExampleEntry(void)
{
    osThreadAttr_t attr;

    IoTGpioInit(LED_TEST_GPIO);
    IoTGpioSetDir(LED_TEST_GPIO, IOT_GPIO_DIR_OUT);

    attr.name = "LedTask";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = LED_TASK_STACK_SIZE;
    attr.priority = LED_TASK_PRIO;

    if (osThreadNew((osThreadFunc_t)LedTask, NULL, &attr) == NULL) {
        printf("[LedExample] Falied to create LedTask!\n");
    }
}

SYS_RUN(LedExampleEntry);//main



