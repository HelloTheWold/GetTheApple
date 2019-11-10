#include"Arduino.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "iochange.h"
#include "seting.h"
//******************************************************
//定时器中断设置
//*****************************************************
void timersetup()
{
   // initialize Timer1
  cli(); // disable global interrupts
  TCCR1A = 0; // set entire TCCR1A register to 0
  TCCR1B = 0; // same for TCCR1B
  
  // set compare match register to desired timer count:
  OCR1A = 4;                                    
  // turn on CTC mode:
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler:
   TCCR1B |= (1 << CS10); 
   TCCR1B |= (1 << CS12);
  // enable timer compare interrupt:
  TIMSK1 |= (1 << OCIE1A);
  // enable global interrupts:
  sei();
}
