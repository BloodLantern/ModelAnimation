#include "montage/animation_montage.h"

#include <algorithm>

AnimationMontage::AnimationMontage(std::string&& name)
    : m_Name(std::move(name))
{
}

void AnimationMontage::Update(float deltaTime)
{
    for (const AnimationMontageCommand& command : m_Commands)
        command.OnUpdate();
}

void AnimationMontage::Add(const AnimationMontageCommand& command)
{
    command.OnAdded(this);
    m_Commands.push_back(command);
}

void AnimationMontage::Remove(const AnimationMontageCommand& command)
{
    command.OnRemoved();
}

void AnimationMontage::UpdateFields()
{
}
