#ifndef STATE_FILEBROWSER_H
#define STATE_FILEBROWSER_H

#include "../State/State.h"
#include <filesystem>
#include <string>
#include <vector>

namespace Ironhead
{
    namespace State
    {
        class FileBrowser : public State
        {
            public:
                FileBrowser();
                ~FileBrowser() override;

                void think(uint32_t delta) override;
                void init() override;
                void render() override;
                void onKeyDown(Event::Keyboard* event) override;
                void onButtonDown(Event::Gamepad* event) override;

            protected:
                float _scrollerY;
                float _scrollerX;
                int32_t _menuPos = 0;
                std::vector<std::filesystem::directory_entry> _entries;
                std::filesystem::path _cwd;
                void cwd(const std::filesystem::path& path);
        };
    }
}
#endif // STATE_FILEBROWSER_H
