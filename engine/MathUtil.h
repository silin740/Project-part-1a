#ifndef MATHUTIL_H
#define MATHUTIL_H

#include <algorithm>
#include <cmath>
#include <iostream>

namespace CMPUT350 {

struct Point2D {
    float x, y;

    Point2D(float x = 0, float y = 0) : x(x), y(y) {}

    double Distance(const Point2D& other) const {
        return std::hypot(x - other.x, y - other.y);
    }
    Point2D operator+(const Point2D& other) const { return {x + other.x, y + other.y}; }
    Point2D operator+(const float& other) const { return {x + other, y + other}; }
    Point2D operator-(const Point2D& other) const { return {x - other.x, y - other.y}; }
    Point2D operator-(const float& other) const { return {x - other, y - other}; }
    Point2D operator*(const float& scalar) const { return {x * scalar, y * scalar}; }
    Point2D& operator+=(const float& scalar) { x += scalar; y += scalar; return *this; }
    Point2D& operator+=(const Point2D& other) { x += other.x; y += other.y; return *this; }
    Point2D& operator-=(const Point2D& other) { x -= other.x; y -= other.y; return *this; }
    bool operator==(const Point2D& other) const { return x == other.x && y == other.y; }
    Point2D& operator*=(const int& scalar) { x *= scalar; y *= scalar; return *this; }
    Point2D& operator/=(const int& scalar) {
        if (scalar != 0) { x /= scalar; y /= scalar; }
        return *this;
    }
    Point2D& operator/=(float scalar) {
        if (scalar != 0.f) { x /= scalar; y /= scalar; }
        return *this;
    }
    Point2D& operator/=(double scalar) {
        return *this /= static_cast<float>(scalar);
    }
    float operator*(const Point2D& other) const { return Dot(other); }
    float Dot(Point2D b) const { return x * b.x + y * b.y; }
    static float Dot(Point2D a, Point2D b) { return a.x * b.x + a.y * b.y; }
    static float Cross(Point2D a, Point2D b) { return a.x * b.y - a.y * b.x; }
    void Normalize() {
        const float length = static_cast<float>(Distance({0, 0}));
        if (length != 0.f) { x /= length; y /= length; }
    }
};

inline std::ostream& operator<<(std::ostream& os, const Point2D& p) {
    return os << "(" << p.x << ", " << p.y << ")";
}

inline Point2D operator*(float number, const Point2D& rhs) { return rhs * number; }

struct Line {
    Point2D p1, p2;

    Line(Point2D p1 = {0, 0}, Point2D p2 = {0, 0}) : p1(p1), p2(p2) {}
    Line(float x1, float y1, float x2, float y2) : p1(x1, y1), p2(x2, y2) {}

    float Length() const { return static_cast<float>(p1.Distance(p2)); }

    Point2D ClosestPoint(const Point2D& p) const {
        const Point2D segment = p2 - p1;
        const float lengthSquared = segment.Dot(segment);
        if (lengthSquared == 0.f) return p1;
        const float t = std::clamp((p - p1).Dot(segment) / lengthSquared, 0.f, 1.f);
        return p1 + segment * t;
    }

    bool Crosses(Line other, Point2D& crossingPoint) const {
        const Point2D r = p2 - p1;
        const Point2D s = other.p2 - other.p1;
        const float denominator = Point2D::Cross(r, s);
        if (std::fabs(denominator) < 0.00001f) return false;

        const Point2D difference = other.p1 - p1;
        const float t = Point2D::Cross(difference, s) / denominator;
        const float u = Point2D::Cross(difference, r) / denominator;
        if (t < 0.f || t > 1.f || u < 0.f || u > 1.f) return false;
        crossingPoint = p1 + r * t;
        return true;
    }
};

inline std::ostream& operator<<(std::ostream& os, const Line& line) {
    return os << "Line(" << line.p1 << " -> " << line.p2 << ")";
}

struct Circle {
    Point2D center;
    float radius;
    Circle(Point2D c = {0, 0}, float r = 0) : center(c), radius(r) {}
    Circle(float x, float y, float r) : center(x, y), radius(r) {}
};

struct Rect {
    Point2D topLeft;
    float width, height;

    Rect(float left = 0, float top = 0, float width = 0, float height = 0)
        : topLeft(left, top), width(width), height(height) {}
    Rect(Point2D tl, float w, float h) : topLeft(tl), width(w), height(h) {}
    Rect(Point2D p1, Point2D p2)
        : topLeft(std::min(p1.x, p2.x), std::min(p1.y, p2.y)),
          width(std::fabs(p1.x - p2.x)), height(std::fabs(p1.y - p2.y)) {}
    Rect(Point2D center, float radius)
        : topLeft(center.x - radius, center.y - radius), width(2 * radius), height(2 * radius) {}

    Rect& operator|=(const Rect& other) {
        const float left = std::min(topLeft.x, other.topLeft.x);
        const float top = std::min(topLeft.y, other.topLeft.y);
        const float right = std::max(topLeft.x + width, other.topLeft.x + other.width);
        const float bottom = std::max(topLeft.y + height, other.topLeft.y + other.height);
        topLeft = {left, top}; width = right - left; height = bottom - top;
        return *this;
    }
    Rect& operator|=(const Point2D& other) { return *this |= Rect(other, other); }
    Rect& operator|=(const Line& other) { *this |= other.p1; return *this |= other.p2; }
    Rect& operator&=(const Rect& other) {
        const float left = std::max(topLeft.x, other.topLeft.x);
        const float top = std::max(topLeft.y, other.topLeft.y);
        const float right = std::min(topLeft.x + width, other.topLeft.x + other.width);
        const float bottom = std::min(topLeft.y + height, other.topLeft.y + other.height);
        topLeft = {left, top}; width = std::max(0.f, right - left); height = std::max(0.f, bottom - top);
        return *this;
    }
    Rect& operator+=(const Point2D& other) { topLeft += other; return *this; }
    Rect operator+(const Point2D& other) const { Rect result = *this; result += other; return result; }
    void Inset(int inset) {
        topLeft += static_cast<float>(inset);
        width = std::max(0.f, width - 2.f * inset);
        height = std::max(0.f, height - 2.f * inset);
    }
    bool IsInside(const Point2D& p) const {
        return p.x >= topLeft.x && p.x <= topLeft.x + width &&
               p.y >= topLeft.y && p.y <= topLeft.y + height;
    }
};

inline std::ostream& operator<<(std::ostream& os, const Rect& rect) {
    return os << "Rect(" << rect.topLeft << ", " << rect.width << " x " << rect.height << ")";
}

}  // namespace CMPUT350

#endif  // MATHUTIL_H
