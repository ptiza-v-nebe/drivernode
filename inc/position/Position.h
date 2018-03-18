/** ****************************************************************
 * @file
 * \addtogroup Position
 * @{
 ******************************************************************
 */

#ifndef POSITION_POSITION_H
#define POSITION_POSITION_H

#include "position/Vector.h"
#include <cstdint>

class Vector;

/**
 * @brief A Position with X and Y coordinates
 *
 * @author  Moritz Hoewer (Moritz.Hoewer@haw-hamburg.de)
 * @version 1.0
 * @date    27.10.2016
 */
class Position {
public:
    uint16_t x; ///< the x Position
    uint16_t y; ///< the y Position

    Position();
    Position(uint16_t x, uint16_t y);
    virtual ~Position() = default;

    float distanceTo(const Position &other) const;
    Vector asVectorFromOrigin() const;

    Vector getVectorTo(const Position& other) const;
    Position operator-(const Vector& vector) const;
    Position operator+(const Vector& vector) const;

    bool operator==(const Position& other) const;
    bool operator!=(const Position& other) const;
};

 #endif /* POSITION_POSITION_H */
/** @} */
