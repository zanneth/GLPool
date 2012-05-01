/*
 * geometry.h
 *
 * Author: Charles Magahern <charles.magahern@arizona.edu>
 * Date Created: 02/20/2012
 */
 
#pragma once

#include <string>
#include <cstring>
#include <cstdio>
#include <complex>


/* Vector Structure */

template <typename T>
struct Vec {
	T x, y, z;
	
	Vec() : x(0), y(0), z(0) {}
	Vec(T x_, T y_) :
		x(x_), y(y_), z(0) {}
	Vec(T x_, T y_, T z_) :
		x(x_), y(y_), z(z_) {}
	
	template <typename U>
	Vec(const Vec<U> &copy)
	{
		x = (T) copy.x;
		y = (T) copy.y;
		z = (T) copy.z;
	}

	Vec operator+(const Vec &other) const
	{
		Vec sum;
		sum.x = x + other.x;
		sum.y = y + other.y;
		sum.z = z + other.z;

		return sum;
	}

	Vec operator-(const Vec &other) const
	{
		Vec diff;
		diff.x = x - other.x;
		diff.y = y - other.y;
		diff.z = z - other.z;

		return diff;
	}
    
    // Dot Product
    T operator*(const Vec &other) const
    {
        return x * other.x + y * other.y + z * other.z;
    }

	// Scalar Multiplication
	friend Vec operator*(const T &scalar, const Vec<T> &vector)
	{
		Vec result;
		result.x = vector.x * scalar;
		result.y = vector.y * scalar;
		result.z = vector.z * scalar;

		return result;
	}

	Vec& operator+=(const Vec &other)
	{
		return (*this = (*this + other));
	}

	Vec& operator-=(const Vec &other)
	{
		return (*this = (*this - other));
	}

	Vec& operator*=(const T &scalar)
	{
		return (*this = (scalar * (*this)));
	}

	std::string description()
	{
		char buffer[128];
		sprintf(buffer, "(%f, %f, %f)", (float) x, (float) y, (float) z);
		return std::string(buffer);
	}
    
    T distance(const Vec &other)
    {
        return std::sqrt((x - other.x) * (x - other.x) +
                         (y - other.y) * (y - other.y) +
                         (z - other.z) * (z - other.z));
    }
    
    T length()
    {
        return std::sqrt((x * x) + (y * y) + (z * z));
    }
    
    Vec normalize()
    {
        T length = this->length();
        if (length == 0) {
            return Vec();
        }
        
        T scale = 1.0f / length;
        return Vec(x * scale, y * scale, z * scale);
    }
    
    Vec negate()
    {
        return Vec(-x, -y, -z);
    }
    
    Vec crossProduct(const Vec &other)
    {
        return Vec(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }
};
typedef Vec<float> Vecf;


/* Rect/Frame Structure */

template <typename T>
struct Rect {
	Vec<T> origin;
	Vec<T> size;
	
	Rect() {}
	Rect(Vec<T> origin_, Vec<T> size_) :
		origin(origin_), size(size_) {}
	Rect(Vec<T> origin_, T width_, T height_) :
		origin(origin_),
		size(Vec<T>(width_, height_)) {}
	Rect(T x_, T y_, T width_, T height_):
		origin(Vec<T>(x_, y_)),
		size(Vec<T>(width_, height_)) {}

	bool containsPoint(Vec<T> pt)
	{
		bool contains = pt.x >= origin.x && pt.x <= origin.x + size.x
					&& pt.y >= origin.y && pt.y <= origin.y + size.y;
		return contains;
	}
};
typedef Rect<float> Rectf;


/* Color Structure */

template <typename T>
struct Color {
	T red, green, blue, alpha;

	Color() : red(0), green(0), blue(0), alpha(0) {}
	Color(T red_, T green_, T blue_, T alpha_) :
		red(red_), green(green_), blue(blue_), alpha(alpha_) {}
	Color(T red_, T green_, T blue_) :
		red(red_), green(green_), blue(blue_), alpha(static_cast<T>(1.0)) {}
    
    // Aggregating colors
    Color operator+(const Color &other) const
    {
        Color result;
        result.red = red + other.red > 1.0 ? 1.0 : red + other.red;
        result.blue = blue + other.blue > 1.0 ? 1.0 : blue + other.blue;
        result.green = green + other.green > 1.0 ? 1.0 : green + other.green;
        result.alpha = alpha + other.alpha > 1.0 ? 1.0 : alpha + other.alpha;
        
        return result;
    }
    
    Color& operator+=(const Color &other)
    {
        return (*this = (*this + other));
    }
    
    bool operator==(const Color &other)
    {
        return red == other.red
            && green == other.green
            && blue == other.blue
            && alpha == other.alpha;
    }
};
typedef Color<unsigned char> Colorb;
typedef Color<float> Colorf;


/* Various Shallow Transformations */

template <typename T>
struct Rotation {
    T angle;
    T x, y, z;
    
    Rotation() :
        angle(0), x(0), y(0), z(0) {}
    Rotation(T angle_, T x_, T y_, T z_) :
        angle(angle_), x(x_), y(y_), z(z_) {}
};
typedef Rotation<float> Rotationf;
typedef Rotation<double> Rotationd;

template <typename T>
struct Translation {
    T x, y, z;
    
    Translation() :
        x(0), y(0), z(0) {}
    Translation(T x_, T y_, T z_) :
        x(x_), y(y_), z(z_) {}
};
typedef Translation<float> Translationf;
typedef Translation<double> Translationd;
