#pragma once

#include "core/object/Object.hpp"
#include "core/Context.hpp"

#include "shared/game/Card.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

#include "functional"

namespace cn::client::game
{
 
class Card final: public core::object::Object, public shared::game::Card 
{
public:
    enum class State
    {
        InDeck,
        OnTable,
        InHand,
        TopInDiscard,
        InDiscard,
        Transiting
    };
    using OnFinishCallback = std::function<void(void)>;
    struct ChangeStateParams
    {
        sf::Vector2f desiredPosition{};
        float desiredRotation = 0.f;
        State desiredState = State::InDeck;
        OnFinishCallback onFinishCallback{};
    };

    Card(const core::Context& _context, sf::Vector2f _position);

    void set(Rank _rank, Suit _suit);
    void changeState(ChangeStateParams _params);

private:
    void onUpdate(sf::Time _dt) override;
    void onDraw(sf::RenderWindow& _window) override;

    void enterTransitingState();

    const core::Context& m_contextRef;

    State m_state = State::InDeck;

    bool m_isCardAvailable = false;

    sf::Sprite m_sprite;

    ChangeStateParams m_params;
    sf::Vector2f m_initPosition{};
    sf::Vector2f m_direction{};

    float m_transitionSpeed = 0.f;
    float m_rotationSpeed = 0.f;
    float m_distance = 0.f;
};

} // namespace cn::client::game
