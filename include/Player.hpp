#include "Entity.hpp"
#include "Raycaster.hpp"

// The Player class represents the player character.
class Player : public Entity
{
    public:
    // Constructs a new Player with the given starting position.
    Player(sf::Vector2f position, Grid& grid);

    // Handles input from the player and updates the Player.
    void handleInput();

    // Updates the Player for a single frame.
    void update(float dt);

    float getFov();

    Raycaster& getRaycaster();

    private:
        // The raycaster used to cast rays.
        Raycaster m_raycaster;

        // The number of rays to cast.
        int m_numRays;

        // The angle range to cast the rays in.
        float m_fov;
};