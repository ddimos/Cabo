#pragma once

#include "core/state/State.hpp"
#include "core/event/Types.hpp"

namespace cn::client::states
{

class JoiningState final : public core::state::State
{
public:
    JoiningState(core::state::Manager& _stateManagerRef);

private:
    void onActivate() override;
    void onRegisterEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered) override;
    core::event::ListenerId m_listenerId{};
};

} // namespace cn::client::states
