#pragma once

#include "math.h"
#include "vector.h"

namespace math
{

class vector4;
class matrix;

class quaternion
{
public:

	vector4 q;
	
public:

	quaternion() {}
	explicit quaternion(const float *);
	quaternion(float x, float y, float z, float w);
	quaternion(const vector3 &axis, float angle);

	quaternion(const quaternion&);

	~quaternion() {}

	float& x() { return q.x(); }
	float& y() { return q.y(); }
	float& z() { return q.z(); }
	float& w() { return q.w(); }

	const float& x() const { return q.x(); }
	const float& y() const { return q.y(); }
	const float& z() const { return q.z(); }
	const float& w() const { return q.w(); }

	bool operator == (const quaternion& ) const;
	bool operator != (const quaternion& ) const;

	quaternion &operator = (const quaternion &a);

	quaternion operator + (const quaternion &a) const;
	quaternion operator - (const quaternion &a) const;
	quaternion operator * (const quaternion &a) const;
	quaternion operator * (const float f)	  const;
	quaternion operator / (const float f)	  const;

	quaternion& operator += (const vector3&	v);
	quaternion& operator -= (const vector3& v);		

	quaternion& operator += (const quaternion &a);
	quaternion& operator -= (const quaternion &a);
	quaternion& operator *= (const quaternion &a);
	quaternion& operator *= (const float f);

	// Unary operators
	quaternion operator + () const;
	quaternion operator - () const;

	quaternion	Lerp(const quaternion &a, const quaternion &b, float delta);
	quaternion	Slerp(const quaternion &q, quaternion &b, float delta);	

	void		Rotate(const vector3 &axis, float angle);

	vector3		RotationAxis() const;
	float		RotationAngle() const;

	static quaternion CreateFromRotationMatrix(const matrix& m);
	static quaternion CreateFromAxisAngle(const vector3& axis, float angle);

	// Rotation from v1 to v2
	quaternion	RotationArc(vector3& s, vector3& t);
	matrix	RotationToMatrix(vector3& s, vector3& t);

	quaternion	Inverse() const;

	quaternion	Conjugate() const;

	float			Length() const;
	float			LengthSq() const;
	float			Dot(const quaternion& a) const;

	void		Normalize();

	void		SetIdentity();
	bool		IsIdentity() const;

	operator vector3();
	operator matrix();

	// TODO: these need to be verified
	void AxisToquaternion(const vector3 &v, const float angle);
	void EulerToquaternion(const vector3 &pitchyawroll);
	void GetAxisAngle(vector3 &axis, float &angle) const;
	void GetEulerAngles(float &pitch, float &yaw, float &roll) const;

	static quaternion Identity;

};
}
