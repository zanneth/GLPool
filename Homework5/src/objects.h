/*
 * objects.h
 *
 * Author: Charles Magahern <charles.magahern@arizona.edu>
 * Date Created: 03/18/2012
 */

#pragma once

#include "geometry.h"

struct Camera {
    Vecf position;
    Vecf look;
    float fov;
};

struct Light : public Camera {
    Colorf color;
    Vecf velocity;
};