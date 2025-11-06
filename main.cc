#include <print>

#include <gfx.hh>

#include "game.hh"

int main() {

    gfx::Window window(1600, 900, "shooter2d", gfx::WindowFlags::None);
    gfx::Renderer renderer(window);

    Game game(renderer, window);

    renderer.draw([&] {

        renderer.clear_background(gfx::Color::black());

        double dt = renderer.get_frame_time();
        game.draw(renderer);
        game.update(dt);

    });

}
