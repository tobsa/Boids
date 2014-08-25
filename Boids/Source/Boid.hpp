////////////////////////////////////////////////////////////////////////////////
// Filename: Boid.hpp
// Author:   Tobias Savinainen
// Year:     2014
////////////////////////////////////////////////////////////////////////////////
#ifndef BOID_HPP
#define BOID_HPP

////////////////////////////////////////////////////////////////////////////////
// Header files
////////////////////////////////////////////////////////////////////////////////
#include <SFML/Graphics/Sprite.hpp>

class Boid : public sf::Drawable
{
public:

    Boid(const sf::Texture& texture, const sf::Vector2f& position);

    const sf::Vector2f& getPosition() const;
    const sf::Vector2f& getVelocity() const;

    void setPosition(const sf::Vector2f& position);
    void setVelocity(const sf::Vector2f& velocity);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:

    sf::Sprite   m_sprite;
    sf::Vector2f m_velocity;
};

#endif