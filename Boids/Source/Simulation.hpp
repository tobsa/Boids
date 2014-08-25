////////////////////////////////////////////////////////////////////////////////
// Filename: Simulation.hpp
// Author:   Tobias Savinainen
// Year:     2014
////////////////////////////////////////////////////////////////////////////////
#ifndef SIMULATION_HPP
#define SIMULATION_HPP

////////////////////////////////////////////////////////////////////////////////
// Header files
////////////////////////////////////////////////////////////////////////////////
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>
#include "Boid.hpp"

class Simulation : public sf::Drawable
{
public:
    Simulation(unsigned int x, unsigned int y, unsigned int width, unsigned int height);

    void addBoid(Boid& boid);
    void popBoid();

    void update(float dt);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    float getCohesion() const;
    float getSeparation() const;
    int getSeparationRadius() const;
    float getAlignment() const;
    float getMaxVelocity() const;
    int getBoids() const;
    float getMouseStrength() const;
    int getMouseRadius() const;
    bool getFollowMouse() const;
    bool getAvoidMouse() const;

    void setCohesion(float cohesion);
    void setSeparation(float separation);
    void setSeperationRadius(int seperationRadius);
    void setAlignment(float alignment);
    void setMaxVelocity(float maxVelocity);
    void setMouseStrength(float mouseStrength);
    void setMouseRadius(int mouseRadius);
    void setMousePosition(const sf::Vector2f& mousePosition);
    void setFollowMouse(bool followMouse);
    void setAvoidMouse(bool avoidMouse);
    void setDrawMouseRadius(bool drawMouseRadius);
    void setWrapEdge(bool wrapEdge);

private:

    sf::Vector2f applyCohesion(Boid& boid) const;
    sf::Vector2f applySeparation(Boid& boid) const;
    sf::Vector2f applyAlignment(Boid& boid) const;
    sf::Vector2f applyScreenBound(Boid& boid) const;
    sf::Vector2f applyMousePosition(Boid& boid) const;

    void applyWrapEdge(Boid& boid) const;
    void applyVelocityLimit(Boid& boid) const;

private:

    std::vector<Boid> m_boids;

    unsigned int m_x;
    unsigned int m_y;
    unsigned int m_width;
    unsigned int m_height;

    float        m_cohesion;
    float        m_separation;
    int          m_separationRadius;
    float        m_alignment;
    float        m_screenBound;
    float        m_maxVelocity;
    float        m_mouseStrength;
    int          m_mouseRadius;
    sf::Vector2f m_mousePosition;
    bool         m_followMouse;
    bool         m_avoidMouse;
    bool         m_drawMouseRadius;
    bool         m_wrapEdge;
};

#endif