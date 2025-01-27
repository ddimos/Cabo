#pragma once

#include <nsf/Types.hpp>

#include <string>

namespace cn
{

using PlayerId = nsf::PeerID;
constexpr PlayerId PlayerIdInvalid = nsf::PEER_ID_INVALID;

struct Player
{
    PlayerId id = PlayerIdInvalid;
    std::string name;
};

} // namespace cn
