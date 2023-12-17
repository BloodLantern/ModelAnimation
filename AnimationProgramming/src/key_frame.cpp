#include "key_frame.h"

KeyFrame::KeyFrame()
	: m_Position(0.f), m_Rotation(0.f)
{
	m_LocalTrs = Matrix4x4::Identity();
}

KeyFrame::KeyFrame(const Vector3& position, const Quaternion& rotation)
	: m_Position(position), m_Rotation(rotation)
{
	m_LocalTrs = Matrix4x4::TRS(position, rotation, Vector3(1.f));
}

const Vector3& KeyFrame::GetPosition() const
{
	return m_Position;
}

const Quaternion& KeyFrame::GetRotation() const
{
	return m_Rotation;
}

const Matrix4x4& KeyFrame::GetTransform() const
{
	return m_LocalTrs;
}
