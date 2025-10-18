#include <gfx.hh>

// 2d twin stick shooter bullet hell game with procedurally generated levels

int main() {

    gfx::Window window(1600, 900, "my window", gfx::WindowFlags::None);
    gfx::Renderer rd(window);

    rd.draw([&]() {
        rd.clear_background(gfx::Color::black());
        rd.draw_rectangle(0, 0, 100, 100, 0_deg, gfx::Color::red());

        if (window.get_key_state(gfx::Key::Escape).pressed()) {
            window.close();
        }

    });

}
