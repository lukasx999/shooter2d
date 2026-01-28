#pragma once

#include <format>
#include <print>

#include <gfx/gfx.h>

#define PRINT(value) std::println("{}: {}", #value, (value))

#define STRINGIFY(x) (static_cast<void>(x), #x)

#define ASSETS_PATH "./assets"

enum class Direction {
    North,
    NorthEast,
    East,
    SouthEast,
    South,
    SouthWest,
    West,
    NorthWest,
};

template <>
struct std::formatter<Direction> : std::formatter<std::string> {
    auto format(const Direction& direction, std::format_context& ctx) const {

        auto fmt = [&] {
            switch (direction) {
                using enum Direction;
                case North:     return STRINGIFY(North);
                case East:      return STRINGIFY(East);
                case South:     return STRINGIFY(South);
                case West:      return STRINGIFY(West);
                case NorthEast: return STRINGIFY(NorthEast);
                case SouthEast: return STRINGIFY(SouthEast);
                case SouthWest: return STRINGIFY(SouthWest);
                case NorthWest: return STRINGIFY(NorthWest);
            }
        }();

        return std::formatter<std::string>::format(fmt, ctx);
    }
};
