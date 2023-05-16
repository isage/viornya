#include "../State/FileBrowser.h"

#include <algorithm>
#include <ctime>
#include <string>
#include <vector>
#include <memory>
#include <filesystem>
#include <iostream>

#include "../Game/Game.h"
#include "../Graphics/Renderer.h"
#include "../ResourceManager.h"
#include "../Settings.h"
#include "../Utils/Logger.h"
#include "../Utils/natural_sort.hpp"
#include "../State/FadeOut.h"
#include "../State/Exit.h"
#include "../Sound/SoundManager.h"


using namespace Ironhead::Graphics;

namespace Ironhead
{
    namespace State
    {
        FileBrowser::FileBrowser() : State()
        {
        }

        FileBrowser::~FileBrowser()
        {
        }

        void FileBrowser::init()
        {
            if (_initialized) return;
            State::init();

            setModal(true);
            setFullscreen(true);

            _scrollerX = 0;
            _scrollerY = -128;

#ifdef __VITA__
            _cwd = "ux0:/data";
#else
            _cwd = "/home/isage";
#endif
            auto last_dir = Settings::getInstance()->musicPath();
            if(!last_dir.empty())
                _cwd = last_dir;

            cwd(_cwd);
        }

        void FileBrowser::cwd(const std::filesystem::path& path)
        {
            _menuPos = 0;
            std::filesystem::path oldcwd = _cwd;
            _cwd = path;

            std::vector<std::filesystem::directory_entry> _direntries;
            std::vector<std::filesystem::directory_entry> _fileentries;

            try {
                for (auto const& dir_entry : std::filesystem::directory_iterator{_cwd})
                {
                    try {
                        if (dir_entry.is_directory())
                            _direntries.push_back(dir_entry);
                        else
                        {
                            if (dir_entry.path().extension() == std::string(".org"))
                                _fileentries.push_back(dir_entry);
                        }
                    } catch (std::filesystem::filesystem_error& e)
                    {
                        // whatever :<
                    }
                }
            } catch (std::filesystem::filesystem_error& e)
            {
                _cwd = oldcwd;
                return;
            }

            Settings::getInstance()->setMusicPath(path.string());
            Settings::getInstance()->save();

            SI::natural::sort(_direntries);
            SI::natural::sort(_fileentries);

            _entries.clear();
            _entries.push_back(std::filesystem::directory_entry{_cwd.parent_path()});
            _entries.insert(_entries.end(), _direntries.begin(), _direntries.end());
            _entries.insert(_entries.end(), _fileentries.begin(), _fileentries.end());


        }

        void FileBrowser::think(uint32_t delta)
        {
            State::think(delta);

            INC_PER_DELTA(_scrollerY);
            INC_PER_DELTA(_scrollerY);

            if (_scrollerY >= 0.0)
                _scrollerY -= 64.0;

            if (_scrollerX < 0.0)
            {
                INC_PER_DELTA(_scrollerX);
                INC_PER_DELTA(_scrollerX);
            }
        }

        void FileBrowser::render()
        {
            Renderer::getInstance()->clearScreen(COLOR_TITLE);

            for(int y=0; y < Renderer::getInstance()->screenHeight + 64; y += 64)
            {
                    int xa = (static_cast<int>(_scrollerX) >> 1);
                    int ya = (static_cast<int>(_scrollerY) + y);

                    Renderer::getInstance()->sprites.drawSprite("Scroller", xa, ya, 0, Sprite::Dir::LEFT);
                    Renderer::getInstance()->sprites.drawSprite("Scroller", (Renderer::getInstance()->screenWidth - 48) - xa, ya, 0, Sprite::Dir::RIGHT);
            }

            int pzx = (Renderer::getInstance()->screenWidth / 2) - (126/2);

            Renderer::getInstance()->drawRect(48, 1, Renderer::getInstance()->screenWidth - 48, Renderer::getInstance()->screenHeight - 50, 0xbb, 0xd9, 0xff);

            Renderer::getInstance()->drawRect(48, Renderer::getInstance()->screenHeight - 48, Renderer::getInstance()->screenWidth - 48, Renderer::getInstance()->screenHeight - 1, 0xbb, 0xd9, 0xff);

            Renderer::getInstance()->font.draw(68, 1, _cwd, 0xffd9bb);

            Renderer::getInstance()->font.draw(68, Renderer::getInstance()->screenHeight - 40, Sound::SoundManager::getInstance()->playing() ? "Playing:" : "Stopped", 0xffd9bb);
            if(Sound::SoundManager::getInstance()->playing())
            {
                Renderer::getInstance()->font.draw(68, Renderer::getInstance()->screenHeight - 28, Sound::SoundManager::getInstance()->currentSong().filename().string(), 0xbbd9bb);
            }

            Renderer::getInstance()->font.draw(68, Renderer::getInstance()->screenHeight - 16, "X - play/stop, O - back, L - Rewind, R - Fast-forward", 0xffd9bb);

            int i = 0;
            std::for_each_n(_entries.begin() + (20 * (_menuPos / 20)), std::min(20, (int)(_entries.size() - (20 * (_menuPos / 20)))), [&i, this](auto& entry) {
                if (entry.path() == _cwd.parent_path())
                    Renderer::getInstance()->font.draw(68, 16 + i*10, "..", 0xbbd9ff);
                else
                    Renderer::getInstance()->font.draw(68, 16 + i*10, entry.path().filename(), entry.is_directory() ? 0xbbd9ff : 0xffffff);
                i++;
            });

            Renderer::getInstance()->sprites.drawSprite("Cursor", 48, 16 + (_menuPos % 20) * 10);

        }

        void FileBrowser::onKeyDown(Event::Keyboard* event)
        {
            auto game = Game::Game::getInstance();

            switch (event->keyCode())
            {
                case SDLK_z:
                    if (_entries.at(_menuPos).is_directory()) cwd(_entries.at(_menuPos).path());
                    else
                    {
                        if (Sound::SoundManager::getInstance()->playing())
                            Sound::SoundManager::getInstance()->stop();
                        else
                            Sound::SoundManager::getInstance()->music(_entries.at(_menuPos).path());
                    }
                    break;
                case SDLK_x:
                    cwd(_entries.at(0).path());
                    break;
                case SDLK_DOWN:
                    _menuPos++;
                    if (_menuPos >= _entries.size()) _menuPos = 0;
                    break;
                case SDLK_UP:
                    _menuPos--;
                    if (_menuPos <0 ) _menuPos = _entries.size() - 1;
                    break;
                    break;
                case SDLK_PAGEDOWN:
                    _menuPos+=20;
                    if (_menuPos >= _entries.size()) _menuPos = 0;
                    break;
                case SDLK_PAGEUP:
                    _menuPos-=20;
                    if (_menuPos < 0) _menuPos = _entries.size() - 1;
                    break;
                case SDLK_END:
                    Sound::SoundManager::getInstance()->ff();
                    break;
                case SDLK_HOME:
                    Sound::SoundManager::getInstance()->rwd();
                    break;
                default:
                  break;
            }
        }

        void FileBrowser::onButtonDown(Event::Gamepad* event)
        {
            auto game = Game::Game::getInstance();

            switch (event->button())
            {
                case SDL_CONTROLLER_BUTTON_A:
                    if (_entries.at(_menuPos).is_directory()) cwd(_entries.at(_menuPos).path());
                    else
                    {
                        if (Sound::SoundManager::getInstance()->playing())
                            Sound::SoundManager::getInstance()->stop();
                        else
                            Sound::SoundManager::getInstance()->music(_entries.at(_menuPos).path());
                    }

                    break;
                case SDL_CONTROLLER_BUTTON_B:
                    cwd(_entries.at(0).path());
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                    _menuPos++;
                    if (_menuPos >= _entries.size()) _menuPos = 0;
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_UP:
                    _menuPos--;
                    if (_menuPos <0 ) _menuPos = _entries.size() - 1;
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                    _menuPos+=20;
                    if (_menuPos >= _entries.size()) _menuPos = 0;
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                    _menuPos-=20;
                    if (_menuPos <0 ) _menuPos = _entries.size() - 1;
                    break;
                case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
                    Sound::SoundManager::getInstance()->rwd();
                    break;
                case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
                    Sound::SoundManager::getInstance()->ff();
                    break;
                default:
                  break;
            }
        }

    }
}
