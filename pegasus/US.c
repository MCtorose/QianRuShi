#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>


#include "ohos_init.h"
#include "cmsis_os2.h"
#include "iot_i2c.h"
#include "iot_gpio.h"
#include "iot_errno.h"tmt041031

#include "iot_gpio_ex.h"

#define Echo  8   //Echo   //GPIO8
#define Trig  7   //Trig   //GPIO7
#define GPIO_FUNC 0

unsigned char GetDistance(void) {
    static unsigned long start_time = 0, time = 0;
    float distance = 0.0;
    IotGpioValue value = IOT_GPIO_VALUE0;
    unsigned int flag = 0;
            /*GPIO通信模式流程 1初始化GPIO */
    IoTIoSetFunc(Echo, GPIO_FUNC);//设置Echo连接IO为普通GPIO模式，无复用
    IoTGpioSetDir(Echo, IOT_GPIO_DIR_IN);//设置Echo连接IO为输入模式
    IoTGpioSetDir(Trig, IOT_GPIO_DIR_OUT);//设置Trig连接IO为输出模式
            /*GPIO通信模式流程 2输出起始信号*/
    IoTGpioSetOutputVal(Trig, IOT_GPIO_VALUE1);//拉高Trig
    IoTUdelay(20);//20us
    IoTGpioSetOutputVal(Trig, IOT_GPIO_VALUE0);//拉低Trig
           /*GPIO通信模式流程 3检测Echo脚输出的高电平时间 */
    while (1) {
        IoTGpioGetInputVal(Echo, &value);//读取Echo脚的电平状态
        if ( value == IOT_GPIO_VALUE1 && flag == 0) {//如果为高
            start_time = IoTGetUs();//获取此时时间
            flag = 1;
        }
        if (value == IOT_GPIO_VALUE0 && flag == 1) {//高电平结束变成低电平
            time = IoTGetUs() - start_time;//计算高电平维持时间
            start_time = 0;
            break;
        }
    }
               /* GPIO通信模式流程 4代入公式计算 */
    distance = time * 0.034 / 2;
    return distance;
}