#pragma once

#include <thread>

#include "skeleton.h"

class UiWindow
{
private:
	std::thread m_Thread;
	bool m_CloseWindow;

	int32_t m_WindowX;
	int32_t m_WindowY;

	Skeleton* m_Skeleton;
	Bone* m_SelectedBone;

	void Main();
	void StartThread();
	void EndThread();

	void DrawBoneInSkeletonHierarchy(Bone& bone);
	void DrawCurrentBoneInfo() const;

public:
	UiWindow();
	~UiWindow();

	void DrawSkeletonHierarchy();
	void SetSkeleton(Skeleton* skeleton);

	void* operator new(size_t count) = delete;
	void operator delete(void* ptr) = delete;
};
