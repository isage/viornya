#include "../State/Start.h"

#include <ctime>
#include <string>
#include <vector>
#include <memory>

#include "../Game/Game.h"
#include "../Graphics/Renderer.h"
#include "../ResourceManager.h"
#include "../Utils/Logger.h"
#include "../State/FadeOut.h"
#include "../State/FileBrowser.h"
#include "../State/Exit.h"
#include "../Sound/SoundManager.h"


using namespace Ironhead::Graphics;

namespace Ironhead
{
    namespace State
    {
        Start::Start() : State()
        {
        }

        Start::~Start()
        {
        }

        void Start::init()
        {
            if (_initialized) return;
            State::init();

            setModal(true);
            setFullscreen(true);

            _logoY = -84;
            _scrollerX = -129;
            _scrollerY = -128;
            _presentedY = Renderer::getInstance()->screenHeight + 50;
            _fadeStarted = false;
        }

        void Start::think(uint32_t delta)
        {
//            auto game = Game::Game::getInstance();
            State::think(delta);

//            LOG_DEBUG("delta: {}", delta);

            if (_logoY < 25.0)
                INC_PER_DELTA(_logoY);

            INC_PER_DELTA(_scrollerY);
            INC_PER_DELTA(_scrollerY);

            if (_scrollerY >= 0.0)
                _scrollerY -= 64.0;

            if (_scrollerX < 0.0)
            {
                INC_PER_DELTA(_scrollerX);
                INC_PER_DELTA(_scrollerX);
            }

            if (_presentedY > 245)
            {
                _presentedY -= 1.0 * PX_PER_DELTA();
                return;
            }

        }

        void Start::render()
        {
            Renderer::getInstance()->clearScreen(COLOR_TITLE);

            int lx = (Renderer::getInstance()->screenWidth / 2) - (Renderer::getInstance()->sprites.getSprite("Logo").width() / 2);

            Renderer::getInstance()->sprites.drawSprite("Logo", lx, _logoY);

//            int ktx = (Renderer::getInstance()->screenWidth / 2) - (Renderer::getInstance()->sprites.getSprite("Kani Title").width() / 2);
//            Renderer::getInstance()->sprites.drawSprite("Kani Title", ktx, 177);


            int prx = (Renderer::getInstance()->screenWidth / 2) - (Renderer::getInstance()->sprites.getSprite("PresentedBy").width() / 2);
            Renderer::getInstance()->sprites.drawSprite("PresentedBy", prx, _presentedY);

            for(int y=0; y < Renderer::getInstance()->screenHeight + 64; y += 64)
            {
                    int xa = (static_cast<int>(_scrollerX) >> 1);
                    int ya = (static_cast<int>(_scrollerY) + y);

                    Renderer::getInstance()->sprites.drawSprite("Scroller", xa, ya, 0, Sprite::Dir::LEFT);
                    Renderer::getInstance()->sprites.drawSprite("Scroller", (Renderer::getInstance()->screenWidth - 48) - xa, ya, 0, Sprite::Dir::RIGHT);
            }

            if (_presentedY > 245)
            {
                return;
            }


            int pzx = (Renderer::getInstance()->screenWidth / 2) - (126/2);

//            Renderer::getInstance()->fillRect(pzx, 106, pzx+126, 156, 0x0, 0x0, 0x21);
            Renderer::getInstance()->drawRect(pzx-2, 104, pzx+128, 158, 0xbb, 0xd9, 0xff);

            Renderer::getInstance()->font.draw(pzx+36, 116, "BROWSE", _menuPos == 0 ? 0xbbd9ff : 0xffffff);
            Renderer::getInstance()->font.draw(pzx+36, 126, "PLAYLISTS", _menuPos == 1 ? 0xbbd9ff : 0xffffff);
            Renderer::getInstance()->font.draw(pzx+36, 136, "QUIT", _menuPos == 2 ? 0xbbd9ff : 0xffffff);

            Renderer::getInstance()->sprites.drawSprite("Cursor", pzx+14, 114 + 10*_menuPos);
        }

        void Start::onKeyDown(Event::Keyboard* event)
        {
            int prx = (Renderer::getInstance()->screenWidth / 2) - (Renderer::getInstance()->sprites.getSprite("PresentedBy").width() / 2);

            if (_presentedY > 245)
            {
                _presentedY = 245;
                _logoY = 25;
                _scrollerX = 0;
                return;
            }

            auto game = Game::Game::getInstance();

            switch (event->keyCode())
            {
                case SDLK_z:
                    LOG_DEBUG("ZEE PRESSED");
                    switch(_menuPos)
                    {
                        case 0:
                          game->pushState(new FadeOut(new FileBrowser()));
                          break;
                        case 1:
                          game->pushState(new FadeOut(NULL));
                          break;
                        case 2:
                          game->pushState(new FadeOut(new Exit()));
                          break;
                    }
                    break;
                case SDLK_DOWN:
                    _menuPos++;
                    if (_menuPos > 2) _menuPos = 0;
                    break;
                case SDLK_UP:
                    _menuPos--;
                    if (_menuPos <0 ) _menuPos = 2;
                    break;
                default:
                  break;
            }
        }

        void Start::onButtonDown(Event::Gamepad* event)
        {
            int prx = (Renderer::getInstance()->screenWidth / 2) - (Renderer::getInstance()->sprites.getSprite("PresentedBy").width() / 2);

            if (_presentedY > 245)
            {
                _presentedY = 245;
                _logoY = 25;
                _scrollerX = 0;
                return;
            }

            auto game = Game::Game::getInstance();

            switch (event->button())
            {
                case SDL_CONTROLLER_BUTTON_A:
                    LOG_DEBUG("ZEE PRESSED");
                    switch(_menuPos)
                    {
                        case 0:
                          game->pushState(new FadeOut(new FileBrowser()));
                          break;
                        case 1:
                          game->pushState(new FadeOut(NULL));
                          break;
                        case 2:
                          game->pushState(new FadeOut(new Exit()));
                          break;
                    }
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                    _menuPos++;
                    if (_menuPos > 2) _menuPos = 0;
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_UP:
                    _menuPos--;
                    if (_menuPos <0 ) _menuPos = 2;
                    break;
                default:
                  break;
            }
        }

    }
}
