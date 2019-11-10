//*********************************************
#include "iochange.h"
#include "timer.h"
#include "seting.h"
#include <Servo.h>
#include "sensor.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"

#define DISTANCE 17.66
#define ANGLE 41.33
//*********************************************
Servo myservo_0;
Servo myservo_1;
Servo myservo_2;

Servo myservo_3;                    //前右
Servo myservo_4;                    //前左

unsigned int Step_Motor=0;          //步数
unsigned int Step_Ball_Delay_Up=0;
unsigned int Step_Ball_Delay_Down=0;
bool upballstate=false;
bool downballstate=false;
bool SlowSpeedBeforeGoUpToTree=false;          //找到树前的减速
unsigned int SlowSpeedSetBeforeGoUpToTree=0;
//**********************************************
int16_t ax, ay, az;
int16_t gx, gy, gz;
String inputString = "";         // a String to hold incoming data
bool stringComplete = true;      // whether the string is complete
bool state_nround=false;
//**********************************************
//设置
//**********************************************
void setup() {
  pinMode(Tree_Position_Set,INPUT);         //树的限位
  //IO口控制驱动的步进电机
  IoInitial();
  StepMotorDisable();
  timersetup();
  //舵机控制
  myservo_0.attach(8); 
  myservo_1.attach(6);
  myservo_2.attach(4);     //收集装置开关下面

  myservo_3.attach(10);    //收集装置前面左边  
  myservo_4.attach(21);     //收集装置前面右边 
  //初始化舵机位置
  myservo_0.write(100);
  myservo_1.write(3);
  myservo_2.write(100);   //收集装置锁死

  myservo_3.write(10);
  myservo_4.write(90);
  

  Serial1.begin(9600);        // 蓝牙
  Serial2.begin(9600);        // 上面舵机控制
  Serial3.begin(9600);        // 下面舵机控制
  Serial.begin(9600);  
}
//**********************************************
//主函数
//**********************************************
void loop() 
{  
       //命令获取
       if(state_nround==true)
       {
            if(Serial3.available())
            {
              char color=Serial3.read();          
              if(color=='g')
              {
                if((upballstate==false)&&(Step_Ball_Delay_Up>0))
                {
                  Step_Ball_Delay_Up=Step_Ball_Delay_Up;
                }else
                {
                  Step_Ball_Delay_Up=1825;
                }    
                upballstate=false; 
              }            
            }
           if(Serial2.available())
           {
              char color_down=Serial2.read();              
              if(color_down=='g')
              {       
                if((downballstate==false)&&(Step_Ball_Delay_Down>0))
                {
                  Step_Ball_Delay_Down=Step_Ball_Delay_Down;
                }else
                {
                  Step_Ball_Delay_Down=1825;
                }
                downballstate=false;
              }
           }
          //**************************************
          if(Step_Ball_Delay_Up==1)
          {
            Step_Ball_Delay_Up--;
            if(upballstate)
            {
              myservo_0.write(3);     //伸出去
            }
            else
            {
               upballstate=true;
               Step_Ball_Delay_Up=3700;
               myservo_0.write(100);    //收回来
            }
          }
          //*****************************************
          if(Step_Ball_Delay_Down==1)
          {
            Step_Ball_Delay_Down--;
            if(downballstate)
            {
               myservo_1.write(100);
            }else
            {
              Step_Ball_Delay_Down=3700;
              downballstate=true;
              myservo_1.write(3);
            } 
          }
          //***************************************** 
       }else
       {
         myservo_0.write(95);
         myservo_1.write(5);
       }
         
        if(stringComplete)
        {
            String SubCmd=inputString.substring(0,2);
            if(SubCmd=="mf")
            {
              Step_Motor=(int)(inputString.substring(2,6).toInt()*DISTANCE);
              up();
              
            }else if(SubCmd=="mb")
            {
               Step_Motor=(int)(inputString.substring(2,6).toInt()*DISTANCE);
               back();
             
            }else if(SubCmd=="ml")
            {
              Step_Motor=(int)(inputString.substring(2,6).toInt()*DISTANCE);
              left();
           
            }else if(SubCmd=="mr")
            {
              Step_Motor=(int)(inputString.substring(2,6).toInt()*DISTANCE);
              right();
             
            }else if(SubCmd=="rc")
            {
              Step_Motor=(int)(inputString.substring(2,6).toInt()*ANGLE);
              sround();
             
            }else if(SubCmd=="ra")
            {
              Step_Motor=(int)(inputString.substring(2,6).toInt()*ANGLE);
              nround();
            }else if(SubCmd=="ms")
            {
              Step_Motor=0;
              StepMotorDisable();  
            }else if(SubCmd=="rt")
            {
              Step_Motor=19864+1500;
              nround();
              
              Step_Ball_Delay_Up=3600;              
              upballstate=true;
              Step_Ball_Delay_Down=3600;
              downballstate=true; 
     
              state_nround=true;
            }else if(SubCmd=="tl")
            {
             // 90° 向外
              //10°  挡住
              unsigned angle=inputString.substring(2,6).toInt();
              myservo_3.write(angle);       
              
              
            }else if(SubCmd=="tr")
            {
              // 10° 向外
              //90°  挡住
              unsigned angle=inputString.substring(2,6).toInt();
              myservo_4.write(angle);
              
            }else if(SubCmd=="op")      // 打开
            {
              myservo_3.write(90);
              myservo_4.write(10);  
              
            }else if(SubCmd=="cl")      //关闭
            {
              myservo_3.write(10);
              myservo_4.write(90);
              
            }else if(SubCmd=="rl")        //释放收集装置
            {
              myservo_2.write(0);
              Serial1.print('j');
              while(Serial1.read()!='l');
              Serial.print("OVER");
            }
         inputString = "";
         stringComplete = false;
       }

}
//***********************************************
//定时器中断程序
//***********************************************
ISR(TIMER1_COMPA_vect)
{ 
  if(Step_Motor>0)
  {
    Step_Motor--;
    if(state_nround)
    {
       if(digitalRead(Tree_Position_Set))
       {
         Step_Motor++;
         SlowSpeedBeforeGoUpToTree=true;
         up();
       }else
       {
          SlowSpeedBeforeGoUpToTree=false;
          int temp=Step_Motor%70;
          if(temp==19)
          {
             up();
          }else if(temp==14)
          {
             nround();
          }
       }
    }
   //*****************************************************
    if(Step_Motor<10)
    {
      if(Step_Motor%2)
      {
         StepMotorMoveOneStep(); 
      }
      if(Step_Motor==0)
      {
         StepMotorEnable();
         if(state_nround)
         {
          state_nround=false;
          sround();
          Step_Motor=1500;
         }else
         {
           Serial.print("OVER");
         }
         
         SlowSpeedBeforeGoUpToTree=false;  
      }
    }
    else
    {
      if(SlowSpeedBeforeGoUpToTree)
      {
        SlowSpeedSetBeforeGoUpToTree++;
        if(SlowSpeedSetBeforeGoUpToTree>3)
        {
          SlowSpeedSetBeforeGoUpToTree=0;
          StepMotorMoveOneStep();  
        }    
      }
      else
      {
          StepMotorMoveOneStep();  
      } 
    }      
  }
  //*****************************************************
  if(Step_Ball_Delay_Up>1)
  {
    if(SlowSpeedBeforeGoUpToTree==false)
    {
        Step_Ball_Delay_Up--;
    }
  }
  //*****************************************************
  if(Step_Ball_Delay_Down>1)
  {
    if(SlowSpeedBeforeGoUpToTree==false)
    {
        Step_Ball_Delay_Down--;
    }
  }
} 
//***********************************************
//串口中断的样子
void serialEvent() 
{
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == 'Z') {
      stringComplete = true;
    }
  }
}
