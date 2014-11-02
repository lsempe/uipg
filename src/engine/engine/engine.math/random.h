#pragma once

namespace math
{

class random
{
public:

	random();
	random(size_t seed);

	// returns an int [0..INT_MAX]
	virtual int Next();

	// returns an int [0..max]
	virtual int Next(int max);
	
	// returns an int [min..max]
	virtual int Next(int min, int max);
	
	// returns a double [0..1)
	virtual double NextDouble();

	// returns a random number [0..1)
	virtual double Sample();

private:

	size_t SampleInternal();
	double SampleForLargeRange();

	size_t m_seedArray[56];
	size_t m_inext;
	size_t m_inextp;
	
};

} // math