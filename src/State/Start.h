#ifndef STATE_START_H
#define STATE_START_H

#include "../State/State.h"

namespace Ironhead
{
    namespace State
    {
        class Start : public State
        {
            public:
                Start();
                ~Start() override;

                void think(uint32_t delta) override;
                void init() override;
                void render() override;
                void onKeyDown(Event::Keyboard* event) override;
                void onButtonDown(Event::Gamepad* event) override;

            protected:
                float _logoY;
                float _presentedY;
                float _scrollerY;
                float _scrollerX;
                bool _fadeStarted;
                int8_t _menuPos = 0;
        };
    }
}
#endif // STATE_START_H
