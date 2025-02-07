#pragma once

#include "core/object/Object.hpp"
#include "server/game/Card.hpp"

namespace cn::server::game
{

class Discard final : public core::object::Object
{
public:
    Discard();

    void discard(Card* _card);
    Card* getLast();
    
private:
    std::vector<Card*> m_cards;
};

} // namespace cn::server::game
