#pragma once

#include "client/game/Participant.hpp"
#include "client/game/Step.hpp"
#include "client/menu/Types.hpp"

#include "shared/player/Types.hpp"

#include "core/event/Dispatcher.hpp"
#include "core/event/Types.hpp"
#include "core/object/Object.hpp"
#include "core/Context.hpp"

#include <SFML/System/Time.hpp>

#include <memory>
#include <vector>

namespace cn::client::game
{

class Board final
{
    enum class State
    {
        None,
        LookingCard,
        Playing,
        Finishing
    };

public:
    using DecideButtons = std::vector<client::menu::item::ButtonPtr>;

    Board(const core::Context& _context, std::vector<game::Participant*>&& _participants);

    void registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered);
    void update(sf::Time _dt);

    const core::Context& getContext() const { return m_contextRef; };
    Participant* getParticipant(PlayerId _id);
    
private:

    const core::Context& m_contextRef;
    
    std::vector<Participant*> m_participants;

    core::event::ListenerId m_listenerId = core::event::ListenerIdInvalid;
    PlayerId m_localPlayerId = PlayerIdInvalid;
    std::unique_ptr<Step> m_localPlayerStep = {}; 
};
   
} // namespace cn::client::game
