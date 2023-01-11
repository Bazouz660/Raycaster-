#include "Grid.hpp"

void updateView(sf::RenderWindow* window)
{
    sf::View view = window->getDefaultView();
    view.setSize(window->getSize().x, window->getSize().y);
    view.setCenter(window->getSize().x / 2, window->getSize().y / 2);
    window->setView(view);
}

