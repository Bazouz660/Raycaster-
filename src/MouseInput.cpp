#include "SFML/Graphics.hpp"
#include "iostream"

void handleMouseInput(sf::RenderWindow& window)
{
    // Check if the left mouse button was pressed.
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        // Get the position of the mouse in screen coordinates.
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        // Print the mouse position to the console.
        std::cout << "Mouse position: (" << mousePos.x << ", " << mousePos.y << ")" << std::endl;
    }
}
