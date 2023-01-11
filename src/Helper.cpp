#include <vector>
#include <string>
#include <sstream>
#include <math.h>
#include <SFML/Graphics.hpp>

// The split function splits a string on a given delimiter character and returns a vector of the resulting substrings.
std::vector<std::string> split(const std::string& str, char delimiter)
{
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        result.push_back(token);
    }
    return result;
}

sf::Vector2f normalize(const sf::Vector2f& v)
{
    float length = std::sqrt(v.x * v.x + v.y * v.y);
    if (length != 0)
        return sf::Vector2f(v.x / length, v.y / length);
    else
        return v;
}

sf::Color SmoothColor(sf::Color c1, sf::Color c2, double prog)
{
    float diff_r = c1.r - c2.r;
    float diff_g = c1.g - c2.g;
    float diff_b = c1.b - c2.b;
    float diff_a = c1.a - c2.a;
    sf::Color res;

    if (prog > 1)
        prog = 1;
    if (prog < 0)
        prog = 0;
    res.r = c1.r + (diff_r * prog);
    res.g = c1.g + (diff_g * prog);
    res.b = c1.b + (diff_b * prog);
    res.a = c1.a + (diff_a * prog);
    return res;
}

sf::Color darkenColor(sf::Color c1, double prog)
{
    sf::Color res = c1;

    if (prog > 1)
        prog = 1;
    if (prog < 0)
        prog = 0;
    res.r = (float)c1.r * prog;
    res.g = (float)c1.g * prog;
    res.b = (float)c1.b * prog;
    return res;
}

std::string removeExtension(const std::string& filename) {
    size_t lastdot = filename.find_last_of(".");
    if (lastdot == std::string::npos) return filename;
    return filename.substr(0, lastdot); 
}

sf::Text createText(const std::string& str, sf::Font& font, int fontSize)
{
    sf::Text text;
    text.setString(str);
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(fontSize);
    text.setOutlineThickness(1);
    text.setFont(font);
    return text;
}

