#pragma once

#include "vector.h"

namespace math
{

class vector3;
class vector4;
class quaternion;

class matrix2
{
public:

	matrix2() {}
	matrix2(float _11, float _12,
			float _21, float _22)			
	{
		m[0][0] = _11; m[0][1] = _12;
		m[1][0] = _21; m[1][1] = _22;
	}

	matrix2(const matrix2& rhs)
	{
		m[0][0] = rhs(0, 0); m[0][1] = rhs(1, 0);
		m[1][0] = rhs(1, 0); m[1][1] = rhs(1, 1);
	}

	float &operator()(int i, int j)
	{
		assert(i >= 0 && i < 2);
		assert(j >= 0 && j < 2);
		return m[i][j];
	}

	const float operator()(int i, int j) const
	{
		assert(i >= 0 && i < 2);
		assert(j >= 0 && j < 2);
		return m[i][j];
	}

	static matrix2 CreateRotation(float angleRadians)
	{
		float c = cosf(angleRadians);
		float s = sinf(angleRadians);
		
		matrix2 r;
		r(0, 0) = c; r(0, 1) = -s;
		r(1, 0) = c; r(1, 1) = s;
		return r;
	}

	vector2 Transform(const vector2& v)
	{
		vector2 r;
		r.x() = v.x() * m[0][0] + v.y() * m[0][1];
		r.y() = v.y() * m[1][0] + v.x() * m[1][1];
		return r;
	}

	static vector2 Multiply(const vector2& v, const matrix2& mat)
	{
		vector2 r;
		r.x() = v.x() * mat(0, 0) + v.y() * mat(0, 1);
		r.y() = v.y() * mat(1, 0) + v.x() * mat(1, 1);
		return r;
	}

	static vector2 Rotate(const vector2& v, float angleRadians)
	{
		const float c = cosf(angleRadians);
		const float s = sinf(angleRadians);
		vector2 r;
		r.x() = (v.x() * c) + (v.y() * -s);
		r.y() = (v.x() * s) + (v.y() * c);
		return r;
	}

	vector2 Rotate(const vector2& v)
	{
		vector2 r;
		r.x() = (v.x() * m[0][0]) + (v.y() * m[0][1]);
		r.y() = (v.y() * m[1][0]) + (v.x() * m[1][1]);
		return r;
	}

protected:

	float m[2][2];
};

class matrix
{
public:

	matrix() {}

	matrix( float _11, float _12, float _13, float _14,
			float _21, float _22, float _23, float _24, 
			float _31, float _32, float _33, float _34,
			float _41, float _42, float _43, float _44)
	{
		m[0][0] = _11; m[0][1] = _12; m[0][2] = _13; m[0][3] = _14;
		m[1][0] = _21; m[1][1] = _22; m[1][2] = _23; m[1][3] = _24;
		m[2][0] = _31; m[2][1] = _32; m[2][2] = _33; m[2][3] = _34;
		m[3][0] = _41; m[3][1] = _42; m[3][2] = _43; m[3][3] = _44;
	}

	matrix(const matrix& rhs)
	{
		m[0][0] = rhs(0,0); m[1][0] = rhs(1,0); m[2][0] = rhs(2,0); m[3][0] = rhs(3,0);
		m[0][1] = rhs(0,1); m[1][1] = rhs(1,1); m[2][1] = rhs(2,1); m[3][1] = rhs(3,1);
		m[0][2] = rhs(0,2); m[1][2] = rhs(1,2); m[2][2] = rhs(2,2); m[3][2] = rhs(3,2);
		m[0][3] = rhs(0,3); m[1][3] = rhs(1,3); m[2][3] = rhs(2,3); m[3][3] = rhs(3,3);		
	}

	matrix(matrix&& rhs)
	{
		if (this != &rhs)
		{
			memmove(m, rhs.m, sizeof(float) * 4 * 4);			
		}
	}

	~matrix()
	{
	}

	explicit matrix(const DirectX::XMMATRIX& dm)
	{
		float* p = &m[0][0];
		for (int i = 0; i < 4; ++i )
		{
			for ( int j = 0; j < 4; ++j )
			{
				DirectX::XMVectorGetByIndexPtr(p++, dm.r[i], j);
			}
		}
	}

	static matrix Identity;
	static matrix Zero;

	void Translate(const vector3& translation);
	void Scale(float scale);

	vector3 Right()		{ return vector3(m[0][0], m[1][0], m[2][0]); }
	vector3 Up()		{ return vector3(m[0][1], m[1][1], m[2][1]); }
	vector3 Forward()	{ return vector3(m[0][2], m[1][2], m[2][2]); }

	const vector3 Right()	const { return vector3(m[0][0], m[1][0], m[2][0]); }
	const vector3 Up()		const { return vector3(m[0][1], m[1][1], m[2][1]); }
	const vector3 Forward() const { return vector3(m[0][2], m[1][2], m[2][2]); }

	vector3 Left()		{ return -Right(); }
	vector3 Down()		{ return -Up(); }
	vector3 Backward()	{ return -Forward(); }

	const vector3 Left()		const { return -Right(); }
	const vector3 Down()		const { return -Up(); }
	const vector3 Backward()	const { return -Forward(); }

	vector3 Translation() { return vector3(m[3][0], m[3][1], m[3][2]); }
	const vector3 Translation() const { return vector3(m[3][0], m[3][1], m[3][2]); }

	static matrix CreateRotationX(float radians);
	static void CreateRotationX(float radians, matrix& rotationmatrix);
	
	static matrix CreateRotationY(float radians);
	static void CreateRotationY(float radians, matrix& rotationmatrix);
	
	static matrix CreateRotationZ(float radians);
	static void CreateRotationZ(float radians, matrix& rotationmatrix);

	static matrix CreateUniformScale(float scale);
	static matrix CreateNonUniformScale(float scaleX, float scaleY, float scaleZ);

	static matrix CreateTranslation(float x, float y, float z);
	static matrix CreateTranslation(const math::vector3& translation);

	static matrix CreateViewAligned(const matrix& view, const vector3& objectPosition);
	static matrix CreateFromQuaternion(const quaternion& q);
		
	static matrix Invert(const matrix& m);
	static void Invert(const matrix& m, matrix& result);

	static float Cofactor(const matrix& m, int i, int j);
	static float Determinant(const matrix& m);
	static void ScaleAdjoint(float scale, const matrix& m, matrix& result);
	static void Multiply(const matrix& a, const matrix& b, matrix& result);

	static matrix Create(const vector3& right, const vector3& up, const vector3& forward, const vector3& translation);

	operator DirectX::XMMATRIX() const;

	matrix operator * (const matrix &rhs) const;
	matrix operator * (float scalar);

	bool operator == (const matrix& rhs) const;
	bool operator != (const matrix& rhs) const;

	float &matrix::operator()(int i, int j)
	{
		assert(i >= 0 && i < 4);
		assert(j >= 0 && j < 4);
		return m[i][j];
	}

	const float matrix::operator()(int i, int j) const
	{
		assert(i >= 0 && i < 4);
		assert(j >= 0 && j < 4);
		return m[i][j];
	}
	
private:

	float m[4][4];
};

class lefthanded
{
public:
	static matrix Perspective(float width, float height, float z_near, float z_far);
	static void Perspective(float width, float height, float z_near, float z_far, matrix& result);
	
	static matrix PerspectiveFOV(float fov_y, float aspectRatio, float z_near, float z_far);
	static void PerspectiveFOV(float fov_y, float aspectRatio, float z_near, float z_far, matrix& result);
	
	static matrix LookAt(const vector3& eyePosition, const vector3& targetPosition, const vector3& up);
	static void LookAt(const vector3& eyePosition, const vector3& targetPosition, const vector3& up, matrix& result);

};

class righthanded
{
public:
	static matrix Perspective(float width, float height, float z_near, float z_far);
	static void Perspective(float width, float height, float z_near, float z_far, matrix& result);
	
	static matrix PerspectiveFOV(float fov_y, float aspectRatio, float z_near, float z_far);
	static void PerspectiveFOV(float fov_y, float aspectRatio, float z_near, float z_far, matrix& result);
	
	static matrix LookAt(const vector3& eyePosition, const vector3& targetPosition, const vector3& up);
	static void LookAt(const vector3& eyePosition, const vector3& targetPosition, const vector3& up, matrix& result);

};

} // math