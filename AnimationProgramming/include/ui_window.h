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

	Skeleton* m_Skeleton;
	Bone* m_SelectedBone;

	AnimationMontage m_Montage = AnimationMontage("WalkingMontage");

	std::vector<Animation>* m_Animations;
	float* m_MixedAnimationAlpha;

	Mesh* m_Mesh;
	Shader* m_Shader;

	std::thread m_Thread;

#ifndef NOENGINE
	bool m_CloseWindow;
#endif

	void Main();

	void DrawSkeletonHierarchy();
	void DrawBoneInSkeletonHierarchy(Bone& parentBone);
	void DrawCurrentBoneInfo() const;
	void DrawAnimations();

#ifndef NOENGINE
	void StartThread();
	void EndThread();
#endif

public:
#ifndef NOENGINE
	UiWindow(Skeleton* skeleton);
#else
	UiWindow();
#endif
	~UiWindow();

	void SetAnimations(std::vector<Animation>* animations);
	void SetMixedAnimationAlpha(float* alpha);
#ifndef NOENGINE
	void Close();
#endif

	void* operator new(size_t count) = delete;
	void operator delete(void* ptr) = delete;
};
