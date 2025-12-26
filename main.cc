#include <gfx.h>

#include "Game.h"

int main() {

    gfx::Window window(1600, 900, "Game", gfx::WindowFlags::Logging);
    gfx::Renderer rd(window);

    Game game(rd);

    rd.draw([&] {

        rd.clear_background(gfx::Color::black());

        double dt = rd.get_frame_time();
        game.update(dt);
        game.draw(rd);

    });

}
