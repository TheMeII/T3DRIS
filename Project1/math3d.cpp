#include "math3d.h"

Vector3f Vector3f::Cross(const Vector3f& v) const
{
	const float _x = y * v.z - z * v.y;
	const float _y = z * v.x - x * v.z;
	const float _z = x * v.y - y * v.x;

	return Vector3f(_x, _y, _z);
}

Vector3f& Vector3f::Normalize()
{
	const float Lenght = sqrtf(x * x + y * y + z * z);

	x /= Lenght;
	y /= Lenght;
	z /= Lenght;

	return *this;
}

void Vector3f::Rotate(float Angle, const Vector3f& Axe)
{
	const float SinHalfAngle = sinf(ToRadian(Angle/2));
	const float CosHalfAngle = cosf(ToRadian(Angle/2));

	const float Rx = Axe.x * SinHalfAngle;
	const float Ry = Axe.y * SinHalfAngle;
	const float Rz = Axe.z * SinHalfAngle;
	const float Rw = CosHalfAngle;
	Quaternion RotationQ(Rx, Ry, Rz, Rw);

	Quaternion ConjugateQ = RotationQ.Conjugate();
	//ConjugateQ.Normalize();
	Quaternion W = RotationQ * (*this) * ConjugateQ;

	x = W.x;
	y = W.y;
	z = W.z;


}


void Matrix4f::InitScaleTransform(float scaleX, float scaleY, float scaleZ)
{
	m[0][0] = scaleX;	m[0][1] = 0.0f;		m[0][2] = 0.0f;		m[0][3] = 0.0f; //X-axis
	m[1][0] = 0.0f;		m[1][1] = scaleY;	m[1][2] = 0.0f;		m[1][3] = 0.0f; //Y-axis
	m[2][0] = 0.0f;		m[2][1] = 0.0f;		m[2][2] = scaleZ;	m[2][3] = 0.0f; //Z-axis
	m[3][0] = 0.0f;		m[3][1] = 0.0f;		m[3][2] = 0.0f;		m[3][3] = 1.0f; //W-axis
}

void Matrix4f::InitRotateTransform(float RotateX, float RotateY, float RotateZ)
{
	Matrix4f rx, ry, rz;

	//CHANGE TO RADIANS
	const float x =ToRadian(RotateX);
	const float y =ToRadian(RotateY);
	const float z =ToRadian(RotateZ);

	//ROTATION AROUND X-AXIS
	rx.m[0][0] = 1.0f;		rx.m[0][1] = 0.0f;		rx.m[0][2] = 0.0f;		rx.m[0][3] = 0.0f; //X-axis
	rx.m[1][0] = 0.0f;		rx.m[1][1] = cosf(x);	rx.m[1][2] = -sinf(x);	rx.m[1][3] = 0.0f; //Y-axis
	rx.m[2][0] = 0.0f;		rx.m[2][1] = sinf(x);	rx.m[2][2] = cosf(x);	rx.m[2][3] = 0.0f; //Z-axis
	rx.m[3][0] = 0.0f;		rx.m[3][1] = 0.0f;		rx.m[3][2] = 0.0f;		rx.m[3][3] = 1.0f; //W-axis
	//ROTAIOTN AROUND Y-AXIS
	ry.m[0][0] = cosf(y);	ry.m[0][1] = 0.0f;		ry.m[0][2] = -sinf(y);	ry.m[0][3] = 0.0f; //X-axis
	ry.m[1][0] = 0.0f;		ry.m[1][1] = 1.0f;		ry.m[1][2] = 0.0f;		ry.m[1][3] = 0.0f; //Y-axis
	ry.m[2][0] = sinf(y);	ry.m[2][1] = 0.0f;		ry.m[2][2] = cosf(y);	ry.m[2][3] = 0.0f; //Z-axis
	ry.m[3][0] = 0.0f;		ry.m[3][1] = 0.0f;		ry.m[3][2] = 0.0f;		ry.m[3][3] = 1.0f; //W-axis
	//ROTATION AROUND Z-AXIS
	rz.m[0][0] = cosf(z);	rz.m[0][1] = -sinf(z);	rz.m[0][2] = 0.0f;		rz.m[0][3] = 0.0f; //X-axis
	rz.m[1][0] = sinf(z);	rz.m[1][1] = cosf(z);	rz.m[1][2] = 0.0f;		rz.m[1][3] = 0.0f; //Y-axis
	rz.m[2][0] = 0.0f;		rz.m[2][1] = 0.0f;		rz.m[2][2] = 1.0f;		rz.m[2][3] = 0.0f; //Z-axis
	rz.m[3][0] = 0.0f;		rz.m[3][1] = 0.0f;		rz.m[3][2] = 0.0f;		rz.m[3][3] = 1.0f; //W-axis

	//combine rotations
	*this = rz * ry * rx;
}

void Matrix4f::InitTranslationTransform(float x, float y, float z)
{
	m[0][0] = 1.0f;		m[0][1] = 0.0f;		m[0][2] = 0.0f;		m[0][3] = x; //X-axis
	m[1][0] = 0.0f;		m[1][1] = 1.0f;		m[1][2] = 0.0f;		m[1][3] = y; //Y-axis
	m[2][0] = 0.0f;		m[2][1] = 0.0f;		m[2][2] = 1.0f;		m[2][3] = z; //Z-axis
	m[3][0] = 0.0f;		m[3][1] = 0.0f;		m[3][2] = 0.0f;		m[3][3] = 1.0f; //W-axis
}

void Matrix4f::InitCameraTransform(const Vector3f& Target, const Vector3f& Up)
{
	Vector3f N = Target;
	N.Normalize();
	Vector3f U = Up;
	U.Normalize();
	U = U.Cross(N);
	Vector3f V = N.Cross(U);

	m[0][0] = U.x;		m[0][1] = U.y;		m[0][2] = U.z;		m[0][3] = 0.0f; //X-axis
	m[1][0] = V.x;		m[1][1] = V.y;		m[1][2] = V.z;		m[1][3] = 0.0f; //Y-axis
	m[2][0] = N.x;		m[2][1] = N.y;		m[2][2] = N.z;		m[2][3] = 0.0f; //Z-axis
	m[3][0] = 0.0f;		m[3][1] = 0.0f;		m[3][2] = 0.0f;		m[3][3] = 1.0f; //W-axis
}


void Matrix4f::InitPerpectiveProj(float FOV, float Width, float Height, float zNear, float zFar)
{
	const float ar = Width / Height;
	const float zRange = zNear - zFar;
	const float tanHalfFOV = tanf(ToRadian(FOV / 2.0f));

	m[0][0] = 1.0f / (tanHalfFOV * ar);	m[0][1] = 0.0f;				m[0][2] = 0.0f;						m[0][3] = 0.0f;
	m[1][0] = 0.0f;						m[1][1] = 1.0f/tanHalfFOV;	m[1][2] = 0.0f;						m[1][3] = 0.0f;
	m[2][0] = 0.0f;						m[2][1] = 0.0f;				m[2][2] = (-zNear -zFar)/zRange;	m[2][3] = 2.0f * zFar * zNear/zRange;
	m[3][0] = 0.0f;						m[3][1] = 0.0f;				m[3][2] = 1.0f;						m[3][3] = 0.0f;


}

Quaternion::Quaternion(float _x, float _y, float _z, float _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

void Quaternion::Normalize()
{
	float Length = sqrtf(x * x + y * y + z * z + w * w);

	x /= Length;
	y /= Length;
	z /= Length;
	w /= Length;
}

Quaternion Quaternion::Conjugate()
{
	Quaternion ret(-x, -y, -z, w); //NEGATE VALUES EXCEPT w
	return ret;
}

Quaternion operator*(const Quaternion& l, const Quaternion& r)
{
	const float w = (l.w * r.w) - (l.x * r.x) - (l.y * r.y) - (l.z * r.z);
	const float x = (l.x * r.w) + (l.w * r.x) + (l.y * r.z) - (l.z * r.y);
	const float y = (l.y * r.w) + (l.w * r.y) + (l.z * r.x) - (l.x * r.z);
	const float z = (l.z * r.w) + (l.w * r.z) + (l.x * r.y) - (l.y * r.x);

	Quaternion ret(x, y, z, w);

	return ret;
}

Quaternion operator*(const Quaternion& q, const Vector3f& v)
{
	const float w = -(q.x * v.x) - (q.y * v.y) - (q.z * v.z);
	const float x = (q.w * v.x) - (q.y * v.z) - (q.z * v.y);
	const float y = (q.w * v.y) - (q.z * v.x) - (q.x * v.z);
	const float z = (q.w * v.z) - (q.x * v.y) - (q.y * v.x);

	Quaternion ret(x, y, z, w);

	return ret;
}

Vector3f& getUnitVector3D( Vector3f &vector) {
    Vector3f newv;
    double d = sqrt(vector.x * vector.x + vector.y * vector.y +  vector.z * vector.z);
    newv.x = vector.x / d;
    newv.y = vector.y / d;
    newv.z = vector.z / d;
    return newv;
}


Vector3f& rotateAroundVector(Vector3f &vect1, Vector3f &vect2, double theta) {
    Vector3f newv;
    Vector3f unit = getUnitVector3D(vect2);	
    //theta = Math.toRadians(theta);
    double q0 = cos(theta/2);
    double q1 = sin(theta/2)*unit.x;
    double q2 = sin(theta/2)*unit.y;
    double q3 = sin(theta/2)*unit.z;

    // column vect
    newv.x = (q0*q0 + q1*q1 - q2*q2 - q3*q3)* +    2*(q2*q1 + q0*q3) * vect1.y +                       2*(q3*q1 - q0*q2) * vect1.z;
    newv.y = 2*(q1*q2 - q0*q3)*vect1.x +             (q0*q0 - q1*q1 + q2*q2 - q3*q3) * vect1.y +       2*(q3*q2 + q0*q1) * vect1.z;
    newv.z = 2*(q1*q3 + q0*q2)*vect1.x +             2*(q2*q3 - q0*q1) * vect1.y +                     (q0*q0 - q1*q1 - q2*q2 + q3*q3) * vect1.z;
    return newv;
}

