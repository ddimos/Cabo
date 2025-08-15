#pragma once

#include <SFML/System/Time.hpp>

namespace cn::shared::game
{
    inline constexpr short unsigned StandartDeckSize = 52;
    inline constexpr short unsigned ExtendedDeckSize = 54;
    inline constexpr short unsigned DefaultInitialNumberOfCards = 4;
    inline constexpr short unsigned MaxNumberOfPlayers = 8;
    inline constexpr short unsigned MaxNumberOfParticipantSlots = 8;
    inline constexpr short unsigned DefaultInitNumberOfParticipantSlots = 4;

    inline constexpr short unsigned TimeBeforeStartS = 2;
    inline constexpr short unsigned TimeForPlayerTurnS = 60;

    const sf::Time MoveUpdateDuration{sf::milliseconds(150)};    
} // namespace cn::shared::game
