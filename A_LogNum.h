//
// Created by matth on 1/6/2021.
//

#ifndef ARNOLDLOGSYSTEM_A_LOGNUM_H
#define ARNOLDLOGSYSTEM_A_LOGNUM_H

#define WBITS 8
#define INTBITS 4
#define FRACBITS (WBITS-INTBITS)
#define NBITS 1
#define BASE 2

#include "ap_fixed.h"
#include "cmath"    // ill advised to use this
//#include "hls_math.h" // for some reason this causes a dumb issue

typedef ap_fixed<WBITS,INTBITS,AP_RND_ZERO,AP_SAT,NBITS> fixedtype;

// Get a mask that is "100000.000000"
static fixedtype MIN_VAL = fixedtype(-1*pow(BASE,INTBITS-1));

class A_LogNum {
public:
    // Default ctor, copy ctor, component-wise ctor
    A_LogNum();
    A_LogNum(const A_LogNum &original);
    A_LogNum(bool sign, fixedtype);

    // basic operations
    static A_LogNum addReals(A_LogNum v1, A_LogNum v2);
    static A_LogNum multiplyReals (A_LogNum v1, A_LogNum v2);
    void MAC(A_LogNum A, A_LogNum B);

    // access to components
    bool getSignBit() const;
    double getLogval();

private:
    // defining fields
    bool signBit;
    fixedtype logval;
};


// Delta function operating on a fixed type
fixedtype deltaPlus(fixedtype);
fixedtype deltaMinus(fixedtype);

#endif //ARNOLDLOGSYSTEM_A_LOGNUM_H
