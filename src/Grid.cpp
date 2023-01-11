#include <SFML/Graphics.hpp>
#include "Grid.hpp"
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "defines.hpp"
#include "Prototypes.hpp"


Grid::Grid(int width, int height, int tileSize)
    : m_width(width), m_height(height), m_tileSize(tileSize),
      m_tiles(width * height)
{
    // Initialize the shapes of the tiles.
    for (int y = 0; y < m_height; y++)
    {
        for (int x = 0; x < m_width; x++)
        {
            Tile &tile = getTile(x, y);
            tile.shape.setSize(sf::Vector2f(m_tileSize, m_tileSize));
            tile.shape.setPosition(x * m_tileSize, y * m_tileSize);
            tile.shape.setOutlineThickness(4);
            tile.shape.setOutlineColor(sf::Color::White);
            if (tile.type == 0) {
                tile.color = sf::Color::Black;
            } else if (tile.type == 1) {
                tile.color = sf::Color::Red;
            } else if (tile.type == 2) {
                tile.color = sf::Color::Green;
            } else if (tile.type == 3) {
                tile.color = sf::Color::Blue;
            } else {
                tile.color = sf::Color::White;
            }

            // Set the type of the edge tiles to 1.
            if (x == 0 || x == m_width - 1 || y == 0 || y == m_height - 1) {
                tile.type = 1;
                tile.color = sf::Color::Red;
            }
            tile.shape.setFillColor(tile.color);
        }
    }
}

Grid::Grid(std::vector<std::vector<int>> data, int tileSize)
    : m_width(data.at(0).size()), m_height(data.size()), m_tileSize(tileSize),
      m_tiles(data.size() * data.at(0).size())
{
    // Initialize the shapes of the tiles.
    for (int y = 0; y < m_height; y++)
    {
        for (int x = 0; x < m_width; x++)
        {
            Tile &tile = getTile(x, y);
            tile.shape.setSize(sf::Vector2f(m_tileSize, m_tileSize));
            tile.shape.setPosition(x * m_tileSize, y * m_tileSize);
            tile.shape.setOutlineThickness(4);
            tile.shape.setOutlineColor(sf::Color::White);
            tile.type = data.at(y).at(x);
            if (tile.type == 0) {
                tile.color = sf::Color::Black;
            } else if (tile.type == 1) {
                tile.color = sf::Color::Red;
            } else if (tile.type == 2) {
                tile.color = sf::Color::Green;
            } else if (tile.type == 3) {
                tile.color = sf::Color::Blue;
            } else {
                tile.color = sf::Color::White;
            }
            tile.shape.setFillColor(tile.color);
        }
    }
}

int Grid::getWidth() const
{
    return m_width;
}

int Grid::getHeight() const
{
    return m_height;
}

int Grid::getTileSize() const
{
    return m_tileSize;
}

Tile &Grid::getTile(int x, int y)
{
    return m_tiles[y * m_width + x];
}

void Grid::setTile(int x, int y, int type, sf::Color color)
{
    m_tiles[y * m_width + x].type = type;
    m_tiles[y * m_width + x].color = color;
}

void Grid::setPosition(float x, float y)
{
    m_position.x = x;
    m_position.y = y;

    // Update the positions of the shapes.
    for (int y = 0; y < m_height; y++)
    {
        for (int x = 0; x < m_width; x++)
        {
            Tile &tile = getTile(x, y);
            tile.shape.setPosition(x * m_tileSize + m_position.x, y * m_tileSize + m_position.y);
        }
    }
}

void Grid::draw(sf::RenderTexture &texture)
{
    for (int y = 0; y < m_height; y++)
    {
        for (int x = 0; x < m_width; x++)
        {
            Tile &tile = getTile(x, y);
            texture.draw(tile.shape);
        }
    }
}

// Converts a grid coordinate to a screen (world) coordinate.
sf::Vector2f gridToScreen(int row, int column, int tileSize)
{
    // Calculate the position in pixels based on the row and column indices and the size of a single tile.
    sf::Vector2f position = sf::Vector2f(column * tileSize, row * tileSize) + \
    sf::Vector2f(tileSize * 0.5, tileSize * 0.5);
    return position;
}

// Converts a screen (world) coordinate to a grid coordinate.
sf::Vector2i screenToGrid(sf::Vector2f position, int tileSize)
{
    // Calculate the row and column indices based on the position in pixels and the size of a single tile.
    sf::Vector2i grid((position.x - tileSize * 0.0) / tileSize, (position.y - tileSize * 0.0) / tileSize);
    return grid;
}

Grid loadGridFromFile(const std::string& filename)
{
    // Open the file.
    std::ifstream file(filename);

    // Check if the file is open.
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return Grid(10, 10, TILESIZE);
    }

    // Check if the file has the .map extension.
    if (filename.substr(filename.find_last_of(".") + 1) != "map") {
        std::cerr << "Invalid file extension: " << filename << std::endl;
        return Grid(10, 10, TILESIZE);
    }

    // Create a vector to hold the data for the grid.
    std::vector<std::vector<int>> data;

    // Read the data from the file.
    std::string line;
    while (std::getline(file, line)) {
        // Check if the line contains only numbers and commas.
        if (line.find_first_not_of("0123456789,") != std::string::npos) {
            std::cerr << "Invalid data in file: " << filename << std::endl;
            return Grid(10, 10, TILESIZE);
        }
        // Split the line on the ',' character.
        std::vector<std::string> cells = split(line, ',');

        // Check if the line has the correct number of cells.
        if (data.empty() || cells.size() == data[0].size()) {
            // Convert the cells to integers and add them to the data vector.
            std::vector<int> row;
            for (const auto& cell : cells) {
                row.push_back(std::stoi(cell));
            }
            data.push_back(row);
        } else {
            std::cerr << "Invalid number of cells in line: " << line << std::endl;
            return Grid(10, 10, TILESIZE);
        }
    }

    // Check if the data is a rectangle.
    if (data.empty() || !std::all_of(data.begin(), data.end(), [&](const auto& row) { return row.size() == data[0].size(); })) {
        std::cerr << "Invalid data in file: " << filename << std::endl;
        return Grid(10, 10, TILESIZE);
    }

    // Create a grid from the data.
    return Grid(data, TILESIZE);
}
