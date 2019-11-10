#include<Servo.h>

#define Motor_lf_En 27
#define Motor_lf_Speed 25  //左前 左转
#define Motor_lf_Dir 23


#define Motor_rb_En 45
#define Motor_rb_Speed 43   //右后  左转
#define Motor_rb_Dir 41

#define Motor_lb_En 33
#define Motor_lb_Speed 31
#define Motor_lb_Dir 29  //左后 向后转

#define Motor_rf_En 35
#define Motor_rf_Speed 37
#define Motor_rf_Dir 39  //右前 向后

#define Motor_lf_MoverOneStep   digitalWrite(Motor_lf_Speed,!digitalRead(Motor_lf_Speed))
#define Motor_lb_MoverOneStep   digitalWrite(Motor_lb_Speed,!digitalRead(Motor_lb_Speed))
#define Motor_rf_MoverOneStep   digitalWrite(Motor_rf_Speed,!digitalRead(Motor_rf_Speed))
#define Motor_rb_MoverOneStep   digitalWrite(Motor_rb_Speed,!digitalRead(Motor_rb_Speed))

char ppj='i';
int led1 = 36;
int led2 = 34;
int led3 = 32;
int led4 = 30;
//int led5 = 34;

Servo myservo1,myservo2;
int pos;

void setup() {
//  myservo1.attach(6);
  myservo2.attach(10);
  
  Serial2.begin(9600);
//  Serial.begin(9600);
//
//  pinMode(led1, INPUT);
//  pinMode(led2, INPUT);
//  pinMode(led3, INPUT);
//  pinMode(led4, INPUT);
//  pinMode(led5, INPUT);
  
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

  
//  pinMode(4,OUTPUT);
//  digitalWrite(4,HIGH); //电磁铁开

  digitalWrite(Motor_lf_Speed,HIGH);  
  digitalWrite(Motor_lb_Speed,HIGH);
  digitalWrite(Motor_rf_Speed,HIGH);
  digitalWrite(Motor_rb_Speed,HIGH);

  
}

void loop() {
  //digitalWrite(4,HIGH); //电磁铁开
  myservo2.write(50);

  delay(500);
  left(9000);
  up(1500);
  stopp(500);       
  while (Serial2.read()!='j') stopp(10);
  left(4500);
  stopp(400);
  right(4000);           //back slowly
  Serial2.println('l');
  right(8000);  //8000
  
  back(5000);
  nround(4500);           // smooth rotate  4500
  delay(200);
  back(6000);
  delay(200);
 
  myservo2.write(179);
  delay(500);  //wait for servo to pose
  
  right(8000);
  back(3000);
   right(6000);
  back(3000);
   right(6000);
  back(3000);
  while(1) stopp(20);     //final stop
  
//    int a,b,c,d,sum;
//    a=0;b=0;c=0;d=0;sum=0;
//    if (digitalRead(led1)==HIGH) a=1; else a=0;
//    if (digitalRead(led2)==HIGH) b=1; else b=0;
//    if (digitalRead(led3)==HIGH) c=1; else c=0;
//    if (digitalRead(led4)==HIGH) d=1; else d=0;
//    sum=80000+1000*a+100*b+10*c+d;
//    Serial.println("ok");
//    Serial.print(a);
//    Serial.print(b);
//    Serial.print(c);Serial.print(d);
//    Serial.println(sum);
//    delay(1000);

 
  
  
  
}


void up(int num){
  digitalWrite(Motor_lb_Dir,HIGH);
  digitalWrite(Motor_rf_Dir,HIGH);

  digitalWrite(Motor_lf_En,LOW);  
  digitalWrite(Motor_lb_En,HIGH);
  digitalWrite(Motor_rf_En,HIGH);
  digitalWrite(Motor_rb_En,LOW);

  for (int i=1;i<=num;i++)
  {
    Motor_lf_MoverOneStep; 
    Motor_lb_MoverOneStep;   
    Motor_rf_MoverOneStep;  
    Motor_rb_MoverOneStep;
    delayMicroseconds(150);
  }
}

void back(int num){
  digitalWrite(Motor_lb_Dir,LOW);
  digitalWrite(Motor_rf_Dir,LOW);

  digitalWrite(Motor_lf_En,LOW);  
  digitalWrite(Motor_lb_En,HIGH);
  digitalWrite(Motor_rf_En,HIGH);
  digitalWrite(Motor_rb_En,LOW);

  for (int i=1;i<=num;i++)
  {
    Motor_lf_MoverOneStep; 
    Motor_lb_MoverOneStep;   
    Motor_rf_MoverOneStep;  
    Motor_rb_MoverOneStep;
    delayMicroseconds(150);
  }
}

void right(int num){
  digitalWrite(Motor_lf_Dir,HIGH);
  digitalWrite(Motor_rb_Dir,HIGH);

  digitalWrite(Motor_lb_En,LOW);  
  digitalWrite(Motor_lf_En,HIGH);
  digitalWrite(Motor_rb_En,HIGH);
  digitalWrite(Motor_rf_En,LOW);

  for (int i=1;i<=num;i++)
  {
    Motor_lf_MoverOneStep; 
    Motor_lb_MoverOneStep;   
    Motor_rf_MoverOneStep;  
    Motor_rb_MoverOneStep;
    delayMicroseconds(150);
  }
}

void left(int num){
  digitalWrite(Motor_lf_Dir,LOW);
  digitalWrite(Motor_rb_Dir,LOW);

  digitalWrite(Motor_lb_En,LOW);  
  digitalWrite(Motor_lf_En,HIGH);
  digitalWrite(Motor_rb_En,HIGH);
  digitalWrite(Motor_rf_En,LOW);

  for (int i=1;i<=num;i++)
  {
    Motor_lf_MoverOneStep; 
    Motor_lb_MoverOneStep;   
    Motor_rf_MoverOneStep;  
    Motor_rb_MoverOneStep;
    delayMicroseconds(150);
  }
}
void nround(int num)
{
  digitalWrite(Motor_lf_En,HIGH);  //使能
  digitalWrite(Motor_lb_En,HIGH);
  digitalWrite(Motor_rf_En,HIGH);
  digitalWrite(Motor_rb_En,HIGH);

  digitalWrite(Motor_lf_Dir,HIGH);
  digitalWrite(Motor_lb_Dir,HIGH);
  digitalWrite(Motor_rf_Dir,LOW);
  digitalWrite(Motor_rb_Dir,LOW);

  for (int i=1;i<=num;i++)
  {
    Motor_lf_MoverOneStep; 
    Motor_lb_MoverOneStep;   
    Motor_rf_MoverOneStep;  
    Motor_rb_MoverOneStep;
    delayMicroseconds(500);
  }
}

void sround(int num)
{
  digitalWrite(Motor_lb_Dir,LOW);
  digitalWrite(Motor_lf_Dir,LOW);
  digitalWrite(Motor_rb_Dir,HIGH);
  digitalWrite(Motor_rf_Dir,HIGH);

  digitalWrite(Motor_lf_En,HIGH);  
  digitalWrite(Motor_lb_En,HIGH);
  digitalWrite(Motor_rf_En,HIGH);
  digitalWrite(Motor_rb_En,HIGH);

  for (int i=1;i<=num;i++)
  {
    Motor_lf_MoverOneStep; 
    Motor_lb_MoverOneStep;   
    Motor_rf_MoverOneStep;  
    Motor_rb_MoverOneStep;
    delayMicroseconds(500);
  }
}

void stopp(int num)
{
  digitalWrite(Motor_lf_En,LOW);  
  digitalWrite(Motor_lb_En,LOW);
  digitalWrite(Motor_rf_En,LOW);
  digitalWrite(Motor_rb_En,LOW);
  for (int i=1;i<=num;i++)
  {
    delay(1);
  }
}
