/** ****************************************************************
 * @file
 * \addtogroup Position
 * @{
 ******************************************************************
 */

#include "position/Vector.h"
#include <cmath>

/**
 * Threshold to consider two floats as equal
 */
static constexpr float EPSILON = 1e-5;

/**
 * Constructs a null vector.
 */
Vector::Vector() :
        x(0), y(0) {
}

/**
 * Constructs a Vector.
 *
 * @param x the x component
 * @param y the y component
 */
Vector::Vector(float x, float y) :
        x(x), y(y) {
}

/**
 * Constructs a Vector using polar coordinates.
 *
 * @param angle  polar angle
 * @param length length of the vector
 */
Vector::Vector(const Angle& angle, float length) :
        x(length * angle.cos()), y(length * angle.sin()) {
}

/**
 * Returns the length of this Vector.
 *
 * Length refers to the euklidean norm of the Vector.
 *
 * @return the length
 */
float Vector::getLength() const {
    return std::sqrt(x * x + y * y);
}

/**
 * Returns the polar angle in the global coordinate system.
 *
 * @return the polar angle
 */
Angle Vector::getPolarAngle() const {
    /*if (std::abs(x) < EPSILON && std::abs(y) < EPSILON) {
        return Angle(0);
    }*/
    return Angle(std::atan2(y, x));
}

/**
 * Converts the Vector to the closest applicable position
 *
 * @return the closest applicable Position that this would be pointing to
 */
Position Vector::asPosition() const {
    return Position(std::round(x), std::round(y));
}

/**
 * Flip the Vector.
 *
 * This inverts the x and y components.
 *
 * @return reference to {@code this} for chaining.
 */
Vector& Vector::flip() {
    multiply(-1);
    return *this;
}

/**
 * Add another Vector to this
 *
 * @param other the Vector to add
 * @return reference to {@code this} for chaining.
 */
Vector& Vector::add(const Vector& other) {
    x += other.x;
    y += other.y;
    return *this;
}

/**
 * Subtract a Vector from this
 *
 * @param other the Vector to subtract
 * @return reference to {@code this} for chaining.
 */
Vector& Vector::subtract(const Vector& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

/**
 * Multiply this vector with a factor
 *
 * @param factor the factor to multiply this with
 * @return reference to {@code this} for chaining.
 */
Vector& Vector::multiply(const float factor) {
    x *= factor;
    y *= factor;
    return *this;
}

/**
 * Divide this vector by a divisor
 *
 * @param divisor the divisor to divide this by
 * @return reference to {@code this} for chaining.
 */
Vector& Vector::divide(const float divisor) {
    x /= divisor;
    y /= divisor;
    return *this;
}

/**
 * Normalize the Vector.
 *
 * @return reference to {@code this} for chaining.
 */
Vector& Vector::normalize() {
    divide(getLength());
    return *this;
}

/**
 * Flip the Vector.
 *
 * This inverts the x and y components.
 *
 * @return an independent copy of the Vector
 */
Vector Vector::operator -() const {
    Vector v = *this;
    return v.flip();
}

/**
 * Add another Vector to this
 *
 * @param other the Vector to add
 * @return an independent copy of the Vector
 */
Vector Vector::operator +(const Vector& other) const {
    Vector v = *this;
    return v.add(other);
}

/**
 * Subtract a Vector from this
 *
 * @param other the Vector to subtract
 * @return an independent copy of the Vector
 */
Vector Vector::operator -(const Vector& other) const {
    Vector v = *this;
    return v.subtract(other);
}

/**
 * Multiply this vector with a factor
 *
 * @param factor the factor to multiply this with
 * @return an independent copy of the Vector
 */
Vector Vector::operator *(const float factor) const {
    Vector v = *this;
    return v.multiply(factor);
}

/**
 * Divide this vector by a divisor
 *
 * @param divisor the divisor to divide this by
 * @return an independent copy of the Vector
 */
Vector Vector::operator /(const float divisor) const {
    Vector v = *this;
    return v.divide(divisor);
}

/**
 * Are the two Vectors equal
 *
 * @param other the Vector to compare to
 *
 * @retval true Vectors are equal
 * @retval false Vectors are different
 */
bool Vector::operator ==(const Vector& other) const {
    return std::abs(x - other.x) < EPSILON && std::abs(y - other.y) < EPSILON;
}

/**
 * Are the two Vectors different
 *
 * @param other the Vector to compare to
 *
 * @retval true Vectors are different
 * @retval false Vectors are equal
 */
bool Vector::operator !=(const Vector& other) const {
    return !(*this == other);
}

/** @} */
