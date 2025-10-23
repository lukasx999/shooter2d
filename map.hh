#pragma once

#include <fstream>
#include <algorithm>
#include <filesystem>
#include <ranges>

#include <gfx.hh>

#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/Layer.hpp>

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
        // TODO: use other layers than the first
        auto& layer = layers.front();
        auto layer_size = layer->getSize();
        auto& tiles = layer->getLayerAs<tmx::TileLayer>().getTiles();
        auto tile_size = m_map.getTileSize();


        auto color = m_map.getBackgroundColour();
        rd.draw_rectangle(0, 0, layer_size.x * tile_size.x, layer_size.y * tile_size.y, tmx_color_to_gfx_color(color));

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

    [[nodiscard]] static constexpr gfx::Color tmx_color_to_gfx_color(tmx::Colour color) {
        // HACK: use a cleaner approach
        return *reinterpret_cast<gfx::Color*>(&color);
    }

};
