#pragma once

#include <thread>

#include "skeleton.h"
#include "animation.h"
#include "mesh.h"
#include "shader.h"
#include "montage/animation_montage.h"

class CSimulation;

class UiWindow
{
	int32_t m_WindowX;
	int32_t m_WindowY;

	Skeleton m_Skeleton;
	Bone* m_SelectedBone;

	AnimationMontage m_Montage = AnimationMontage("WalkingMontage");

	std::vector<Animation>* m_Animations;
	float* m_MixedAnimationAlpha;

	Mesh* m_Mesh;
	Shader* m_Shader;

	void Main();

	void DrawSkeletonHierarchy();
	void DrawBoneInSkeletonHierarchy(Bone& parentBone);
	void DrawCurrentBoneInfo() const;
	void DrawAnimations();

public:
	UiWindow();

	void SetAnimations(std::vector<Animation>* animations);
	void SetMixedAnimationAlpha(float* alpha);

	void* operator new(size_t count) = delete;
	void operator delete(void* ptr) = delete;
};
