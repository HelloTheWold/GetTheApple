#include"Arduino.h"
#include "timer.h"
#include "seting.h"

//步进电机运动一下
#define Motor_lf_MoverOneStep   digitalWrite(Motor_lf_Speed,!digitalRead(Motor_lf_Speed))
#define Motor_lb_MoverOneStep   digitalWrite(Motor_lb_Speed,!digitalRead(Motor_lb_Speed))
#define Motor_rf_MoverOneStep   digitalWrite(Motor_rf_Speed,!digitalRead(Motor_rf_Speed))
#define Motor_rb_MoverOneStep   digitalWrite(Motor_rb_Speed,!digitalRead(Motor_rb_Speed))
//*********************************************************
//步进电机初始化各个舵机和电机的引脚
//*********************************************************
void IoInitial()
{

  pinMode(LEDPIN,OUTPUT);         //LED 初始化

	pinMode(Motor_lf_Speed,OUTPUT); //左前轮steps：脉冲个数
	pinMode(Motor_lf_Dir,OUTPUT);   //左前轮方向
	pinMode(Motor_lf_En,OUTPUT);    //左前轮使能 enable：可以用单片机端口控制，也可以直接连接GND使能

	pinMode(Motor_lb_Speed,OUTPUT);
	pinMode(Motor_lb_Dir,OUTPUT);   //左后轮
	pinMode(Motor_lb_En,OUTPUT);

	pinMode(Motor_rf_Speed,OUTPUT);
	pinMode(Motor_rf_Dir,OUTPUT);   //右前轮
	pinMode(Motor_rf_En,OUTPUT);

	pinMode(Motor_rb_Speed,OUTPUT);
	pinMode(Motor_rb_Dir,OUTPUT);   //右后轮
	pinMode(Motor_rb_En,OUTPUT);
}
//*********************************************************
//步进电机使能
//*********************************************************
void StepMotorEnable()
{
	digitalWrite(Motor_lf_En,HIGH);  //使能
	digitalWrite(Motor_lb_En,HIGH);
	digitalWrite(Motor_rf_En,HIGH);
	digitalWrite(Motor_rb_En,HIGH);
}
//*********************************************************
//步进电机去使能
//*********************************************************
void StepMotorDisable()
{
	digitalWrite(Motor_lf_En,LOW);  //使能
	digitalWrite(Motor_lb_En,LOW);
	digitalWrite(Motor_rf_En,LOW);
	digitalWrite(Motor_rb_En,LOW);
}
//*********************************************************
//旋转
//*********************************************************
void sround()
{
  //方向控制
  digitalWrite(Motor_lf_Dir,LOW);
  digitalWrite(Motor_lb_Dir,HIGH);
  digitalWrite(Motor_rf_Dir,HIGH);
  digitalWrite(Motor_rb_Dir,HIGH);
  //使能
  digitalWrite(Motor_lf_En,HIGH);  
  digitalWrite(Motor_lb_En,HIGH);
  digitalWrite(Motor_rf_En,HIGH);
  digitalWrite(Motor_rb_En,HIGH);
}
//*********************************************************
//反向旋转
//*********************************************************
void nround()
{
   //方向控制
  digitalWrite(Motor_lb_Dir,LOW);
  digitalWrite(Motor_lf_Dir,HIGH);
  digitalWrite(Motor_rb_Dir,LOW);
  digitalWrite(Motor_rf_Dir,LOW);
  //使能
  digitalWrite(Motor_lf_En,HIGH);  
  digitalWrite(Motor_lb_En,HIGH);
  digitalWrite(Motor_rf_En,HIGH);
  digitalWrite(Motor_rb_En,HIGH);
}
//*********************************************************
//向前
//*********************************************************
void up(){
  digitalWrite(Motor_lb_Dir,HIGH);
  digitalWrite(Motor_rf_Dir,LOW);

  digitalWrite(Motor_lf_En,LOW);  
  digitalWrite(Motor_lb_En,HIGH);
  digitalWrite(Motor_rf_En,HIGH);
  digitalWrite(Motor_rb_En,LOW);

}
//*********************************************************
//旋转
//*********************************************************
void back(){
  digitalWrite(Motor_lb_Dir,LOW);
  digitalWrite(Motor_rf_Dir,HIGH);


  digitalWrite(Motor_lf_En,LOW);  
  digitalWrite(Motor_lb_En,HIGH);
  digitalWrite(Motor_rf_En,HIGH);
  digitalWrite(Motor_rb_En,LOW);
}
//*********************************************************
//左转
//*********************************************************
void left(){
  digitalWrite(Motor_lf_Dir,HIGH);
  digitalWrite(Motor_rb_Dir,HIGH);

  digitalWrite(Motor_lf_En,HIGH);  
  digitalWrite(Motor_lb_En,LOW);
  digitalWrite(Motor_rf_En,LOW);
  digitalWrite(Motor_rb_En,HIGH);
}
//*********************************************************
//右转
//*********************************************************
void right(){
  digitalWrite(Motor_lf_Dir,LOW);
  digitalWrite(Motor_rb_Dir,LOW);

  digitalWrite(Motor_lf_En,HIGH);  
  digitalWrite(Motor_lb_En,LOW);
  digitalWrite(Motor_rf_En,LOW);
  digitalWrite(Motor_rb_En,HIGH);
}
//*********************************************************
//步进电机运动一下
//*********************************************************
void StepMotorMoveOneStep()
{
  digitalWrite(Motor_lb_Speed,!digitalRead(Motor_lb_Speed));
  digitalWrite(Motor_lf_Speed,!digitalRead(Motor_lf_Speed));
  digitalWrite(Motor_rb_Speed,!digitalRead(Motor_rb_Speed));
  digitalWrite(Motor_rf_Speed,!digitalRead(Motor_rf_Speed));
}

void StepMotorMoveOneStep_n()          //步进电机运动一下
{
  digitalWrite(Motor_lb_Speed,!digitalRead(Motor_lb_Speed));
  digitalWrite(Motor_lf_Speed,!digitalRead(Motor_lf_Speed));
  digitalWrite(Motor_rb_Speed,!digitalRead(Motor_rb_Speed));
  digitalWrite(Motor_rf_Speed,!digitalRead(Motor_rf_Speed));
}
