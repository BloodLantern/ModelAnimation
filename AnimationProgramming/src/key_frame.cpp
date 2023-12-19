#include "key_frame.h"

KeyFrame::KeyFrame()
	: m_Position(0.f), m_Rotation(0.f)
{
}

KeyFrame::KeyFrame(const Vector3& position, const Quaternion& rotation)
	: m_Position(position), m_Rotation(rotation)
{
}

const Vector3& KeyFrame::GetPosition() const
{
	return m_Position;
}

const Quaternion& KeyFrame::GetRotation() const
{
	return m_Rotation;
}
