/** ****************************************************************
 * @file
 * \addtogroup Position
 * @{
 ******************************************************************
 */

#include <string>
#include "position/Position.h"

/**
 * Constructs a position with x = 0 and y = 0
 */
Position::Position() :
        x(0), y(0) {
}

/**
 * Constructs a position with x and y values.
 *
 * @param x the x value
 * @param y the y value
 */
Position::Position(int16_t x, int16_t y) :
        x(x), y(y) {
}

/**
 * Calculates the distance between this and other.
 *
 * @param other the other Position
 * @return the euclidian distance between this an other.
 */
float Position::distanceTo(const Position& other) const {
    return getVectorTo(other).getLength();
}

/**
 * Convert Position to a Vector.
 *
 * @return the position vector of this position
 */
Vector Position::asVectorFromOrigin() const {
    return Vector(x, y);
}

/**
 * Adding a vector moves the position appropriately.
 *
 * @param vector the vector describing the translation.
 * @return a Position that has been moved
 */
Position Position::operator +(const Vector& vector) const {
    return (asVectorFromOrigin() + vector).asPosition();
}

/**
 * Subtracting a vector moves the position appropriately.
 *
 * @param vector the vector describing the translation.
 * @return a Position that has been moved
 */
Position Position::operator -(const Vector& vector) const {
    return (asVectorFromOrigin() - vector).asPosition();
}

/**
 * Calculates the Vector from this Position to other.
 *
 * @param other the target Position
 * @return the Vector from this to other
 */
Vector Position::getVectorTo(const Position& other) const {
    return (other.asVectorFromOrigin() - asVectorFromOrigin());
}

/**
 * Are the two Positions equal
 *
 * @param other the Position to compare to
 *
 * @retval true Positions are equal
 * @retval false Positions are different
 */
bool Position::operator ==(const Position& other) const {
    return (x == other.x) && (y == other.y);
}

/**
 * Are the two Positions different
 *
 * @param other the Position to compare to
 *
 * @retval true Positions are different
 * @retval false Positions are equal
 */
bool Position::operator !=(const Position& other) const {
   return !(*this == other);
}

/** @} */
