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
    Card card;
    menu::item::SimpleImage* cardImage = nullptr;
    menu::item::Button* button; // can be a ref
    bool isCardValid = false;
};

using shared::game::StepId;
using shared::game::BoardState;
using shared::game::ActionType;
using shared::game::PileType;
using shared::game::InputType;
using shared::game::ClickSlotInputData;

} // namespace cn::client::game
