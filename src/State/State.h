#ifndef STATE_H
#define STATE_H

#include <map>
#include <memory>
#include <vector>

#include "../Event/EventTarget.h"
#include "../Event/Keyboard.h"
#include "../Event/Gamepad.h"
#include "../Event/Mouse.h"
#include "../Graphics/Point.h"

namespace Ironhead
{
    namespace Event
    {
        class Event;
        class State;
    }
    namespace Game
    {
        class Game;
    }
    namespace State
    {
        using Graphics::Point;

        class State : public Event::EventTarget
        {
            public:
                State();
                virtual ~State();

                virtual bool fullscreen() const;
                virtual void setFullscreen(bool value);

                virtual bool modal() const;
                virtual void setModal(bool value);

                virtual bool active();
                virtual void setActive(bool value);

                virtual bool initialized();
                virtual void init();

                /**
                 * @brief Handle all OS events (Mouse, Keyboard, etc.) by this state.
                 * Used in Event Capturing process.
                 * This method is called first in the main loop (before think() and render()).
                 */
                virtual void handle(Event::Event* event);
                /**
                 * @brief Process all real-time logic of this state.
                 * This method is called after handle() but before render() in the main loop.
                 */
                virtual void think(uint32_t delta);
                /**
                 * @brief Renders all visible objects of this state on screen.
                 * This method is called last in the main loop (after handle() and think()).
                 */
                virtual void render();

                virtual void onStateActivate(Event::State* event);
                virtual void onStateDeactivate(Event::State* event);

                virtual void onKeyDown(Event::Keyboard* event);
                virtual void onKeyUp(Event::Keyboard* event);

                virtual void onButtonDown(Event::Gamepad* event);
                virtual void onButtonUp(Event::Gamepad* event);
                virtual void onAxis(Event::Gamepad* event);

                /**
                 * Invoked when state becomes active to receive events (when first pushed and after other modal states are removed from "above").
                 */
                Event::StateHandler& activateHandler();
                /**
                 * Invoked when state becomes inactive and won't receive any more events (when popped and also when other modal state is pushed on top of it).
                 */
                Event::StateHandler& deactivateHandler();
                /**
                 * Invoked when state is pushed to the stack, right before the first "activate".
                 */
                Event::StateHandler& pushHandler();
                /**
                 * Invoked when state is popped from the stack, right after last "deactivate".
                 */
                Event::StateHandler& popHandler();

                Event::KeyboardHandler& keyDownHandler();
                Event::KeyboardHandler& keyUpHandler();

                Event::GamepadHandler& buttonDownHandler();
                Event::GamepadHandler& buttonUpHandler();
                Event::GamepadHandler& axisHandler();


            protected:
                bool _modal = false; // prevents all states before this one to call think() method
                bool _active = false;

                bool _fullscreen = true; // prevents render all states before this one
                bool _initialized = false;

                Event::StateHandler _activateHandler, _deactivateHandler, _pushHandler, _popHandler;

                Event::KeyboardHandler _keyDownHandler, _keyUpHandler;
                Event::GamepadHandler _buttonDownHandler, _buttonUpHandler, _axisHandler;
        };
    }
}
#endif // STATE_H
