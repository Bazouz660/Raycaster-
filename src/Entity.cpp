#include "Entity.hpp"
#include "ResourceManager.hpp"
#include "defines.hpp"

// Constructs a new Entity with the given starting position.
Entity::Entity(sf::Vector2f position)
    : m_position(position), m_velocity(0.0f, 0.0f)
{
    // Set the size and position of the hitbox.
    m_hitbox.setSize(sf::Vector2f(32.0f, 32.0f));
    m_hitbox.setOrigin(m_hitbox.getSize() * 0.5f);
    m_hitbox.setPosition(position);
    m_hitbox.setOutlineThickness(1);
    m_hitbox.setOutlineColor(sf::Color::Red);
    m_hitbox.setFillColor(sf::Color::Green);

    // Set the texture of the sprite.
    //m_sprite.setTexture(ResourceManager::getInstance().getTexture("wall"));
}

// Handles collisions with the tiles in the grid.
bool Entity::handleCollisions(Grid &grid)
{
    // Get the bounds of the entity.
    sf::FloatRect bounds = m_hitbox.getGlobalBounds();

    // Calculate the indices of the tiles that the entity is colliding with.
    int minRow = std::max(0, (int)(bounds.top / grid.getTileSize()));
    int maxRow = std::min((int)(grid.getHeight() - 1),
    (int)((bounds.top + bounds.height) / grid.getTileSize()));
    int minColumn = std::max(0, (int)(bounds.left / grid.getTileSize()));
    int maxColumn = std::min((int)(grid.getWidth() - 1),
    (int)((bounds.left + bounds.width) / grid.getTileSize()));

    // Check for collisions with the tiles in the grid.
    for (int row = minRow; row <= maxRow; ++row) {
        for (int column = minColumn; column <= maxColumn; ++column) {
            // Skip empty tiles.
            if (grid.getTile(column, row).type == 0)
                continue;

            // Get the bounds of the tile.
            sf::FloatRect tileBounds(column * grid.getTileSize(),
            row * grid.getTileSize(), grid.getTileSize(), grid.getTileSize());

            // Check for a collision with the tile.
            sf::FloatRect intersect;
            if (bounds.intersects(tileBounds, intersect)) {
                // Calculate the overlap between the entity and the tile.
                sf::Vector2f overlap(intersect.width, intersect.height);

                // Check which axis has the smallest overlap.
                if (overlap.x < overlap.y) {
                    // Resolve the collision along the x-axis.
                    if (bounds.left < tileBounds.left) {
                        // Entity is to the left of the tile.
                        m_position.x -= overlap.x;
                        m_velocity.x = 0;
                    } else {
                        // Entity is to the right of the tile.
                        m_position.x += overlap.x;
                        m_velocity.x = 0;
                    }
                } else {
                    // Resolve the collision along the y-axis.
                    if (bounds.top < tileBounds.top) {
                        // Entity is above the tile.
                        m_position.y -= overlap.y;
                        m_velocity.y = 0;
                    } else {
                        // Entity is below the tile.
                        m_position.y += overlap.y;
                        m_velocity.y = 0;
                    }
                }
            }
        }
    }
    return false;
}

// Returns the current position of the Entity.
sf::Vector2f Entity::getPosition() const
{
    return m_position;
}

// Sets the current position of the Entity.
void Entity::setPosition(sf::Vector2f position)
{
    m_position = position;
    m_hitbox.setPosition(position);
}

// Returns the current velocity of the Entity.
sf::Vector2f Entity::getVelocity() const
{
    return m_velocity;
}

// Sets the current velocity of the Entity.
void Entity::setVelocity(sf::Vector2f velocity)
{
    m_velocity = velocity;
}

// Updates the Entity for a single frame.
void Entity::update(float dt)
{
    // Update the position based on the velocity.
    m_position += m_velocity * dt;
    m_hitbox.setPosition(m_position);
    m_sprite.setPosition(m_position);
}

sf::Vector2f Entity::getDirection()
{
    m_direction = sf::Vector2f(std::cos(m_angle * RAD), std::sin(m_angle * RAD));
    return m_direction;
}

sf::Vector2f Entity::getDirection(float angle)
{
    m_direction = sf::Vector2f(std::cos(angle * RAD), std::sin(angle * RAD));
    return m_direction;
}

// Draws the Entity to the given window.
void Entity::draw(sf::RenderTarget &target)
{
    // Derived classes should override this function to draw themselves to the window.
    //window.draw(m_sprite);
    target.draw(m_hitbox);
    //target.draw(m_sprite);
}