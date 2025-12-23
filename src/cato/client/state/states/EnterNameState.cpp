#include "client/state/states/EnterNameState.hpp"
#include "client/state/StateIds.hpp"
#include "client/ResourceIds.hpp"

#include "client/menu/item/Button.hpp"
#include "client/menu/item/Input.hpp"
#include "client/menu/item/SimpleText.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace
{
constexpr cn::core::object::Container::Id MenuContainerId = 1;
} // namespace

namespace cn::client::states
{

EnterNameState::EnterNameState(core::state::Manager& _stateManagerRef)
    : State(_stateManagerRef)
{
    createContainer(MenuContainerId);
    
    auto& windowRef = getContext().get<sf::RenderWindow>();
    auto& textureHolderRef = getContext().get<TextureHolder>();
    auto& fontHolderRef = getContext().get<FontHolder>();

    auto nextButton = std::make_shared<menu::item::Button>(
        menu::Position{
            .m_position = sf::Vector2f(200.f, 100.f), .m_parentSize = sf::Vector2f(windowRef.getSize()),
            .m_specPositionX = menu::Position::Special::OFFSET_FROM_END, .m_specPositionY = menu::Position::Special::OFFSET_FROM_END
        },
        textureHolderRef.get(TextureIds::MainMenuJoinButton),   // TODO change the button
        sf::IntRect{0,   0, 200, 62},
        sf::IntRect{200, 0, 200, 62},
        [this](){
            if (m_inputPtr->getEnteredText().empty())
                return;

            auto& save = getContext().get<SaveHolder>().get(SaveIds::PlayerName);
            save.setValue(m_inputPtr->getEnteredText());

            save.write();

            pop();
            push(id::MainMenu);
        },
        sf::Mouse::Button::Left
    );
    m_buttonPtr = nextButton.get();
    nextButton->setActivationOption(core::object::Object::ActivationOption::Manually);
    getContainer(MenuContainerId).add(nextButton);

    auto text = std::make_shared<menu::item::SimpleText>(
        menu::Position{
            .m_position = sf::Vector2f(0.f, -95.f), .m_parentSize = sf::Vector2f(windowRef.getSize()),
            .m_specPositionX = menu::Position::Special::CENTER_ALLIGNED, .m_specPositionY = menu::Position::Special::OFFSET_FROM_CENTER
        },
        "Enter your name",
        fontHolderRef.get(FontIds::Main),
        24,
        sf::Color::White
    );
    getContainer(MenuContainerId).add(text);

    auto inputText = std::make_shared<menu::item::SimpleText>(
        menu::Position{},
        "Name",
        fontHolderRef.get(FontIds::Main),
        22,
        sf::Color::Black
    );
    
    auto input = std::make_shared<menu::item::Input>(
        menu::Position{
            .m_position = sf::Vector2f(0.f, 0.f), .m_parentSize = sf::Vector2f(windowRef.getSize()),
            .m_specPositionX = menu::Position::Special::CENTER_ALLIGNED, .m_specPositionY = menu::Position::Special::CENTER_ALLIGNED
        },
        textureHolderRef.get(TextureIds::Field),
        *inputText,
        "Name",
        [](uint32_t, const std::string&){ return true; },
        [](const std::string& ){},
        32
    );
    m_inputPtr = input.get();
    getContainer(MenuContainerId).add(input);
    getContainer(MenuContainerId).add(inputText);
}

core::state::Return EnterNameState::onUpdate(sf::Time _dt)
{
    auto& save = getContext().get<SaveHolder>().get(SaveIds::PlayerName);
    if (!save.getValue().empty())
    {
        pop();
        push(id::MainMenu);
    }

    if (m_inputPtr->getEnteredText().empty())
    {
        if (m_buttonPtr->isActivated())
            m_buttonPtr->requestDeactivated();
    }
    else
    {
        if (!m_buttonPtr->isActivated())
            m_buttonPtr->requestActivated();
    }

    return core::state::Return::Break;
}

} // namespace cn::client::states
