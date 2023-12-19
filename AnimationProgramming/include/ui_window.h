#pragma once

#include <thread>

#include "skeleton.h"
#include "animation.h"
#include "mesh.h"
#include "shader.h"

class UiWindow
{
	std::thread m_Thread;
	bool m_CloseWindow;

	int32_t m_WindowX;
	int32_t m_WindowY;

	Skeleton* m_Skeleton;
	Bone* m_SelectedBone;

	std::vector<Animation>* m_Animations;
	float* m_MixedAnimationAlpha;

	Mesh* m_Mesh;
	Shader* m_Shader;

	void Main();
	void StartThread();
	void EndThread();

	void DrawSkeletonHierarchy();
	void DrawBoneInSkeletonHierarchy(Bone& bone);
	void DrawCurrentBoneInfo() const;
	void DrawAnimations();

public:
	UiWindow();
	~UiWindow();

	void SetSkeleton(Skeleton* skeleton);
	void SetAnimations(std::vector<Animation>* animations);
	void SetMixedAnimationAlpha(float* alpha);

	void* operator new(size_t count) = delete;
	void operator delete(void* ptr) = delete;
};
