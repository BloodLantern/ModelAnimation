#pragma once

#include "maths/vector3.hpp"
#include "maths/quaternion.hpp"
#include "maths/matrix4x4.hpp"

class KeyFrame
{
private:
	Vector3 m_Position;
	Quaternion m_Rotation;

	Matrix4x4 m_LocalTrs;

public:
	KeyFrame();
	KeyFrame(const Vector3& position, const Quaternion& rotation);

	_NODISCARD const Vector3& GetPosition() const;
	_NODISCARD const Quaternion& GetRotation() const;
	_NODISCARD const Matrix4x4& GetTransform() const;
};