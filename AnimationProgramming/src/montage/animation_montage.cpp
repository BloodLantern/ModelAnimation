#include "montage/animation_montage.h"
#include "montage/animation_montage_command.h"
#include <algorithm>

AnimationMontage::AnimationMontage(std::string&& name)
    : m_Name(std::move(name))
{
}

AnimationMontage::~AnimationMontage()
{
    for (size_t i = 0; i < m_Commands.size(); i++)
        delete m_Commands[i];
}

void AnimationMontage::Update(float deltaTime)
{
    if (m_Commands[m_CurrentCommand]->OnUpdate(deltaTime))
    {
        m_Commands[m_CurrentCommand]->OnEnd();
        m_CurrentCommand = (m_CurrentCommand + 1) % m_Commands.size();
    }
}

void AnimationMontage::AddCommand(AnimationMontageCommand* command)
{
    command->OnAdded(this);
    m_Commands.push_back(command);
}

void AnimationMontage::RemoveCommand(AnimationMontageCommand* command)
{
    command->OnRemoved();
}

size_t AnimationMontage::AddAnimation(Animation& anim)
{
    m_Animations.push_back(AnimationMontageAnimation(anim, 0.f));

    return m_Animations.size() - 1;
}

Animation& AnimationMontage::GetAnimation(const size_t id)
{
    assert(id >= 0 && id < m_Animations.size() && "Animation doesn't exist in the montage");
    __assume(id >= 0 && id < m_Animations.size());

    return m_Animations[id].Anim;
}

void AnimationMontage::UpdateFields()
{
}
