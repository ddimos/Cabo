#pragma once

#include "shared/game/StepId.hpp"
#include "shared/player/Types.hpp"

#include "core/event/Dispatcher.hpp"

#include <SFML/System/Time.hpp>

#include <cstdint>
#include <functional>
#include <limits>
#include <map>

namespace cn::shared::game
{

class Step
{
public:
    using StateId = uint16_t;
    constexpr static StateId StateIdInvalid = std::numeric_limits<StateId>::max();

    struct State
    {
        std::function<void(void)> onEnter;
        std::function<void(sf::Time)> onUpdate;
    };
    using StateMap = std::map<StateId, State>;

    Step(PlayerId _managedPlayerId, StateMap&& _states);
    virtual ~Step() = default;
    virtual void registerEvents(core::event::Dispatcher&, bool) {}
    virtual bool isFinished() const { return true; } // TODO  = 0
    virtual StepId getNextStepId() const { return StepId::Idle; } // = 0
    void update(sf::Time);

protected:
    core::event::ListenerId getListenerId() const { return m_listenerId; }
    StateId getCurrentStateId() const { return m_stateId; }
    PlayerId getManagedPlayerId() const { return m_managedPlayerId; }

    void requestFollowingState();
    void requestState(StateId _id);
private:
    core::event::ListenerId m_listenerId = core::event::ListenerIdInvalid;
    std::map<StateId, State> m_states;
    StateId m_stateId = StateIdInvalid;
    StateId m_nextStateId = StateIdInvalid;
    PlayerId m_managedPlayerId = PlayerIdInvalid;
};

} // namespace cn::shared::game
