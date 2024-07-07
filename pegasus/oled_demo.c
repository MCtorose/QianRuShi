#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "ohos_init.h"
#include "cmsis_os2.h"
#include "iot_i2c.h"
#include "iot_gpio.h"
#include "iot_errno.h"

#include "oled_ssd1306.h"
#include "US.h"
#include "sample_ive_main.h"

#define AHT20_BAUDRATE (400 * 1000)
#define AHT20_I2C_IDX 0

//延时函数
void delay(int count)
{
	int i;
	for(i=1;i<=count;i++);
}

static void OledmentTask(const char *arg)
{
    int i = 2; int j;
    (void)arg;
    OledInit();
    OledFillScreen(0);
    IoTI2cInit(AHT20_I2C_IDX, AHT20_BAUDRATE);
    i = GetDistance(void);
    j = SAMPLE_IVE_Kcf(HI_VOID);
    while (1)
    {
        switch (SAMPLE_IVE_Kcf(HI_VOID))
        {
        case 1: 
            {
            switch (i)
               {
            case 6:
                OledShowString(1, 2, "distance:", 1); /* 屏幕第1列2行显示1行 */
                OledShowString(1, 3, "         6", 1);
                OledShowString(1, 4, "car", 1); /* 屏幕第1列4行显示1行 */
                break:
            case 5:
                OledShowString(1, 2, "distance:", 1); /* 屏幕第1列2行显示1行 */
                OledShowString(1, 3, "         5", 1);
                OledShowString(1, 4, "car", 1); /* 屏幕第1列4行显示1行 */
                break:
            case 4:
                OledShowString(1, 2, "distance:", 1); /* 屏幕第1列2行显示1行 */
                OledShowString(1, 3, "         4", 1);
                OledShowString(1, 4, "car", 1); /* 屏幕第1列4行显示1行 */
                break:
            case 3:
                OledShowString(1, 2, "distance:", 1); /* 屏幕第1列2行显示1行 */
                OledShowString(1, 3, "         3", 1);
                OledShowString(1, 4, "car", 1); /* 屏幕第1列4行显示1行 */ /
                    break:
            case 2:
                OledShowString(1, 2, "distance:", 1); /* 屏幕第1列2行显示1行 */
                OledShowString(1, 3, "         2", 1);
                OledShowString(1, 4, "car", 1); /* 屏幕第1列4行显示1行 */
                break:
            default:
                OledShowString(1, 2, "!!!       ", 1); /* 屏幕第1列2行显示1行 */
                OledShowString(1, 3, "!!!        ", 1);
                OledShowString(1, 4, "!!!      ", 1); /* 屏幕第1列4行显示1行 */
            }
        }
        case 0:
        {
            switch (i)
            {
            case 6:
                OledShowString(1, 2, "distance:", 1); /* 屏幕第1列2行显示1行 */
                OledShowString(1, 3, "         6", 1);
                OledShowString(1, 4, "car", 1); /* 屏幕第1列4行显示1行 */
                break:
            case 5:
                OledShowString(1, 2, "distance:", 1); /* 屏幕第1列2行显示1行 */
                OledShowString(1, 3, "         5", 1);
                OledShowString(1, 4, "car", 1); /* 屏幕第1列4行显示1行 */
                break:
            case 4:
                OledShowString(1, 2, "distance:", 1); /* 屏幕第1列2行显示1行 */
                OledShowString(1, 3, "         4", 1);
                OledShowString(1, 4, "car", 1); /* 屏幕第1列4行显示1行 */
                break:
            case 3:
                OledShowString(1, 2, "distance:", 1); /* 屏幕第1列2行显示1行 */
                OledShowString(1, 3, "         3", 1);
                OledShowString(1, 4, "car", 1); /* 屏幕第1列4行显示1行 */ /
                    break:
            case 2:
                OledShowString(1, 2, "distance:", 1); /* 屏幕第1列2行显示1行 */
                OledShowString(1, 3, "         2", 1);
                OledShowString(1, 4, "car", 1); /* 屏幕第1列4行显示1行 */
                break:
            default:
                OledShowString(1, 2, "!!!       ", 1); /* 屏幕第1列2行显示1行 */
                OledShowString(1, 3, "!!!        ", 1);
                OledShowString(1, 4, "!!!      ", 1); /* 屏幕第1列4行显示1行 */
            }
        }
        }
    }
    
}

static void OledDemo(void)
{
    osThreadAttr_t attr;
    attr.name = "OledmentTask";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 4096; /* 任务大小4096 */
    attr.priority = osPriorityNormal;

    if (osThreadNew(OledmentTask, NULL, &attr) == NULL) {
        printf("[OledDemo] Falied to create OledmentTask!\n");
    }
}


APP_FEATURE_INIT(OledDemo);