#include "shared/game/object/Discard.hpp"
#include "shared/game/object/Card.hpp"

namespace cn::shared::game::object
{

Discard::Discard(Id _id) 
    : Object(_id)
{}

void Discard::discard(Card* _card)
{
    CN_LOG_I_FRM("Add to discard {}, size {}", _card->getId().value(), m_cards.size());
    _card->addToDiscard();
    m_cards.push_back(_card);
}

void Discard::visit(std::function<void(Card&)> _visitor) const
{
    for (auto* card : m_cards)
        _visitor(*card);
}

void Discard::onUpdate(sf::Time)
{
    m_cards.erase(
        std::remove_if(m_cards.begin(), m_cards.end(),
            [this](Card* _card){
                bool remove = !_card->contains(getPosition());
                if (remove)
                {
                    _card->removeFromDiscard();
                    CN_LOG_I_FRM("Remove discard {}", _card->getId().value());
                }
                return remove;
            }
        ),
        m_cards.end()
    );
}

} // namespace cn::shared::game::object
