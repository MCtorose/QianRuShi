#include <REGX52.H>
#include "car.h"
#include <intrins.h>

sbit D1=P2^7;//右边红外模块
sbit D2=P2^6;
sbit D3=P2^5;
sbit D4=P2^4;//左边红外模块

void Delay50ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 90;
	j = 163;
	do
	{
		while (--j);
	} while (--i);
}
void xunji()
{
  if(D1==1&&D2==1&&D3==1&&D4==1)//全检测黑线
  {car_go();}
	if(D1==0&&D2==1&&D3==0&&D4==0)//中间右侧检测到黑线，小车偏左，小车向右移动
	{
	  car_rightgo ();
		if(D1==0&&D2==0&&D3==0&&D4==0)
		{car_go();}
	}	
	 if(D1==0&&D2==0&&D3==1&&D4==0)//中间左侧检测到黑线，小车偏右，小车向左移动
	 {
	  car_leftgo(); 
		if(D1==0&&D2==0&&D3==0&&D4==0)
		{car_go();}
	 }
	 if(D1==0&&D2==0&&D3==1&&D4==1)//直角左拐
	 {
	   car_go();
		 Delay50ms();
		 if(D1==0&&D2==0&&D3==0&&D4==0)
		 {
		   car_stop();
			 Delay50ms();
			 car_leftstop();
		 }
	 } 
	  if(D1==1&&D2==1&&D3==0&&D4==0)//直角右拐
		{
		  car_go();
			Delay50ms();
		if(D1==0&&D2==0&&D3==0&&D4==0)
			{
			  car_stop;
				Delay50ms();
				car_rightstop();
			}	
		}	
		if((D1==0&&D2==0&&D3==0&&D4==0)||(D1==0&&D2==1&&D3==0&&D4==1)||(D1==0&&D2==1&&D3==1&&D4==1))//锐角左拐
		{
		  car_go();
			Delay50ms();
			Delay50ms();
		if(D1==0&&D2==0&&D3==0&&D4==0)
		{
      car_stop();
      Delay50ms();
      car_leftstop();			
		}			
	}	
		if((D1==1&&D2==0&&D3==0&&D4==0)||(D1==1&&D2==0&&D3==1&&D4==0)||(D1==1&&D2==1&&D3==1&&D4==0))//锐角右拐
		{
		  car_go();
			Delay50ms();
			Delay50ms();
    if(D1==0&&D2==0&&D3==0&&D4==0)
		{
      car_stop();
      Delay50ms();
      car_leftstop();	
		}
	}		
		if(D1==0&&D2==1&&D3==1&&D4==0)
		{
			if(D1==1)
			{
		  car_rightstop();
			}
		}
		if(D1==0&&D2==1&&D3==1&&D4==0)
		{
		  if(D4==1)
			{
			  car_rightstop();
			}		
		}		
}	
























