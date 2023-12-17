#include "bone.h"

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

_NODISCARD std::vector<Bone*>& Bone::GetChildren()
{
	return m_Children;
}

_NODISCARD const std::string& Bone::GetName() const
{
	return m_Name;
}

Matrix4x4& Bone::GetLocalTransform()
{
	return m_LocalTrs;
}
