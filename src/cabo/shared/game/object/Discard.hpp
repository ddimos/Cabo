#pragma once

#include "shared/game/object/Object.hpp"
#include <functional>
#include <vector>

namespace cn::shared::game::object
{

class Card;

class Discard : public Object
{
public:
    Discard(Id _id);

    void discard(Card* _card);
    void visit(std::function<void(Card&)> _visitor) const;

private:
    void onUpdate(sf::Time) override;

    std::vector<Card*> m_cards;
};

} // namespace cn::shared::game::object
