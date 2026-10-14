/*#############################################################
 *#                                                           #
 *#    Author: C. Distefano                                   #
 *#                                                           #
 *#    Date: 10 May 2021                                      #
 *#    Copied from antcc Geometry.hh                          #
 *#    Allow to compile mupage without antcc                  #
 *#    compiled only if mupage is not linked to antcc         #
 *#                                                           #
 *#############################################################
 */

#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

#include <fstream>
#include <iostream>

#ifdef ROOTSUPPORT
#include "TObject.h"
#endif
/* -------------------------------------------------------------------------- */
class Vec2D {
 public:
  double x, y;

 public:
  Vec2D() : x(0), y(0) {}                        ///< Constructor
  Vec2D(double xx, double yy) : x(xx), y(yy) {}  ///< Complete constructor
  ~Vec2D() {}                                    ///< destructor

  inline double dot(Vec2D& v) const {
    return (x * v.x + y * v.y);
  }  ///< dot product

  //! Euclidian distance- bug corrected on sept. 2001 - Th.S.
  inline double distance(Vec2D& v) const {
    return sqrt((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y));
  }
};

/* -------------------------------------------------------------------------- */
class Vec3D
#ifdef ROOTSUPPORT
    : public TObject
#endif
{
 public:
  double x, y, z;  // Vector components

 public:
  Vec3D()
      : x(0), y(0), z(0) {}  ///< Default constructor: initialises with (0,0,0)
  Vec3D(double xx, double yy, double zz);  ///<: x(xx), y(yy), z(zz) {}
  ~Vec3D() {}

  void set(double xx, double yy,
           double zz);  ///< desc: give vector value (xx,yy,zz)

  double dot(const Vec3D& v) const;  ///< dot-product with another vector
  double distance(
      const Vec3D& v) const;  ///< distance to another vector: length(this-v)
  double length() const;      ///< lenght of the vector

  Vec3D& normalize();           ///< scale vector so its length is 1
  Vec3D& operator*=(double a);  ///< multiply with a scalar
  Vec3D& operator*=(
      const Vec3D& v);          ///< Multiply elementwise by another vector
  Vec3D& operator/=(double a);  ///< divide by a scalar
  Vec3D& operator+=(const Vec3D& v);  ///< add another vector to this one
  Vec3D& operator-=(const Vec3D& v);  ///< subtract another vector from this one
  std::istream& Streamer(std::istream& in);         ///< read vector from stream
  std::ostream& Streamer(std::ostream& out) const;  ///< write vector to stream

#ifdef ROOTSUPPORT
  ClassDef(Vec3D, 1)
#endif
};

/* binary functions with Vec3D's */

inline Vec3D operator*(const Vec3D& v, double a) {
  Vec3D w = v;
  return w *= a;
}
inline Vec3D operator/(const Vec3D& v, double a) {
  Vec3D w = v;
  return w /= a;
}
inline Vec3D operator*(double a, const Vec3D& v) {
  Vec3D w = v;
  return w *= a;
}
inline Vec3D operator+(const Vec3D& v, const Vec3D& w) {
  Vec3D q = v;
  return q += w;
}
inline Vec3D operator-(const Vec3D& v, const Vec3D& w) {
  Vec3D q = v;
  return q -= w;
}
inline Vec3D operator*(const Vec3D& v, const Vec3D& w) {
  Vec3D q = v;
  return q *= w;
}
inline Vec3D crossproduct(const Vec3D& a, const Vec3D& b) {
  return Vec3D(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
               a.x * b.y - a.y * b.x);
}

/* inline function definitions for Vec3D */
inline double Vec3D::dot(const Vec3D& v) const {
  return (x * v.x + y * v.y + z * v.z);
}
inline double Vec3D::distance(const Vec3D& v) const {
  Vec3D w = v - *this;
  return w.length();
}
inline double Vec3D::length() const {
  double ll = x * x + y * y + z * z;
  if (ll <= 0) ll = 0;
  return sqrt(ll);
}

inline std::ostream& operator<<(std::ostream& out, const Vec3D& v) {
  return v.Streamer(out);
}
inline std::istream& operator>>(std::istream& in, Vec3D& v) {
  return v.Streamer(in);
}

inline void Vec3D::set(double xx, double yy, double zz) {
  x = xx;
  y = yy;
  z = zz;
}

inline Vec3D& Vec3D::operator*=(double a) {
  x *= a;
  y *= a;
  z *= a;
  return *this;
}
inline Vec3D& Vec3D::operator*=(const Vec3D& v) {
  x *= v.x;
  y *= v.y;
  z *= v.z;
  return *this;
}
inline Vec3D& Vec3D::operator/=(double a) {
  x /= a;
  y /= a;
  z /= a;
  return *this;
}
inline Vec3D& Vec3D::operator+=(const Vec3D& v) {
  x += v.x;
  y += v.y;
  z += v.z;
  return *this;
}
inline Vec3D& Vec3D::operator-=(const Vec3D& v) {
  x -= v.x;
  y -= v.y;
  z -= v.z;
  return *this;
}
inline std::istream& Vec3D::Streamer(std::istream& in) {
  return in >> x >> y >> z;
}
inline std::ostream& Vec3D::Streamer(std::ostream& out) const {
  return out << x << " " << y << " " << z;
}
inline Vec3D& Vec3D::normalize() {
  double n = length();
  x /= n;
  y /= n;
  z /= n;
  return *this;
}

#endif
