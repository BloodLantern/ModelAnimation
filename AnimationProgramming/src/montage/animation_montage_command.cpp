#include "montage/animation_montage_command.h"

AnimationMontageCommand::AnimationMontageCommand(std::string&& displayName)
    : m_DisplayName(std::move(displayName))
{
}

void AnimationMontageCommand::OnAdded(AnimationMontage* montage) const
{
}

void AnimationMontageCommand::OnUpdate() const
{
}

void AnimationMontageCommand::OnRemoved() const
{
}
