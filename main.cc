#include <print>

#include <gfx.hh>

#include "game.hh"

// 2d twin stick shooter bullet hell game with procedurally generated levels

int main() {

    gfx::Window window(1600, 900, "shooter2d", gfx::WindowFlags::None);
    gfx::Renderer renderer(window);

    gfx::Texture tex("/home/lukas/Pictures/forest.jpg");
    gfx::Texture tex2(tex);
    // Game game(renderer, window);

    renderer.draw([&] {

        renderer.clear_background(gfx::Color::black());

        renderer.draw_texture(0, 0, 500, 500, tex2);
        // double dt = renderer.get_frame_time();
        // game.draw(renderer);
        // game.update(dt);

    });

}
