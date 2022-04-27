/*
 This example works with the Mosiwi Basic learning board.
 
 Learning target:
  1. NEC Infrared protocol
  2. Timer interruption
  3. #define common methods
  4. struct variable
  
 Web: http://mosiwi.com/
 Wiki: http://wiki.mosiwi.com/
 Designer: jalen
 Date：2022-3-22
*/
#include <Arduino.h>     
 
// Some useful constants
#define USECPERTICK 50  // microseconds per clock interrupt tick
#define RAWBUF 100      // Length of raw duration buffer

// Pulse parms are *50-100 for the Mark and *50+100 for the space
// First MARK is the one after the long gap
// pulse parameters in usec
#define NEC_HDR_MARK	9000
#define NEC_HDR_SPACE	4500
#define NEC_BIT_MARK	560
#define NEC_ONE_SPACE	1600
#define NEC_ZERO_SPACE	560
#define NEC_RPT_SPACE	2250

// Minimum map between transmissions
#define GAP_TICKS (5000/USECPERTICK)

// percent tolerance in measurements
#define TOLERANCE 25  
#define LTOL (1.0 - TOLERANCE/100.)  // 75%
#define UTOL (1.0 + TOLERANCE/100.)  // 125%
#define TICKS_LOW(us) (int) (((us)*LTOL/USECPERTICK))   
#define TICKS_HIGH(us) (int) (((us)*UTOL/USECPERTICK))  

// IR detector output is active low
#define MARK  0
#define SPACE 1

// receiver states
#define STATE_IDLE     2
#define STATE_MARK     3
#define STATE_SPACE    4
#define STATE_STOP     5

#define NEC_BITS 32
#define ERR 0
#define DECODED 1

// Values for decode_type
#define NEC 1
#define UNKNOWN -1

// Decoded value for NEC when a repeat code is received
#define REPEAT 0xffffffff

// main Arduino clock
#define SYSCLOCK 16000000

// defines for timer2 (8 bits)
#define TIMER_RESET
#define TIMER_ENABLE_INTR     (TIMSK2 = _BV(OCIE2A))
#define TIMER_DISABLE_INTR    (TIMSK2 = 0)
#define TIMER_COUNT_TOP       ((SYSCLOCK / 8) * USECPERTICK / 1000000)
// Set timer 2 to interrupt every 50 seconds.
#define TIMER_CONFIG_NORMAL() ({TCCR2A = _BV(WGM21); TCCR2B = _BV(CS21); OCR2A = TIMER_COUNT_TOP; TCNT2 = 0;})

// information for the interrupt handler
typedef struct {
  uint8_t recvpin;             // pin for IR data from detector
  uint8_t rcvstate;            // state machine
  unsigned int timer;          // state timer, counts 50uS ticks.
  unsigned int rawbuf[RAWBUF]; // raw data
  uint8_t rawlen;              // counter of entries in rawbuf
} irparams_t;
// Defined in IRremote.cpp
volatile irparams_t irparams;

// Results returned from the decoder
class decode_results {
public:
  int decode_type;               // NEC, SONY, RC5, UNKNOWN
  unsigned long value;           // Decoded value
  int bits;                      // Number of bits in decoded value
  volatile unsigned int *rawbuf; // Raw intervals in .5 us ticks
  int rawlen;                    // Number of records in rawbuf.
};

// main class for receiving IR
class IRreceiver{
public:
  IRreceiver(int recvpin);
  int decode(decode_results *results);
  void enableIRIn();
  void resume();
private:
  // These are called by decode
  long decodeNEC(decode_results *results);
};

IRreceiver::IRreceiver(int recvpin){
  irparams.recvpin = recvpin;
}

// initialization
void IRreceiver::enableIRIn(){
  // disable interrupts
  cli();
  // setup pulse clock timer interrupt
  // Prescale/8 (16M/8 = 0.5 microseconds per tick)
  // Therefore, the timer interval can range from 0.5 to 128 microseconds
  // depending on the reset value (255 to 0)
  TIMER_CONFIG_NORMAL();
  //Timer2 Overflow Interrupt Enable
  TIMER_ENABLE_INTR;
  TIMER_RESET;
  // enable interrupts
  sei();  

  // initialize state machine variables
  irparams.rcvstate = STATE_IDLE;
  irparams.rawlen = 0;

  // set pin modes
  pinMode(irparams.recvpin, INPUT);
}

// TIMER2 interrupt code to collect raw data.
// Widths of alternating SPACE, MARK are recorded in rawbuf.
// Recorded in ticks of 50 microseconds.
// rawlen counts the number of entries recorded so far.
// First entry is the SPACE between transmissions.
// As soon as a SPACE gets long, ready is set, state switches to IDLE, timing of SPACE continues.
// As soon as first MARK arrives, gap width is recorded, ready is cleared, and new logging starts
ISR(TIMER2_COMPA_vect)
{
  // Reset timer
  TIMER_RESET;
  uint8_t irdata = (uint8_t)digitalRead(irparams.recvpin);
  irparams.timer++; // One more 50us tick
  if (irparams.rawlen >= RAWBUF) {
    // Buffer overflow
    irparams.rcvstate = STATE_STOP;
  }
  switch(irparams.rcvstate) {
  case STATE_IDLE:            // In the middle of a gap
    if (irdata == MARK) {
      if (irparams.timer < GAP_TICKS) {  
        // Greater than 96ms(108ms - 11.8ms = 96.2ms) between two data points. 	  
        // Not big enough to be a gap.
        irparams.timer = 0;
      } 
      else {
        // gap just ended, record duration and start recording transmission
        irparams.rawlen = 0;
        irparams.rawbuf[irparams.rawlen++] = irparams.timer;
        irparams.timer = 0;
        irparams.rcvstate = STATE_MARK;
      }
    }
    break;
  case STATE_MARK:           // timing MARK
    if (irdata == SPACE) {   // MARK ended, record time
      irparams.rawbuf[irparams.rawlen++] = irparams.timer;
      irparams.timer = 0;
      irparams.rcvstate = STATE_SPACE;
    }
    break;
  case STATE_SPACE:          // timing SPACE
    if (irdata == MARK) {    // SPACE just ended, record it
      irparams.rawbuf[irparams.rawlen++] = irparams.timer;
      irparams.timer = 0;
      irparams.rcvstate = STATE_MARK;
    } 
    else {                   // SPACE
      if (irparams.timer > GAP_TICKS) {
        // big SPACE, indicates gap between codes
        // Mark current code as ready for processing
        // Switch to STOP
        // Don't reset timer; keep counting space width
        irparams.rcvstate = STATE_STOP;
      } 
    }
    break;
  case STATE_STOP:           // waiting, measuring gap
    if (irdata == MARK) {    // reset gap timer
      irparams.timer = 0;
    }
    break;
  }
}

void IRreceiver::resume() {
  irparams.rcvstate = STATE_IDLE;
  irparams.rawlen = 0;
}

// Decodes the received IR message
// Returns 0 if no data ready, 1 if data ready.
// Results of decoding are stored in results
int IRreceiver::decode(decode_results *results) {
  results->rawbuf = irparams.rawbuf;
  results->rawlen = irparams.rawlen;
  if (irparams.rcvstate != STATE_STOP){
    return ERR;
  }
  if (decodeNEC(results)) {
    return DECODED;
  }
  
  // Throw away and start over
  resume();
  return ERR;
}

int MATCH(int measured, int desired) {return measured >= TICKS_LOW(desired) && measured <= TICKS_HIGH(desired);}
int MATCH_MARK(int measured_ticks, int desired_us) {return MATCH(measured_ticks, desired_us);}
int MATCH_SPACE(int measured_ticks, int desired_us) {return MATCH(measured_ticks, desired_us);}
// NECs have a repeat only 4 items long
long IRreceiver::decodeNEC(decode_results *results) {
  long data = 0;
  int offset = 1; // Skip first space
  // Initial mark
  if (!MATCH_MARK(results->rawbuf[offset], NEC_HDR_MARK)) {
    return ERR;
  }
  offset++;
  // Check for repeat
  if (irparams.rawlen == 4 &&
    MATCH_SPACE(results->rawbuf[offset], NEC_RPT_SPACE) &&
    MATCH_MARK(results->rawbuf[offset+1], NEC_BIT_MARK)) {
    results->bits = 0;
    results->value = REPEAT;
    results->decode_type = NEC;
    return DECODED;
  }
  if (irparams.rawlen < 2 * NEC_BITS + 4) {
    return ERR;
  }
  // Initial space  
  if (!MATCH_SPACE(results->rawbuf[offset], NEC_HDR_SPACE)) {
    return ERR;
  }
  offset++;
  for (int i = 0; i < NEC_BITS; i++) {
    if (!MATCH_MARK(results->rawbuf[offset], NEC_BIT_MARK)) {
      return ERR;
    }
    offset++;
    if (MATCH_SPACE(results->rawbuf[offset], NEC_ONE_SPACE)) {
      data = (data << 1) | 1;
    } 
    else if (MATCH_SPACE(results->rawbuf[offset], NEC_ZERO_SPACE)) {
      data <<= 1;
    } 
    else {
      return ERR;
    }
    offset++;
  }
  // Success
  results->bits = NEC_BITS;
  results->value = data;
  results->decode_type = NEC;
  return DECODED;
}


// Define infrared receiving pins
int RECV_PIN = 4;
// Create an infrared receiver class
IRreceiver irrecv(RECV_PIN);
// Create an infrared receive result class to store infrared receive data
decode_results results;

void setup()
{
  Serial.begin(9600);
  // The infrared receiver function is enabled
  irrecv.enableIRIn(); 
}

void loop() {
  // Determine whether the infrared data can be decoded, and if so, 
  // decode and store the decoded data in the Results class.
  if (irrecv.decode(&results)) {  
    // Prints the decoded data to the serial port monitor
    Serial.println(results.value, HEX);
	// Receive the next value
    irrecv.resume(); 
  }
}

