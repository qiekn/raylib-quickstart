#pragma once

#include <cmath>
#include <iostream>

class Vector2Int {
public:
  int x, y;

  Vector2Int() : x(0), y(0) {}
  Vector2Int(int x, int y) : x(x), y(y) {}
  Vector2Int(float x, float y) : x(static_cast<int>(x)), y(static_cast<int>(y)) {}
  Vector2Int(const Vector2Int& other) : x(other.x), y(other.y) {}

  // 赋值运算符
  Vector2Int& operator=(const Vector2Int& other) {
    if (this != &other) {
      x = other.x;
      y = other.y;
    }
    return *this;
  }

  // 算术运算符
  Vector2Int operator+(const Vector2Int& other) const { return Vector2Int(x + other.x, y + other.y); }

  Vector2Int operator-(const Vector2Int& other) const { return Vector2Int(x - other.x, y - other.y); }

  Vector2Int operator*(int scalar) const { return Vector2Int(x * scalar, y * scalar); }

  Vector2Int operator/(int scalar) const {
    if (scalar == 0) {
      throw std::invalid_argument("Division by zero");
    }
    return Vector2Int(x / scalar, y / scalar);
  }

  // 复合赋值运算符
  Vector2Int& operator+=(const Vector2Int& other) {
    x += other.x;
    y += other.y;
    return *this;
  }

  Vector2Int& operator-=(const Vector2Int& other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  Vector2Int& operator*=(int scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
  }

  Vector2Int& operator/=(int scalar) {
    if (scalar == 0) {
      throw std::invalid_argument("Division by zero");
    }
    x /= scalar;
    y /= scalar;
    return *this;
  }

  // 比较运算符
  bool operator==(const Vector2Int& other) const { return x == other.x && y == other.y; }

  bool operator!=(const Vector2Int& other) const { return !(*this == other); }

  // 一元运算符
  Vector2Int operator-() const { return Vector2Int(-x, -y); }

  // 向量操作
  int dot(const Vector2Int& other) const { return x * other.x + y * other.y; }

  double Magnitude() const { return std::sqrt(x * x + y * y); }

  int MagnitudeSquared() const { return x * x + y * y; }

  double Distance(const Vector2Int& other) const { return (*this - other).Magnitude(); }

  int DistanceSquared(const Vector2Int& other) const { return (*this - other).MagnitudeSquared(); }

  // 设置方法
  void Set(int newX, int newY) {
    x = newX;
    y = newY;
  }

  void Reset() {
    x = 0;
    y = 0;
  }

  // 静态方法 - 常用向量
  static Vector2Int zero() { return Vector2Int(0, 0); }
  static Vector2Int one() { return Vector2Int(1, 1); }
  static Vector2Int up() { return Vector2Int(0, 1); }
  static Vector2Int down() { return Vector2Int(0, -1); }
  static Vector2Int left() { return Vector2Int(-1, 0); }
  static Vector2Int right() { return Vector2Int(1, 0); }

  // 输出流运算符
  friend std::ostream& operator<<(std::ostream& os, const Vector2Int& v) {
    os << "(" << v.x << ", " << v.y << ")";
    return os;
  }
};

// 标量在左侧的乘法
inline Vector2Int operator*(int scalar, const Vector2Int& v) { return v * scalar; }
