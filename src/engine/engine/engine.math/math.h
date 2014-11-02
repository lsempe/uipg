#pragma once

#include <math.h>
#include <random>

namespace math
{


// TOOD: Remove this as soon as VS201x adds constexpr support, in the meantime the values that use it will remain
// dynamically initialized constants so:
// - The compiler may allow the use of expressions that have not been yet initialized (dynamic constant initialization)
// - These values may not be used anywhere where constant expressions are required (ie. int myArray[ MyConstant ])
#ifndef CONSTEXPR
#define constexpr const
#endif

// constant expressions

constexpr float Pi = 3.1415926535897932384626433832795f;
constexpr float TwoPi = Pi * 2.0f;
constexpr float HalfPi = Pi / 2.0f;
constexpr float QuarterPi = Pi / 4.0f;
constexpr float Epsilon = 1.0e-10f;
constexpr float E = 2.71828182846f;

// templates

template <typename T>
typename std::enable_if<std::is_signed<T>::value, T>::type const Abs(T a)
{
	return (a < 0) ? -a : a; 
}

template <typename T>
typename std::enable_if<std::is_unsigned<T>::value, T>::type const Abs(T a)
{
	return a; 
}

template <typename T>
constexpr T Square(T x) { return x * x; }

template <typename T>
constexpr T Min(T a, T b) {  return ( a < b ) ? a : b; }

template <typename T>
constexpr T Max(T a, T b) {  return ( a > b ) ? a : b; }

template <typename T>
constexpr void Clamp(T& value, const T low, const T high) { ( value < low ) ? (value = low) : (value > high) ? ( value = high ) : (value); }

template <typename T>
T Lerp(const T a, const T b, T t) { return a + t * ( b - a ); }

template <typename T>
T Smooth(const T x) { return x * x * (3 - 2 * x); }

template <typename T>
typename std::enable_if<std::is_integral<T>::value, bool>::type IsOdd(T x)
{ return x & 0x1; }

template <typename T>
typename std::enable_if<std::is_integral<T>::value, bool>::type IsEven(T x)
{ return (x % 2 == 0); }

// inlines 

template <typename T>
constexpr float DegreesToRadians(T degrees) { return degrees * 0.017453292519f; }

template <typename T>
constexpr float RadiansToDegrees(T radians) { return radians * 57.29577951308f; }

inline constexpr bool IsEqual(float x, float y, float epsilon = Epsilon)
{
	return Abs(x-y) <= epsilon;
}

inline constexpr bool IsWithin(float x, float epsilon = Epsilon)
{
	return Abs(x-epsilon) <= epsilon;
}

// Returns a random gaussian
inline float RandomGaussian(float deviation, float mean)
{
	float n = (((float)rand() / (float)RAND_MAX) * 2.0f * deviation - deviation) + mean;

	if ((float)rand() / (float)RAND_MAX > 0.5f)
	{
		while ((float)rand() / (float)RAND_MAX > 0.5f)
		{
			n += ((float)rand() / (float)RAND_MAX) * deviation;
		}
	}
	else
	{
		while ((float)rand() / (float)RAND_MAX > 0.5f)
		{
			n -= ((float)rand() / (float)RAND_MAX) * deviation;
		}
	}

	return n;
}

template <typename T>
typename std::enable_if<std::numeric_limits<size_t>::digits == 64, T>::type NearestPowerOfTwo(size_t value)
{
	size_t v = value; // compute the next highest power of 2 of 64-bit v

	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v |= v >> 32;	
	v++;

	return v;
}

template <typename T>
typename std::enable_if<std::numeric_limits<size_t>::digits != 64, T>::type NearestPowerOfTwo(size_t value)
{
	size_t v = value; // compute the next highest power of 2 of 32-bit v

	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;	
	v++;

	return v;
}

} // math