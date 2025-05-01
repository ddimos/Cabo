#pragma once

#include "core/Identifier.hpp"

#include <string>

namespace cn
{

struct Player;
using PlayerId = core::Identifier<Player, uint8_t>;

struct Player
{
    std::string name;
    PlayerId id{};
};

} // namespace cn

template <>
struct std::hash<cn::PlayerId>
{
    std::size_t operator()(const cn::PlayerId& _id) const
    {
        return std::hash<cn::PlayerId::Type>()(_id.value());
    }
};
