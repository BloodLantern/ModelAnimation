#pragma once

#include <mutex>

#include "Simulation.h"
#include "ui_window.h"
#include "montage/animation_montage.h"

class CSimulation : public ISimulation
{
	UiWindow m_UiWindow;
	std::vector<Animation> m_Animations;
	float m_MixedAnimationAlpha = 0.f;

	AnimationMontage m_Montage = AnimationMontage("WalkingMontage");

public:
	std::mutex skeletonMutex;
	Skeleton skeleton;
	
	CSimulation();

private:
	void LoadSkeleton();

	void LoadAnimation(std::string&& name);

	void LoadMontage();

	void SetUiVariables();

	void Init() override;

	void Update(const float frameTime) override;
};
