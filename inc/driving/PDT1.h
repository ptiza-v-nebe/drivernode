/** ****************************************************************
 * @file
 * \addtogroup Driving
 * @{
 ******************************************************************
 */

#ifndef DRIVING_PDT1_H
#define DRIVING_PDT1_H

/**
 * @brief A PDT1 controller
 *
 * @author  Max Zimmermann (max.zimmermann@haw-hamburg.de)
 * @version 1.0
 * @date    10.01.2018
 */
class PDT1 {
private:
    float Ts; ///< sampling time
    float ks, T; ///< system parameters
    float kr, Td; ///< controller parameters
    float u, y; ///< values for next calculation
    float a1, b0, b1; ///< controller values

public:
    PDT1(float ks, float T, float Ts);
    float calc(float u);
};

#endif /* DRIVING_PDT1_H */
/** @} */
