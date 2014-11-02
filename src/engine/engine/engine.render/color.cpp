#include <memory.h>

#include "color.h"

using namespace render;

const color color::BLACK(0.0f, 0.0f, 0.0f, 1.f);
const color color::WHITE(1.0f, 1.0f, 1.0f, 1.f);
const color color::RED	(1.0f, 0.0f, 0.0f, 1.f);
const color color::GREEN(0.0f, 1.0f, 0.0f, 1.f);
const color color::BLUE	(0.0f, 0.0f, 1.0f, 1.f);

const color color::ALICEBLUE(0xf0, 0xf8);
const color color::ANTIQUEWHITE(0xfa, 0xeb, 0xd7);
const color color::AQUA(0x00, 0xff, 0xff);
const color color::AQUAMARINE(0x7f, 0xff, 0xd4);
const color color::AZURE(0xf0, 0xff, 0xff);
const color color::BEIGE(0xf5, 0xf5, 0xdc);
const color color::BISQUE(0xff, 0xe4, 0xc4);
const color color::BLANCHEDALMOND(0xff, 0xeb, 0xcd);
const color color::BLUEVIOLET(0x8a, 0x2b, 0xe2);
const color color::BROWN(0xa5, 0x2a, 0x2a);
const color color::BURLYWOOD(0xde, 0xb8, 0x87);
const color color::CADETBLUE(0x5f, 0x9e, 0xa0);
const color color::CHARTREUSE(0x7f, 0xff, 0x00);
const color color::CHOCOLATE(0xd2, 0x69, 0x1e);
const color color::CORAL(0xff, 0x7f, 0x50);
const color color::CORNFLOWERBLUE(0x64, 0x95, 0xed);
const color color::CORNSILK(0xff, 0xf8, 0xdc);
const color color::CRIMSON(0xdc, 0x14, 0x3c);
const color color::CYAN(0x00, 0xff, 0xff);
const color color::DARKBLUE(0,0,0x8b);
const color color::DARKCYAN(0, 0x8b, 0x8b);
const color color::DARKGOLDENROD(0xb8, 0x86, 0x0b);
const color color::DARKGRAY(0xa9, 0xa9, 0xa9);
const color color::DARKGREY(0xa9, 0xa9, 0xa9);
const color color::DARKGREEN(0, 0x64, 0);
const color color::DARKKHAKI(0xbd, 0xb7, 0x68);
const color color::DARKMAGENTA(0x8b, 0, 0x8b);
const color color::DARKOLIVEGREEN(0x55, 0x6b, 0x2f);
const color color::DARKORANGE(0xff, 0x8c, 0);
const color color::DARKORCHID(0x99, 0x32, 0xcc);
const color color::DARKRED(0x8b,0,0);
const color color::DARKSALMON(0xe9, 0x96, 0x7a);
const color color::DARKSEAGREEN(0x8f, 0xbc, 0x8f);
const color color::DARKSLATEBLUE(0x48, 0x3d, 0x8b);
const color color::DARKSLATEGRAY(0x2f, 0x4f, 0x4f);
const color color::DARKSLATEGREY(0x2f, 0x4f, 0x4f);
const color color::DARKTURQUOISE(0, 0xce, 0xd1);
const color color::DARKVIOLET(0x94, 0, 0xd3);
const color color::DEEPPINK(0xff, 0x14, 0x93);
const color color::DEEPSKYBLUE(0, 0xbf);
const color color::DIMGRAY(0x69, 0x69, 0x69);
const color color::DIMGREY(0x69, 0x69, 0x69);
const color color::DODGERBLUE(0x1e, 0x90);
const color color::FIREBRICK(0xb2, 0x22, 0x22);
const color color::FLORALWHITE(0xff, 0xaf, 0xf0);
const color color::FORESTGREEN(0x22, 0x8b, 0x22);
const color color::FUCSHIA(0xff, 0);
const color color::GAINSBORO(0xdc, 0xdc, 0xdc);
const color color::GHOSTWHITE(0xf8, 0xf8);
const color color::GOLD(0xff, 0xd7, 0);
const color color::GOLDENROD(0xda, 0xa5, 0x20);
const color color::GRAY(0x80, 0x80, 0x80);
const color color::GREY(0x80, 0x80, 0x80);
const color color::GREENYELLOW(0xad, 0xff, 0x2f);
const color color::HONEYDEW(0xf0, 0xff, 0xf0);
const color color::HOTPINK(0xff, 0x69, 0xb4);
const color color::INDIANRED(0xcd, 0x5c, 0x5c);
const color color::INDIGO(0x4b, 0, 0x82);
const color color::IVORY(0xff, 0xff, 0xf0);
const color color::KHAKI(0xf0, 0xe6, 0x8c);
const color color::LAVENDER(0xe6, 0xe6, 0xfa);
const color color::LAWNGREEN(0x7c, 0xfc, 0);
const color color::LEMONCHIFFON(0xff, 0xfa, 0xcd);
const color color::LIGHTBLUE(0xad, 0xd8, 0x86);
const color color::LIGHTCYAN(0xf0, 0x80, 0x80);
const color color::LIGHTGOLDENRODYELLOW(0xfa, 0xfa, 0xd2);
const color color::LIGHTGRAY(0xd3, 0xd3, 0xd3);
const color color::LIGHTGREY(0xd3, 0xd3, 0xd3);
const color color::LIGHTGREEN(0x90, 0xee, 0x90);
const color color::LIGHTPINK(0xff, 0xb6, 0xc1);
const color color::LIGHTSALMON(0xff, 0xa0, 0x7a);
const color color::LIGHTSEAGREEN(0x20, 0xb2, 0xaa);
const color color::LIGHTSKYBLUE(0x87, 0xce, 0xfa);
const color color::LIGHTSLATEGRAY(0x77, 0x88, 0x99);
const color color::LIGHTSLATEGREY(0x77, 0x88, 0x99);
const color color::LIGHTSTEELBLUE(0xb0, 0xc4, 0xde);
const color color::LIGHTYELLOW(0xff, 0xff, 0xe0);
const color color::LIME(0, 0xff, 0);
const color color::LIMEGREEN(0x32, 0xcd, 0x32);
const color color::LINEN(0xfa, 0xf0, 0xe6);
const color color::MAGENTA(0xff, 0);
const color color::MARRON(0x80, 0, 0);
const color color::MEDIUMAQUAMARINE(0x66, 0xcd, 0xaa);
const color color::MEDIUMBLUE(0, 0, 0xcd);
const color color::MEDIUMORCHID(0xba, 0x55, 0xd3);
const color color::MEDIUMPURPLE(0x93, 0x70, 0xd8);
const color color::MEDIUMSEAGREEN(0x3c, 0xb3, 0x71);
const color color::MEDIUMSLATEBLUE(0x7b, 0x68, 0xee);
const color color::MEDIUMSPRINGGREEN(0, 0xfa, 0x9a);
const color color::MEDIUMTURQUOISE(0x48, 0xd1, 0xcc);
const color color::MEDIUMVIOLETRED(0xc7, 0x15, 0x85);
const color color::MIDNIGHTBLUE(0x19, 0x19, 0x70);
const color color::MINTCREAM(0xf5, 0xff, 0xfa);
const color color::MISTYROSE(0xff, 0xe4, 0xe1);
const color color::MOCCASIN(0xff, 0xe4, 0xb5 );
const color color::NAVAJOWHITE(0xff, 0xde, 0xad );
const color color::NAVY(0x00, 0x00, 0x80 );
const color color::OLDLACE(0xfd, 0xf5, 0xe6 );
const color color::OLIVE(0x80, 0x80, 0x00 );
const color color::OLIVEDRAB(0x6b, 0x8e, 0x23 );
const color color::ORANGE(0xff, 0xa5, 0x00 );
const color color::ORANGERED(0xff, 0x45, 0x00);
const color color::ORCHID(0xda, 0x70, 0xd6 );
const color color::PALEGOLDENROD(0xee, 0xe8, 0xaa );
const color color::PALEGREEN(0x98, 0xfb, 0x98 );
const color color::PALETURQUOISE(0xaf, 0xee, 0xee );
const color color::PALEVIOLETRED(0xd8, 0x70, 0x93 );
const color color::PAPAYAWHIP(0xff, 0xef, 0xd5 );
const color color::PEACHPUFF(0xff, 0xda, 0xb9 );
const color color::PERU(0xcd, 0x85, 0x3f );
const color color::PINK(0xff, 0xc0, 0xcb );
const color color::PLUM(0xdd, 0xa0, 0xdd );
const color color::POWDERBLUE(0xb0, 0xe0, 0xe6 );
const color color::PURPLE(0x80, 0x00, 0x80 );
const color color::ROSYBROWN(0xbc, 0x8f, 0x8f );
const color color::ROYALBLUE(0x41, 0x69, 0xe1 );
const color color::SADDLEBROWN(0x8b, 0x45, 0x13 );
const color color::SALMON(0xfa, 0x80, 0x72 );
const color color::SANDYBROWN(0xf4, 0xa4, 0x60 );
const color color::SEAGREEN(0x2e, 0x8b, 0x57 );
const color color::SEASHELL(0xff, 0xf5, 0xee );
const color color::SIENNA(0xa0, 0x52, 0x2d );
const color color::SILVER(0xc0, 0xc0, 0xc0);
const color color::SKYBLUE(0x87, 0xce, 0xeb );
const color color::SLATEBLUE(0x6a, 0x5a, 0xcd );
const color color::SLATEGRAY(0x70, 0x80, 0x90 );
const color color::SLATEGREY(0x70, 0x80, 0x90);
const color color::SNOW(0xff, 0xfa, 0xfa );
const color color::SPRINGGREEN(0x00, 0xff, 0x7f );
const color color::STEELBLUE(0x46, 0x82, 0xb4 );
const color color::TAN(0xd2, 0xb4, 0x8c );
const color color::TEAL(0x00, 0x80, 0x80 );
const color color::THISTLE(0xd8, 0xbf, 0xd8 );
const color color::TOMATO(0xff, 0x63, 0x47 );
const color color::TRANSPARENT(1.f, 0.f, 1.f, 0.f);
const color color::TURQUOISE(0x40, 0xe0, 0xd0 );
const color color::VIOLET(0xee, 0x82, 0xee );
const color color::WHEAT(0xf5, 0xde, 0xb3);
const color color::WHITESMOKE(0xf5, 0xf5, 0xf5);
const color color::YELLOWGREEN(0x9a, 0xcd, 0x32);
const color color::YELLOW(0xff, 0xff, 0x32);

const color color::INVALID(-1.f,-1.f,-1.f,-1.f);

namespace
{
	template <typename T>
	T Clamp(T value, T low, T high) 
	{ 
		( value < low ) ? (value = low) : (value > high) ? ( value = high ) : (value);
		return value;
	}
}

color::color (float fR, float fG, float fB, float fA)
{
	m_color[channel::R] = fR;
	m_color[channel::G] = fG;
	m_color[channel::B] = fB;
	m_color[channel::A] = fA;
}

color::color (float* fColor)
{
	m_color[channel::R] = fColor[channel::R];
	m_color[channel::G] = fColor[channel::G];
	m_color[channel::B] = fColor[channel::B];
	m_color[channel::A] = fColor[channel::A];
}

color::color (const color& rhs)
{
	m_color[channel::R] = rhs.m_color[channel::R];
	m_color[channel::G] = rhs.m_color[channel::G];
	m_color[channel::B] = rhs.m_color[channel::B];
	m_color[channel::A] = rhs.m_color[channel::A];
}

color::color(unsigned int color)
{
	const float k = 1.f/255.f;
	m_color[channel::R] = k * ((color >> 16) & 0x000000ff);
	m_color[channel::G] = k * ((color >> 8)  & 0x000000ff);
	m_color[channel::B] = k * (color         & 0x000000ff);
	m_color[channel::A] = k * (color >> 24);
}

color::color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
{
	m_color[channel::R] = static_cast<float>(red) / 255.f;
	m_color[channel::G] = static_cast<float>(green) / 255.f;
	m_color[channel::B] = static_cast<float>(blue) / 255.f;
	m_color[channel::A] = static_cast<float>(alpha) / 255.f;
}

color& color::operator = (const color& rhs)
{
	m_color[channel::R] = rhs.m_color[channel::R];
	m_color[channel::G] = rhs.m_color[channel::G];
	m_color[channel::B] = rhs.m_color[channel::B];
	m_color[channel::A] = rhs.m_color[channel::A];
	return *this;
}

bool color::operator == (const color& rhs) const
{
	return
		m_color[channel::R] == rhs.m_color[channel::R] &&
		m_color[channel::G] == rhs.m_color[channel::G] &&
		m_color[channel::B] == rhs.m_color[channel::B] &&
		m_color[channel::A] == rhs.m_color[channel::A];
}

bool color::operator != (const color& rhs) const
{
	return
		m_color[channel::R] != rhs.m_color[channel::R] ||
		m_color[channel::G] != rhs.m_color[channel::G] ||
		m_color[channel::B] != rhs.m_color[channel::B] ||
		m_color[channel::A] != rhs.m_color[channel::A];
}

color color::operator + (const color& rhs) const
{
	return color(
		::Clamp(m_color[channel::R] + rhs.m_color[channel::R], 0.f, 1.f),
		::Clamp(m_color[channel::G] + rhs.m_color[channel::G], 0.f, 1.f),
		::Clamp(m_color[channel::B] + rhs.m_color[channel::B], 0.f, 1.f),
		::Clamp(m_color[channel::A] + rhs.m_color[channel::A], 0.f, 1.f));
}

color color::operator - (const color& rhs) const
{
	return color(
		::Clamp(m_color[channel::R] - rhs.m_color[channel::R], 0.f, 1.f),
		::Clamp(m_color[channel::G] - rhs.m_color[channel::G], 0.f, 1.f),
		::Clamp(m_color[channel::B] - rhs.m_color[channel::B], 0.f, 1.f),
		::Clamp(m_color[channel::A] - rhs.m_color[channel::A], 0.f, 1.f));
}

color color::operator * (const color& rhs) const
{
	return color(
		::Clamp(m_color[channel::R] * rhs.m_color[channel::R], 0.f, 1.f),
		::Clamp(m_color[channel::G] * rhs.m_color[channel::G], 0.f, 1.f),
		::Clamp(m_color[channel::B] * rhs.m_color[channel::B], 0.f, 1.f),
		::Clamp(m_color[channel::A] * rhs.m_color[channel::A], 0.f, 1.f));
}

color color::operator * (float fScalar) const
{
	return color(
		::Clamp(fScalar * m_color[channel::R], 0.f, 1.f),
		::Clamp(fScalar * m_color[channel::G], 0.f, 1.f),
		::Clamp(fScalar * m_color[channel::B], 0.f, 1.f),
		::Clamp(fScalar * m_color[channel::A], 0.f, 1.f));
}

color color::operator/ (float fScalar) const
{
	if ( fScalar == 0.f )
		return *this;

	return color(
		m_color[channel::R] / fScalar,
		m_color[channel::G] / fScalar,
		m_color[channel::B] / fScalar,
		m_color[channel::A] / fScalar);
}

color operator* (float fScalar, const color& rhs)
{
	return color(
		::Clamp(fScalar * rhs.R(), 0.f, 1.f),
		::Clamp(fScalar * rhs.G(), 0.f, 1.f),
		::Clamp(fScalar * rhs.B(), 0.f, 1.f),
		::Clamp(fScalar * rhs.A(), 0.f, 1.f));
}

color& color::operator+= (const color& rhs)
{
	m_color[channel::R] += rhs.m_color[channel::R];
	m_color[channel::G] += rhs.m_color[channel::G];
	m_color[channel::B] += rhs.m_color[channel::B];
	m_color[channel::A] += rhs.m_color[channel::A];
	Clamp();
	return *this;
}

color& color::operator-= (const color& rhs)
{
	m_color[channel::R] -= rhs.m_color[channel::R];
	m_color[channel::G] -= rhs.m_color[channel::G];
	m_color[channel::B] -= rhs.m_color[channel::B];
	m_color[channel::A] -= rhs.m_color[channel::A];
	Clamp();
	return *this;
}

color& color::operator*= (const color& rhs)
{
	m_color[channel::R] *= rhs.m_color[channel::R];
	m_color[channel::G] *= rhs.m_color[channel::G];
	m_color[channel::B] *= rhs.m_color[channel::B];
	m_color[channel::A] *= rhs.m_color[channel::A];
	Clamp();
	return *this;
}

color& color::operator*= (float fScalar)
{
	m_color[channel::R] *= fScalar;
	m_color[channel::G] *= fScalar;
	m_color[channel::B] *= fScalar;
	m_color[channel::A] *= fScalar;
	Clamp();
	return *this;
}

void color::Clamp ()
{
	for (auto& c : m_color)
	{
		::Clamp(c, 0.f, 1.f);
	}
}

unsigned int color::ToU32RGBA() const
{
	unsigned int dwR = m_color[channel::R] >= 1.0f ? 0xff : m_color[channel::R] <= 0.0f ? 0x00 : (unsigned int) (m_color[channel::R] * 255.0f + 0.5f);
	unsigned int dwG = m_color[channel::G] >= 1.0f ? 0xff : m_color[channel::G] <= 0.0f ? 0x00 : (unsigned int) (m_color[channel::G] * 255.0f + 0.5f);
	unsigned int dwB = m_color[channel::B] >= 1.0f ? 0xff : m_color[channel::B] <= 0.0f ? 0x00 : (unsigned int) (m_color[channel::B] * 255.0f + 0.5f);
	unsigned int dwA = m_color[channel::A] >= 1.0f ? 0xff : m_color[channel::A] <= 0.0f ? 0x00 : (unsigned int) (m_color[channel::A] * 255.0f + 0.5f);

	return (dwR << 24) | (dwG << 16) | (dwB << 8) | dwA;
}

unsigned int color::ToU32() const
{
	unsigned int dwR = m_color[channel::R] >= 1.0f ? 0xff : m_color[channel::R] <= 0.0f ? 0x00 : (unsigned int) (m_color[channel::R] * 255.0f + 0.5f);
	unsigned int dwG = m_color[channel::G] >= 1.0f ? 0xff : m_color[channel::G] <= 0.0f ? 0x00 : (unsigned int) (m_color[channel::G] * 255.0f + 0.5f);
	unsigned int dwB = m_color[channel::B] >= 1.0f ? 0xff : m_color[channel::B] <= 0.0f ? 0x00 : (unsigned int) (m_color[channel::B] * 255.0f + 0.5f);
	unsigned int dwA = m_color[channel::A] >= 1.0f ? 0xff : m_color[channel::A] <= 0.0f ? 0x00 : (unsigned int) (m_color[channel::A] * 255.0f + 0.5f);

	return (dwA << 24) | (dwR << 16) | (dwG << 8) | dwB;
}

unsigned int color::ToU32ABGR() const
{
	unsigned int dwR = m_color[channel::R] >= 1.0f ? 0xff : m_color[channel::R] <= 0.0f ? 0x00 : (unsigned int) (m_color[channel::R] * 255.0f + 0.5f);
	unsigned int dwG = m_color[channel::G] >= 1.0f ? 0xff : m_color[channel::G] <= 0.0f ? 0x00 : (unsigned int) (m_color[channel::G] * 255.0f + 0.5f);
	unsigned int dwB = m_color[channel::B] >= 1.0f ? 0xff : m_color[channel::B] <= 0.0f ? 0x00 : (unsigned int) (m_color[channel::B] * 255.0f + 0.5f);
	unsigned int dwA = m_color[channel::A] >= 1.0f ? 0xff : m_color[channel::A] <= 0.0f ? 0x00 : (unsigned int) (m_color[channel::A] * 255.0f + 0.5f);

	return (dwA << 24) | (dwB << 16) | (dwG << 8) | dwR;
}

color::operator DirectX::XMVECTOR() const
{
	DirectX::XMVECTOR vector;
	vector = DirectX::XMVectorSet(m_color[channel::R], m_color[channel::G], m_color[channel::B], m_color[channel::A]);
	return vector;
}