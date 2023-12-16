#pragma once

#include <vector>
#include <string>

class Bone
{
private:
	Bone* m_Parent;
	std::vector<Bone*> m_Children;

	std::string m_Name;

public:
	Bone(const std::string& name);

	void SetFamily(Bone* parent, std::vector<Bone*>& children);
	void SetParent(Bone* parent);
	void AddChild(Bone* child);

	_NODISCARD const std::vector<Bone*>& GetChildren() const;
	_NODISCARD const std::string& GetName() const;
};
