#include "states/FinishState.hpp"

#include "states/StateIds.hpp"
#include "ResourceIds.hpp"

#include "menu/item/SimpleText.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace cn::states
{

FinishState::FinishState(core::state::Manager& _stateManagerRef)
    : State(_stateManagerRef)
{
    auto text = std::make_shared<menu::item::SimpleText>(
        menu::Position{
            .m_position = sf::Vector2f(0.f, -45.f), .m_parentSize = sf::Vector2f(getContext().windowRef.getSize()),
            .m_specPositionX = menu::Position::Special::CENTER_ALLIGNED, .m_specPositionY = menu::Position::Special::OFFSET_FROM_CENTER
        },
        "Finish",
        getContext().fontHolderRef.get(FontIds::Main),
        20,
        sf::Color::White
    );
    getMenuContainer().add(text);
}

core::state::Return FinishState::onHandleEvent(const sf::Event& _event)
{
    if (_event.type == sf::Event::KeyReleased)
    {
        pop();
        push(id::MainMenu);
    }
    return core::state::Return::Break;
}

} // namespace cn::states
