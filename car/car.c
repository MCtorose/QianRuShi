/*****С���ƶ�����*******/
/*��ǰֱ�С��ĵ����ת��    */
/*���ֱ�С��ĵ����ת��    */
/*��ǰ��ա��ҵ����ת������ֹͣ��*/
/*��ǰ�ҹա��ҵ��ֹͣ��������ת��*/
/*�����ա��ҵ����ת������ֹͣ��*/
/*����ҹա��ҵ��ֹͣ��������ת��*/
/*ֹͣ���ĵ��ֹͣ��    */
/*********С���ƶ�����*********/

#include <REGX52.H>
#include "motor.h"
#include "Buzzer.h"

sbit EN1A=P0^7;//��ǰ
sbit EN1B=P0^2;//��ǰ
sbit EN2A=P3^7;//���
sbit EN2B=P3^2;//���

unsigned char counter1,speed;//����ֵ�ͱȽ�ֵ���������PWM

void car_stop()//С��ֹͣ
{
	speed=0;
  LF_motor_stop();
  RF_motor_stop();
  LR_motor_stop();
  RR_motor_stop();	
}

void Timer0_Init()		//100΢��@11.0592MHz
{
	TMOD &= 0xF0;			//���ö�ʱ��ģʽ
	TMOD |=0X01;
	TL0 = 0xA4;				//���ö�ʱ��ʼֵ
	TH0 = 0xFF;				//���ö�ʱ��ʼֵ
	TF0 = 0;				//���TF0��־
	TR0 = 1;				//��ʱ��0��ʼ��ʱ
	ET0=1;
	EA=1;
	PT0=0;
}
void Timer0_Routine() interrupt 1//�жϺ���
{
  TL0=0xA4;
  TH0=0XFF;

  counter1++;
  if(counter1>=100)
	{
     counter1=0;	
		 car_stop();
		 Buzzer_Time(50);
	}	
	if (counter1<speed)
	{
	EN1A=1;
	EN1B=1;
  EN2A=1;
  EN2B=1;			
	}	
	else
	{
	EN1A=0;
	EN1B=0;
  EN2A=0;
  EN2B=0;	
	}	
}
void car_go()//С��ǰֱ��
{
  speed=20;
	LF_motor_go();
  RF_motor_go();
	LR_motor_go();
	RR_motor_go();
}	
void car_back()//С����ֱ��
{
  speed=18;
  LF_motor_back();
  RF_motor_back();
  LR_motor_back();
  RR_motor_back();	
}	

void car_leftgo()//С����ǰ���
{
  speed=18;
	RF_motor_go();
  RR_motor_go();
	LF_motor_stop();
	LR_motor_stop();
}	
void car_rightgo()//С����ǰ�ҹ�
{
  speed=18;
  LF_motor_go();
  LR_motor_go();
  RF_motor_stop();
	RR_motor_stop();	
}	
void car_leftback() //С��������
{
  speed=18;
	LF_motor_back();
  LR_motor_back();
	LF_motor_stop();
	LR_motor_stop();
}
void car_rightback()//С������ҹ�
{
  speed=18;
	LF_motor_back();
  LR_motor_back();
  RF_motor_stop();
  RR_motor_stop();	
}
void car_rightstop()//С��ԭ���ҹ�
{
  speed=20;
	LF_motor_go();
  LR_motor_go();
  RF_motor_back();
  RR_motor_back();
}	
void car_leftstop()//С��ԭ�����
{
  speed=20;
	LF_motor_back();
  LR_motor_back();
  RF_motor_go();
	RR_motor_go();
}	






