#pragma once

#include "client/game/Card.hpp"
#include "client/menu/item/Button.hpp"
#include "client/menu/item/SimpleImage.hpp"

#include "shared/game/StepId.hpp"
#include "shared/game/Types.hpp"

#include <SFML/System/Vector2.hpp>

namespace cn::client::game
{

struct PlayerSpawnPoint
{
    sf::Vector2f pos{};
    float angleDeg = 0;
};

using shared::game::ParticipantSlotId;

struct ParticipantSlot : public shared::game::ParticipantSlot
{
    Card* cardPtr = nullptr;
    menu::item::Button& buttonRef;
    sf::Vector2f position{};
    float rotation = 0.f;
};

using shared::game::StepId;
using shared::game::BoardState;
using shared::game::ActionType;
using shared::game::PileType;
using shared::game::InputType;
using shared::game::ClickSlotInputData;
using shared::game::InputDataVariant;

struct CardPositions
{
    sf::Vector2f discardPos{};
    sf::Vector2f firstMatchPos{};
};

} // namespace cn::client::game
