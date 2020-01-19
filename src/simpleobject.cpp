#include "simpleobject.h"

#include <QTime>


#define BRIGHTNESSFILE "/sys/class/backlight/backlight/brightness"

extern "C"{
#ifndef __LINUX_LIGHT_SENSOR_H
#define __LINUX_LIGHT_SENSOR_H

#define PSENSOR_IOCTL_MAGIC 'p'
#define PSENSOR_IOCTL_GET_ENABLED               _IOR(PSENSOR_IOCTL_MAGIC, 1, int *)
#define PSENSOR_IOCTL_ENABLE                    _IOW(PSENSOR_IOCTL_MAGIC, 2, int *)
#define PSENSOR_IOCTL_DISABLE                   _IOW(PSENSOR_IOCTL_MAGIC, 3, int *)

#define LIGHTSENSOR_IOCTL_MAGIC 'l'
#define LIGHTSENSOR_IOCTL_GET_ENABLED _IOR(LIGHTSENSOR_IOCTL_MAGIC, 1, int *)
#define LIGHTSENSOR_IOCTL_ENABLE _IOW(LIGHTSENSOR_IOCTL_MAGIC, 2, int *)
#define LIGHTSENSOR_IOCTL_SET_RATE _IOW(LIGHTSENSOR_IOCTL_MAGIC, 3, short)

#endif

#include <stdio.h>
#include <linux/types.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>


#define LS_NAME "/dev/lightsensor"
#define DEV_PATH "/dev/input/event5"
}

SimpleObject::SimpleObject()
{

    run();
}

SimpleObject::~SimpleObject()
{
}

void SimpleObject::run(){

    int ret = 0;
    int fd,dev_fd;
    int en = 1;
    struct input_event ie;

    fd = open(LS_NAME, O_RDONLY);
    if (fd < 0) {
        printf("open %s failed\n",LS_NAME);
        return;
    }

    //打开传感器设备文件
    ret = ioctl(fd ,LIGHTSENSOR_IOCTL_ENABLE,&en);

    dev_fd = open(DEV_PATH,O_RDONLY);
    if (dev_fd <0 ) {
        printf("open %s failed\n",DEV_PATH);
        return;
    }

    int preSensorValue = -1;

    //传感器 0 - 7
    QList<int> lightValueList;
    lightValueList  << 1 << 13 << 28 << 40 << 54 << 69 << 81 << 100;

    QTime startTime, preChangeTime, curChangeTime;

    startTime = QTime::currentTime();

    while(1){

        if(read(dev_fd ,&ie ,sizeof(ie)) ) {
            if(ie.code == 40){
                printf("lightvalue: %d\n", ie.value);

                int sensorValue = ie.value;

                //计算差值
                int diff = sensorValue - preSensorValue;

                //渐变间隔
                int interval = -1;

                //屏幕背光默认值
                int lightValue = 50;

                //获取当前时间
                curChangeTime = QTime::currentTime();

                //每2秒检测屏幕亮度自动调节的开关
                if (preChangeTime.secsTo(curChangeTime) % 2){

                }

                //获取背光值
                if (qAbs(diff) > 4 && preChangeTime.msecsTo(curChangeTime) < 1000){
                    printf("2 floor continue, sensor: %d; light: %d\n", sensorValue, lightValue);
                    preSensorValue = ie.value;
                    //2档渐变过快，跳过
                    continue;


//                } else if (qAbs(diff) > 4){
//                    //2档渐变
//                    printf("2 floor, sensor: %d; light: %d\n", sensorValue, lightValue);
                } else {
                    //1档渐变
                    lightValue = lightValueList.at(sensorValue);
                    printf("1 floor, sensor: %d; light: %d\n", sensorValue, lightValue);


                }


                if (preSensorValue != -1){
                    //改变背光
                    QString cmd = QString("echo %1 > %2").arg(lightValue).arg(BRIGHTNESSFILE);
                    QByteArray ba;
                    ba = cmd.toUtf8();
                    system(ba.data());
                }

                preChangeTime = QTime::currentTime();
                preSensorValue = ie.value;


            }
//            printf("key:%d, %d\n",ie.code,ie.value);
//            emit lightChanged(ie.value);
        }
    }

    close(dev_fd);


    //关闭传感器设备
    en = 0;
    ret = ioctl(fd ,LIGHTSENSOR_IOCTL_ENABLE,&en);
    close(fd);
}
