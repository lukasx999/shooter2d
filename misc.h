#pragma once

#include <gfx.h>

#define STRINGIFY(x) (static_cast<void>(x), #x)

struct CollisionData {
    gfx::Vec resolved_position;
};
