//
// Created by Michael on 2020-08-19.
//

#include "Particle.h"

Particle::Particle() :
position(vec2(0)), velocity(vec2(0)), acceleration(vec2(0)), radius(0), colour(ColorA(0,0,0,1))
{}

Particle::Particle(const vec2 &p, float r, const ColorA &c) {
    radius = r;
    position = p;
    velocity = vec2(0);
    acceleration = vec2(0);
    colour = c;
}

void Particle::update() {
    velocity += acceleration;
    position += velocity;

    acceleration = vec2(0);
}

void Particle::draw() {
    gl::color(colour);
    gl::drawSolidCircle(position, radius);
}

void Particle::applyForce(const vec2 &f) {
    acceleration += f;
}

vec2 Particle::getPosition() const {
    return vec2(position);
}

float Particle::getRadius() const {
    return radius;
}

float Particle::getArea() const {
    return M_PI * radius * radius;
}
