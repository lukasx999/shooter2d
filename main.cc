#include <print>
#include <tuple>
#include <fstream>
#include <algorithm>
#include <ranges>
#include <filesystem>

#include <gfx.hh>

#include "player.hh"
#include "projectiles.hh"
#include "world.hh"

#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/Layer.hpp>

// 2d twin stick shooter bullet hell game with procedurally generated levels

namespace {

class Game {
    gfx::Renderer& m_renderer;
    gfx::Window& m_window;
    gfx::Font m_font;
    Player m_player;
    World m_world;
    ProjectileSystem m_projectiles;

    double m_last_shot = 0.0;
    static constexpr double m_shot_delay = 0.25;

public:
    explicit Game(gfx::Renderer& renderer, gfx::Window& window)
        : m_renderer(renderer)
        , m_window(window)
        , m_font(m_renderer.load_font("/usr/share/fonts/TTF/JetBrainsMonoNerdFont-Regular.ttf"))
        , m_player({ m_window.get_width() / 2.0f, m_window.get_height() / 2.0f })
        , m_world(m_window)
    { }

    void draw(gfx::Renderer& rd) const {

        rd.set_camera(m_player.get_position());
        rd.with_camera([&] {
            m_world.draw(rd);
            m_player.draw(rd);
            m_projectiles.draw(rd);
        });

        int size = 50;
        auto text = "Health: 0";
        int text_width = m_font.measure_text(text, size);
        rd.draw_text(m_window.get_width()/2.0 - text_width/2.0, size, size, text, m_font, gfx::Color::red());

    }

    void update(double dt) {
        m_projectiles.update(dt);


        bool can_shoot = m_window.get_time() > m_last_shot + m_shot_delay;

        if (m_window.get_mouse_button_state(gfx::MouseButton::Left).pressed() && can_shoot) {
            m_last_shot = m_window.get_time();
            auto ppos = m_player.get_position();
            auto mpos = m_window.get_mouse_pos();

            auto direction = (mpos - ppos).normalized();
            m_projectiles.add(ppos, direction, 500.0);

        }

        if (m_window.get_key_state(gfx::Key::W).pressed())
            m_player.move(Direction::North, dt);

        if (m_window.get_key_state(gfx::Key::S).pressed())
            m_player.move(Direction::South, dt);

        if (m_window.get_key_state(gfx::Key::D).pressed())
            m_player.move(Direction::East, dt);

        if (m_window.get_key_state(gfx::Key::A).pressed())
            m_player.move(Direction::West, dt);

        if (m_window.get_key_state(gfx::Key::Escape).pressed())
            m_window.close();

        m_world.resolve_collisions(m_player, dt);

    }

};

class Map {
    tmx::Map m_map;

public:
    explicit Map(const char* path) {

        bool success = m_map.load(path);
        if (!success) {
            std::println("failed to load map");
            exit(1);
        }

        // std::ifstream image(path);
        // std::vector<char> img(std::istreambuf_iterator<char>(image), {});
        // gfx::Texture tex(path);

    }

    void draw(gfx::Renderer& rd) const {

        auto& layers = m_map.getLayers();
        auto& layer = layers[0];
        auto layer_size = layer->getSize();
        auto& tiles = layer->getLayerAs<tmx::TileLayer>().getTiles();

        for (auto&& [i, tile] : tiles | std::views::enumerate) {
            uint32_t gid = tile.ID;

            int dest_x = i % layer_size.x;
            int dest_y = i / layer_size.x;

            if (gid == 0) {
                // TODO: empty, fill with bg color
                continue;
            }

            auto ts = find_tileset(gid);

            // auto tex_path = ts.getImagePath();
            // if (!std::filesystem::exists(tex_path)) {
            //     std::println("image doesnt exist");
            //     exit(1);
            // }

            auto tileset_size = ts.getTileSize();
            auto tileset_columns = ts.getColumnCount();
            auto tileset_rows = ts.getTileCount() / tileset_columns;
            uint32_t local_id = gid - ts.getFirstGID();
            int src_x = local_id % tileset_columns;
            int src_y = local_id / tileset_columns;

            auto tile_size = m_map.getTileSize();

            rd.draw_rectangle(dest_x*tile_size.x, dest_y*tile_size.y, tile_size.x, tile_size.y, gfx::Color::red());

        }
    }

private:
    [[nodiscard]] const tmx::Tileset& find_tileset(uint32_t gid) const {

        auto& tilesets = m_map.getTilesets();
        auto ts = std::ranges::find_if(tilesets, [&](const tmx::Tileset& ts) {
            return gid >= ts.getFirstGID() && gid <= ts.getLastGID();
        });

        assert(ts != tilesets.cend());
        return *ts;
    }

};

} // namespace

int main() {

    Map map("./assets/map.tmx");

    gfx::Window window(1600, 900, "shooter2d", gfx::WindowFlags::None);
    gfx::Renderer renderer(window);

    Game game(renderer, window);

    renderer.draw([&] {

        renderer.clear_background(gfx::Color::black());

        map.draw(renderer);

        // double dt = renderer.get_frame_time();
        // game.draw(renderer);
        // game.update(dt);

        if (window.get_key_state(gfx::Key::Escape).pressed())
            window.close();

    });

}
