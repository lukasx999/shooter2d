#include <print>

#include <gfx.h>

namespace {

[[nodiscard]] constexpr gfx::Vec snap_to_grid(gfx::Vec v, float scale) {
    int x = v.x / scale;
    int y = v.y / scale;
    return { x * scale, y * scale, };
}

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

    if (argc != 2) {
        std::println(stderr, "no path.");
        return EXIT_FAILURE;
    }

    const char* path = argv[1];

    gfx::Vec start;

    int width = 1600;
    int height = 900;

    gfx::Window window(width, height, "Spritesheet Editor", gfx::WindowFlags::Logging | gfx::WindowFlags::DisableCursor);
    gfx::Renderer renderer(window);

    gfx::Texture texture(path);

    renderer.draw([&] {

        renderer.clear_background(gfx::Color::black());

        float scale = 15;
        renderer.draw_texture(0, 0, texture.get_width() * scale, texture.get_height() * scale, texture);

        gfx::Vec mouse_pos = window.get_mouse_pos();
        auto snapped = snap_to_grid(mouse_pos, scale);
        renderer.draw_rectangle({ snapped.x, snapped.y, scale, scale }, gfx::Color::red());

        auto grid_color = gfx::Color::white().set_alpha(0x7f);
        draw_grid(renderer, grid_color, scale, width, height);

        if (window.get_mouse_button_state(gfx::MouseButton::Left).pressed()) {

            gfx::Vec a = snap_to_grid(mouse_pos, scale);
            gfx::Vec b = snap_to_grid(start, scale);

            float x = std::min(a.x, b.x);
            float y = std::min(a.y, b.y);
            float width = std::abs(a.x - b.x);
            float height = std::abs(a.y - b.y);

            gfx::Rect rect { x, y, width, height };
            renderer.draw_rectangle(rect, gfx::Color::blue().set_alpha(0x7f));

        } else {
            start = mouse_pos;
        }

        if (window.get_key_state(gfx::Key::Escape).pressed())
            window.close();

    });

}
