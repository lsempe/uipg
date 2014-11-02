#pragma once

#include "math.h"
#include "DirectXMath.h"

namespace math
{

class vector2;
class vector3;
class quaternion;
class matrix;

class vector4
{
public:
	float v[4];

public:

	vector4(int _x, int _y, int _z, int _w) { v[0] = (float)_x; v[1] = (float)_y; v[2] = (float)_z; v[3] = (float)_w; }
	vector4(unsigned int _x, unsigned int _y, unsigned int _z, unsigned int _w) { v[0] = (float)_x; v[1] = (float)_y; v[2] = (float)_z; v[3] = (float)_w; }
	vector4(float _x, float _y, float _z, float _w) { v[0] = _x; v[1] = _y; v[2] = _z; v[3] = _w; }
	explicit vector4(const DirectX::XMVECTOR& v);

	vector4();
	~vector4()	{}

	float& x() { return v[0]; }
	float& y() { return v[1]; }
	float& z() { return v[2]; }
	float& w() { return v[3]; }

	const float& x() const { return v[0]; }
	const float& y() const { return v[1]; }
	const float& z() const { return v[2]; }
	const float& w() const { return v[3]; }

	vector4 operator - ();
	float operator [](int i) const;
	operator vector3();
	operator quaternion();
	float *AsArray() { return (float*)v; }
	operator DirectX::XMVECTOR() const;

	const vector4 &operator = (const vector4 &v2);
	const vector4 &operator = (const DirectX::XMVECTOR &v2);

	// Equality operations
	bool operator == (const vector4 &v) const;
	bool operator != (const vector4 &v) const;

	// Vector - Vector arithmetic operations
	const vector4 operator + (const vector4 &v2) const;
	const vector4 operator - (const vector4 &v2) const;
	const vector4 operator * (const vector4 &v2) const;
	const vector4 operator / (const vector4 &v2) const;

	// Arithmetic assignement
	vector4 &operator += (const vector4 &v2);
	vector4 &operator -= (const vector4 &v2);
	vector4 &operator *= (const vector4 &v2);
	vector4 &operator /= (const vector4 &v2);

	// Vector - Scalar operations
	vector4 operator + (float s) const;
	vector4 operator - (float s) const;
	vector4 operator * (float s) const;
	vector4 operator / (float s) const;

	// Arithmetic Assignement
	vector4 &operator += (const float s);
	vector4 &operator -= (const float s);
	vector4 &operator *= (const float s);
	vector4 &operator /= (const float s);

	vector4 operator * (const matrix& m) const;

	// Returns the dot product of two vectors
	float Dot(const vector4 &v2) const;
	static float Dot(const vector4& v1, const vector4& v2);

	// Returns the cross product of two vectors
	vector4 Cross(const vector4 &v2) const;

	// Homogenize the vector
	vector4 &Homogenize();

	// Returns the length of the vector
	float Length() const;

	// Returns the length of the vector
	float LengthSquared() const;

	// Normalize the vector
	vector4 Normalize();

	// Returns the distance between vectors
	float Distance(const vector4 &v) const;

	// Scale the vector uniformally by 1
	void Scale(float s);	

	static vector4 Transform(const vector2& v, const matrix& m);
	static vector4 Transform(const vector3& v, const matrix& m);
	static vector4 Transform(const vector4& v, const matrix& m);

	static vector4 One;
	static vector4 Zero;
	static vector4 UnitX;
	static vector4 UnitY;
	static vector4 UnitZ;
	static vector4 UnitW;

};

class vector3
{
public:
	
	float v[3];	

public:

	vector3(int _x, int _y, int _z) { v[0] = (float)_x; v[1] = (float)_y; v[2] = (float)_z; }
	vector3(unsigned int _x, unsigned int _y, unsigned int _z) { v[0] = (float)_x; v[1] = (float)_y; v[2] = (float)_z; }
	vector3(float _x, float _y, float _z) { v[0] = _x; v[1] = _y; v[2] = _z; }
	explicit vector3(const DirectX::XMVECTOR& v);
	vector3(vector3&& r)
	{
		*this = r;
	}

	vector3();
	~vector3();

	float& x() { return v[0]; }
	float& y() { return v[1]; }
	float& z() { return v[2]; }

	const float& x() const { return v[0]; }
	const float& y() const { return v[1]; }
	const float& z() const { return v[2]; }

	vector3 operator - ();
	float operator [](int i) const;
	operator vector4();
	operator vector2();
	float *AsArray() { return (float*)v; }
	operator DirectX::XMVECTOR() const;

	const vector3 &operator = (const vector3 &v2);
	const vector3 &operator = (const DirectX::XMVECTOR &v2);

	// Equality operations
	bool operator == (const vector3 &v) const;
	bool operator != (const vector3 &v) const;

	// Vector - Vector arithmetic operations
	const vector3 operator + (const vector3 &v2) const;
	const vector3 operator - (const vector3 &v2) const;
	const vector3 operator * (const vector3 &v2) const;
	const vector3 operator / (const vector3 &v2) const;

	// Arithmetic assignement
	vector3 &operator += (const vector3 &v2);
	vector3 &operator -= (const vector3 &v2);
	vector3 &operator *= (const vector3 &v2);
	vector3 &operator /= (const vector3 &v2);

	// Vector - Scalar operations
	vector3 operator + (const float s) const;
	vector3 operator - (const float s) const;
	vector3 operator * (const float s) const;
	vector3 operator / (const float s) const;

	// Arithmetic Assignement
	vector3 &operator += (const float s);
	vector3 &operator -= (const float s);
	vector3 &operator *= (const float s);
	vector3 &operator /= (const float s);

	vector3 operator - () const
	{
		return vector3(-v[0], -v[1], -v[2]);	
	}

	vector3 operator * (const matrix& m) const;

	// Returns the dot product of two vectors
	float Dot(const vector3 &v2) const;
	static float Dot(const vector3& v1, const vector3& v2);

	// Returns the cross product of two vectors
	vector3 Cross(const vector3 &v2) const;
	static vector3 Cross(const vector3& v1, const vector3& v2);

	// Returns the length of the vector
	float Length() const;

	// Returns the square length of the vector
	float LengthSquared() const;

	// Normalize the vector
	vector3 Normalize();
	static vector3 Normalize(const vector3& source);

	// Returns the distance between vectors
	float Distance(const vector3 &v) const;

	// Scale the vector uniformally by 1
	void Scale(float s);

	static vector3 Transform(const vector3& v, const math::matrix& m);

	static float AngleBetweenVectors(const vector3& v0, const vector3& v1);

	static vector3 One;
	static vector3 Zero;
	static vector3 UnitX;
	static vector3 UnitY;
	static vector3 UnitZ;
};

class vector2
{
public:

	float v[2];

public:

	vector2(int _x, int _y) { v[0] = (float)_x; v[1] = (float)_y; }
	vector2(unsigned int _x, unsigned int _y) { v[0] = (float)_x; v[1] = (float)_y; }
	vector2(float _x, float _y) { v[0] = _x; v[1] = _y; }
	explicit vector2(const DirectX::XMVECTOR& v);

	vector2();
	~vector2()	{}

	vector2 operator - ();
	float operator [](int i) const;
	operator vector3();
	operator DirectX::XMVECTOR() const;

	float& x() { return v[0]; }
	float& y() { return v[1]; }

	const float& x() const { return v[0]; }
	const float& y() const { return v[1]; }

	const vector2 &operator = (const vector2 &v2);
	vector2 &operator = (const DirectX::XMVECTOR &v2);
	
	// Equality operations
	bool operator == (const vector2 &v) const;
	bool operator != (const vector2 &v) const;

	// Vector - Vector arithmetic operations
	const vector2 operator + (const vector2 &v2) const;
	const vector2 operator - (const vector2 &v2) const;
	const vector2 operator * (const vector2 &v2) const;
	const vector2 operator / (const vector2 &v2) const;

	// Arithmetic assignement
	vector2 &operator += (const vector2 &v2);
	vector2 &operator -= (const vector2 &v2);
	vector2 &operator *= (const vector2 &v2);
	vector2 &operator /= (const vector2 &v2);

	// Vector - Scalar operations
	vector2 operator + (const float s) const;
	vector2 operator - (const float s) const;
	vector2 operator * (const float s) const;
	vector2 operator / (const float s) const;

	// Arithmetic Assignement
	vector2 &operator += (const float s);
	vector2 &operator -= (const float s);
	vector2 &operator *= (const float s);
	vector2 &operator /= (const float s);

	vector2 operator * (const matrix& m) const;

	// Returns the dot product of two vectors
	float Dot(const vector2 &v2) const;
	static float Dot(const vector2& v1, const vector2& v2);

	// Returns the length of the vector
	float Length() const;

	// Returns the length of the vector
	float LengthSquared() const;

	// Normalize the vector
	vector2 Normalize();

	// Returns the distance between vectors
	float Distance(const vector2 &v) const;

	// Scale the vector uniformally by 1
	void Scale(float s);

	static vector2 Transform(const vector2& v, const math::matrix& m);

	static vector2 One;
	static vector2 Zero;
	static vector2 UnitX;
	static vector2 UnitY;

};

} // math