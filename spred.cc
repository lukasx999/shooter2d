#include <print>

#include <gfx.h>

// "use" the value by casting to void, so language servers will
// rename the argument of the macro
#define STRINGIFY(x) (static_cast<void>(x), #x)

namespace {

void draw_button(gfx::Renderer& rd, const gfx::Font& font) {
    const char* button_text = "save";
    gfx::Vec button_pos { 0, 500 };
    int fontsize = 50;
    int w = font.measure_text(button_text, fontsize);
    rd.draw_rectangle(button_pos, w, fontsize, gfx::Color::grey());
    rd.draw_text(button_pos, fontsize, button_text, font, gfx::Color::white());
}

[[nodiscard]] constexpr gfx::Vec snap_to_grid(gfx::Vec v, float scale) {
    int x = v.x / scale;
    int y = v.y / scale;
    return { x * scale, y * scale, };
}

[[nodiscard]] gfx::Rect get_selection_rectangle(gfx::Vec a, gfx::Vec b, float scale) {

    a = snap_to_grid(a, scale);
    b = snap_to_grid(b, scale);

    float x = std::min(a.x, b.x);
    float y = std::min(a.y, b.y);
    float width = std::abs(a.x - b.x);
    float height = std::abs(a.y - b.y);

    return { x, y, width, height };
}

void draw_grid(gfx::Renderer& rd, gfx::Color grid_color, int size, int width, int height) {
    for (int x = 0; x < size * width; x += size) {
        rd.draw_line(x, 0, x, height, grid_color);
    }

    for (int y = 0; y < size * height; y += size) {
        rd.draw_line(0, y, width, y, grid_color);
    }
}

void draw_cursor(gfx::Renderer& rd, float grid_size) {
    gfx::Vec mouse_pos = rd.get_window().get_mouse_pos();
    auto snapped = snap_to_grid(mouse_pos, grid_size);
    rd.draw_rectangle({ snapped.x, snapped.y, grid_size, grid_size }, gfx::Color::red());
}

enum class State {
    Idle,
    Selecting,
    Editing,
};

[[nodiscard]] constexpr const char* stringify_state(State state) {
    switch (state) {
        using enum State;
        case Idle:      return STRINGIFY(Idle);
        case Selecting: return STRINGIFY(Selecting);
        case Editing:   return STRINGIFY(Editing);
    }
}

} // namespace

int main(int argc, char** argv) {

    if (argc != 2) {
        std::println(stderr, "no path.");
        return EXIT_FAILURE;
    }

    const char* path = argv[1];


    int width = 1600;
    int height = 900;

    gfx::Window window(width, height, "Spritesheet Editor", gfx::WindowFlags::Logging | gfx::WindowFlags::DisableCursor);
    gfx::Renderer renderer(window);

    auto font = renderer.load_font("/usr/share/fonts/Adwaita/AdwaitaMono-Regular.ttf");

    gfx::Texture texture(path);

    State state = State::Idle;
    std::vector<gfx::Rect> selections;
    gfx::Vec start;
    bool pressed_old = false;

    renderer.draw([&] {

        renderer.clear_background(gfx::Color::black());

        float grid_size = 15;
        renderer.draw_texture(0, 0, texture.get_width() * grid_size, texture.get_height() * grid_size, texture);

        renderer.draw_text(0, 0, 50, stringify_state(state), font, gfx::Color::white());

        gfx::Vec mouse_pos = window.get_mouse_pos();

        draw_grid(renderer, gfx::Color::white().set_alpha(0x7f), grid_size, width, height);

        bool pressed = window.get_mouse_button_state(gfx::MouseButton::Left).pressed();

        if (!pressed && pressed_old) {
            auto rect = get_selection_rectangle(mouse_pos, start, grid_size);
            selections.push_back(rect);
        }

        if (pressed) {
            state = State::Selecting;

        } else {
            state = State::Idle;
        }

        for (auto& selection : selections) {
            renderer.draw_rectangle(selection, gfx::Color::orange().set_alpha(0x7f));
        }

        switch (state) {
            using enum State;

            case Idle: {
                start = mouse_pos;
                draw_cursor(renderer, grid_size);
            } break;

            case Selecting: {
                auto rect = get_selection_rectangle(mouse_pos, start, grid_size);
                renderer.draw_rectangle(rect, gfx::Color::blue().set_alpha(0x7f));

            } break;

            case Editing: {

            } break;
        }

        if (window.get_key_state(gfx::Key::Escape).pressed())
            window.close();

        draw_button(renderer, font);

        pressed_old = pressed;

    });

}
