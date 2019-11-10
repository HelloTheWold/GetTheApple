#ifndef __IOCHANGE_H__
#define __IOCHANGE_H__

void IoInitial();						//初始化io
void StepMotorEnable();					//步进电机使能
void StepMotorDisable();				//步进电机去使能



void nround();   //
void sround();
void up();
void back();
void left();
void right();

void StepMotorMoveOneStep();          //步进电机运动一下
void StepMotorMoveOneStep_n();          //步进电机运动一下

#endif
