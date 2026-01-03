#pragma once

#include <algorithm>
#include <filesystem>
#include <ranges>
#include <span>
#include <print>

#include <gfx.h>

#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include <tmxlite/Object.hpp>

#include "GameObject.h"
#include "Entity.h"
#include "Emitter.h"
#include "misc.h"

class Map : public GameObject {
    tmx::Map m_map;
    std::unordered_map<const tmx::Tileset*, gfx::Texture> m_textures;

public:
    explicit Map(const char* path) {

        bool success = m_map.load(path);
        if (!success)
            throw std::runtime_error("failed to load map");

        load_tile_textures();
    }

    void update([[maybe_unused]] double dt) override { }
    void draw(gfx::Renderer& rd) const override;
    void resolve_collisions(const gfx::Window& window, std::span<std::reference_wrapper<Entity>> entities, double dt);

private:
    // get the scaling factor so the map fits the window
    [[nodiscard]] gfx::Vec get_map_scaling_factor(const gfx::Window& window) const;
    void for_each_tile(std::function<void(uint32_t gid, gfx::Vec dest)> fn) const;

    void load_tile_textures() {
        auto& tilesets = m_map.getTilesets();
        for (auto& tileset : tilesets) {

            auto tex_path = tileset.getImagePath();
            if (!std::filesystem::exists(tex_path)) {
                std::println("image doesnt exist");
                exit(1);
            }

            m_textures.emplace(&tileset, gfx::Texture(tex_path));

        }
    }

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
