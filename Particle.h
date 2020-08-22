//
// Created by Michael on 2020-08-19.
//

#ifndef PARTICLEMOVEMENTAPP_PARTICLE_H
#define PARTICLEMOVEMENTAPP_PARTICLE_H

#include "cinder/gl/gl.h"

using namespace ci;

class Particle {
public:
    Particle();
    Particle(const vec2&, float, const ColorA&);

    void update();
    void draw();
    void applyForce(const vec2&);

    [[nodiscard]] vec2 getPosition() const;
    [[nodiscard]] float getRadius() const;
    [[nodiscard]] float getArea() const;

private:
    ColorA colour;
    float radius;
    vec2 position;
    vec2 velocity;
    vec2 acceleration;

};

#endif //PARTICLEMOVEMENTAPP_PARTICLE_H
