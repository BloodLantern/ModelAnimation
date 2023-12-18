#pragma once

#include "animation.h"
#include "montage/animation_montage_command.h"

struct AnimationMontageAnimation
{
    Animation Anim;
    float Offset;
};

class AnimationMontage
{
    std::string m_Name;
    
    std::vector<AnimationMontageAnimation> m_Animations;
    std::vector<KeyFrame> m_LastKeyFrames;
    
    std::vector<AnimationMontageCommand> m_Commands;

    float m_Time;
    float m_Duration;

public:
    _NODISCARD explicit AnimationMontage(std::string&& name);

    void Update(float deltaTime);

    void Add(const AnimationMontageCommand& command);
    void Remove(const AnimationMontageCommand& command);

private:
    void UpdateFields();
};
