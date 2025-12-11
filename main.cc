#include <print>

#include <gfx.h>

#include "Game.h"

int main() {

    gfx::Window window(1600, 900, "shooter2d", gfx::WindowFlags::Resizable);
    gfx::Renderer renderer(window);

    Game game(renderer);

    renderer.draw([&] {

        renderer.clear_background(gfx::Color::black());

        double dt = renderer.get_frame_time();
        game.update(dt);
        game.draw(renderer);

    });

}
