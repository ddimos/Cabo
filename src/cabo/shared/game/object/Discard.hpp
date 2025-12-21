#pragma once

#include "shared/game/object/Object.hpp"
#include <vector>

namespace cn::shared::game::object
{

class Card;

class Discard : public Object
{
public:
    Discard(Id _id);

    void discard(Card* _card);
    Card* getLast();

private:
    void onUpdate(sf::Time) override;

    std::vector<Card*> m_cards;
};

} // namespace cn::shared::game::object
