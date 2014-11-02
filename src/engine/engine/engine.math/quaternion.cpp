#include <math.h>

#include "engine.core/sys.h"
#include "assert.h"
#include "math.h"
#include "quaternion.h"
#include "matrix.h"

using namespace math;

quaternion quaternion::Identity(0.0f, 0.0f, 0.0f, 1.0f);

quaternion::quaternion(const float *f)
{
	if (f != nullptr)
	{
		q.x() = f[0]; q.y() = f[1]; q.z() = f[2]; q.w() = f[3];
	}
}

quaternion::quaternion(float x, float y, float z, float w)
	: q(x, y, z, w)
{
}

_inline quaternion::quaternion(const quaternion &a)
{
	q = a.q;
}

quaternion::quaternion(const vector3 &axis, float angle)
{		
	q.x() = axis.x(); q.y() = axis.y(); q.z() = axis.z();
	q.w() = angle;
}

_inline quaternion quaternion::operator + () const
{
	return *this;
}

_inline quaternion quaternion:: operator - () const
{
	return quaternion(-x(), -y(), -z(), -w());
}

quaternion &quaternion::operator = (const quaternion &a)
{
	q = a.q;
	return *this;
}

quaternion quaternion::operator + (const quaternion &a) const
{
	return quaternion(x() + a.x(), y() + a.y(), z() + a.z(), w() + a.w());
}

quaternion quaternion::operator - (const quaternion &a) const
{
	return quaternion(x() - a.x(), y() - a.y(), z() - a.z(), w() - a.w());
}

quaternion quaternion::operator * (const quaternion &a) const
{
	quaternion out;

	float A, B, C, D, E, F, G, H;

	A = (w() + x())*(a.w() + a.x());
	B = (z() - y())*(a.y() - a.z());
	C = (w() - x())*(a.y() + a.z()); 
	D = (y() + z())*(a.w() - a.x());
	E = (x() + z())*(a.x() + a.y());
	F = (x() - z())*(a.x() - a.y());
	G = (w() + y())*(a.w() - a.z());
	H = (w() - y())*(a.w() + a.z());

	out.w() = B + (-E - F + G + H) * 0.5f;
	out.x() = A - (E + F + G + H)  * 0.5f; 
	out.y() = C + (E - F + G - H)  * 0.5f; 
	out.z() = D + (E - F - G + H)  * 0.5f;

	return out;
}
	
quaternion& quaternion::operator *= (const quaternion &a)
{
	quaternion q = (*this * a);
	*this = q;
	return *this;
}

quaternion quaternion::operator * (const float f) const
{
	return quaternion(x() * f, y() * f, z() * f, w() * f);
}

quaternion quaternion::operator / (const float f) const
{
	float fInv = 1.0f / f;
	return quaternion(x() * fInv, y() * fInv, z() * fInv, w() * fInv);
}

quaternion& quaternion::operator += (const vector3& v)
{
	quaternion q(v.x(), v.y(), v.z(), 0.f);
	q *= *this;
	x() += q.x() * 0.5f;
	y() += q.y() * 0.5f;
	z() += q.z() * 0.5f;
	w() += q.w() * 0.5f;
	return *this;
}	

void quaternion::SetIdentity()
{
	x() = y() = z() = 0.0f;
	w() = 1.0f;	
}

float quaternion::Length() const
{
	return sqrt(Square(x()) + Square(y()) + Square(z()) + Square(w()));
}

_inline float quaternion::LengthSq() const
{
	return (Square(x()) + Square(y()) + Square(z()) + Square(w()));
}

_inline float quaternion::Dot(const quaternion& a) const
{
	return (x() * a.x() + y() * a.y() + z() * a.z() + w() * a.z());	
}

quaternion quaternion::Conjugate() const
{
	return quaternion(-x(), -y(), -z(), w());
}

_inline bool quaternion::IsIdentity() const
{
	return IsEqual(x(), 0.0f) && IsEqual(y(), 0.0f) && IsEqual(z(),  0.0f) && IsEqual(w(), 1.0f);
}

quaternion::operator vector3()
{
	return vector3(x(), y(), z());
}

/*static */quaternion quaternion::CreateFromRotationMatrix(const matrix& m)
{
	 quaternion result;
    if ((m(0,0) + m(1,1) +m(2,2)) > 0.0F)
    {
        float M1 = (float)sqrt((double)(m(0,0) + m(1,1) +m(2,2) + 1.0F));
        result.w() = M1 * 0.5F;
        M1 = 0.5F / M1;
        result.x() = (m(1,2) - m(2,1)) * M1;
        result.y() = (m(2,0) - m(0,2)) * M1;
        result.z() = (m(0,1) - m(1,0)) * M1;
        return result;
    }
    if ((m(0,0) >= m(1,1)) && (m(0,0) >= m(2,2)))
    {
        float M2 = (float)sqrt((double)(1.0F + m(0,0) - m(1,1) -m(2,2)));
        float M3 = 0.5F / M2;
        result.x() = 0.5F * M2;
        result.y() = (m(0,1) + m(1,0)) * M3;
        result.z() = (m(0,2) + m(2,0)) * M3;
        result.w() = (m(1,2) - m(2,1)) * M3;
        return result;
    }
    if (m(1,1) >m(2,2))
    {
        float M4 = (float)sqrt((double)(1.0F + m(1,1) - m(0,0) -m(2,2)));
        float M5 = 0.5F / M4;
        result.x() = (m(1,0) + m(0,1)) * M5;
        result.y() = 0.5F * M4;
        result.z() = (m(2,1) + m(1,2)) * M5;
        result.w() = (m(2,0) - m(0,2)) * M5;
        return result;
    }
    float M6 = (float)sqrt((double)(1.0F +m(2,2) - m(0,0) - m(1,1)));
    float M7 = 0.5F / M6;
    result.x() = (m(2,0) + m(0,2)) * M7;
    result.y() = (m(2,1) + m(1,2)) * M7;
    result.z() = 0.5F * M6;
    result.w() = (m(0,1) - m(1,0)) * M7;
    return result;
}

// 

quaternion quaternion::Lerp(const quaternion &from, const quaternion &to, float t)
{
	quaternion r =  (to - from) * t + from;	
	return r;
}

quaternion quaternion::Slerp(const quaternion &from, quaternion &to, float t)
{
	quaternion out;
	float omega, cosom, sinom, scale0, scale1;

	cosom = from.Dot(to);

	// adjust signs
	if (cosom < 0.0f) 
	{ 
		cosom = -cosom; 
		to.x() = -to.x();
		to.y() = -to.y();
		to.z() = -to.z();
		to.w() = -to.w();
	}

	if (1.0f - cosom > Epsilon)
	{
		omega = acos(cosom);
		sinom = sin(omega);
		scale0 = sin((1.0f - t) * omega) / sinom;
		scale1 = sin(t * omega) / sinom;
	}
	else
	{
		// from and to quaternions are very close, do a linear interpolation
		scale0 = 1.0f - t;
		scale1 = t;
	}

	out.x() = scale0 * from.x() + scale1 * to.x();
	out.y() = scale0 * from.y() + scale1 * to.y();
	out.z() = scale0 * from.z() + scale1 * to.z();
	out.w() = scale0 * from.w() + scale1 * to.w();

	return out;

}


quaternion quaternion::RotationArc(vector3& s, vector3& t)
{
	quaternion q;

	assert(s != -t);	// although unlikely, this function becomes numerically unstable when v1 approaches -v2

	s.Normalize();
	t.Normalize();

	vector3 c = s.Cross(t);
	float d = s.Dot(t);
	float r = (float)sqrt((1+d)*2);
	float ir = 1.0f / r;
	q.x() = c.x() * ir;
	q.y() = c.y() * ir;
	q.z() = c.z() * ir;
	q.w() = r * 0.5f;

	return q;
}

void quaternion::Normalize()
{
	float length = Length();

	if ( !IsEqual(length, 0.f) )
	{
		x() /= length;
		y() /= length;
		z() /= length;
		w() /= length;

		Clamp(x(), -1.0f, 1.0f);
		Clamp(y(), -1.0f, 1.0f);
		Clamp(z(), -1.0f, 1.0f);
		Clamp(w(), -1.0f, 1.0f);
	}
}

void quaternion::AxisToquaternion(const vector3 &v, const float angle)
{
	float rad, scale;

	float length = v.Length();
	if (IsEqual(0.0f, length))
	{
		SetIdentity();
		return;
	}

	assert(!IsEqual(1.0f, length)); // vector must be normalized

	rad		= angle * 0.5f;
	w()		= cos(rad);
	scale	= sin(rad);

	x() = float(x() * scale);
	y() = float(y() * scale);
	z() = float(z() * scale);

	Normalize();
}

void quaternion::EulerToquaternion(const vector3 &pitchyawroll)
{
	float cr, cp, cy, sr, sp, sy, cpcy, spsy;

	float half_x = pitchyawroll.x() * 0.5f;
	float half_y = pitchyawroll.y() * 0.5f;
	float half_z = pitchyawroll.z() * 0.5f;

	// calculate trig identities		
	cr = cos(half_z); // roll
	cp = cos(half_x); // pitch
	cy = cos(half_y); // yaw

	sr = sin(half_z); // roll
	sp = sin(half_x); // pitch
	sy = sin(half_y); // yaw

	cpcy = cp * cy;
	spsy = sp * sy;

	w() = cr * cpcy + sr * spsy;
	x() = sr * cpcy - cr * spsy;
	y() = cr * sp * cy + sr * cp * sy;
	z() = cr * cp * sy - sr * sp * cy;
}

// Returns the Axis and Angle of this quaternion
void quaternion::GetAxisAngle(vector3 &axis, float &angle) const
{
	// TODO: verify this

	float tmpAngle = acos(w());
	float scale = Length();

	assert(0 <= tmpAngle);			// make sure angle is within 0 - PI range
	assert(Pi >= tmpAngle);

	if (IsEqual(0.0f, scale))
	{
		angle = 0.0f;
		axis = vector3(0.0f, 0.0f, 1.0f);
	}
	else
	{
		angle = float(tmpAngle * 2.0f);
		axis = vector3(float(x() / scale), float(y() / scale), float(z() / scale));
		axis.Normalize();	
	}
}

void quaternion::GetEulerAngles(float &pitch, float &yaw, float &roll) const
{
	UNREFERENCED(pitch);
	UNREFERENCED(yaw);
	UNREFERENCED(roll);
	
	// TODO: implement this
}

void quaternion::Rotate(const vector3 &axis, float angleRadians)
{
	vector3 rot_axis = axis;
	rot_axis.Normalize();

	float fsine = sinf(angleRadians * 0.5f);

	x() = rot_axis.x() * fsine;
	y() = rot_axis.y() * fsine;
	z() = rot_axis.z() * fsine;
	w() = cos( angleRadians * 0.5f );

	Normalize();
}

/*static */quaternion quaternion::CreateFromAxisAngle(const vector3& axis, float angleRadians)
{
	const float sine = sinf(angleRadians * 0.5f);
	return quaternion(axis.x() * sine, axis.y() * sine, axis.z() * sine, cosf(angleRadians * 0.5f) );

}

