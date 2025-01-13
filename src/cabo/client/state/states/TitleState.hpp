#pragma once

#include "core/event/Types.hpp"
#include "core/state/State.hpp"

namespace core::object
{
    class Object;
}

namespace cn::client::states
{

class TitleState final : public core::state::State
{
public:
    TitleState(core::state::Manager& _stateManagerRef);

private:
    core::state::Return onUpdate(sf::Time _dt) override;
    void onDraw() override;
    void onRegisterEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered) override;

    std::shared_ptr<core::object::Object> m_text;

    bool m_showText = true;
    sf::Time m_toggleEffectTimeS = sf::seconds(1.f);
    sf::Time m_toggleEffectTimeDt = sf::Time::Zero;

    core::event::ListenerId m_listenerId = core::event::ListenerIdInvalid;
};
} // namespace cn::client::states
