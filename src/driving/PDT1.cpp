/** ****************************************************************
 * @file
 * \addtogroup Driving
 * @{
 ******************************************************************
 */

#include <driving/PDT1.h>

/**
 * Constructs a PDT1 controller
 *
 * @param ks system gain value
 * @param T system delay time
 * @param Ts samping time
 */
PDT1::PDT1(float ks, float T, float Ts) {
    this->ks = ks;
    this->T = T;
    this->Ts = Ts;

    u = 0.0;
    y = 0.0;

    Td = T/10;
    kr = 1/(4*ks*Td);

    b0 = kr*(Ts+2*T)/(Ts+2*Td);
    b1 = kr*(Ts-2*T)/(Ts+2*Td);
    a1 = (Ts-2*Td)/(Ts+2*Td);
}

/**
 * Calculates the next control output
 *
 * @param u
 * @return controller output for the actor
 */
float PDT1::calc(float u) {
    float y = b0*u + b1*this->u - a1*this->y;
    this->u = u;
    this->y = y;

    return this->y;
}
