/** ****************************************************************
 * @file
 * \addtogroup Position
 * @{
 ******************************************************************
 */

#ifndef POSITION_ANGLE_H
#define POSITION_ANGLE_H

/**
 * the number PI
 */
constexpr float PI = 3.1415926535;

/**
 * @brief Represents an Angle
 *
 * Angle has range from 0 to just under 2 PI.
 *
 * @author  Moritz Hoewer (Moritz.Hoewer@haw-hamburg.de)
 * @version 1.0
 * @date 16.03.2017
 */
class Angle {
private:
    float radian; ///< the value of the angle in radians
public:
	Angle();
    Angle(float radians);
    virtual ~Angle() = default;

	static Angle getFromDegrees(float degrees);

    float getAngleInDegrees() const;
    float getAngleInDegreesAround(float degreesCenter) const;
	float getAngleInRadian() const;
	float getAngleInRadianAround(float radianCenter) const;

	float sin() const;
	float cos() const;

	void add(const Angle& other);
	void subtract(const Angle& other);

	Angle& operator=(float radians);
	Angle operator-() const;

private:
	void setRadian(float radians);
};

Angle operator+(const Angle& left, const Angle& right);
Angle operator+(float left, const Angle& right);
Angle operator+(const Angle& left, float right);

Angle operator-(const Angle& left, const Angle& right);
Angle operator-(float left, const Angle& right);
Angle operator-(const Angle& left, float right);

Angle operator ""_deg(long double deg);
Angle operator ""_deg(unsigned long long deg);

#endif /* POSITION_ANGLE_H */
/** @} */
