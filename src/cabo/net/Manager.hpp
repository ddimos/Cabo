#pragma once

#include "net/Factory.hpp"

#include "core/Context.hpp"
#include <nsf/NSF.hpp>

namespace cn::net
{

class Manager final
{
public:
    Manager(core::Context& _contextRef, bool _isServer);
    ~Manager();

    void init();

    void connect(nsf::NetworkAddress _address);

    void send(bool _isReliable, const core::event::Event& _event);

    void updateReceive();
    void updateSend();

private:
    core::Context& m_contextRef;
    bool m_isServer = false;
    std::unique_ptr<nsf::INSF> m_network;

    Factory m_factory;
};

} // namespace cn::net
