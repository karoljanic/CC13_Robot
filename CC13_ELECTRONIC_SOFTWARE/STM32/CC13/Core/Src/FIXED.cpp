#include "FIXED.h"


#if FIXED_OVERFLOW_DETECTION != FIXED_OVERFLOW_IGNORED
  #ifndef FIXED_USE_EXTERNAL_OVERFLOW_MESSAGE
	#ifdef FIXED_PRINT_OVERFLOW_MESSAGE
	  #include <stdio.h>
	#endif
  #endif
#endif

#if FIXED_OVERFLOW_DETECTION!=FIXED_OVERFLOW_IGNORED
  #if FIXED_OVERFLOW_DETECTION!=FIXED_OVERFLOW_SATURATED
	#if FIXED_OVERFLOW_DETECTION!=FIXED_OVERFLOW_ERROR
	  #error "Bad definition of FIXED_OVERFLOW_DETECTION"
	#endif
  #endif
#endif


// Conversion from fixed to float type
inline float fixed_to_float(fixed a) {
	return ((float) a.fix) / FIXED_ONE;
}

// Conversion from float to fixed type. The number a must be in the range (-2.0, 2.0)
fixed float_to_fixed(float a)
{
#if FIXED_OVERFLOW_DETECTION != FIXED_OVERFLOW_IGNORED
	if (a > fixed_to_float(FIXED_MAXIMUM))
	{
#ifdef FIXED_PRINT_OVERFLOW_MESSAGE
		FixedOverflowMessage(__FILE__, __LINE__, __func__);
#endif
#if FIXED_OVERFLOW_DETECTION==FIXED_OVERFLOW_SATURATED
		return FIXED_MAXIMUM;
#else
		return FIXED_OVERFLOW;
#endif
	}
	if (a < fixed_to_float(FIXED_MINIMUM))
	{
#ifdef FIXED_PRINT_OVERFLOW_MESSAGE
		FixedOverflowMessage(__FILE__, __LINE__, __func__);
#endif
#if FIXED_OVERFLOW_DETECTION==FIXED_OVERFLOW_SATURATED
		return FIXED_MINIMUM;
#else
		return FIXED_OVERFLOW;
#endif
	}
#endif
	int32_t tmp = a * (1L<<15);
    return (tmp>>1) + (tmp & 1L);  // correction of the last bit
}

//The array data structure necessary for the next two functions
static char fixed_txt_buffer[20];

// Conversion from fixed type to character string in binary system
const char* fixed_to_bin(fixed liczba)
{
	if (liczba==FIXED_OVERFLOW)
	{
#if FIXED_OVERFLOW_DETECTION == FIXED_OVERFLOW_IGNORED
		return "-2.00000000000000";
#else
		return "Overflow error   ";
#endif
	}
	uint8_t p = 0;
	int8_t i;
	fixed_txt_buffer[p++] = (liczba >> 15) & 1 ? '-' : ' ';
	if (liczba<0) liczba = -liczba;
	fixed_txt_buffer[p++] = (liczba >> 14) & 1 ? '1' : '0';
	fixed_txt_buffer[p++] = '.';
	for(i = 13; i >= 0; i--) fixed_txt_buffer[p++] = (liczba >> i) & 1 ? '1' : '0';
	fixed_txt_buffer[p++] = '\0';
	return fixed_txt_buffer;
}

// Conversion from fixed type to character string in decimal system
const char* fixed_to_dec(fixed liczba)
{
#if (FIXED_OVERFLOW_DETECTION!=FIXED_OVERFLOW_IGNORED)
     if (liczba==FIXED_OVERFLOW){ return "Overflow "; }
#endif
	unsigned char p = 0;
	fixed_txt_buffer[p++] = (liczba >> 15) & 1 ? '-' : ' ';
	if (liczba < 0) liczba = -liczba;
	fixed_txt_buffer[p++] = (liczba >> 14) & 1 ? '1' : '0';
	fixed_txt_buffer[p++] = '.';
	uint32_t fr = liczba & 0b0011111111111111;
	do
	{
		fr = (fr & 0b0011111111111111) * 10;
		fixed_txt_buffer[p++] = '0' + (fr >> 14);
	}
	while (fr != 0);
	fixed_txt_buffer[p++] = '\0';
	return fixed_txt_buffer;
}


/*
 * fixed operator + (fixed a, fixed b);
 *
 * Addition with overflow detection.
 * The versions without overflow detection are inlined in the header.
 */
#if (!(FIXED_OVERFLOW_DETECTION == FIXED_OVERFLOW_IGNORED))
fixed operator + (fixed a, fixed b)
{
#if FIXED_OVERFLOW_DETECTION==FIXED_OVERFLOW_ERROR
	if (a==FIXED_OVERFLOW || b==FIXED_OVERFLOW)
		return FIXED_OVERFLOW;
#endif

	// Use unsigned integers because overflow with signed integers is
	// an undefined operation (http://www.airs.com/blog/archives/120).
	uint16_t sum = (uint16_t)a.fix + (uint16_t)b.fix;

	if ((a.fix^b.fix) & 0x8000)  // if sign(a) != sign(b) then overflow can't happen
	{
		return sum;
	}
#if FIXED_OVERFLOW_DETECTION==FIXED_OVERFLOW_SATURATED
	if ((a.fix ^ sum) & 0x8000)
	{
		sum = (a.fix >= 0) ? FIXED_MAXIMUM : FIXED_MINIMUM;
#ifdef FIXED_PRINT_OVERFLOW_MESSAGE
		FixedOverflowMessage(__FILE__, __LINE__, __func__);
#endif
	} else if (sum==FIXED_OVERFLOW)
	{
		sum = FIXED_MINIMUM;
#ifdef FIXED_PRINT_OVERFLOW_MESSAGE
		FixedOverflowMessage(__FILE__, __LINE__, __func__);
#endif
	}
#endif
#if FIXED_OVERFLOW_DETECTION==FIXED_OVERFLOW_ERROR
	if (((a.fix ^ sum) & 0x8000))
	{ sum = FIXED_OVERFLOW;
	}
#ifdef FIXED_PRINT_OVERFLOW_MESSAGE
	if (sum==FIXED_OVERFLOW)
		FixedOverflowMessage(__FILE__, __LINE__, __func__);
#endif
#endif
	return sum;
}
#endif


/*
 * fixed fixed_sub(fixed a, fixed b);
 *
 * Subtraction with overflow detection.
 * The versions without overflow detection are inlined in the header.
 */
#if FIXED_OVERFLOW_DETECTION != FIXED_OVERFLOW_IGNORED
fixed operator - (fixed a, fixed b)
{
#if FIXED_OVERFLOW_DETECTION==FIXED_OVERFLOW_ERROR
	if (a==FIXED_OVERFLOW || b==FIXED_OVERFLOW)
		return FIXED_OVERFLOW;
#endif
	int16_t diff = a.fix - b.fix;
	if (!((a.fix^b.fix) & 0x8000))  // if sign(a) == sign(b) then overflow can't happen
	{
		return diff;
	}
#if FIXED_OVERFLOW_DETECTION==FIXED_OVERFLOW_SATURATED
	if ((a.fix ^ diff) & 0x8000)
	{
		diff = (a.fix >= 0) ? FIXED_MAXIMUM : FIXED_MINIMUM;
#ifdef FIXED_PRINT_OVERFLOW_MESSAGE
		FixedOverflowMessage(__FILE__, __LINE__, __func__);
#endif
	} else if (diff==FIXED_OVERFLOW)
	{
		diff = FIXED_MINIMUM;
#ifdef FIXED_PRINT_OVERFLOW_MESSAGE
		FixedOverflowMessage(__FILE__, __LINE__, __func__);
#endif
	}
#endif
#if FIXED_OVERFLOW_DETECTION==FIXED_OVERFLOW_ERROR
	if ((a.fix ^ diff) & 0x8000)
	{ diff = FIXED_OVERFLOW;
	}
#ifdef FIXED_PRINT_OVERFLOW_MESSAGE
	if (diff==FIXED_OVERFLOW)
		FixedOverflowMessage(__FILE__, __LINE__, __func__);
#endif
#endif
	return diff;
}
#endif


/*
 * fixed fixed_mul(fixed a, fixed b)
 *
 * Multiplication with overflow detection.
 * The versions without overflow detection are inlined in the header.
 */
#if FIXED_OVERFLOW_DETECTION != FIXED_OVERFLOW_IGNORED
fixed operator * (fixed a, fixed b)
{
#if FIXED_OVERFLOW_DETECTION==FIXED_OVERFLOW_ERROR
	if (a==FIXED_OVERFLOW || b==FIXED_OVERFLOW)
		return FIXED_OVERFLOW;
#endif
	fixed mul = (((int32_t)a.fix * (int32_t)b.fix) >> FIXED_FBITS);

#if FIXED_OVERFLOW_DETECTION==FIXED_OVERFLOW_SATURATED
	if (((a.fix ^ b.fix ^ mul) & 0x8000) && a.fix && b.fix)
	{
		mul = ((a.fix ^ b.fix) & 0x8000) ? FIXED_MINIMUM : FIXED_MAXIMUM;
#ifdef FIXED_PRINT_OVERFLOW_MESSAGE
		FixedOverflowMessage(__FILE__, __LINE__, __func__);
#endif
	} else if (mul==FIXED_OVERFLOW)
	{
		mul = FIXED_MINIMUM;
#ifdef FIXED_PRINT_OVERFLOW_MESSAGE
		FixedOverflowMessage(__FILE__, __LINE__, __func__);
#endif
	}
#endif
#if FIXED_OVERFLOW_DETECTION==FIXED_OVERFLOW_ERROR
	if (((a.fix ^ b.fix ^ mul.fix) & 0x8000) && a.fix && b.fix)
	{ mul = FIXED_OVERFLOW;
	}
#ifdef FIXED_PRINT_OVERFLOW_MESSAGE
	if (mul==FIXED_OVERFLOW)
		FixedOverflowMessage(__FILE__, __LINE__, __func__);
#endif
#endif
	return mul;
}
#endif


/*
 * fixed fixed_div(fixed a, fixed b);
 *
 * Division with overflow detection or rounding.
 * The version without overflow detection is inlined in the header.
 */
#if FIXED_OVERFLOW_DETECTION != FIXED_OVERFLOW_IGNORED
fixed operator / (fixed a, fixed b)
{
#if FIXED_OVERFLOW_DETECTION==FIXED_OVERFLOW_ERROR
	if (a==FIXED_OVERFLOW || b==FIXED_OVERFLOW)
		return FIXED_OVERFLOW;
#endif
    if (b.fix==0) // dzielenie przez zero
	{
#ifdef FIXED_PRINT_OVERFLOW_MESSAGE
		FixedOverflowMessage(__FILE__, __LINE__, __func__);
#endif
#if FIXED_OVERFLOW_DETECTION==FIXED_OVERFLOW_SATURATED
			return (a.fix>=0) ? FIXED_MAXIMUM : FIXED_MINIMUM;
#endif
#if FIXED_OVERFLOW_DETECTION==FIXED_OVERFLOW_ERROR
			return FIXED_OVERFLOW;
#endif
    }
	int32_t div = (((int32_t)a.fix << (FIXED_FBITS+1)) / (int32_t)b.fix);
	if (!((a.fix ^ b.fix) & 0x8000))
	{
		if (div & 0xffff0000)
		{
#ifdef FIXED_PRINT_OVERFLOW_MESSAGE
			FixedOverflowMessage(__FILE__, __LINE__, __func__);
#endif
#if FIXED_OVERFLOW_DETECTION==FIXED_OVERFLOW_SATURATED
			return div = FIXED_MAXIMUM;
#endif
#if FIXED_OVERFLOW_DETECTION==FIXED_OVERFLOW_ERROR
			return div = FIXED_OVERFLOW;
#endif
		}
		div = (div>>1) + (div & 1L);
	} else
	{
		if ((~(div | 0x0000ffff)) && (div))
		{
#ifdef FIXED_PRINT_OVERFLOW_MESSAGE
			FixedOverflowMessage(__FILE__, __LINE__, __func__);
#endif
#if FIXED_OVERFLOW_DETECTION==FIXED_OVERFLOW_SATURATED
			return div = FIXED_MINIMUM;
#endif
#if FIXED_OVERFLOW_DETECTION==FIXED_OVERFLOW_ERROR
			return div = FIXED_OVERFLOW;
#endif
		}
		div = (div>>1) + (div & 1L);
	}
	return div;
}
#endif // FIXED_OVERFLOW_DETECTION


// Calculation of the square root of the fixed type number
fixed sqrt(fixed x)
{
#if FIXED_OVERFLOW_DETECTION==FIXED_OVERFLOW_ERROR
	if (x==FIXED_OVERFLOW)
		return FIXED_OVERFLOW;
#endif
    if(x<0) x=-x;	// Incompatibility with the definition of a sqrt, but why not
	uint32_t op, res, one;
	op = x << (FIXED_FBITS + 2);
	one = 1 << (2 * FIXED_FBITS + 2);
	res = 0;
	while (one > op)
		one >>= 2;

	while (one != 0) {
		if (op >= res + one) {
			op -= res + one;
			res = (res >> 1) + one;
		} else
			res >>= 1;
		one >>= 2;
	}
	return (res >> 1) + (res & 1L);
}


/*	void FixedOverflowMessage(char *surce_file, int source_line, char *message)
 * Generation message of an exceedance a scope while performing arithmetic operations
 * If FIXED_USE_EXTERNAL_OVERFLOW_MESSAGE has not been defined,the following function prints the message text to the standard output stream.
 * If another action is required, then the FIXED_USE_EXTERNAL_OVERFLOW_MESSAGE constants and an external function with the above prototype must be defined.
 */
#ifdef FIXED_PRINT_OVERFLOW_MESSAGE
  #ifndef FIXED_USE_EXTERNAL_OVERFLOW_MESSAGE
	#if FIXED_OVERFLOW_DETECTION == FIXED_OVERFLOW_SATURATED
		void FixedOverflowMessage(const char *surce_file, int source_line, const char *message)
		{
			printf("\n\rOVERFLOW detected in method %s  (%s : %d)", message, surce_file, source_line);
			printf("\nSaturated Arithmetic was used!\n\r");
		}
	#elif FIXED_OVERFLOW_DETECTION == FIXED_OVERFLOW_ERROR
		void FixedOverflowMessage(const char *surce_file, int source_line, const char *message)
		{
			printf("\n\rOVERFLOW detected in method %s  (%s : %d)", message, surce_file, source_line);
			printf("\n\rThe FIXED_OVERFLOW value was returned!\n\r");
		}
	#endif
  #endif
#endif


