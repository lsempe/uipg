#include <math.h>

#include "assert.h"
#include "vector.h"
#include "quaternion.h"
#include "matrix.h"

using namespace math;

// vector4 ///////////////////////////////////////////////////////////////////////////

vector4::vector4()
{
}

vector4::vector4(const DirectX::XMVECTOR &v2)
{
	x() = DirectX::XMVectorGetX(v2);
	y() = DirectX::XMVectorGetY(v2);
	z() = DirectX::XMVectorGetZ(v2);
	w() = DirectX::XMVectorGetW(v2);	
}

vector4::operator quaternion()
{
	quaternion q(x(), y(), z(), w());
	return q;
}

// Allows a cast to a vector3, returns a homogenized vector
vector4::operator vector3()
{	
	vector3 r;
	float rcp = (float)1.0 / w();

	r.x() = x() * rcp;
	r.y() = y() * rcp;
	r.z() = z() * rcp;

	return r;
}

vector4 vector4::operator - ()
{
	vector4 r;
	r.x() = -x();
	r.y() = -y();
	r.z() = -z();
	r.w() = -w();
	return r;
}

float vector4::operator [](int i) const
{
	assert(i < 4);
	return v[i];
}

vector4::operator DirectX::XMVECTOR() const
{
	return DirectX::XMVectorSet(v[0], v[1], v[2], v[3]);
}

// Equality operations
bool vector4::operator == (const vector4 &v) const
{
	return ((x() == v.x()) && (y() == v.y()) && (z() == v.z()) && (w() == v.w()));
}

bool vector4::operator != (const vector4 &v) const
{
	return (!(*this == v));
}

// Returns the dot product of two vectors
float vector4::Dot(const vector4 &v2) const
{
	return ((x() * v2.x()) + (y() * v2.y()) + (z() * v2.z()) + (w() * v2.w()));
}

float vector4::Dot(const vector4& v1, const vector4& v2)
{
	return v1.Dot(v2);
}

// Returns the cross product of two vectors
vector4 vector4::Cross(const vector4 &v2) const
{
	vector4 r;
	r.x() = (y() * v2.z()) - (z() * v2.y());
	r.y() = (z() * v2.x()) - (x() * v2.z());
	r.z() = (x() * v2.y()) - (y() * v2.x());
	r.w() = w();	
	return r;
}

// Homogenize the vector
vector4 &vector4::Homogenize()
{	
	float rcp = 1.f / w();
	x() *= rcp;
	y() *= rcp;
	z() *= rcp;
	w() = 1.f;
	return *this;
}

// Returns the length of the vector
float vector4::Length() const
{
	return (float)sqrt(x()*x() + y()*y() + z()*z() + w()*w());
}

// Returns the length of the vector
float vector4::LengthSquared() const
{
	return (float)(x()*x() + y()*y() + z()*z() + w()*w());
}

// Normalize the vector
vector4 vector4::Normalize()
{
	float len = 1.0f / Length();
	return ((*this) *= len);
}

// Scale the vector uniformally by 1
void vector4::Scale(float s)
{
	float rcp = s / Length();
	x() *= rcp;
	y() *= rcp;
	z() *= rcp;
}

// Returns the distance between vectors
float vector4::Distance(const vector4 &v) const
{
	vector4 t = *this - v;	
	return t.Length();
}


// vector - vector arithmetic operations
const vector4 vector4::operator + (const vector4 &v2) const
{
	vector4 r;
	r.x() = x() + v2.x();
	r.y() = y() + v2.y();
	r.z() = z() + v2.z();
	r.w() = w() + v2.w();
	return r;
}

const vector4 vector4::operator - (const vector4 &v2) const
{
	vector4 r;
	r.x() = x() - v2.x();
	r.y() = y() - v2.y();
	r.z() = z() - v2.z();
	r.w() = w() - v2.w();
	return r;
}

const vector4 vector4::operator * (const vector4 &v2) const
{
	vector4 r;
	r.x() = x() * v2.x();
	r.y() = y() * v2.y();
	r.z() = z() * v2.z();
	r.w() = w() * v2.w();
	return r;
}

const vector4 vector4::operator / (const vector4 &v2) const
{
	vector4 r;
	r.x() = x() / v2.x();
	r.y() = y() / v2.y();
	r.z() = z() / v2.z();
	r.w() = w() / v2.w();
	return r;
}

// Arithmetic assignement
vector4 &vector4::operator += (const vector4 &v2)
{		
	x() += v2.x();
	y() += v2.y();
	z() += v2.z();
	w() += v2.w();	
	return *this;
}

vector4 &vector4::operator -= (const vector4 &v2)
{		
	x() -= v2.x();
	y() -= v2.y();
	z() -= v2.z();
	w() -= v2.w();	
	return *this;
}

vector4 &vector4::operator *= (const vector4 &v2)
{		
	x() *= v2.x();
	y() *= v2.y();
	z() *= v2.z();
	w() *= v2.w();	
	return *this;
}

vector4 &vector4::operator /= (const vector4 &v2)
{
	x() /= v2.x();
	y() /= v2.y();
	z() /= v2.z();
	w() /= v2.w();	
	return *this;
}

// vector - Scalar operations
vector4 vector4::operator + (float s) const
{
	vector4 r;
	r.x() = x() + s;
	r.y() = y() + s;
	r.z() = z() + s;
	r.w() = w() + s;
	return r;
}

vector4 vector4::operator - (float s) const 
{
	vector4 r;
	r.x() = x() - s;
	r.y() = y() - s;
	r.z() = z() - s;
	r.w() = w() - s;
	return r;
}

vector4 vector4::operator * (float s) const
{
	vector4 r;
	r.x() = x() * s;
	r.y() = y() * s;
	r.z() = z() * s;
	r.w() = w() * s;
	return r;
}

vector4 vector4::operator / (float s) const
{
	assert(s != 0.0);
	vector4 r;
	r.x() = x() / s;
	r.y() = y() / s;
	r.z() = z() / s;
	r.w() = w() / s;
	return r;
}

// Arithmetic Assignement
vector4 &vector4::operator += (const float s)
{
	x() += s;
	y() += s;
	z() += s;
	w() += s;
	return *this;
}

vector4 &vector4::operator -= (const float s)
{
	x() -= s;
	y() -= s;
	z() -= s;
	w() -= s;
	return *this;
}

vector4 &vector4::operator *= (const float s)
{
	x() *= s;
	y() *= s;
	z() *= s;
	w() *= s;
	return *this;
}

vector4 &vector4::operator /= (const float s)
{
	assert(s != 0.0);
	x() /= s;
	y() /= s;
	z() /= s;
	w() /= s;	
	return *this;
}

const vector4 &vector4::operator = (const vector4 &v2)
{
	x() = v2.x();
	y() = v2.y();
	z() = v2.z();
	w() = v2.w();
	return *this;
}

const vector4 &vector4::operator = (const DirectX::XMVECTOR &v2)
{
	x() = DirectX::XMVectorGetX(v2);
	y() = DirectX::XMVectorGetY(v2);
	z() = DirectX::XMVectorGetZ(v2);
	w() = DirectX::XMVectorGetW(v2);

	return *this;
}

vector2 vector2::Transform(const vector2& v, const matrix& m)
{
	vector2 result = vector2( 
								(v.x() * m(0,0)) + (v.y() * m(0,1)) + m(3,0),
								(v.x() * m(1,0)) + (v.y() * m(1,1)) + m(3,1));
	return result;
	
}

vector2 vector2::operator * (const matrix& m) const
{
	return Transform(*this, m);
}

vector3 vector3::Transform(const vector3& v, const matrix& m)
{
	vector3 result = vector3(	
						(v.x() * m(0,0)) + (v.x() * m(1,0)) + (v.x() * m(2,0))  + m(3,0),
						(v.y() * m(0,1)) + (v.y() * m(1,1)) + (v.y() * m(2,1))  + m(3,1),
						(v.z() * m(0,2)) + (v.z() * m(1,2)) + (v.z() * m(2,2) ) + m(3,2));
	return result;
	
}

vector3 vector3::operator * (const matrix& m) const
{
	return Transform(*this, m);
}

vector4 vector4::Transform(const vector2& v, const matrix& m)
{
	vector4 result = vector4(	
						(v.x() * m(0,0)) + (v.y() * m(1,0)) + m(3,0),
						(v.x() * m(0,1)) + (v.y() * m(1,1)) + m(3,1),
						(v.x() * m(0,2)) + (v.y() * m(1,2)) + m(3,2),
						(v.x() * m(0,3)) + (v.y() * m(1,3)) + m(3,3));
	return result;
	
}

vector4 vector4::operator * (const matrix& m) const
{
	return Transform(*this, m);
}


vector4 vector4::Transform(const vector3& v, const matrix& m)
{
	vector4 result = vector4(	
						(v.x() * m(0,0)) + (v.y() * m(1,0)) + (v.z() * m(2,0))  + m(3,0),
						(v.x() * m(0,1)) + (v.y() * m(1,1)) + (v.z() * m(2,1))  + m(3,1),
						(v.x() * m(0,2)) + (v.y() * m(1,2)) + (v.z() * m(2,2) ) + m(3,2),
						(v.x() * m(0,3)) + (v.y() * m(1,3)) + (v.z() * m(2,3) ) + m(3,3));
	return result;
	
}

vector4 vector4::Transform(const vector4& v, const matrix& m)
{
	vector4 result = vector4( 
						(v.x() * m(0,0)) + (v.y() * m(1,0)) + (v.z() * m(2,0)) + (v.w() * m(3,0)),
						(v.x() * m(0,1)) + (v.y() * m(1,1)) + (v.z() * m(2,1)) + (v.w() * m(3,1)),
						(v.x() * m(0,2)) + (v.y() * m(1,2)) + (v.z() * m(2,2)) + (v.w() * m(3,2)),
						(v.x() * m(0,3)) + (v.y() * m(1,3)) + (v.z() * m(2,3)) + (v.w() * m(3,3)) );
	return result;
	
}


vector4 vector4::One = vector4(1.f,1.f,1.f,1.f);
vector4 vector4::Zero = vector4(0.f,0.f,0.f,0.f);
vector4 vector4::UnitX = vector4(1.f,0.f,0.f,0.f);
vector4 vector4::UnitY = vector4(0.f,1.f,0.f,0.f);
vector4 vector4::UnitZ = vector4(0.f,0.f,1.f,0.f);
vector4 vector4::UnitW = vector4(0.f,0.f,0.f,1.f);

// vector3 ///////////////////////////////////////////////////////////////////////////

vector3::vector3()
{
}

vector3::vector3(const DirectX::XMVECTOR& v2)
{
	x() = DirectX::XMVectorGetX(v2);
	y() = DirectX::XMVectorGetY(v2);
	z() = DirectX::XMVectorGetZ(v2);
}

vector3::~vector3()
{}

vector3 vector3::operator - ()
{
	vector3 r;
	r.x() = -x();
	r.y() = -y();
	r.z() = -z();
	return r;
}

float vector3::operator [](int i) const
{
	assert(i < 3);
	return v[i];
}

vector3::operator vector2()
{
	vector2 r;
	r.x() = x();
	r.y() = y();
	return r;
}

vector3::operator vector4()
{
	vector4 r;
	r.x() = x();
	r.y() = y();
	r.z() = z();
	r.w() = 1.0f;
	return r;
}

vector3::operator DirectX::XMVECTOR() const
{
	return DirectX::XMVectorSet(v[0], v[1], v[2], 0.f);
}


const vector3 &vector3::operator = (const vector3 &v2)
{
	x() = v2.x();
	y() = v2.y();
	z() = v2.z();
	return *this;
}

const vector3 &vector3::operator = (const DirectX::XMVECTOR &v2)
{
	x() = DirectX::XMVectorGetX(v2);
	y() = DirectX::XMVectorGetY(v2);
	z() = DirectX::XMVectorGetZ(v2);

	return *this;
}


// Equality operations
bool vector3::operator == (const vector3 &v) const
{
	return ((x() == v.x()) && (y() == v.y()) && (z() == v.z()));
}

bool vector3::operator != (const vector3 &v) const
{
	return (!(*this == v));
}

// vector - vector arithmetic operations
const vector3 vector3::operator + (const vector3 &v2) const
{
	vector3 r;
	r.x() = x() + v2.x();
	r.y() = y() + v2.y();
	r.z() = z() + v2.z();
	return r;
}

const vector3 vector3::operator - (const vector3 &v2) const
{
	vector3 r;	
	r.x() = x() - v2.x();
	r.y() = y() - v2.y();
	r.z() = z() - v2.z();
	return r;
}

const vector3 vector3::operator * (const vector3 &v2) const
{
	vector3 r;
	r.x() = x() * v2.x();
	r.y() = y() * v2.y();
	r.z() = z() * v2.z();
	return r;
}

const vector3 vector3::operator / (const vector3 &v2) const
{
	vector3 r;
	r.x() = x() / v2.x();
	r.y() = y() / v2.y();
	r.z() = z() / v2.z();
	return r;
}

// Arithmetic assignement
vector3 &vector3::operator += (const vector3 &v2)
{		
	x() += v2.x();
	y() += v2.y();
	z() += v2.z();
	return *this;
}

vector3 &vector3::operator -= (const vector3 &v2)
{		
	x() -= v2.x();
	y() -= v2.y();
	z() -= v2.z();
	return *this;
}

vector3 &vector3::operator *= (const vector3 &v2)
{		
	x() *= v2.x();
	y() *= v2.y();
	z() *= v2.z();
	return *this;
}

vector3 &vector3::operator /= (const vector3 &v2)
{
	x() /= v2.x();
	y() /= v2.y();
	z() /= v2.z();
	return *this;
}

// vector - Scalar operations
vector3 vector3::operator + (const float s) const
{
	vector3 r;
	r.x() = x() + s;
	r.y() = y() + s;
	r.z() = z() + s;
	return r;
}

vector3 vector3::operator - (const float s) const 
{
	vector3 r;
	r.x() = x() - s;
	r.y() = y() - s;
	r.z() = z() - s;
	return r;
}

vector3 vector3::operator * (const float s) const
{
	vector3 r;
	r.x() = x() * s;
	r.y() = y() * s;
	r.z() = z() * s;
	return r;
}

vector3 vector3::operator / (const float s) const
{
	assert(s != 0.0);
	vector3 r;
	r.x() = x() / s;
	r.y() = y() / s;
	r.z() = z() / s;
	return r;
}

// Arithmetic Assignement
vector3 &vector3::operator += (const float s)
{
	x() += s;
	y() += s;
	z() += s;
	return *this;
}

vector3 &vector3::operator -= (const float s)
{
	x() -= s;
	y() -= s;
	z() -= s;
	return *this;
}

vector3 &vector3::operator *= (const float s)
{
	x() *= s;
	y() *= s;
	z() *= s;
	return *this;
}

vector3 &vector3::operator /= (const float s)
{
	assert(!IsEqual(s, 0.f));
	if ( !IsEqual(s, 0.f) )
	{
		float factor = 1.f / s;
		x() *= factor;
		y() *= factor;
		z() *= factor;
	}
	return *this;
}

// Returns the dot product of two vectors
float vector3::Dot(const vector3 &v2) const
{
	return ((x() * v2.x()) + (y() * v2.y()) + (z() * v2.z()));
}

// Returns the cross product of two vectors
vector3 vector3::Cross(const vector3 &v2) const
{
	vector3 r;
	r.x() = (y() * v2.z()) - (z() * v2.y());
	r.y() = (z() * v2.x()) - (x() * v2.z());
	r.z() = (x() * v2.y()) - (y() * v2.x());
	return r;
}

// Returns the length of the vector
float vector3::Length() const
{
	return (float)sqrt(x()*x() + y()*y() + z()*z());
}

// Returns the square length of the vector
float vector3::LengthSquared() const
{
	return (float)(x()*x() + y()*y() + z()*z());
}

// Normalize the vector
vector3 vector3::Normalize()
{
	if ( IsEqual(Length(), 0.f) )
		return *this;

	float len = 1.0f / Length();
	return ((*this) *= len);
}

vector3 vector3::Normalize(const vector3& source)
{
	float len = 1.f / source.Length();
	return source * len;
}

vector3 vector3::Cross(const vector3& v1, const vector3& v2)
{
	return v1.Cross(v2);
}

float vector3::Dot(const vector3& v1, const vector3& v2)
{
	return v1.Dot(v2);
}

// Returns the distance between vectors
float vector3::Distance(const vector3 &v) const
{
	vector3 t = *this - v;	
	return t.Length();
}

// Scale the vector uniformally by 1
void vector3::Scale(float s)
{
	float rcp = s / Length();
	x() *= rcp;
	y() *= rcp;
	z() *= rcp;
}

float vector3::AngleBetweenVectors(const vector3& v0, const vector3& v1)
{
	float d = v0.Dot(v1);
	if ( d >= -1.f && d <= 1.f )
	{
		return acosf( d / ( v0.Length() * v1.Length() ) );
	}

	return 0.f;
}

vector3 vector3::One = vector3(1.f,1.f,1.f);
vector3 vector3::Zero = vector3(0.f,0.f,0.f);
vector3 vector3::UnitX = vector3(1.f,0.f,0.f);
vector3 vector3::UnitY = vector3(0.f,1.f,0.f);
vector3 vector3::UnitZ = vector3(0.f,0.f,1.f);


// vector2 ///////////////////////////////////////////////////////////////////////////

vector2::vector2()
{
}

vector2::vector2(const DirectX::XMVECTOR &v2)
{
	x() = DirectX::XMVectorGetX(v2);
	y() = DirectX::XMVectorGetY(v2);
}

vector2 vector2::operator - ()
{
	vector2 r;
	r.x() = -x();
	r.y() = -y();
	return r;
}

float vector2::operator [](int i) const
{
	assert(i < 2);
	return v[i];
}

vector2::operator vector3()
{
	vector3 r;
	r.x() = x();
	r.y() = y();
	r.z() = (float)0.0;
	return r;
}

vector2::operator DirectX::XMVECTOR() const
{
	return DirectX::XMVectorSet(v[0], v[1], 0.f, 0.f);
}

const vector2 &vector2::operator = (const vector2 &v2)
{
	x() = v2.x();
	y() = v2.y();
	return *this;
}

vector2 &vector2::operator = (const DirectX::XMVECTOR &v2)
{
	x() = DirectX::XMVectorGetX(v2);
	y() = DirectX::XMVectorGetY(v2);

	return *this;
}

// Returns the dot product of two vectors
float vector2::Dot(const vector2 &v2) const
{
	return ((x() * v2.x()) + (y() * v2.y()));
}

float vector2::Dot(const vector2& v1, const vector2& v2)
{
	return v1.Dot(v2);
}

// Equality operations
bool vector2::operator == (const vector2 &v) const
{
	return ((x() == v.x()) && (y() == v.y()));
}

bool vector2::operator != (const vector2 &v) const
{
	return (!(*this == v));
}

// vector - vector arithmetic operations
const vector2 vector2::operator + (const vector2 &v2) const
{
	vector2 r;
	r.x() = x() + v2.x();
	r.y() = y() + v2.y();
	return r;
}

const vector2 vector2::operator - (const vector2 &v2) const
{
	vector2 r;
	r.x() = x() - v2.x();
	r.y() = y() - v2.y();
	return r;
}

const vector2 vector2::operator * (const vector2 &v2) const
{
	vector2 r;
	r.x() = x() * v2.x();
	r.y() = y() * v2.y();
	return r;
}

const vector2 vector2::operator / (const vector2 &v2) const
{
	vector2 r;
	r.x() = x() / v2.x();
	r.y() = y() / v2.y();
	return r;
}

// Arithmetic assignement
vector2 &vector2::operator += (const vector2 &v2)
{		
	x() += v2.x();
	y() += v2.y();
	return *this;
}

vector2 &vector2::operator -= (const vector2 &v2)
{		
	x() += v2.x();
	y() += v2.y();
	return *this;
}

vector2 &vector2::operator *= (const vector2 &v2)
{		
	x() *= v2.x();
	y() *= v2.y();
	return *this;
}

vector2 &vector2::operator /= (const vector2 &v2)
{
	x() /= v2.x();
	y() /= v2.y();
	return *this;
}

// vector - Scalar operations
vector2 vector2::operator + (const float s) const
{
	vector2 r;
	r.x() = x() + s;
	r.y() = y() + s;
	return r;
}

vector2 vector2::operator - (const float s) const 
{
	vector2 r;
	r.x() = x() - s;
	r.y() = y() - s;
	return r;
}

vector2 vector2::operator * (const float s) const
{
	vector2 r;
	r.x() = x() * s;
	r.y() = y() * s;
	return r;
}

vector2 vector2::operator / (const float s) const
{
	assert(s != 0.0);
	vector2 r;
	r.x() = x() / s;
	r.y() = y() / s;	
	return r;
}

// Arithmetic Assignement
vector2 &vector2::operator += (const float s)
{
	x() += s;
	y() += s;
	return *this;
}

vector2 &vector2::operator -= (const float s)
{
	x() -= s;
	y() -= s;
	return *this;
}

vector2 &vector2::operator *= (const float s)
{
	x() *= s;
	y() *= s;
	return *this;
}

vector2 &vector2::operator /= (const float s)
{
	assert(s != 0.0);
	x() /= s;
	y() /= s;
	return *this;
}

// Returns the length of the vector
float vector2::Length() const
{
	return (float)sqrt(x()*x() + y()*y());
}

// Returns the length of the vector
float vector2::LengthSquared() const
{
	return (float)(x()*x() + y()*y());
}

// Normalize the vector
vector2 vector2::Normalize()
{
	float len = 1.0f / Length();
	return ((*this) *= len);
}

// Returns the distance between vectors
float vector2::Distance(const vector2 &v) const
{
	vector2 t = *this;
	t = t - v;	
	return t.Length();
}

// Scale the vector uniformally by 1
void vector2::Scale(float s)
{
	float rcp = s / Length();
	x() *= rcp;
	y() *= rcp;
}

vector2 vector2::One = vector2(1.f,1.f);
vector2 vector2::Zero = vector2(0.f,0.f);
vector2 vector2::UnitX = vector2(1.f,0.f);
vector2 vector2::UnitY = vector2(0.f,1.f);
