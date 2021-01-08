//
// Created by matth on 1/6/2021.
//

#ifndef ARNOLDLOGSYSTEM_A_LOGNUM_H
#define ARNOLDLOGSYSTEM_A_LOGNUM_H

#include "ap_fixed.h"
#include "cmath"    // ill advised to use this
//#include "hls_math.h" // for some reason this causes a dumb issue

#define WBITS 5
#define INTBITS 2
#define FRACBITS (WBITS-INTBITS)
#define NBITS 1
#define BASE 2
#define TABLEROWS (1<<WBITS)  // that was a fucking clever way to get around variable array length not allowed issues
#define OFFSET (pow(2,WBITS-1))

typedef ap_fixed<WBITS,INTBITS,AP_RND_ZERO,AP_SAT,NBITS> fixedtype;
typedef ap_fixed<WBITS+FRACBITS,INTBITS+FRACBITS,AP_RND_ZERO,AP_SAT,NBITS> fixedtype2; // use to multiply & shift

// Get a mask that is "100000.000000"
static fixedtype MIN_VAL = fixedtype(-1*pow(BASE,INTBITS-1));

// Get the increment of our fixed nums "00000.00001"
static fixedtype RESOLUTION = fixedtype(1>>FRACBITS);

// Define delta tables & a dummy var to control their initialization
static fixedtype dPLUSvals[TABLEROWS];
static fixedtype dMINUSvals[TABLEROWS];

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
uint32_t getIndex(fixedtype);
void arrayInit();

#endif //ARNOLDLOGSYSTEM_A_LOGNUM_H
