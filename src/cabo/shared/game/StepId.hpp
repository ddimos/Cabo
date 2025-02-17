#pragma once

#include <cstdint>

namespace cn::shared::game
{

enum class StepId : uint8_t
{
    Idle,
    SeeOwnCard,
    DrawFromDeck
};

} // namespace cn::shared::game