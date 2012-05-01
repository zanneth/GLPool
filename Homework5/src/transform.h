/*
 * transform.h
 *
 * Author: Charles Magahern <charles.magahern@arizona.edu>
 * Date Created: 02/20/2012
 */
 
#pragma once

#include <cmath>
#include <iostream>
#include <string>
#include "geometry.h"

#define DEGREES_TO_RADIANS(x) (x * (M_PI / 180.0f))

struct Transform {
    float matrix[16];
    
    Transform()
    {
        memset(matrix, 0, 16 * sizeof(float));
    }
    
    Transform(const float matrixArray[])
    {
        for (unsigned i = 0; i < 16; ++i) {
            matrix[i] = matrixArray[i];
        }
    }
    
    
    /* Custom Constructors */
    
    static Transform identityTransform()
    {
        float ida[16] = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };
        
        return ida;
    }
    
    static Transform rotationTransform(float degrees, float x, float y, float z)
    {
        float radians = DEGREES_TO_RADIANS(degrees);
        
        Vecf v = Vecf(x, y, z).normalize();
        float cos = cosf(radians);
        float cosp = 1.0f - cos;
        float sin = sinf(radians);
        
        float m[16] = {
            cos + cosp * v.x * v.x,
            cosp * v.x * v.y + v.z * sin,
            cosp * v.x * v.z - v.y * sin,
            0.0f,
            cosp * v.x * v.y - v.z * sin,
            cos + cosp * v.y * v.y,
            cosp * v.y * v.z + v.x * sin,
            0.0f,
            cosp * v.x * v.z + v.y * sin,
            cosp * v.y * v.z - v.x * sin,
            cos + cosp * v.z * v.z,
            0.0f,
            0.0f,
            0.0f,
            0.0f,
            1.0f
        };
        
        return Transform(m);
    }
    
    static Transform translationTransform(float tx, float ty, float tz)
    {
        Transform tr = identityTransform();
        tr.matrix[12] = tx;
        tr.matrix[13] = ty;
        tr.matrix[14] = tz;
        
        return tr;
    }
    
    static Transform scalingTransform(float sx, float sy, float sz)
    {
        Transform sc = identityTransform();
        sc.matrix[0] = sx;
        sc.matrix[5] = sy;
        sc.matrix[10] = sz;
        
        return sc;
    }
    
    
    /* Projection Transforms */
    
    static Transform frustum(float left, float right, float bottom, float top, float nearZ, float farZ)
    {
        Transform t = identityTransform();
        float deltaX = right - left;
        float deltaY = top - bottom;
        float deltaZ = farZ - nearZ;
        
        t.matrix[0] = 2.0f * nearZ / deltaX;
        t.matrix[1] = t.matrix[2] = t.matrix[3] = 0.0f;
        
        t.matrix[4] = t.matrix[6] = t.matrix[7] = 0.0f;
        t.matrix[5] = 2.0f * nearZ / deltaY;
        
        t.matrix[8] = (right + left) / deltaX;
        t.matrix[9] = (top + bottom) / deltaY;
        t.matrix[10] = -(nearZ + farZ) / deltaZ;
        t.matrix[11] = -1.0f;
        
        t.matrix[12] = t.matrix[14] = t.matrix[15] = 0.0f;
        t.matrix[14] = -2.0f * nearZ * farZ / deltaZ;
        
        return t;
    }
    
    static Transform perspective(float fovy, float aspect, float nearZ, float farZ)
    {
        float frustumHeight = std::tan(fovy / 360.0f * M_PI) * nearZ;
        float frustumWidth  = frustumHeight * aspect;
        
        return frustum(-frustumWidth, frustumWidth, -frustumHeight, frustumHeight, nearZ, farZ);
    }
    
    static Transform lookAt(Vecf eye, Vecf center, Vecf up)
    {
        Vecf n = (eye + center.negate()).normalize();
        Vecf u = (up.crossProduct(n)).normalize();
        Vecf v = n.crossProduct(u);
        
        float m[16] = {
            u.x, v.x, n.x, 0.0f,
            u.y, v.y, n.y, 0.0f,
            u.z, v.z, n.z, 0.0f,
            u.negate() * eye, v.negate() * eye, n.negate() * eye, 1.0f
        };
        
        return Transform(m);
    }
    
    
    /* Operators */
    
    Transform operator*(const Transform &other)
    {
        Transform t;
        for (unsigned i = 0; i < 4; ++i) {
            for (unsigned j = 0; j < 4; ++j) {
                for (unsigned k = 0; k < 4; ++k) {
                    t.matrix[i * 4 + j] += matrix[i * 4 + k] * other.matrix[k * 4 + j];
                }
            }
        }
        
        return t;
    }
    
    Transform& operator*=(const Transform &other)
    {
        return (*this = (*this * other));
    }
    
    Transform& operator=(const float matrixArray[])
    {
        *this = Transform(matrixArray);
        return *this;
    }
    
    // Applies a Transform to a Point
	template <typename T>
	Vec<T> operator*(const Vec<T> &point)
	{
		T result[4];
		T ptArray[4];
        
		ptArray[0] = point.x;
		ptArray[1] = point.y;
		ptArray[2] = point.z;
		ptArray[3] = 1.0;
        
		for (unsigned i = 0; i < 4; ++i) {
			T sum(0);
			for (unsigned j = 0; j < 4; ++j) {
				sum += -matrix[i * 4 + j] * ptArray[j];
			}
            
			result[i] = sum;
		}
        
		Vec<T> tPoint;
		tPoint.x = result[0] / result[3];
		tPoint.y = result[1] / result[3];
		tPoint.z = result[2] / result[3];
        
		return tPoint;
	}
    
    template <typename T>
    Rect<T> operator*(const Rect<T> &rect)
    {
        Rect<T> newRect;
        newRect.origin = (*this) * rect.origin;
        
        Vec<T> endPt = rect.origin + rect.size;
        Vec<T> tendPt = (*this) * endPt;
        newRect.size = tendPt - newRect.origin;
        
        return newRect;
    }
    
    
    /* Other Methods */
    
    std::string description()
    {
        std::stringstream buf;
        for (unsigned i = 0; i < 16; ++i) {
            if (i != 0 && i % 4 == 0) {
                buf << '\n';
            } else if (i != 0) {
                buf << '\t';
            }
            
            buf << matrix[i];
        }
        
        return buf.str();
    }
};
