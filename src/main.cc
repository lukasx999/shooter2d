#include <gfx.h>

#include "Game.h"

int main() {

    gfx::Window window(1600, 900, "Game", gfx::WindowFlags::Logging);
    gfx::Renderer rd(window);

    Game game(window);

    rd.draw([&] {

        rd.clear_background(gfx::Color::black());

        game.draw(rd);
        game.update(rd.get_frame_time());

    });

}
