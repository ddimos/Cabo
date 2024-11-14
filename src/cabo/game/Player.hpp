#pragma once

#include "core/object/Object.hpp"
#include "game/Card.hpp"

#include <vector>

namespace cn::game
{

class Player final : public core::object::Object
{
public:
    Player();

    void deal(const std::vector<Card>& _cards);

private:
    std::vector<Card> m_cards;
};

} // namespace cn::game
