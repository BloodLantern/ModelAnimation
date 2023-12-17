// AnimationProgramming.cpp : Defines the entry point for the console application.
//

#include "Engine.h"
#include "Simulation.h"

#include <thread>

#include "ui_window.h"
#include "skeleton.h"
#include "animation.h"
#include "Engine_extensions.h"


class CSimulation : public ISimulation
{
private:
	UiWindow m_UiWindow;
	Skeleton m_Skeleton;
	std::vector<Animation> m_Animations;

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

			m_Skeleton.AddBone(bone);
		}

		m_Skeleton.SetupFamily();
		m_UiWindow.SetSkeleton(&m_Skeleton);
	}

	void LoadAnimation(std::string&& name)
	{
		Animation anim(std::move(name), GetAnimKeyCount(name.c_str()), m_Skeleton);

		for (size_t i = 0; i < anim.GetKeyCount(); i++)
		{
			for (size_t j = 0; j < anim.GetBoneCount(); j++)
			{
				Vector3 p;
				Quaternion r;
				EngineExt::GetAnimLocalBoneTransform(anim, (int) j, (int) i, p, r);

				anim.AddKeyFrame(i, j, p, r);

				// printf("%s : %d ; %d | %f ; %f ; %f | %f ; %f ; %f ; %f\n", anim.GetName().c_str(), i, j, p.x, p.y, p.z, r.real, r.imaginary.x, r.imaginary.y, r.imaginary.z);
			}
		}

		m_Animations.push_back(anim);
	}

	virtual void Init() override
	{
		LoadSkeleton();
		LoadAnimation("ThirdPersonWalk.anim");
		LoadAnimation("ThirdPersonRun.anim");

		m_UiWindow.SetAnimations(&m_Animations);
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

