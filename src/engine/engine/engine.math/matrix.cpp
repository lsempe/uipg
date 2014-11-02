
#include "assert.h"
#include "math.h"
#include "vector.h"
#include "matrix.h"
#include "quaternion.h"

using namespace math;


matrix matrix::Identity = matrix(1.0, 0.0, 0.0, 0.0,
								 0.0, 1.0, 0.0, 0.0,
								 0.0, 0.0, 1.0, 0.0,
								 0.0, 0.0, 0.0, 1.0);

matrix matrix::Zero = matrix(0.0, 0.0, 0.0, 0.0,
							 0.0, 0.0, 0.0, 0.0,
							 0.0, 0.0, 0.0, 0.0,
							 0.0, 0.0, 0.0, 0.0);

void matrix::Translate(const vector3& translation)
{
	m[3][0] = translation.x();
	m[3][1] = translation.y();
	m[3][2] = translation.z();
	m[3][3] = 1.f;
}

void matrix::Scale(float scale)
{
	matrix m = Identity;
	m(0,0) = m(1,1) = m(2,2) = scale;
	*this = *this * m;
}

matrix matrix::CreateRotationX(float radians)
{
	const float cosine = cos(radians);
	const float sine = sin(radians);

	return matrix(  1.0,	0.0,	0.0,	0.0, 
					0.0,	cosine,	-sine,	0.0,
					0.0,	sine,	cosine,	0.0,
					0.0,	0.0,	0.0,	1.0); 
	
}

void matrix::CreateRotationX(float radians, matrix& rotationmatrix)
{
	rotationmatrix = CreateRotationX(radians);
}

matrix matrix::CreateRotationY(float radians)
{
	const float cosine = cos(radians);
	const float sine = sin(radians);

	return matrix(  cosine,	0.0,	sine,	0.0, 
					0.0,	1.0,	0.0,	0.0,
					-sine,	0.0,	cosine,	0.0,
					0.0,	0.0,	0.0,	1.0); 
}

void matrix::CreateRotationY(float radians, matrix& rotationmatrix)
{
	rotationmatrix = CreateRotationX(radians);
}


matrix matrix::CreateRotationZ(float radians)
{
	const float cosine = cos(radians);
	const float sine = sin(radians);

	return matrix(  cosine,	-sine,	0.0,	0.0, 
					sine,	cosine,	0.0,	0.0,
					0.0,	0.0,	1.0,	0.0,
					0.0,	0.0,	0.0,	1.0); 
}

void matrix::CreateRotationZ(float radians, matrix& rotationmatrix)
{
	rotationmatrix = CreateRotationX(radians);
}

matrix matrix::CreateUniformScale(float scale)
{
	return matrix(scale, 0.0f, 0.0f, 0.0f,
					0.0f, scale, 0.0f, 0.0f, 
					0.0f, 0.0f, scale, 0.0f, 
					0.0f, 0.0f, 0.0f, 1.f );
}

matrix matrix::CreateNonUniformScale(float scaleX, float scaleY, float scaleZ)
{
	return matrix(scaleX, 0.0f, 0.0f,	0.0f,
					0.0f, scaleY, 0.0f, 0.0f, 
					0.0f, 0.0f, scaleZ, 0.0f, 
					0.0f, 0.0f, 0.0f,	1.f );
}

matrix matrix::CreateTranslation(float x, float y, float z)
{
	return CreateTranslation(vector3(x,y,z));
}

matrix matrix::CreateViewAligned(const matrix& view, const vector3& objectPosition)
{
	math::matrix inverseView = math::matrix::Invert(view);
	math::vector3 v =  objectPosition - inverseView.Translation() ;
	v.Normalize();
	math::vector3 right = v.Cross(view.Up());
	return math::matrix::Create(right, view.Up(), v, objectPosition);
}

matrix matrix::CreateTranslation(const math::vector3& translation)
{
	matrix m = Identity;
	m(3,0) = translation.x();
	m(3,1) = translation.y();
	m(3,2) = translation.z();
	return m;
}

matrix::operator DirectX::XMMATRIX() const
{
	DirectX::XMMATRIX ret(m[0][0], m[0][1], m[0][2], m[0][3],
						  m[1][0], m[1][1], m[1][2], m[1][3],
						  m[2][0], m[2][1], m[2][2], m[2][3],
						  m[3][0], m[3][1], m[3][2], m[3][3] );
	return ret;
}

matrix matrix::operator * (const matrix &rhs) const
{
	matrix result;
	Multiply(*this, rhs, result);
	return result;
}

void matrix::Multiply(const matrix& a, const matrix& b, matrix& result)
{
	for (int i = 0; i < 4; ++i )
	{
		result(i,0) = a(i,0) * b(0,0) + a(i,1) * b(1,0) + a(i,2) * b(2,0) + a(i,3) * b(3,0);
		result(i,1) = a(i,0) * b(0,1) + a(i,1) * b(1,1) + a(i,2) * b(2,1) + a(i,3) * b(3,1);
		result(i,2) = a(i,0) * b(0,2) + a(i,1) * b(1,2) + a(i,2) * b(2,2) + a(i,3) * b(3,2);
		result(i,3) = a(i,0) * b(0,3) + a(i,1) * b(1,3) + a(i,2) * b(2,3) + a(i,3) * b(3,3);
	}
}

matrix matrix::operator * (float scalar)
{
	matrix result;
	float* p = &m[0][0];
	for (int i = 0; i < 4; ++i )
	{
		for (int j = 0; j < 4; ++j )
		{
			result(i,j) = (*p++) * scalar;
		}
	}
	return result;
}

bool matrix::operator == (const matrix& rhs) const
{
	for (int i = 0; i < 4; ++i )
	{
		for (int j = 0; j < 4; ++j )
		{
			if ( !math::IsEqual(m[i][j], rhs.m[i][j], 0.00001f) )
				return false;
		}
	}
	return true;
}

bool matrix::operator != (const matrix& rhs) const
{
	return !(*this == rhs);
}

float matrix::Determinant(const matrix& m)
{
	float cofactor = Cofactor(m, 0, 0);
	float d = m(0,0) * cofactor;
	cofactor = Cofactor(m, 0, 1);
	d += m(0,1) * cofactor;
	cofactor = Cofactor(m, 0, 2);
	d += m(0,2) * cofactor;
	cofactor = Cofactor(m, 0, 3);
	d += m(0,3) * cofactor;

	return d;
}

float matrix::Cofactor(const matrix& m, int i, int j)
{
	int ii[4], jj[4], k;

   for (k=0; k<i; ++k) ii[k] = k;
   for (k=i; k<3; ++k) ii[k] = k+1;
   for (k=0; k<j; ++k) jj[k] = k;
   for (k=j; k<3; ++k) jj[k] = k+1;

   float cofactor = 0.f;
   cofactor =  m(ii[0],jj[0]) * (m(ii[1],jj[1]) * m(ii[2],jj[2]) - m(ii[1],jj[2]) * m(ii[2],jj[1]));
   cofactor -= m(ii[0],jj[1]) * (m(ii[1],jj[0]) * m(ii[2],jj[2]) - m(ii[1],jj[2]) * m(ii[2],jj[0]));
   cofactor += m(ii[0],jj[2]) * (m(ii[1],jj[0]) * m(ii[2],jj[1]) - m(ii[1],jj[1]) * m(ii[2],jj[0]));

   k = i+j;
   if ( k != (k/2)*2 )
   {
      cofactor = -cofactor;
   }

   return cofactor;
}

matrix matrix::Invert(const matrix& m)
{
	matrix result;
	Invert(m, result);
	return result;
}

void matrix::Invert(const matrix& m, matrix& result)
{
	float determinant = Determinant(m);

	float tmp = 1.f / determinant;

	ScaleAdjoint(tmp, m, result);
}

void matrix::ScaleAdjoint(float scale, const matrix& m, matrix& result)
{
	for ( int i = 0; i < 4; ++i )
	{
		for ( int j = 0; j < 4; ++j )
		{
			result(j,i) = Cofactor(m, i, j);
			result(j,i) *= scale;
		}
	}
}

/*static */matrix matrix::Create(const vector3& right, const vector3& up, const vector3& forward, const vector3& translation)
{
	math::matrix t = math::matrix::Identity;
	
	t(0,0) = right.x(); t(1, 0) = up.x(); t(2, 0) = forward.x();
	t(0,1) = right.y(); t(1, 1) = up.y(); t(2, 1) = forward.y();
	t(0,2) = right.z(); t(1, 2) = up.z(); t(2, 2) = forward.z();					

	t.Translate(translation);

	return t;
}

/*static */matrix matrix::CreateFromQuaternion(const quaternion& q)
{
	matrix result = Identity;
	result(0,0) = 1.f - 2.f * (q.y() * q.y() + q.z() * q.z());
	result(1,0) = 2.f * (q.x() * q.y() + q.w() * q.z());
	result(2,0) = 2.f * (q.x() * q.z() - q.w() * q.y());
	
	result(0,1) = 2.f * (q.x() * q.y() - q.w() * q.z());
	result(1,1) = 1.f - 2.f * (q.x() * q.x() + q.z() * q.z());
	result(2,1) = 2.f * (q.y() * q.z() + q.w() * q.x());

	result(0,2) = 2.f * (q.x() * q.z() + q.w() * q.y());
	result(1,2) = 2.f * (q.y() * q.z() - q.w() * q.x());
	result(2,2) = 1.f - 2.f * (q.x() * q.x( ) + q.y() * q.y());

	return result;	 
}


matrix lefthanded::Perspective(float width, float height, float z_near, float z_far)
{
	matrix result;
	//2*zn/w  0       0              0
	//0       2*zn/h  0              0
	//0       0       zf/(zf-zn)     1
	//0       0       zn*zf/(zn-zf)  0
	result = matrix::Zero;
	result(0,0) = 2.f * z_near / width;
	result(1,1) = 2.f * z_near / height;
	result(2,2) = z_far / (z_far - z_near);
	result(3,2) = z_near * z_far / (z_near - z_far);
	result(2,3) = 1.f;
	result(3,3) = 0.f;	
	return result;
}

void lefthanded::Perspective(float width, float height, float z_near, float z_far, matrix& result)
{
	result = Perspective(width, height, z_near, z_far);
}

matrix lefthanded::PerspectiveFOV(float fov_y, float aspectRatio, float z_near, float z_far)
{
	assert(fov_y >= 0.f && fov_y <= math::Pi);
	assert(z_near > 0.f);
	assert(z_far > 0.f);
	assert(z_far >= z_near);

	matrix result = matrix::Zero;
	float yscale = 1.f / tanf(fov_y / 2.f);
	float xscale = yscale / aspectRatio;
	result(0,0) = xscale;
	result(1,1) = yscale;
	result(2,2) = z_far / (z_far - z_near);
	result(2,3) = 1.f;
	result(3,2) = -(z_near * z_far) / (z_far - z_near);
	return result;
}

void lefthanded::PerspectiveFOV(float fov_y, float aspectRatio, float z_near, float z_far, matrix& result)
{
	result = lefthanded::PerspectiveFOV(fov_y, aspectRatio, z_near, z_far);
}

matrix lefthanded::LookAt(const vector3& eyePosition, const vector3& targetPosition, const vector3& up)
{	
	// zaxis = normal(targetPosition - eyePosition)
	// xaxis = normal(cross(up, zaxis))
	// yaxis = cross(zaxis, xaxis)
	//    
	// xaxis.x           yaxis.x           zaxis.x          0
	// xaxis.y           yaxis.y           zaxis.y          0
	// xaxis.z           yaxis.z           zaxis.z          0
	// -dot(xaxis, eye)  -dot(yaxis, eye)  -dot(zaxis, eye)  1
	
	vector3 vz = vector3::Normalize(targetPosition - eyePosition);
    vector3 vx = vector3::Normalize(vector3::Cross(up, vz));
    vector3 vy = vector3::Cross(vz, vx);
	matrix result = matrix::Identity;
    result(0,0) = vx.x();
    result(0,1) = vx.y();
    result(0,2) = vx.z();
    result(1,0) = vy.x();
    result(1,1) = vy.y();
    result(1,2) = vy.z();
    result(2,0) = vz.x();
    result(2,1) = vz.y();
    result(2,2) = vz.z();
    result(3,0) = -vector3::Dot(vx, eyePosition);
    result(3,1) = -vector3::Dot(vy, eyePosition);
    result(3,2) = -vector3::Dot(vz, eyePosition);

	return result;
}

void lefthanded::LookAt(const vector3& eyePosition, const vector3& targetPosition, const vector3& up, matrix& result)
{
	result = lefthanded::LookAt(eyePosition, targetPosition, up);
}

matrix righthanded::Perspective(float width, float height, float z_near, float z_far)
{
	// 2*znearPlane/width  0                    0                                            0
	// 0                   2*znearPlane/height  0                                            0
	// 0                   0                    zfarPlane/(znearPlane-zfarPlane)            -1
	// 0                   0                    znearPlane*zfarPlane/(znearPlane-zfarPlane)  0
	matrix result = matrix::Zero;
	result(0,0) = 2.f * z_near / width;
	result(1,1) = 2.f * z_near / height;
	result(2,2) = z_far / (z_near - z_far);
	result(3,2) = z_near * z_far / (z_near - z_far);
	result(2,3) = -1.f;
	result(3,3) = 0.f;	
	return result;
}

void righthanded::Perspective(float width, float height, float z_near, float z_far, matrix& result)
{
	result = righthanded::Perspective(width, height, z_near, z_far);
}

matrix righthanded::PerspectiveFOV(float fov_y, float aspectRatio, float z_near, float z_far)
{
	// w       0       0                                              0
	// 0       h       0                                              0
	// 0       0       zfarPlane/(znearPlane-zfarPlane)              -1
	// 0       0       znearPlane*zfarPlane/(znearPlane-zfarPlane)    0
	// h = cot(fieldOfViewY/2)
	// w = h * aspectRatio
	float height = tanf(fov_y / 2.f);
	float width = height * aspectRatio;

	matrix result = matrix::Zero;
	result(0,0) = width;
	result(1,1) = height;
	result(2,2) = z_far / (z_near - z_far);
	result(3,2) = z_near * z_far / (z_near - z_far);
	result(2,3) = -1.f;
	result(3,3) = 0.f;	
	return result;
}

void righthanded::PerspectiveFOV(float fov_y, float aspectRatio, float z_near, float z_far, matrix& result)
{
	result = righthanded::PerspectiveFOV(fov_y, aspectRatio, z_near, z_far);
}

matrix righthanded::LookAt(const vector3& eyePosition, const vector3& targetPosition, const vector3& up)
{	
	// zaxis = normal(Eye - At)
	// xaxis = normal(cross(Up, zaxis))
	// yaxis = cross(zaxis, xaxis)
	//    
	// xaxis.x           yaxis.x           zaxis.x          0
	// xaxis.y           yaxis.y           zaxis.y          0
	// xaxis.z           yaxis.z           zaxis.z          0
	// dot(xaxis, eye)   dot(yaxis, eye)   dot(zaxis, eye)  1

	vector3 vz = vector3::Normalize(eyePosition - targetPosition);
    vector3 vx = vector3::Normalize(vector3::Cross(up, vz));
    vector3 vy = vector3::Cross(vz, vx);
	matrix result = matrix::Identity;
    result(0,0) = vx.x();
    result(0,1) = vy.x();
    result(0,2) = vz.x();
    result(1,0) = vx.y();
    result(1,1) = vy.y();
    result(1,2) = vz.y();
    result(2,0) = vx.z();
    result(2,1) = vy.z();
    result(2,2) = vz.z();
    result(3,0) = vector3::Dot(vx, eyePosition);
    result(3,1) = vector3::Dot(vy, eyePosition);
    result(3,2) = vector3::Dot(vz, eyePosition);

	return result;
}

void righthanded::LookAt(const vector3& eyePosition, const vector3& targetPosition, const vector3& up, matrix& result)
{
	result = righthanded::LookAt(eyePosition, targetPosition, up);
}