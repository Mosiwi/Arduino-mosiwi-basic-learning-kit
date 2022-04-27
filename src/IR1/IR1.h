#ifndef IR1_h
#define IR1_h

// If DEBUG is defined, a lot of debugging output will be printed during decoding.
// TEST must be defined for the IRtest unittests to work.  It will make some
// methods virtual, which will be slightly slower, which is why it is optional.
// #define DEBUG
// #define TEST

// Results returned from the decoder
class decode_results {
public:
  int decode_type;               // NEC, SONY, RC5, UNKNOWN
  unsigned long value;           // Decoded value
  int bits;                      // Number of bits in decoded value
  volatile unsigned int *rawbuf; // Raw intervals in .5 us ticks
  int rawlen;                    // Number of records in rawbuf.
};

// Values for decode_type
#define NEC 1
#define UNKNOWN -1

// Decoded value for NEC when a repeat code is received
#define REPEAT 0xffffffff

// main class for receiving IR
class IRreceiver1
{
public:
  IRreceiver1(int recvpin);
  int decode1(decode_results *results);
  void enableIRIn1();
  void resume1();
private:
  // These are called by decode
  long decodeNEC1(decode_results *results);

};

// Only used for testing; can remove virtual for shorter code
#ifdef TEST
#define VIRTUAL virtual
#else
#define VIRTUAL
#endif

class IRemission1
{
public:
  IRemission1() {}
  void sendNEC1(unsigned long data, int nbits);

  // private:
  void enableIROut1(int khz);
  VIRTUAL void mark1(int usec);
  VIRTUAL void space1(int usec);
};

// Some useful constants
#define USECPERTICK 50  // microseconds per clock interrupt tick
#define RAWBUF 100 // Length of raw duration buffer

// Marks tend to be 100us too long, and spaces 100us too short
// when received due to sensor lag.
#define MARK_EXCESS 100

#endif
