////////////////////////////////////////////////////////////////////////////////
// Filename: Simulation.cpp
// Author:   Tobias Savinainen
// Year:     2014
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header files
////////////////////////////////////////////////////////////////////////////////
#include "Simulation.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFX/Utility/Utility.hpp>

////////////////////////////////////////////////////////////////////////////////
// Methods
////////////////////////////////////////////////////////////////////////////////
static float getMagnitude(const sf::Vector2f& vector)
{
    return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}

Simulation::Simulation(unsigned int x, unsigned int y, unsigned int width, unsigned int height) :
    m_x (x),
    m_y (y),
    m_width (width),
    m_height (height)
{
    m_cohesion = 100.f;
    m_separation = 1.0f;
    m_separationRadius = 25;
    m_alignment = 100.f;
    m_screenBound = 200.f;
    m_maxVelocity = 400.f;
}

void Simulation::addBoid(Boid& boid)
{
    m_boids.push_back(boid);
}

void Simulation::popBoid()
{
    if(m_boids.size() > 2)
        m_boids.pop_back();
}

void Simulation::update(float dt)
{
    for(auto& boid : m_boids)
    {
        sf::Vector2f velocity;

        velocity += applyCohesion(boid)    / m_cohesion;
        velocity += applySeparation(boid)  * m_separation;
        velocity += applyAlignment(boid)   / m_alignment;
        velocity += applyScreenBound(boid) * m_screenBound;

        boid.setVelocity(boid.getVelocity() + velocity);
        applyVelocityLimit(boid);
        boid.setPosition(boid.getPosition() + boid.getVelocity() * dt);
    }
}
    
void Simulation::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for(const auto& boid : m_boids)
        target.draw(boid, states);
}

sf::Vector2f Simulation::applyCohesion(Boid& boid) const
{
    sf::Vector2f v;
    for(const auto& b : m_boids)
        if(&b != &boid) 
            v += b.getPosition();

    v /= m_boids.size() - 1.f;
    return v - boid.getPosition();
}

sf::Vector2f Simulation::applySeparation(Boid& boid) const
{
    sf::Vector2f v;
    for(const auto& b : m_boids)
        if(&b != &boid && getMagnitude(b.getPosition() - boid.getPosition()) < m_separationRadius)
            v -= b.getPosition() - boid.getPosition();

    return v;
}

sf::Vector2f Simulation::applyAlignment(Boid& boid) const
{
    sf::Vector2f v;
    for(const auto& b : m_boids)
        if(&b != &boid)
            v += b.getVelocity();

    v /= m_boids.size() - 1.f;
    return v - boid.getVelocity();
}

sf::Vector2f Simulation::applyScreenBound(Boid& boid) const
{
    sf::Vector2f v;
    if(boid.getPosition().x < m_x)
        v.x = 1.f;
    if(boid.getPosition().x > m_width)
        v.x -= 1.f;
    if(boid.getPosition().y < m_y)
        v.y = 1.f;
    if(boid.getPosition().y > m_height)
        v.y -= 1.f;

    return v;
}

void Simulation::applyVelocityLimit(Boid& boid) const
{
    float magnitude = getMagnitude(boid.getVelocity());
    if(magnitude > m_maxVelocity)
        boid.setVelocity(boid.getVelocity() / magnitude * m_maxVelocity);
}

float Simulation::getCohesion() const
{
    return m_cohesion;
}

float Simulation::getSeparation() const
{
    return m_separation;
}

int Simulation::getSeparationRadius() const
{
    return m_separationRadius;
}

float Simulation::getAlignment() const
{
    return m_alignment;
}

float Simulation::getMaxVelocity() const
{
    return m_maxVelocity;
}

void Simulation::setCohesion(float cohesion)
{
    m_cohesion = cohesion;
}

void Simulation::setSeparation(float separation)
{
    m_separation = separation;
}

void Simulation::setSeperationRadius(int seperationRadius)
{
    m_separationRadius = seperationRadius;
}

void Simulation::setAlignment(float alignment)
{
    m_alignment = alignment;
}

void Simulation::setMaxVelocity(float maxVelocity)
{
    m_maxVelocity = maxVelocity;
}