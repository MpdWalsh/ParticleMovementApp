#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Perlin.h"
#include "cinder/Rand.h"
#include <list>

#include "Particle.h"

#define PERLIN_SCALE 0.076f  // arbitrarily chosen increment for Perlin noise
#define AIR_DENSITY  1.2f    // 1.2 kg/m^3 (15˚C at sea level)

using namespace ci;
using namespace ci::app;

class ParticleMovementApp : public App {
public:
    static void prepareSettings(Settings*);
    void setup() override;
    void mouseDrag(MouseEvent e) override;
    void update() override;
    void draw() override;

private:
    std::list<Particle> particles;
    Perlin wind;

    float getWindForce(const Particle&);
};

void ParticleMovementApp::prepareSettings(Settings *s) {
    s->setTitle("ParticleMovementApp");
    s->setHighDensityDisplayEnabled();
}

void ParticleMovementApp::setup() {
    gl::enableVerticalSync();
    gl::setMatricesWindow(getWindowSize());

    wind = Perlin();
}

void ParticleMovementApp::mouseDrag(MouseEvent e) {
    if(!e.isLeft()) return;

    vec2 particlePos ((float)e.getX(), (float)e.getY());
    float radius = randInt(5, 25);
    float mass = randFloat(0.1, 0.5);
    ColorA colour (randFloat(), randFloat(), randFloat(), 0.75f);

    particles.emplace_back(Particle(particlePos, radius, mass, colour));
}

// https://www.engineeringtoolbox.com/wind-load-d_1775.html
float ParticleMovementApp::getWindForce(const Particle &p) {
    float windSpeed = wind.noise(getElapsedFrames() * PERLIN_SCALE) / 100; // big divide to make a gentle breeze
    float area = p.getArea();

    return 0.5f * AIR_DENSITY * windSpeed * windSpeed * area;
}

bool isOffscreen(const Particle &p) {
    auto width = (float)getWindowWidth();
    auto height = (float)getWindowHeight();

    float radius = p.getRadius();
    vec2 p_pos = p.getPosition();
    float x = p_pos.x;
    float y = p_pos.y;

    return (x - radius > width || x + radius < 0 || y - radius > height || y + radius < 0);
}

void ParticleMovementApp::update() {
    // TODO forces:
    // BUOYANCY

    for (auto p = particles.begin(); p != particles.end(); ++p) {
        if (isOffscreen(*p)) {
            particles.erase(p);
            continue;
        }

        vec2 gravityForce (0, 0.1 * p->getMass()); // artificial gravity
        p->applyForce(gravityForce);

        vec2 windForce(getWindForce(*p), 0.0); // restrict wind to x-plane BECAUSE I SAID SO
        p->applyForce(windForce);

//        p->applyForce(buoyantForce);

        p->update();
    }
}

void ParticleMovementApp::draw() {
    gl::clear();

    for (auto p : particles) {
        p.draw();
    }
}

CINDER_APP(ParticleMovementApp, RendererGl, &ParticleMovementApp::prepareSettings)
