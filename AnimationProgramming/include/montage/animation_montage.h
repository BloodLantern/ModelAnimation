#pragma once

#include "animation.h"
#include "montage/animation_montage_command.h"

struct AnimationMontageAnimation
{
    Animation& Anim;
    float Offset;

    AnimationMontageAnimation(Animation& anim, const float offset)
        : Anim(anim), Offset(offset)
    {}
};

class AnimationMontage
{
private:
    std::string m_Name;
    
    std::vector<AnimationMontageAnimation> m_Animations;
    std::vector<KeyFrame> m_LastKeyFrames;
    
    std::vector<AnimationMontageCommand*> m_Commands;
    size_t m_CurrentCommand = 0;

    float m_Time = 0.f;
    float m_Duration = 0.f;

public:
    _NODISCARD explicit AnimationMontage(std::string&& name);
    ~AnimationMontage();

    void Update(float deltaTime);

    void AddCommand(AnimationMontageCommand* command);
    void RemoveCommand(AnimationMontageCommand* command);

    _NODISCARD size_t AddAnimation(Animation& anim);

    Animation& GetAnimation(const size_t id);

private:
    void UpdateFields();
};
