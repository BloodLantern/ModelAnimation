#ifndef NOENGINE
#include "animation_programming.h"

#include "Engine.h"
#include "Simulation.h"

#include <thread>
#include <iostream>
#include <mutex>

#include "ui_window.h"
#include "skeleton.h"
#include "animation.h"
#include "Engine_extensions.h"
#include "montage/animation_montage.h"
#include "montage/anim_command_play.h"
#include "montage/anim_command_wait.h"
#include "montage/anim_command_cross_fade.h"
#include "montage/anim_command_play_mixed.h"

CSimulation::CSimulation()
	: m_UiWindow(&skeleton)
{
}

CSimulation::~CSimulation()
{
	m_UiWindow.Close();
}

void CSimulation::LoadSkeleton()
{
	skeletonMutex.lock();
	const size_t boneCount = GetSkeletonBoneCount() - 4;
	skeleton.Reserve(boneCount);

	for (size_t i = 0; i < boneCount; i++)
	{
		const int index = static_cast<int>(i);
		Bone bone(GetSkeletonBoneName(index));

		EngineExt::GetSkeletonBoneLocalBindTransform(index, bone.Position, bone.Rotation);
		bone.ComputeTransform();

		skeleton.AddBone(bone, i);
	}

	skeleton.SetupFamily();
	skeletonMutex.unlock();
}

void CSimulation::LoadAnimation(std::string&& name)
{
	Animation anim(std::move(name), GetAnimKeyCount(name.c_str()), &skeleton);

	for (size_t i = 0; i < anim.GetKeyCount(); i++)
	{
		for (size_t j = 0; j < anim.GetBoneCount(); j++)
		{
			Vector3 p;
			Quaternion r;
			EngineExt::GetAnimLocalBoneTransform(anim, static_cast<int>(j), static_cast<int>(i), p, r);

			anim.AddKeyFrame(i, j, p, r);
		}
	}

	m_Animations.push_back(anim);
}

void CSimulation::LoadMontage()
{
	const size_t walkingId = m_Montage.AddAnimation(m_Animations[0]);
	const size_t runningId = m_Montage.AddAnimation(m_Animations[1]);

	m_Montage.AddCommand(new AnimCmdPlayMixed(&m_MixedAnimationAlpha, walkingId, runningId));
	// m_Montage.AddCommand(new AnimCmdPlay(-1.f, walkingId));
}

void CSimulation::SetUiVariables()
{
	//m_UiWindow.SetSkeleton(&skeleton);
	m_UiWindow.SetAnimations(&m_Animations);
	m_UiWindow.SetMixedAnimationAlpha(&m_MixedAnimationAlpha);
}

void CSimulation::Init()
{
	LoadSkeleton();
	LoadAnimation("ThirdPersonWalk.anim");
	LoadAnimation("ThirdPersonRun.anim");

	LoadMontage();

	SetUiVariables();
}

void CSimulation::Update(const float frameTime)
{
	// X axis
	DrawLine(0, 0, 0, 100, 0, 0, 1, 0, 0);

	// Y axis
	DrawLine(0, 0, 0, 0, 100, 0, 0, 1, 0);

	// Z axis
	DrawLine(0, 0, 0, 0, 0, 100, 0, 0, 1);

	skeletonMutex.lock();
	skeleton.Draw();
	skeletonMutex.unlock();
	m_Montage.Update(frameTime);
}
#endif
