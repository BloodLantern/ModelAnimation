#pragma once

#include <string>

class AnimationMontage;

class AnimationMontageCommand
{
    std::string m_DisplayName;
    
protected:
    AnimationMontage* m_Montage = nullptr;

    bool m_HasStarted;

    float m_Time;
    
    _NODISCARD explicit AnimationMontageCommand(std::string&& displayName);
    
public:
    AnimationMontageCommand() = delete;
    
    virtual ~AnimationMontageCommand() = default;

    virtual void OnAdded(AnimationMontage* montage);
    virtual bool OnUpdate(const float deltaTime);
    virtual void OnRemoved() {}

    virtual void OnBegin();
    virtual void OnEnd();
};
