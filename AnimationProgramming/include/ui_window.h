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

	void Main();
	void StartThread();
	void EndThread();

	void DrawBone(const Bone& bone) const;

public:
	UiWindow();
	~UiWindow();

	void DrawSkeletonHierarchy() const;
	void SetSkeleton(Skeleton* skeleton);

	void* operator new(size_t count) = delete;
	void operator delete(void* ptr) = delete;
};
