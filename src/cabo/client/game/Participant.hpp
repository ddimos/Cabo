#pragma once

#include "shared/game/Participant.hpp"
#include "core/Context.hpp"
#include "core/Interpolator.hpp"

#include <SFML/System/Clock.hpp>

namespace cn::client::game
{

class Participant final : public shared::game::Participant
{
public:
    Participant(const core::Context& _context, shared::board::Participant& _boardParticipant, bool _isLocal);

    bool isLocal() const { return m_isLocal; }
    void addPosition(sf::Vector2f _pos);

private:
    void onUpdate(sf::Time _dt) override;

    bool m_isLocal = false;

    core::Interpolator<sf::Vector2f> m_interpolatorPos;
};

} // namespace cn::client::game
