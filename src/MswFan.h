/*
 * Project: PWM signal generation
 * Function: Fan drive
 * Github: https://github.com/mosiwi
 * Wiki: http://wiki.mosiwi.com
 * Web: http://mosiwi.com
 * Engineer: Jalen
 * date: 2022-4-10
 */
#ifndef MswFan_h
#define MswFan_h
#include <Arduino.h>
	
// PWM frequency : 1/(0.00001 * 250) = 400Hz
#ifdef FAN_USE_TIMER1 
#ifndef FAN_USE_TIMER2
#include "TimerOne/TimerOne.h"
#endif
#endif

// PWM frequency : 1/(0.00005 * 250) = 80Hz
#ifdef FAN_USE_TIMER2
#ifndef FAN_USE_TIMER1
#include "MsTimer2/MsTimer2.h"
#endif
#endif

uint8_t CW  = 0;
uint8_t CCW = 1;

namespace MswFan{
  // Timer timing 0.01ms.
  uint8_t fanTiming = 10;
  // PWM cycle time: cycle * timing
  int fanCycle = 250;
  // Store the speed variable
  uint8_t fanSpeed = 0;
  // Timer counter
  int timerCounter = 0;
  // direction of rotation
  bool fan_cw_ccw = true;
  // define control pins
  uint8_t fan_in1 ;
  uint8_t fan_in2 ;

  // A function that interrupts execution	
  void fanRun(void){
    if(fan_cw_ccw){     // CCW
	  digitalWrite(fan_in2, LOW);
	  if(timerCounter < fanSpeed){
	    digitalWrite(fan_in1, HIGH);
	  }else{
	    digitalWrite(fan_in1, LOW);
	  }
    }else{  // CW
	  digitalWrite(fan_in1, LOW);
	  if(timerCounter < fanSpeed){
	    digitalWrite(fan_in2, HIGH);
	  }else{
	    digitalWrite(fan_in2, LOW);
	  }
    }
    timerCounter ++;
    if(timerCounter >= fanCycle+1){
	  timerCounter = 0;
    }
  }
  
  void init(uint8_t p1, uint8_t p2){
    fan_in1 = p1;
    fan_in2 = p2;
    pinMode(fan_in1, OUTPUT);
    pinMode(fan_in2, OUTPUT);
	
    #ifdef FAN_USE_TIMER1
    Timer1.initialize(fanTiming);
    Timer1.attachInterrupt(fanRun);
    #endif 
  
    // PWM frequency : 1/(0.001 * 12.5) = 80Hz
    #ifdef FAN_USE_TIMER2
    MsTimer2::setUs(fanTiming/2, fanRun);
    MsTimer2::start();
    #endif
  }
  
  // Speed allowed range: 0-255
  void speed(uint8_t sp){
	if(sp > 255 || sp < 0)
	  return;
    fanSpeed = map(sp, 0, 255, 0, fanCycle);
  }

  void direction(uint8_t CW_ccw){
	cli();
    if(CW_ccw == 1){
	  fan_cw_ccw = true;
    }else{
	  fan_cw_ccw = false;
    }
	sei();
  }

  void run(void){	
    #ifdef FAN_USE_TIMER1
    Timer1.start();
    #endif 
  
    #ifdef FAN_USE_TIMER2
    MsTimer2::start();
    #endif
  }

  void stop(void){
    #ifdef FAN_USE_TIMER1
    Timer1.stop();
    #endif 
  
    #ifdef FAN_USE_TIMER2
    MsTimer2::stop();
    #endif
	
	digitalWrite(fan_in1, LOW);
	digitalWrite(fan_in2, LOW);
  }
};

#endif




