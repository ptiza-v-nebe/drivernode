/** ****************************************************************
 * @file
 * \addtogroup Position
 * @{
 ******************************************************************
 */

#ifndef POSITION_VECTOR_H
#define POSITION_VECTOR_H

#include "position/Angle.h"
#include "position/Position.h"

class Position;

/**
 * @brief A Vector with X and Y components
 *
 * @author  Moritz Hoewer (Moritz.Hoewer@haw-hamburg.de)
 * @version 1.0
 * @date    24.05.2017
 */
class Vector {
public:
    float x; ///< the x component
    float y; ///< the y component
public:
    Vector();
    Vector(float x, float y);
    Vector(const Angle& angle, float length);
    virtual ~Vector() = default;

    float getLength() const;
    Angle getPolarAngle() const;

    Position asPosition() const;

    Vector& flip();
    Vector& add(const Vector& other);
    Vector& subtract(const Vector& other);
    Vector& multiply(const float factor);
    Vector& divide(const float divisor);

    Vector& normalize();

    Vector operator-() const;
    Vector operator+(const Vector& other) const;
    Vector operator-(const Vector& other) const;
    Vector operator*(const float factor) const;
    Vector operator/(const float divisor) const;

    bool operator==(const Vector& other) const;
    bool operator!=(const Vector& other) const;
};

#endif /* POSITION_VECTOR_H */
/** @} */
