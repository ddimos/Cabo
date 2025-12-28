#include "client/state/states/EnteringState.hpp"
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

EnteringState::EnteringState(core::state::Manager& _stateManagerRef)
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
        textureHolderRef.get(TextureIds::JoinButton),
        sf::IntRect{0,   0, 200, 62},
        sf::IntRect{200, 0, 200, 62},
        [this](){
            if (m_inputNamePtr->getEnteredText().empty() || m_inputServerPtr->getEnteredText().empty())
                return;

            auto& saveName = getContext().get<SaveHolder>().get(SaveIds::PlayerName);
            saveName.setValue(m_inputNamePtr->getEnteredText());
            saveName.write();

            auto& saveServer = getContext().get<SaveHolder>().get(SaveIds::ServerAddress);
            saveServer.setValue(m_inputServerPtr->getEnteredText());
            saveServer.write();

            pop();
            push(id::Joining);
        },
        sf::Mouse::Button::Left
    );
    m_buttonPtr = nextButton.get();
    nextButton->setActivationOption(core::object::Object::ActivationOption::Manually);
    getContainer(MenuContainerId).add(nextButton);

    auto textName = std::make_shared<menu::item::SimpleText>(
        menu::Position{
            .m_position = sf::Vector2f(0.f, -140.f), .m_parentSize = sf::Vector2f(windowRef.getSize()),
            .m_specPositionX = menu::Position::Special::CENTER_ALLIGNED, .m_specPositionY = menu::Position::Special::OFFSET_FROM_CENTER
        },
        "Enter your name",
        fontHolderRef.get(FontIds::Main),
        24,
        sf::Color::White
    );
    getContainer(MenuContainerId).add(textName);

    auto inputTextName = std::make_shared<menu::item::SimpleText>(
        menu::Position{},
        "",
        fontHolderRef.get(FontIds::Main),
        22,
        sf::Color::Black
    );
    
    auto inputName = std::make_shared<menu::item::Input>(
        menu::Position{
            .m_position = sf::Vector2f(0.f, -60.f), .m_parentSize = sf::Vector2f(windowRef.getSize()),
            .m_specPositionX = menu::Position::Special::CENTER_ALLIGNED, .m_specPositionY = menu::Position::Special::OFFSET_FROM_CENTER
        },
        textureHolderRef.get(TextureIds::Field),
        *inputTextName,
        "Name",
        [](uint32_t, const std::string&){ return true; },
        [](const std::string& ){},
        32
    );
    m_inputNamePtr = inputName.get();
    getContainer(MenuContainerId).add(inputName);
    getContainer(MenuContainerId).add(inputTextName);

    auto textServer = std::make_shared<menu::item::SimpleText>(
        menu::Position{
            .m_position = sf::Vector2f(0.f, 60.f), .m_parentSize = sf::Vector2f(windowRef.getSize()),
            .m_specPositionX = menu::Position::Special::CENTER_ALLIGNED, .m_specPositionY = menu::Position::Special::OFFSET_FROM_CENTER
        },
        "Enter the server address",
        fontHolderRef.get(FontIds::Main),
        24,
        sf::Color::White
    );
    getContainer(MenuContainerId).add(textServer);

    auto inputTextServer = std::make_shared<menu::item::SimpleText>(
        menu::Position{},
        "",
        fontHolderRef.get(FontIds::Main),
        22,
        sf::Color::Black
    );
    auto inputServer = std::make_shared<menu::item::Input>(
        menu::Position{
            .m_position = sf::Vector2f(0.f, 140.f), .m_parentSize = sf::Vector2f(windowRef.getSize()),
            .m_specPositionX = menu::Position::Special::CENTER_ALLIGNED, .m_specPositionY = menu::Position::Special::OFFSET_FROM_CENTER
        },
        textureHolderRef.get(TextureIds::Field),
        *inputTextServer,
        "xxx.xxx.xxx:xxxxx",
        [](uint32_t _enteredChar, const std::string& _enteredText){
            (void)_enteredText; // TODO validate the address
            return (_enteredChar == 46 || (_enteredChar >= 48 && _enteredChar <= 58)); // Only numbers, a colon, a dot
        },
        [](const std::string& ){},
        32
    );
    m_inputServerPtr = inputServer.get();
    getContainer(MenuContainerId).add(inputServer);
    getContainer(MenuContainerId).add(inputTextServer);

    // TODO pre-fill the input if the save is not empty
}

core::state::Return EnteringState::onUpdate(sf::Time _dt)
{
    if (m_inputNamePtr->getEnteredText().empty() || m_inputServerPtr->getEnteredText().empty())
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
