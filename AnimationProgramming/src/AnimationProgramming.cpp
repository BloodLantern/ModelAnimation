// AnimationProgramming.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Engine.h"
#include "Simulation.h"

#include <iostream>
#include <thread>

#include "GLFW/glfw3.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

#include "ui_window.h"
#include "skeleton.h"
#include "Engine_extensions.h"


class CSimulation : public ISimulation
{
private:
	UiWindow m_UiWindow;
	Skeleton m_Skeleton;

	virtual void Init() override
	{
		size_t boneCount = GetSkeletonBoneCount() - 4;
		m_Skeleton.Reserve(boneCount);

		for (size_t i = 0; i < boneCount; i++)
		{
			Bone bone(GetSkeletonBoneName(i));

			EngineExt::GetSkeletonBoneLocalBindTransform(i, bone.Position, bone.Rotation);
			bone.ComputeTransform();

			m_Skeleton.AddBone(bone);
		}

		m_Skeleton.SetupFamily();
		m_UiWindow.SetSkeleton(&m_Skeleton);
		
		int spine01 = GetSkeletonBoneIndex("spine_01");

		int spineParent = GetSkeletonBoneParentIndex(spine01);
		const char* spineParentName = GetSkeletonBoneName(spineParent);
		
		float posX, posY, posZ, quatW, quatX, quatY, quatZ;
		size_t keyCount = GetAnimKeyCount("ThirdPersonWalk.anim");
		GetAnimLocalBoneTransform("ThirdPersonWalk.anim", spineParent, keyCount / 2, posX, posY, posZ, quatW, quatX, quatY, quatZ);
		
		printf("Spine parent bone : %s\n", spineParentName);
		printf("Anim key count : %ld\n", keyCount);
		printf("Anim key : pos(%.2f,%.2f,%.2f) rotation quat(%.10f,%.10f,%.10f,%.10f)\n", posX, posY, posZ, quatW, quatX, quatY, quatZ);
	}

	virtual void Update(float frameTime) override
	{
		// X axis
		DrawLine(0, 0, 0, 100, 0, 0, 1, 0, 0);

		// Y axis
		DrawLine(0, 0, 0, 0, 100, 0, 0, 1, 0);

		// Z axis
		DrawLine(0, 0, 0, 0, 0, 100, 0, 0, 1);

		m_Skeleton.Draw();
	}
};

int main()
{
	CSimulation simulation;

	Run(&simulation, 1400, 800);

	return 0;
}

