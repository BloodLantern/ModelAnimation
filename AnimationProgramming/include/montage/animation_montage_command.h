#pragma once

#include <string>

class AnimationMontage;

class AnimationMontageCommand
{
    std::string m_DisplayName;
    
protected:
    AnimationMontage* m_Montage = nullptr;
    
    _NODISCARD explicit AnimationMontageCommand(std::string&& displayName);
    
public:
    AnimationMontageCommand() = delete;
    
    virtual ~AnimationMontageCommand() = default;

    virtual void OnAdded(AnimationMontage* montage) const;
    virtual void OnUpdate(const float deltaTime) const;
    virtual void OnRemoved() const;
};
