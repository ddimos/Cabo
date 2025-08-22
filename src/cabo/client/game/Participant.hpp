#pragma once

#include "shared/game/object/Participant.hpp"
#include "core/Context.hpp"
#include "core/Interpolator.hpp"

#include <SFML/System/Clock.hpp>

namespace cn::client::game
{

class Participant final : public shared::game::object::Participant
{
public:
    Participant(const core::Context& _context, shared::game::object::Id _id, PlayerId _playerId, bool _isLocal);

    bool isLocal() const { return m_isLocal; }
    
private:
    void onUpdate(sf::Time _dt) override;
    void setMousePosition(sf::Vector2f _pos) override;

    bool m_isLocal = false;

    core::Interpolator<sf::Vector2f> m_interpolatorPos;
};

} // namespace cn::client::game
