#pragma once

namespace ui
{

class digits
{
public:
	
	digits(int number)
		: m_number(number)
	{
		int n = number;
		while ( n > 10 )
		{
			m_digits.push_back(n % 10);
			n /= 10;
		}
		
		m_digits.push_back(n);

	}

	wchar_t operator()(unsigned int position)
	{
		if ( m_number >= pow(10, position) )
			return static_cast<wchar_t>(L'0' + m_digits[ m_digits.size() - 1 - position ]);

		return 0;
	}

private:

	std::vector<int> m_digits;
	int m_number;
};

} // ui	