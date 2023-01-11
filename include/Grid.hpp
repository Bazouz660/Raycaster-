#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

// A Tile represents a single square on the grid.
struct Tile {
    // The type of the tile.
    int type;

    // The color of the tile.
    sf::Color color;

    // The shape to use when drawing the tile.
    sf::RectangleShape shape;
};

// The Grid class represents the entire grid of tiles.
class Grid {
public:
    // Constructs a new grid with the given dimensions and tile size.
    Grid(int width, int height, int tileSize);
    Grid (std::vector<std::vector<int>> data, int tileSize);

    // Returns the width of the grid, in tiles.
    int getWidth() const;

    // Returns the height of the grid, in tiles.
    int getHeight() const;

    // Returns the size of a single tile, in pixels.
    int getTileSize() const;

    // Returns the tile at the given coordinates.
    Tile& getTile(int x, int y);

    // Sets the type and color of the tile at the given coordinates.
    void setTile(int x, int y, int type, sf::Color color);

    // Sets the position of the grid.
    void setPosition(float x, float y);

    // Draws the grid to the given window.
    void draw(sf::RenderTexture& window);

private:
    // The dimensions of the grid, in tiles.
    int m_width;
    int m_height;

    // The size of a single tile, in pixels.
    int m_tileSize;

    // The position of the grid.
    sf::Vector2f m_position;

    // The grid of tiles.
    std::vector<Tile> m_tiles;
};