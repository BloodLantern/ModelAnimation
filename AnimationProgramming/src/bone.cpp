#include "bone.h"

Bone::Bone(const std::string& name)
	: m_Name(name), m_Parent(nullptr)
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
