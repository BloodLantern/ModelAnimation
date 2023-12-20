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
	int m_ParentIndex;
	int m_Index;
	std::vector<Bone*> m_Children;

	std::string m_Name;

	Matrix4x4 m_LocalTrs;
	Matrix4x4 m_GlobalTrs;
	Matrix4x4 m_GlobalInvTrs;

public:
	Vector3 Position;
	Quaternion Rotation;

	Bone();
	Bone(const std::string& name);
	Bone(const std::string& name, const Vector3& position, const Quaternion& rotation);
	Bone(const std::string& name, int index, int parentIndex);

	void ComputeTransform();

	void SetFamily(Bone* parent, std::vector<Bone*>& children);
	void SetParent(Bone* parent, const int parentId);
	void AddChild(Bone* child);

	void ComputeMatrices();

	_NODISCARD std::vector<Bone*>& GetChildren();
	_NODISCARD const int GetParentIndex() const;
	_NODISCARD const std::string& GetName() const;
	_NODISCARD const Matrix4x4& GetLocalTransform() const;
	_NODISCARD const Matrix4x4& GetGlobalTransform() const;
	_NODISCARD const Matrix4x4& GetGlobalInvTransform() const;
};
