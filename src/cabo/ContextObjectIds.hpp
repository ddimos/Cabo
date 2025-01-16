#pragma once
#include "core/Context.hpp"
#include "core/event/Dispatcher.hpp"
#include "net/Manager.hpp"

#include "ResourceIds.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace cn
{
namespace context::id
{

enum : core::ContextObjectId
{
    EventDispatcher,
    FontHolder,
    NetManager,
    TextureHolder,
    Window
};

} // namespace context::id

template <>
core::ContextObjectId core::getContextObjectId<sf::RenderWindow>();
template <>
core::ContextObjectId core::getContextObjectId<TextureHolder>();
template <>
core::ContextObjectId core::getContextObjectId<FontHolder>();
template <>
core::ContextObjectId core::getContextObjectId<core::event::Dispatcher>();
template <>
core::ContextObjectId core::getContextObjectId<net::Manager>();

} // namespace cn
