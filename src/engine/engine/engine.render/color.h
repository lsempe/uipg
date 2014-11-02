#pragma once

#include "assert.h"
#include "engine.math/vector.h"
#include "DirectXMath.h"

#ifdef TRANSPARENT
#undef TRANSPARENT
#endif

namespace render
{

	namespace channel
	{
		enum { R, G, B, A };
	}

	class color
	{

	public:
	
		color(float fR, float fG, float fB, float fA);
		color(const color& rhs);
		explicit color(float* fColor);
		explicit color(unsigned int clr);
		color(unsigned char red = 0xff, unsigned char green = 0xff, unsigned char blue = 0xff, unsigned char alpha = 0xff);
	
		operator const unsigned int() const;
		operator unsigned int();
		operator const float*() const;
		operator float* ();
		float operator[] (int i) const;
		float& operator[] (int i);

		operator math::vector4() const { return math::vector4(m_color[0],m_color[1],m_color[2],m_color[3]); }

		operator DirectX::XMVECTOR() const;

		float R() const;
		float& R();
		float G() const;
		float& G();
		float B() const;
		float& B();
		float A() const;
		float& A();

		// assignement
		color& operator = (const color& rhs);	

		// comparison
		bool color::operator == (const color& rhs) const;
		bool color::operator != (const color& rhs) const;

		// arithmetic operations
		color operator+ (const color& rhs) const;
		color operator- (const color& rhs) const;
		color operator* (const color& rhs) const;
		color operator* (float fScalar) const;
		color operator/ (float fScalar) const;

		friend color operator* (float fScalar, const color& rhs);

		// arithmetic updates
		color& operator+= (const color& rhs);
		color& operator-= (const color& rhs);
		color& operator*= (const color& rhs);
		color& operator*= (float fScalar);

		void Clamp ();

		static const color BLACK;		// = (0,0,0) 
		static const color WHITE;		// = (1,1,1)
		static const color RED;			// = (1,0,0)
		static const color GREEN;		// = (0,1,0)
		static const color BLUE;		// = (0,0,1)
	
		static const color ALICEBLUE;
		static const color ANTIQUEWHITE;
		static const color AQUA;
		static const color AQUAMARINE;
		static const color AZURE;
		static const color BEIGE;
		static const color BISQUE;
		static const color BLANCHEDALMOND;
		static const color BLUEVIOLET;
		static const color BROWN;
		static const color BURLYWOOD;
		static const color CADETBLUE;
		static const color CHARTREUSE;
		static const color CHOCOLATE;
		static const color CORAL;
		static const color CORNFLOWERBLUE;
		static const color CORNSILK;
		static const color CRIMSON;
		static const color CYAN;
		static const color DARKBLUE;
		static const color DARKCYAN;
		static const color DARKGOLDENROD;
		static const color DARKGRAY;
		static const color DARKGREY;
		static const color DARKGREEN;
		static const color DARKKHAKI;
		static const color DARKMAGENTA;
		static const color DARKOLIVEGREEN;
		static const color DARKORANGE;
		static const color DARKORCHID;
		static const color DARKRED;
		static const color DARKSALMON;
		static const color DARKSEAGREEN;
		static const color DARKSLATEBLUE;
		static const color DARKSLATEGRAY;
		static const color DARKSLATEGREY;
		static const color DARKTURQUOISE;
		static const color DARKVIOLET;
		static const color DEEPPINK;
		static const color DEEPSKYBLUE;
		static const color DIMGRAY;
		static const color DIMGREY;
		static const color DODGERBLUE;
		static const color FIREBRICK;
		static const color FLORALWHITE;
		static const color FORESTGREEN;
		static const color FUCSHIA;
		static const color GAINSBORO;
		static const color GHOSTWHITE;
		static const color GOLD;
		static const color GOLDENROD;
		static const color GRAY;
		static const color GREY;
		static const color GREENYELLOW;
		static const color HONEYDEW;
		static const color HOTPINK;
		static const color INDIANRED;
		static const color INDIGO;
		static const color IVORY;
		static const color KHAKI;
		static const color LAVENDER;
		static const color LAWNGREEN;
		static const color LEMONCHIFFON;
		static const color LIGHTBLUE;
		static const color LIGHTCYAN;
		static const color LIGHTGOLDENRODYELLOW;
		static const color LIGHTGRAY;
		static const color LIGHTGREY;
		static const color LIGHTGREEN;
		static const color LIGHTPINK;
		static const color LIGHTSALMON;
		static const color LIGHTSEAGREEN;
		static const color LIGHTSKYBLUE;
		static const color LIGHTSLATEGRAY;
		static const color LIGHTSLATEGREY;
		static const color LIGHTSTEELBLUE;
		static const color LIGHTYELLOW;
		static const color LIME;
		static const color LIMEGREEN;
		static const color LINEN;
		static const color MAGENTA;
		static const color MARRON;
		static const color MEDIUMAQUAMARINE;
		static const color MEDIUMBLUE;
		static const color MEDIUMORCHID;
		static const color MEDIUMPURPLE;
		static const color MEDIUMSEAGREEN;
		static const color MEDIUMSLATEBLUE;
		static const color MEDIUMSPRINGGREEN;
		static const color MEDIUMTURQUOISE;
		static const color MEDIUMVIOLETRED;
		static const color MIDNIGHTBLUE;
		static const color MINTCREAM;
		static const color MISTYROSE;
		static const color MOCCASIN;
		static const color NAVAJOWHITE;
		static const color NAVY;
		static const color OLDLACE;
		static const color OLIVE;
		static const color OLIVEDRAB;
		static const color ORANGE;
		static const color ORANGERED;
		static const color ORCHID;
		static const color PALEGOLDENROD;
		static const color PALEGREEN;
		static const color PALETURQUOISE;
		static const color PALEVIOLETRED;
		static const color PAPAYAWHIP;
		static const color PEACHPUFF;
		static const color PERU;
		static const color PINK;
		static const color PLUM;
		static const color POWDERBLUE;
		static const color PURPLE;
		static const color ROSYBROWN;
		static const color ROYALBLUE;
		static const color SADDLEBROWN;
		static const color SALMON;
		static const color SANDYBROWN;
		static const color SEAGREEN;
		static const color SEASHELL;
		static const color SIENNA;
		static const color SILVER;
		static const color SKYBLUE;
		static const color SLATEBLUE;
		static const color SLATEGRAY;
		static const color SLATEGREY;
		static const color SNOW;
		static const color SPRINGGREEN;
		static const color STEELBLUE;
		static const color TAN;
		static const color TEAL;
		static const color THISTLE;
		static const color TOMATO;
		static const color TRANSPARENT;
		static const color TURQUOISE;
		static const color VIOLET;
		static const color WHEAT;
		static const color WHITESMOKE;
		static const color YELLOWGREEN;
		static const color YELLOW;	
		static const color INVALID;	// = (-1,-1,-1)

		unsigned int ToU32RGBA() const;
		unsigned int ToU32() const;
		unsigned int ToU32ABGR() const;

	protected:

		float m_color[4];

	};

	#include "color.inl"

}