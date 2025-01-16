#pragma once

#include "core/Context.hpp"

#include <nsf/NSF.hpp>

namespace cn::net
{

class Manager final
{
public:
    Manager(core::Context& _contextRef);
    ~Manager();

    void init(bool _isServer);
    // void deinit();

    void connect(nsf::NetworkAddress _address);

    void updateReceive();
    void updateSend();

private:
    core::Context& m_contextRef;
    std::unique_ptr<nsf::INSF> m_network;
};

} // namespace cn::net
