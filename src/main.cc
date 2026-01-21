#include <gfx.h>

#include "Game.h"

// TODO: entity: move to state pattern and start_walk() end_walk() interface

int main() {

    gfx::Window window(1600, 900, "Game");

    Game game(window);

    window.draw_loop([&](gfx::Renderer& rd) {

        rd.clear_background(gfx::Color::black());

        game.draw(rd);
        game.update(window.get_frame_time());

    });

}
