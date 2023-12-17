#pragma once

#include <vector>
#include <string>

#include "maths/vector3.hpp"
#include "maths/quaternion.hpp"
#include "maths/matrix4x4.hpp"

class Bone
{
private:
	Bone* m_Parent;
	std::vector<Bone*> m_Children;

	std::string m_Name;

	Matrix4x4 m_LocalTrs;

public:
	Vector3 Position;
	Quaternion Rotation;

	Bone();
	Bone(const std::string& name);
	Bone(const std::string& name, const Vector3& position, const Quaternion& rotation);

	void ComputeTransform();

	void SetFamily(Bone* parent, std::vector<Bone*>& children);
	void SetParent(Bone* parent);
	void AddChild(Bone* child);

	_NODISCARD std::vector<Bone*>& GetChildren();
	_NODISCARD const std::string& GetName() const;
	_NODISCARD Matrix4x4& GetLocalTransform();
};
