#include "quaternion.hpp"

#define SQ(x) ((x) * (x))

constexpr Quaternion Quaternion::Zero()
{
	return Quaternion();
}

constexpr Quaternion Quaternion::UnitX()
{
	return Quaternion(1.f, 0.f, 0.f, 0.f);
}

constexpr Quaternion Quaternion::UnitY()
{
	return Quaternion(0.f, 1.f, 0.f, 0.f);
}

constexpr Quaternion Quaternion::UnitZ()
{
	return Quaternion(0.f, 0.f, 1.f, 0.f);
}

constexpr Quaternion Quaternion::UnitW()
{
	return Quaternion(0.f, 0.f, 0.f, 1.f);
}

constexpr Quaternion Quaternion::Identity()
{
	return UnitW();
}

constexpr Quaternion::Quaternion()
	: real()
{
}

constexpr Quaternion::Quaternion(const Vector4& values)
	: imaginary((Vector3)values)
	, real(values.w)
{
}

constexpr Quaternion::Quaternion(const Vector3& imaginary, const float real)
	: imaginary(imaginary)
	, real(real)
{
}

constexpr Quaternion::Quaternion(const float xyzw)
	: imaginary(xyzw)
	, real(xyzw)
{
}

constexpr Quaternion::Quaternion(const float x, const float y, const float z, const float w)
	: imaginary(x, y, z)
	, real(w)
{
}

constexpr float& Quaternion::X()
{
	return imaginary.x;
}

constexpr float& Quaternion::Y()
{
	return imaginary.y;
}

constexpr float& Quaternion::Z()
{
	return imaginary.z;
}

constexpr float& Quaternion::W()
{
	return real;
}

Quaternion Quaternion::Conjugate() const
{
	return Quaternion(-imaginary, real);
}

float Quaternion::Length() const
{
	return std::sqrt(SquaredLength());
}

constexpr float Quaternion::SquaredLength() const
{
	return SQ(imaginary.x) + SQ(imaginary.y) + SQ(imaginary.z) + SQ(real);
}

constexpr Quaternion Quaternion::Inverse() const
{
	float sqLength = SquaredLength();
	if (sqLength > 0.f)
	{
		return Conjugate() / sqLength;
	}
	return Quaternion::Zero();
}

constexpr float Quaternion::X() const
{
	return imaginary.x;
}

constexpr float Quaternion::Y() const
{
	return imaginary.y;
}

constexpr float Quaternion::Z() const
{
	return imaginary.z;
}

constexpr float Quaternion::W() const
{
	return real;
}

constexpr float Quaternion::operator[](const size_t i) const
{
	return (&imaginary.x)[i];
}

constexpr float& Quaternion::operator[](const size_t i)
{
	return (&imaginary.x)[i];
}

Quaternion::operator Vector<3>() const
{
	return (Vector<3>) imaginary;
}

constexpr Quaternion::operator Vector3() const
{
	return imaginary;
}

Quaternion::operator Vector<4>() const
{
	return
	{
		imaginary.x,
		imaginary.y,
		imaginary.z,
		real
	};
}

Quaternion Quaternion::FromAxisAngle(const Vector3& axis, const float angle)
{
	float halfAngle = angle * 0.5f;
	float s = std::sin(halfAngle);
	float c = std::cos(halfAngle);

	Quaternion quat;

	quat.X() = axis.x * s;
	quat.Y() = axis.y * s;
	quat.Z() = axis.z * s;
	quat.W() = c;

	return quat;
}

Quaternion Quaternion::FromYawPitchRoll(const Vector3& rotation)
{
	float halfX = rotation.x * 0.5f;
	float cr = std::cos(halfX);
	float sr = std::sin(halfX);

	float halfY = rotation.y * 0.5f;
	float cp = std::cos(halfY);
	float sp = std::sin(halfY);

	float halfZ = rotation.z * 0.5f;
	float cy = std::cos(halfZ);
	float sy = std::sin(halfZ);

	Quaternion quat;

	quat.X() = cy * sp * cr + sy * cp * sr;
	quat.Y() = sy * cp * cr - cy * sp * sr;
	quat.Z() = cy * cp * sr - sy * sp * cr;
	quat.W() = cy * cp * cr + sy * sp * sr;

	return quat;
}

Quaternion Quaternion::FromRotationMatrix(const Matrix4x4& rotation)
{
	float trace = rotation.Trace();

	Quaternion q;

	if (trace > 0.f)
	{
		float s = std::sqrt(trace + 1.f);
		q.W() = s * 0.5f;
		s = 0.5f / s;
		q.X() = (rotation[2][3] - rotation[3][2]) * s;
		q.Y() = (rotation[3][1] - rotation[1][3]) * s;
		q.Z() = (rotation[1][2] - rotation[2][1]) * s;
	}
	else
	{
		if (rotation[1][1] >= rotation[2][2] && rotation[1][1] >= rotation[3][3])
		{
			float s = std::sqrt(1.f + rotation[1][1] - rotation[2][2] - rotation[3][3]);
			float invS = 0.5f / s;
			q.X() = 0.5f * s;
			q.Y() = (rotation[1][2] + rotation[2][1]) * invS;
			q.Z() = (rotation[1][3] + rotation[3][1]) * invS;
			q.W() = (rotation[2][3] - rotation[3][2]) * invS;
		}
		else if (rotation[2][2] > rotation[3][3])
		{
			float s = std::sqrt(1.f + rotation[2][2] - rotation[1][1] - rotation[3][3]);
			float invS = 0.5f / s;
			q.X() = (rotation[2][1] + rotation[1][2]) * invS;
			q.Y() = 0.5f * s;
			q.Z() = (rotation[3][2] + rotation[2][3]) * invS;
			q.W() = (rotation[3][1] - rotation[1][3]) * invS;
		}
		else
		{
			float s = std::sqrt(1.f + rotation[3][3] - rotation[1][1] - rotation[2][2]);
			float invS = 0.5f / s;
			q.X() = (rotation[3][1] + rotation[1][3]) * invS;
			q.Y() = (rotation[3][2] + rotation[2][3]) * invS;
			q.Z() = 0.5f * s;
			q.W() = (rotation[1][2] - rotation[2][1]) * invS;
		}
	}

	return q;
}

constexpr Quaternion::operator Vector4() const
{
	return Vector4(imaginary, real);
}

Quaternion operator+(const Quaternion& a, const Quaternion& b)
{
	return Quaternion(a.imaginary + b.imaginary, a.real + b.real);
}

Quaternion operator-(const Quaternion& a, const Quaternion& b)
{
	return Quaternion(a + -b);
}

Quaternion operator-(const Quaternion& a)
{
	return Quaternion(-a.imaginary, -a.real);
}

Quaternion operator*(const Quaternion& a, const Quaternion& b)
{
	return Quaternion(
		a.X() * b.W() + a.W() * b.X() - a.Z() * b.Y() + a.Y() * b.Z(),
		a.Y() * b.W() + a.Z() * b.X() + a.W() * b.Y() - a.X() * b.Z(),
		a.Z() * b.W() - a.Y() * b.X() + a.X() * b.Y() + a.W() + b.Z(),
		a.W() * b.W() - a.X() * b.X() - a.Y() * b.Y() - a.Z() * b.Z()
	);
}

Vector3 operator*(const Quaternion& q, const Vector3& v)
{
	return (Vector3)(q * Quaternion(v, 0.f));
}

Vector3 operator*(const Vector3& v, const Quaternion& q)
{
	return (Vector3)(Quaternion(v, 0.f) * q);
}

Quaternion operator*(const Quaternion& v, const float factor)
{
	return Quaternion(v.imaginary * factor, v.real * factor);
}

Quaternion operator/(const Quaternion& a, const Quaternion& b)
{
	return Quaternion(a.imaginary / b.imaginary, a.real / b.real);
}

Quaternion operator/(const Quaternion& v, const float factor)
{
	return Quaternion(v.imaginary / factor, v.real / factor);
}

Quaternion& operator+=(Quaternion& a, const Quaternion& b)
{
	return a = a + b;
}

Quaternion& operator-=(Quaternion& a, const Quaternion& b)
{
	return a = a - b;
}

Quaternion& operator*=(Quaternion& a, const Quaternion& b)
{
	return a = a * b;
}

Quaternion& operator*=(Quaternion& v, const float factor)
{
	return v = v * factor;
}

Quaternion& operator/=(Quaternion& a, const Quaternion& b)
{
	return a = a / b;
}

Quaternion& operator/=(Quaternion& v, const float factor)
{
	return v = v * factor;
}

std::ostream& operator<<(std::ostream& out, const Quaternion& v)
{
	char buffer[10];
	out << "[ ";
	(void)sprintf_s(buffer, sizeof(buffer), "%6.3f", v.imaginary.x);
	out << buffer << ", ";
	(void)sprintf_s(buffer, sizeof(buffer), "%6.3f", v.imaginary.y);
	out << buffer << ", ";
	(void)sprintf_s(buffer, sizeof(buffer), "%6.3f", v.imaginary.z);
	out << buffer << ", ";
	(void)sprintf_s(buffer, sizeof(buffer), "%6.3f", v.real);
	return out << buffer << " ]";
}
