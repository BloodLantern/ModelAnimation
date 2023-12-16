#include "bone.h"

Bone::Bone()
	: m_Name(""), m_Parent(nullptr)
{
}

Bone::Bone(const std::string& name)
	: m_Name(name), m_Parent(nullptr)
{
}

Bone::Bone(const std::string& name, const Vector3& position, const Quaternion& rotation)
	: m_Name(name), m_Parent(nullptr), Position(position), Rotation(rotation)
{
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

_NODISCARD const std::vector<Bone*>& Bone::GetChildren() const
{
	return m_Children;
}

_NODISCARD const std::string& Bone::GetName() const
{
	return m_Name;
}
