/*
 This example works with the Mosiwi Basic learning board.
 
 Web: http://mosiwi.com/
 Wiki: http://wiki.mosiwi.com/
 Designer: jalen
 Date：2022-3-17
 */
#ifndef MswIR1_h
#define MswIR1_h
#include <Arduino.h>

#ifdef IR_USE_TIMER1 
#ifndef IR_USE_TIMER2
#include "IR1/IR1.h"
#endif
#endif

#ifdef IR_USE_TIMER2
#ifndef IR_USE_TIMER1
#include "IR2/IR2.h"
#endif
#endif

class MswIR{
public:
  unsigned long value;
  
  MswIR(uint8_t pin):
  #ifdef IR_USE_TIMER1
  irrecv(IRreceiver1(pin))
  #endif
  
  #ifdef IR_USE_TIMER2
  irrecv(IRreceiver(pin))
  #endif
  {}
  
  void enable(void){
    #ifdef IR_USE_TIMER1
    irrecv.enableIRIn1();
    #endif
	
    #ifdef IR_USE_TIMER2
    irrecv.enableIRIn();
    #endif	  
  }
  
  int decode(void){
    #ifdef IR_USE_TIMER1
	if(!irrecv.decode1(&results))
      return 0;
    value = results.value;
    irrecv.resume1();
    #endif
	
    #ifdef IR_USE_TIMER2
    if(!irrecv.decode(&results))
	  return 0;
    value = results.value;
    irrecv.resume();
    #endif
	
	return 1;
  }
 
 
private:
  #ifdef IR_USE_TIMER1 
  IRreceiver1 irrecv;
  #endif  
  
  #ifdef IR_USE_TIMER2 
  IRreceiver irrecv;
  #endif 
  
  decode_results results;
};

#endif
