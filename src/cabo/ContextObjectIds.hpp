#pragma once
#include "core/Context.hpp"
#include "core/event/Dispatcher.hpp"
#include "shared/net/Manager.hpp"

#include "client/ResourceIds.hpp"
#include "client/player/Manager.hpp"

#include "server/player/Manager.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>

namespace cn
{
namespace context::id
{

enum : core::ContextObjectId
{
    EventDispatcher,
    FontHolder,
    NetManager,
    PlayerManager,
    SystemClock,
    TextureHolder,
    Window
};

} // namespace context::id

template <>
core::ContextObjectId core::getContextObjectId<sf::RenderWindow>();
template <>
core::ContextObjectId core::getContextObjectId<client::TextureHolder>();
template <>
core::ContextObjectId core::getContextObjectId<client::FontHolder>();
template <>
core::ContextObjectId core::getContextObjectId<core::event::Dispatcher>();
template <>
core::ContextObjectId core::getContextObjectId<net::Manager>();
template <>
core::ContextObjectId core::getContextObjectId<sf::Clock>();
// TODO to fix this, create two version of this file
template <>
core::ContextObjectId core::getContextObjectId<client::player::Manager>();
template <>
core::ContextObjectId core::getContextObjectId<server::player::Manager>();

} // namespace cn
