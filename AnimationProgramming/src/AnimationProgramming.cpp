// AnimationProgramming.cpp : Defines the entry point for the console application.
//

#include "Engine.h"
#include "Simulation.h"

#include <thread>
#include <iostream>

#include "maths/matrix3x3.hpp"

#include "ui_window.h"
#include "skeleton.h"
#include "animation.h"
#include "Engine_extensions.h"
#include "montage/animation_montage.h"
#include "montage/anim_command_play.h"
#include "montage/anim_command_wait.h"
#include "montage/anim_command_cross_fade.h"
#include "montage/anim_command_play_mixed.h"


class CSimulation : public ISimulation
{
private:
	UiWindow m_UiWindow;
	Skeleton m_Skeleton;
	std::vector<Animation> m_Animations;
	float m_MixedAnimationAlpha;

	AnimationMontage m_Montage = AnimationMontage("WalkingMontage");

	void LoadSkeleton()
	{
		const size_t boneCount = GetSkeletonBoneCount() - 4;
		m_Skeleton.Reserve(boneCount);

		for (size_t i = 0; i < boneCount; i++)
		{
			const int index = (int) i;
			Bone bone(GetSkeletonBoneName(index));

			EngineExt::GetSkeletonBoneLocalBindTransform(index, bone.Position, bone.Rotation);
			bone.ComputeTransform();

			m_Skeleton.AddBone(bone, i);
		}

		m_Skeleton.SetupFamily();
	}

	void LoadAnimation(std::string&& name)
	{
		Animation anim(std::move(name), GetAnimKeyCount(name.c_str()), &m_Skeleton);

		for (size_t i = 0; i < anim.GetKeyCount(); i++)
		{
			for (size_t j = 0; j < anim.GetBoneCount(); j++)
			{
				Vector3 p;
				Quaternion r;
				EngineExt::GetAnimLocalBoneTransform(anim, (int) j, (int) i, p, r);

				anim.AddKeyFrame(i, j, p, r);
			}
		}

		m_Animations.push_back(anim);
	}

	void LoadMontage()
	{
		const size_t walkingId = m_Montage.AddAnimation(m_Animations[0]);
		const size_t runningId = m_Montage.AddAnimation(m_Animations[1]);

		m_Montage.AddCommand(new AnimCmdPlayMixed(&m_MixedAnimationAlpha, walkingId, runningId));
	}

	void SetUiVariables()
	{
		m_UiWindow.SetSkeleton(&m_Skeleton);
		m_UiWindow.SetAnimations(&m_Animations);
		m_UiWindow.SetMixedAnimationAlpha(&m_MixedAnimationAlpha);
	}

	void Init() override
	{
		LoadSkeleton();
		LoadAnimation("ThirdPersonWalk.anim");
		LoadAnimation("ThirdPersonRun.anim");

		LoadMontage();

		SetUiVariables();
	}

	void Update(const float frameTime) override
	{
		// X axis
		DrawLine(0, 0, 0, 100, 0, 0, 1, 0, 0);

		// Y axis
		DrawLine(0, 0, 0, 0, 100, 0, 0, 1, 0);

		// Z axis
		DrawLine(0, 0, 0, 0, 0, 100, 0, 0, 1);

		m_Skeleton.Draw();
		m_Montage.Update(frameTime);
	}
};

int main()
{
	CSimulation simulation;

	Run(&simulation, 1400, 800);

	return 0;
}

