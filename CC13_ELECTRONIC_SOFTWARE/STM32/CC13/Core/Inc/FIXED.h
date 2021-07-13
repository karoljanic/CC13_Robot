#ifndef FIXED_H_
#define FIXED_H_

#include <stdint.h>


// constants coded in a fixed system Q1.14
#define FIXED_FBITS      14                           // number of bits for fractional part
#define FIXED_MASK       (short)0b0011111111111111    // bit mask for fractional part
#define FIXED_ONE	     (short)0b0100000000000000    // constant value 1.0
#define FIXED_ONE_HALF   (short)0b0010000000000000    // constant value 0.5
#define FIXED_MAXIMUM    (short)0b0111111111111111    // the maximum value of fixed ( 1.999938964843750)
#define FIXED_MINIMUM    (short)0b1000000000000001    // the minimum value of fixed (-1.999938964843750)
#define FIXED_OVERFLOW   (short)0b1000000000000000    // the OVERFLOW value


// Action in the case of exceeding the permitted range during arithmetic operations:
// print out an error message
#define FIXED_PRINT_OVERFLOW_MESSAGE

// Whether an external function for error of exceeding the permitted range during arithmetic operations handling is defined?
// If yes, define the FixedOverflowMessage function. If this function is missing, will be used the default function
//#define FIXED_USE_EXTERNAL_OVERFLOW_MESSAGE


#define FIXED_OVERFLOW_IGNORED    1
#define FIXED_OVERFLOW_SATURATED  2
#define FIXED_OVERFLOW_ERROR      3

// Operation of the programme when the FIXED number range is exceeded:
// a) no detection
//#define FIXED_OVERFLOW_DETECTION   FIXED_OVERFLOW_IGNORED

// b) use of saturated arithmetic rule, if the constant FIXED_PRINT_OVERFLOW_MESSAGE is defined
// the FixedOverflowMessage function will be called
//#define FIXED_OVERFLOW_DETECTION   FIXED_OVERFLOW_SATURATED

// c) return FIXED_OVERFLOW value, if the constant FIXED_PRINT_OVERFLOW_MESSAGE is defined
// the FixedOverflowMessage function will be called
#define FIXED_OVERFLOW_DETECTION   FIXED_OVERFLOW_ERROR



//The fixed class is designed to store real fixed-point numbers.
// Format: Q1.14:
//  - bits 0-13 -> fractional part
//  - bit 14 -> integer part
// - bit 15 -> sign( if 1 the number is negative )
// Note: the number is coded in the supplementary system to two
// Range: ( -2.0, 2.0 )		Note: 2 and -2 are unattainable
// Accuracy: ~5 decimal places

class fixed {

public:
	int16_t  fix;
  inline fixed(short f = 0){ fix = f;};

  inline friend int8_t operator == (fixed a, fixed b){return a.fix==b.fix; };
  inline friend int8_t operator != (fixed a, fixed b){return a.fix!=b.fix; };

  inline friend short operator >> (fixed f, uint8_t n){return f.fix >> n; };
  inline friend uint32_t operator << (fixed f, uint8_t n){return f.fix << n; };

  inline friend int8_t operator < (fixed a, fixed b){return a.fix < b.fix; };
  inline friend int8_t operator > (fixed a, fixed b){return a.fix > b.fix; };


  inline friend short operator & (fixed f, short b){ return f.fix & b; };

  inline friend fixed operator - (fixed f){ return fixed(-(f.fix)); };

  // Basic operations on fixed numbers:
  #if FIXED_OVERFLOW_DETECTION == FIXED_OVERFLOW_IGNORED
  inline friend fixed operator + (fixed a, fixed b) {return (a.fix + b.fix);}
  inline friend fixed operator - (fixed a, fixed b) {return (a.fix - b.fix);}
  inline friend fixed operator * (fixed a, fixed b) { return (((int32_t)a.fix * (int32_t)b.fix) >> FIXED_FBITS); }
  inline friend fixed operator / (fixed a, fixed b){
	int32_t div = (((int32_t)a.fix << (FIXED_FBITS+1)) / (int32_t)b.fix);
    return fixed((div>>1) + (div & 1L));
  }
  #else
  friend fixed operator + (fixed a, fixed b);
  friend fixed operator - (fixed a, fixed b);
  friend fixed operator * (fixed a, fixed b);
  friend fixed operator / (fixed a, fixed b);
  #endif

  inline friend fixed operator /= (fixed a, fixed b){return a/b;};
  inline friend fixed operator *= (fixed a, fixed b){return a*b;};
  inline friend fixed operator -= (fixed a, fixed b){return a-b;};
  inline friend fixed operator += (fixed a, fixed b){return a+b;};

  friend float fixed_to_float(fixed a);
  friend fixed float_to_fixed(float a);
  friend const char* fixed_to_bin(fixed liczba);
  friend const char* fixed_to_dec(fixed liczba);
  friend fixed sqrt(fixed a);

};


// Prototype of the FixedOverflowMessage function,
// which prints a message in the event of an exceedance during the calculation on a fixed type
#if FIXED_OVERFLOW_DETECTION != FIXED_OVERFLOW_IGNORED
  #ifdef FIXED_PRINT_OVERFLOW_MESSAGE
	#ifdef FIXED_USE_EXTERNAL_OVERFLOW_MESSAGE
 		extern void FixedOverflowMessage(const char *surce_file, int source_line, const char *message);
#else
 		void FixedOverflowMessage(const char *surce_file, int source_line, const char *message);
	#endif
  #endif
#endif


#endif /* FIXED_H_ */
