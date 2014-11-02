#include "random.h"
#include <random>
#include <time.h>

#include "math.h"

namespace math
{

#define MBIG SIZE_MAX
#define MSEED (161803398)

random::random()
	: m_inext(0)
	, m_inextp(0)

{
	random(static_cast<size_t>(time(nullptr)));
}

random::random(size_t seed)
	: m_inext(0)
	, m_inextp(0)
{
	size_t ii;
    size_t mj, mk;
  
	size_t subtraction = (seed == 0) ? SIZE_MAX : seed;
    mj = MSEED - subtraction;
    m_seedArray[55] = mj;
    mk=1;
    for (int i=1; i<55; i++) 
	{  
        ii = (21 * i) % 55;
        m_seedArray[ii] = mk;
        mk = mj - mk;
        
		if (mk<0) 
			mk+=MBIG;

        mj = m_seedArray[ii];
    }
    
	for (int k=1; k<5; k++) 
	{
        for (int i=1; i<56; i++) 
		{
			m_seedArray[i] -= m_seedArray[1+(i+30)%55];
			if (m_seedArray[i] < 0) 
				m_seedArray[i] += MBIG;
        }
    }

    m_inext=0;
    m_inextp = 21;
}

double random::Sample()
{
	return (SampleInternal() * (1.0 / MBIG));
}

int random::Next()
{
	return Next(INT_MAX);
}

int random::Next(int max)
{
	if ( max < 0 )
	{
		throw std::invalid_argument("value of max should be positive");
	}

	return static_cast<int>( Sample() * max );
}

int random::Next(int min, int max)
{	
	if ( min > max )
	{
		throw std::invalid_argument("min should be less than max");
	}

	size_t range = max - min;
    if( range <= SIZE_MAX) 
	{
    	return ((int)(Sample() * range) + min);
    }
    else 
	{
		return (int)((SampleForLargeRange() * range) + min);
	}
}

double random::NextDouble()
{
	return Sample();
}

size_t random::SampleInternal()
{
	size_t locINext = m_inext;
	size_t locINextp = m_inextp;
	
	if (++locINext >= 56) locINext = 1;
	if (++locINextp>= 56) locINextp = 1;
 
    size_t retVal = ( m_seedArray[locINext] - m_seedArray[locINextp] );
 
    if (retVal == MBIG) 
		--retVal;

    m_seedArray[locINext] = retVal;
 
    m_inext = locINext;
    m_inextp = locINextp;
 
    return retVal;
}

double random::SampleForLargeRange() 
{
    // The distribution of double value returned by Sample
    // is not distributed well enough for a large range.
    // If we use Sample for a range [Int32.MinValue..Int32.MaxValue)
    // We will end up getting even numbers only.
 
    int result = (int)SampleInternal();
    // Note we can't use addition here. The distribution will be bad if we do that.
    bool negative = (SampleInternal() % 2 == 0) ? true : false;  // decide the sign based on second sample
    if( negative) 
	{
        result = -result;
    }
    double d = result;
    d += (SIZE_MAX - 1); // get a number in range [0 .. 2 * SIZE_MAX - 1)
    d /= 2.0 * SIZE_MAX - 1 ;
    return d;
}
  
} // math