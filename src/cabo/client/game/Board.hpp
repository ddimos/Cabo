#pragma once

#include "core/event/Dispatcher.hpp"
#include "core/object/Object.hpp"
#include "core/Context.hpp"

#include "client/game/Participant.hpp"
#include "client/menu/Types.hpp"

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

    void start();
    bool hasGameStarted() const;

    const core::Context& getContext() const { return m_contextRef; };

    void onLocalPlayerClickDeck();

private:

    const core::Context& m_contextRef;
    

    std::vector<game::Participant*> m_participants;


};
   
} // namespace cn::client::game
