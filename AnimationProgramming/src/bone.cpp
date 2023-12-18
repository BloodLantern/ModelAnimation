#include "bone.h"
#include <iostream>

Bone::Bone()
	: m_Name(""), m_Parent(nullptr), m_LocalTrs(Matrix4x4::Identity())
{
}

Bone::Bone(const std::string& name)
	: m_Name(name), m_Parent(nullptr), m_LocalTrs(Matrix4x4::Identity())
{
}

Bone::Bone(const std::string& name, const Vector3& position, const Quaternion& rotation)
	: m_Name(name), m_Parent(nullptr), Position(position), Rotation(rotation)
{
	m_LocalTrs = Matrix4x4::TRS(position, rotation, Vector3(1.f));
}

void Bone::ComputeTransform()
{
	m_LocalTrs = Matrix4x4::TRS(Position, Rotation, Vector3(1.f));
}

void Bone::SetFamily(Bone* parent, std::vector<Bone*>& children)
{
	m_Parent = parent;
	m_Children = children;
}

void Bone::SetParent(Bone* parent)
{
	m_Parent = parent;
}

void Bone::AddChild(Bone* child)
{
	m_Children.push_back(child);
}

void Bone::ComputeMatrixes()
{
	Bone* parent = m_Parent;

	if (parent)
	{
		m_GlobalTrs = parent->m_GlobalTrs * m_LocalTrs;
	}
	else
	{
		m_GlobalTrs = m_LocalTrs;
	}

	m_GlobalInvTrs = Matrix4x4::Inverse(m_GlobalTrs);
}

_NODISCARD std::vector<Bone*>& Bone::GetChildren()
{
	return m_Children;
}

_NODISCARD const std::string& Bone::GetName() const
{
	return m_Name;
}

const Matrix4x4& Bone::GetLocalTransform() const
{
	return m_LocalTrs;
}

const Matrix4x4& Bone::GetGlobalTransform() const
{
	return m_GlobalTrs;
}

const Matrix4x4& Bone::GetGlobalInvTransform() const
{
	return m_GlobalInvTrs;
}
