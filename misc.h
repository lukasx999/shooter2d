#pragma once

#include <gfx.h>

#define STRINGIFY(x) (static_cast<void>(x), #x)

struct CollisionEvent {
    gfx::Vec resolved_position;
};
