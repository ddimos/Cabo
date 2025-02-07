#pragma once

#include "client/game/Card.hpp"
#include "client/menu/item/Button.hpp"

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
    Card* card = nullptr;
    menu::item::Button* button; // can be a ref
};

} // namespace cn::client::game
