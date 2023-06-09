#include "../State/State.h"

#include <algorithm>
#include <memory>

#include "../Event/State.h"
#include "../Game/Game.h"
#include "../Graphics/Renderer.h"


namespace Ironhead
{
    namespace State
    {
        State::State() : Event::EventTarget(Game::Game::getInstance()->eventDispatcher())
        {
            activateHandler().add([this](Event::State* event){ this->onStateActivate(event); });
            deactivateHandler().add([this](Event::State* event){ this->onStateDeactivate(event); });

            keyDownHandler().add([this](Event::Keyboard* event) { this->onKeyDown(event); });
            keyUpHandler().add([this](Event::Keyboard* event) { this->onKeyUp(event); });

            buttonDownHandler().add([this](Event::Gamepad* event) { this->onButtonDown(event); });
            buttonUpHandler().add([this](Event::Gamepad* event) { this->onButtonUp(event); });
            axisHandler().add([this](Event::Gamepad* event) { this->onAxis(event); });

        }

        State::~State()
        {
        }

        void State::init()
        {
            _initialized = true;
        }

        void State::think(uint32_t delta)
        {
        }

        bool State::initialized()
        {
            return _initialized;
        }

        bool State::fullscreen() const
        {
            return _fullscreen;
        }

        void State::setFullscreen(bool value)
        {
            _fullscreen = value;
        }

        bool State::modal() const
        {
            return _modal;
        }

        void State::setModal(bool value)
        {
            _modal = value;
        }

        void State::handle(Event::Event* event)
        {
            if (event->handled()) return;

            if (auto keyboardEvent = dynamic_cast<Event::Keyboard*>(event))
            {
                switch (keyboardEvent->originalType())
                {
                    case Event::Keyboard::Type::KEY_UP:
                    {
                        emitEvent(std::make_unique<Event::Keyboard>(*keyboardEvent), keyUpHandler());
                        break;
                    }
                    case Event::Keyboard::Type::KEY_DOWN:
                    {
                        emitEvent(std::make_unique<Event::Keyboard>(*keyboardEvent), keyDownHandler());
                        break;
                    }
                }
            }

            if (auto gamepadEvent = dynamic_cast<Event::Gamepad*>(event))
            {
                switch (gamepadEvent->originalType())
                {
                    case Event::Gamepad::Type::BUTTON_UP:
                    {
                        emitEvent(std::make_unique<Event::Gamepad>(*gamepadEvent), buttonUpHandler());
                        break;
                    }
                    case Event::Gamepad::Type::BUTTON_DOWN:
                    {
                        emitEvent(std::make_unique<Event::Gamepad>(*gamepadEvent), buttonDownHandler());
                        break;
                    }
                    case Event::Gamepad::Type::AXIS:
                    {
                        emitEvent(std::make_unique<Event::Gamepad>(*gamepadEvent), axisHandler());
                        break;
                    }
                }
            }
        }

        void State::render()
        {
        }

        void State::onStateActivate(Event::State* event)
        {
        }

        void State::onStateDeactivate(Event::State* event)
        {
        }

        void State::onKeyDown(Event::Keyboard* event)
        {
        }

        void State::onKeyUp(Event::Keyboard* event)
        {
        }

        void State::onButtonDown(Event::Gamepad* event)
        {
        }

        void State::onButtonUp(Event::Gamepad* event)
        {
        }

        void State::onAxis(Event::Gamepad* event)
        {
        }

        bool State::active()
        {
            return _active;
        }

        void State::setActive(bool value)
        {
            _active = value;
        }

        Event::StateHandler& State::pushHandler()
        {
            return _pushHandler;
        }

        Event::StateHandler& State::popHandler()
        {
            return _popHandler;
        }

        Event::StateHandler& State::activateHandler()
        {
            return _activateHandler;
        }

        Event::StateHandler& State::deactivateHandler()
        {
            return _deactivateHandler;
        }

        Event::KeyboardHandler& State::keyDownHandler()
        {
            return _keyDownHandler;
        }

        Event::KeyboardHandler& State::keyUpHandler()
        {
            return _keyUpHandler;
        }

        Event::GamepadHandler& State::buttonUpHandler()
        {
            return _buttonUpHandler;
        }

        Event::GamepadHandler& State::buttonDownHandler()
        {
            return _buttonDownHandler;
        }

        Event::GamepadHandler& State::axisHandler()
        {
            return _axisHandler;
        }
    }
}
