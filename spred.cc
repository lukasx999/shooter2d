#include <print>

#include <gfx.h>

namespace {

void draw_grid(gfx::Renderer& rd, gfx::Color grid_color, int size, int width, int height) {
    for (int x = 0; x < size * width; x += size) {
        rd.draw_line(x, 0, x, height, grid_color);
    }

    for (int y = 0; y < size * height; y += size) {
        rd.draw_line(0, y, width, y, grid_color);
    }
}

} // namespace

int main(int argc, char** argv) {

    // if (argc != 2) {
    //     std::println(stderr, "no path.");
    //     return EXIT_FAILURE;
    // }
    //
    // const char* path = argv[1];


    int width = 1600;
    int height = 900;

    gfx::Window window(width, height, "Spritesheet Editor", gfx::WindowFlags::Logging | gfx::WindowFlags::DisableCursor);
    gfx::Renderer renderer(window);

    gfx::Texture texture("./assets/Cute_Fantasy_Free/Player/Player.png");

    renderer.draw([&] {

        renderer.clear_background(gfx::Color::black());

        float scale = 15;
        renderer.draw_texture(0, 0, texture.get_width() * scale, texture.get_height() * scale, texture);

        gfx::Vec mouse_pos = window.get_mouse_pos();
        int x = mouse_pos.x / scale;
        int y = mouse_pos.y / scale;
        renderer.draw_rectangle({ static_cast<float>(x) * scale, static_cast<float>(y) * scale, scale, scale }, gfx::Color::red());

        auto grid_color = gfx::Color::white().set_alpha(0x7f);
        draw_grid(renderer, grid_color, scale, width, height);


        if (window.get_key_state(gfx::Key::Escape).pressed())
            window.close();

    });

}
