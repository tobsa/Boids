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
#include <SFML/Graphics/CircleShape.hpp>

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
    m_height (height),
    m_followMouse (false),
    m_avoidMouse (false),
    m_drawMouseRadius (false),
    m_wrapEdge (false)
{
    m_cohesion = 100.f;
    m_separation = 1.0f;
    m_separationRadius = 25;
    m_alignment = 100.f;
    m_screenBound = 200.f;
    m_maxVelocity = 400.f;
    m_mouseStrength = 1.f;
    m_mouseRadius   = 100;
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

        if(m_wrapEdge)
            applyWrapEdge(boid);
        else
            velocity += applyScreenBound(boid) * m_screenBound;

        if(m_followMouse)
            velocity += applyMousePosition(boid) * m_mouseStrength;
        if(m_avoidMouse)
            velocity -= applyMousePosition(boid) * m_mouseStrength;

        boid.setVelocity(boid.getVelocity() + velocity);
        applyVelocityLimit(boid);
        boid.setPosition(boid.getPosition() + boid.getVelocity() * dt);
    }
}
    
void Simulation::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(m_drawMouseRadius)
    {
        sf::CircleShape shape(static_cast<float>(m_mouseRadius));
        shape.setOutlineColor(sf::Color::White);
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineThickness(1.f);
        shape.setPosition(m_mousePosition);
        shape.setOrigin(shape.getLocalBounds().width / 2.f, shape.getLocalBounds().height / 2.f);

        target.draw(shape);
    }

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

void Simulation::applyWrapEdge(Boid& boid) const
{
    if(boid.getPosition().x < m_x)
        boid.setPosition({static_cast<float>(m_width), boid.getPosition().y});
    if(boid.getPosition().x > m_width)
        boid.setPosition({static_cast<float>(m_x), boid.getPosition().y});
    if(boid.getPosition().y < m_y)
        boid.setPosition({boid.getPosition().x, static_cast<float>(m_height)});
    if(boid.getPosition().y > m_height)
        boid.setPosition({boid.getPosition().x, static_cast<float>(m_y)});
}

sf::Vector2f Simulation::applyMousePosition(Boid& boid) const
{
    if(m_mousePosition.x < 200)
        return sf::Vector2f();

    if(sfx::getDistance(boid.getPosition(), m_mousePosition) < m_mouseRadius)
        return m_mousePosition - boid.getPosition();

    return sf::Vector2f();
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

int Simulation::getBoids() const
{
    return m_boids.size();
}

float Simulation::getMouseStrength() const
{
    return m_mouseStrength;
}

int Simulation::getMouseRadius() const
{
    return m_mouseRadius;
}

bool Simulation::getFollowMouse() const
{
    return m_followMouse;
}

bool Simulation::getAvoidMouse() const
{
    return m_avoidMouse;
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

void Simulation::setMouseStrength(float mouseStrength)
{
    m_mouseStrength = mouseStrength;
}

void Simulation::setMouseRadius(int mouseRadius)
{
    m_mouseRadius = mouseRadius;
}

void Simulation::setMousePosition(const sf::Vector2f& mousePosition)
{
    m_mousePosition = mousePosition;
}

void Simulation::setFollowMouse(bool followMouse)
{
    m_followMouse = followMouse;
}

void Simulation::setAvoidMouse(bool avoidMouse)
{
    m_avoidMouse = avoidMouse;
}

void Simulation::setDrawMouseRadius(bool drawMouseRadius)
{
    m_drawMouseRadius = drawMouseRadius;
}

void Simulation::setWrapEdge(bool wrapEdge)
{
    m_wrapEdge = wrapEdge;
}