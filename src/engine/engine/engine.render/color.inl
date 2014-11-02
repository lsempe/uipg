//////////////////////////////////////////////////////////////////////////
// Inlines

inline color::operator const unsigned int() const 
{
	unsigned char r = m_color[channel::R] >= 1.0f ? 0xff : m_color[channel::R] <= 0.0f ? 0x00 : static_cast<unsigned char> (m_color[channel::R] * 255.0f + 0.5f);
	unsigned char g = m_color[channel::G] >= 1.0f ? 0xff : m_color[channel::G] <= 0.0f ? 0x00 : static_cast<unsigned char> (m_color[channel::G] * 255.0f + 0.5f);
	unsigned char b = m_color[channel::B] >= 1.0f ? 0xff : m_color[channel::B] <= 0.0f ? 0x00 : static_cast<unsigned char> (m_color[channel::B] * 255.0f + 0.5f);
	unsigned char a = m_color[channel::A] >= 1.0f ? 0xff : m_color[channel::A] <= 0.0f ? 0x00 : static_cast<unsigned char> (m_color[channel::A] * 255.0f + 0.5f);

	return (a << 24) | (r << 16) | (g << 8) | b;
}

inline color::operator unsigned int()
{
	unsigned char r = m_color[channel::R] >= 1.0f ? 0xff : m_color[channel::R] <= 0.0f ? 0x00 : static_cast<unsigned char> (m_color[channel::R] * 255.0f + 0.5f);
	unsigned char g = m_color[channel::G] >= 1.0f ? 0xff : m_color[channel::G] <= 0.0f ? 0x00 : static_cast<unsigned char> (m_color[channel::G] * 255.0f + 0.5f);
	unsigned char b = m_color[channel::B] >= 1.0f ? 0xff : m_color[channel::B] <= 0.0f ? 0x00 : static_cast<unsigned char> (m_color[channel::B] * 255.0f + 0.5f);
	unsigned char a = m_color[channel::A] >= 1.0f ? 0xff : m_color[channel::A] <= 0.0f ? 0x00 : static_cast<unsigned char> (m_color[channel::A] * 255.0f + 0.5f);

	return (a << 24) | (r << 16) | (g << 8) | b;
}

inline color::operator const float* () const
{
	return m_color;
}

inline color::operator float* ()
{
	return m_color;
}

inline float color::operator[] (int i) const
{
	assert( 0 <= i && i <= 2 );
	if ( i < 0 )
		i = 0;
	else if ( i > 2 )
		i = 2;

	return m_color[i];
}

inline float& color::operator[] (int i)
{
	assert( 0 <= i && i <= 2 );
	if ( i < 0 )
		i = 0;
	else if ( i > 2 )
		i = 2;

	return m_color[i];
}

inline float color::R () const
{
	return m_color[channel::R];
}

inline float& color::R ()
{
	return m_color[channel::R];
}

inline float color::G () const
{
	return m_color[channel::G];
}

inline float& color::G ()
{
	return m_color[channel::G];
}

inline float color::B () const
{
	return m_color[channel::B];
}

inline float& color::B ()
{
	return m_color[channel::B];
}

inline float color::A () const
{
	return m_color[channel::A];
}

inline float& color::A ()
{
	return m_color[channel::A];
}

