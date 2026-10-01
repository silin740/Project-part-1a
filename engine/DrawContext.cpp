#include "DrawContext.h"

#include <cmath>

namespace CMPUT350 {

DrawContext::DrawContext(std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<sf::Font> font)
    : mWindow(window), mFont(font) {}

void DrawContext::DrawCenteredText(const std::string &text, int pixelSize, Point2D p, RGBColor c) {
    sf::Text drawable(*mFont, text, pixelSize);
    drawable.setFillColor(sf::Color(c.r, c.g, c.b));

    const sf::FloatRect bounds = drawable.getLocalBounds();
    drawable.setOrigin({bounds.position.x + bounds.size.x / 2.f,
                        bounds.position.y + bounds.size.y / 2.f});
    drawable.setPosition({p.x, p.y});

    mWindow->draw(drawable);
}

void DrawContext::DrawText(const std::string &text, int pixelSize, Point2D p, RGBColor c) {
    sf::Text drawable(*mFont, text, pixelSize);
    drawable.setFillColor(sf::Color(c.r, c.g, c.b));
    drawable.setPosition({p.x, p.y});

    mWindow->draw(drawable);
}

void DrawContext::DrawCircle(Point2D p, float radius, RGBColor c) {
    sf::CircleShape circle(radius);
    circle.setFillColor(sf::Color(c.r, c.g, c.b));
    circle.setOrigin({radius, radius});
    circle.setPosition({p.x, p.y});

    mWindow->draw(circle);
}

void DrawContext::DrawRect(Rect r, RGBColor c) {
    sf::RectangleShape rectangle({r.width, r.height});
    rectangle.setPosition({r.topLeft.x, r.topLeft.y});
    rectangle.setFillColor(sf::Color(c.r, c.g, c.b));
    mWindow->draw(rectangle);
}

void DrawContext::FrameRect(Rect r, float width, RGBColor c) {
    sf::RectangleShape rectangle({r.width, r.height});
    rectangle.setPosition({r.topLeft.x, r.topLeft.y});
    rectangle.setFillColor(sf::Color::Transparent);
    rectangle.setOutlineColor(sf::Color(c.r, c.g, c.b));
    rectangle.setOutlineThickness(width);
    mWindow->draw(rectangle);
}

/**
 * @brief Draws a line between two points with a specified width and color.
 *
 * @param from The starting point of the line (Point2D).
 * @param to The ending point of the line (Point2D).
 * @param width The width of the line in pixels.
 * @param c The color of the line, specified as an RGBColor object.
 *
 * This function calculates the distance and angle between the two points
 * and uses a polygone shape to represent the line. The line is drawn
 * relative to the world offset and rendered onto the associated window.
 */
void DrawContext::DrawLine(Point2D from, Point2D to, float width, RGBColor c) {
    const Point2D delta = to - from;
    const float length = static_cast<float>(from.Distance(to));
    if (length == 0.f) {
        DrawCircle(from, width / 2.f, c);
        return;
    }

    const Point2D normal(-delta.y / length * width / 2.f,
                         delta.x / length * width / 2.f);
    sf::ConvexShape line(4);
    line.setPoint(0, {from.x + normal.x, from.y + normal.y});
    line.setPoint(1, {to.x + normal.x, to.y + normal.y});
    line.setPoint(2, {to.x - normal.x, to.y - normal.y});
    line.setPoint(3, {from.x - normal.x, from.y - normal.y});
    line.setFillColor(sf::Color(c.r, c.g, c.b));
    mWindow->draw(line);
}

int DrawContext::GetWindowWidth() { return mWindow->getSize().x; }

int DrawContext::GetWindowHeight() { return mWindow->getSize().y; }

}  // namespace CMPUT350
