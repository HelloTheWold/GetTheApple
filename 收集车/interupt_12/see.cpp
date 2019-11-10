//#include <Servo.h>
//Servo myservo;
//
//#define red 8
//#define green 7
//void setup() {
//  Serial.begin(9600);
//  // put your setup code here, to run once:
//  pinMode(red, INPUT);
//  pinMode(green, INPUT);
//}
//int pos;
//
//
//void loop() {
//  // put your main code here, to run repeatedly:
//  myservo.attach(9);
//  delay(1);
//  if (digitalRead(red) == HIGH && digitalRead(green) == HIGH) {
//    Serial.println("both");
//  }
//  else if (digitalRead(red) == LOW && digitalRead(green) == HIGH)
//  { Serial.println("greeeeen");
//    for(pos=120;pos>=70;pos--){myservo.write(pos);delay(10);}
//    delay(20);
//    for(pos=70;pos<=120;pos++){myservo.write(pos);delay(10);}
//  }
//  else if (digitalRead(red) == HIGH && digitalRead(green) == LOW)
//  { Serial.println("red");
//  }
//  else if (digitalRead(red) == LOW && digitalRead(green) == LOW)
//  {// Serial.println("0");
//  }
//  else
//  {
//  }
//
//}
