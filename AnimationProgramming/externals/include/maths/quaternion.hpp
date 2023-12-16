#pragma once

#include "vector3.hpp"
#include "vector4.hpp"
#include "vector.hpp"
#include "matrix4x4.hpp"

#include <ostream>

class Quaternion
{
public:
	Vector3 imaginary;
	float real;

	static constexpr Quaternion Zero();
	static constexpr Quaternion UnitX();
	static constexpr Quaternion UnitY();
	static constexpr Quaternion UnitZ();
	static constexpr Quaternion UnitW();
	static constexpr Quaternion Identity();

	constexpr Quaternion();
	constexpr explicit Quaternion(const Vector4& values);
	constexpr Quaternion(const Vector3& imaginary, const float real);
	constexpr explicit Quaternion(const float xyzw);
	constexpr Quaternion(const float x, const float y, const float z, const float w);

	__forceinline constexpr float X() const;
	__forceinline constexpr float Y() const;
	__forceinline constexpr float Z() const;
	__forceinline constexpr float W() const;

	__forceinline constexpr float& X();
	__forceinline constexpr float& Y();
	__forceinline constexpr float& Z();
	__forceinline constexpr float& W();

	Quaternion Conjugate() const;
	float Length() const;
	constexpr float SquaredLength() const;
	constexpr Quaternion Inverse() const;

	[[nodiscard]]
	constexpr float  operator[](const size_t i) const;
	[[nodiscard]]
	constexpr float& operator[](const size_t i);
	explicit operator Vector<3>() const;
	constexpr explicit operator Vector3() const;
	explicit operator Vector<4>() const;
	constexpr explicit operator Vector4() const;

	static Quaternion FromAxisAngle(const Vector3& axis, const float angle);
	static Quaternion FromYawPitchRoll(const Vector3& rotation);
	static Quaternion FromRotationMatrix(const Matrix4x4& rotation);

	// Automatically generates all comparison operators
	[[nodiscard]]
	friend auto operator<=>(const Quaternion& a, const Quaternion& b) = default;
};

[[nodiscard]]
Quaternion operator+(const Quaternion& a, const Quaternion& b);
[[nodiscard]]
Quaternion operator-(const Quaternion& a, const Quaternion& b);
[[nodiscard]]
Quaternion operator-(const Quaternion& a);
[[nodiscard]]
Quaternion operator*(const Quaternion& a, const Quaternion& b);
[[nodiscard]]
Vector3 operator*(const Quaternion& q, const Vector3& v);
[[nodiscard]]
Vector3 operator*(const Vector3& v, const Quaternion& q);
[[nodiscard]]
Quaternion operator*(const Quaternion& v, const float factor);
[[nodiscard]]
Quaternion operator/(const Quaternion& a, const Quaternion& b);
[[nodiscard]]
Quaternion operator/(const Quaternion& v, const float factor);

Quaternion& operator+=(Quaternion& a, const Quaternion& b);
Quaternion& operator-=(Quaternion& a, const Quaternion& b);
Quaternion& operator*=(Quaternion& a, const Quaternion& b);
Quaternion& operator*=(Quaternion& v, const float factor);
Quaternion& operator/=(Quaternion& a, const Quaternion& b);
Quaternion& operator/=(Quaternion& v, const float factor);

std::ostream& operator<<(std::ostream& out, const Quaternion& v);

using quat = Quaternion;
