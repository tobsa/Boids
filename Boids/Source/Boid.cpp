////////////////////////////////////////////////////////////////////////////////
// Filename: Boid.cpp
// Author:   Tobias Savinainen
// Year:     2014
////////////////////////////////////////////////////////////////////////////////
#include "Boid.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

Boid::Boid(const sf::Texture& texture, const sf::Vector2f& position) :
    m_sprite (texture)
{
    m_sprite.setPosition(position);
    m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2.f, m_sprite.getLocalBounds().height / 2.f);
}

const sf::Vector2f& Boid::getPosition() const
{
    return m_sprite.getPosition();
}

const sf::Vector2f& Boid::getVelocity() const
{
    return m_velocity;
}

void Boid::setPosition(const sf::Vector2f& position)
{
    m_sprite.setPosition(position);
}

void Boid::setVelocity(const sf::Vector2f& velocity)
{
    m_velocity = velocity;
}

void Boid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_sprite, states);
}