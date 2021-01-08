//
// Created by matth on 1/6/2021.
//

#include "A_LogNum.h"
// with no better information, generate for x(real) = 1
// so sgn(x) = 1, log2(x) = 0
A_LogNum::A_LogNum(): signBit(true), logval(0) {}

// copy ctor...needed for MAC
A_LogNum::A_LogNum(const A_LogNum &original): signBit(original.getSignBit()),logval(original.logval) {}

// ctor to instantiate for a specific value (someone else's job to convert)
A_LogNum::A_LogNum(bool sign, fixedtype fixedNum):
        signBit(sign), logval(fixedNum){}

A_LogNum A_LogNum::addReals(A_LogNum v1, A_LogNum v2) {
    A_LogNum result;

    result.signBit = v1.logval > v2.logval ? v1.signBit : v2.signBit;

    fixedtype d = v1.logval > v2.logval ?
                  v1.logval - v2.logval : v2.logval - v1.logval;

    fixedtype larger = v1.logval > v2.logval ? v1.logval : v2.logval;

    result.logval  = v1.signBit == v2.signBit ?
                     (larger + deltaPlus(d)): (larger + deltaMinus(d));

    return result;
}

A_LogNum A_LogNum::multiplyReals(A_LogNum v1, A_LogNum v2) {
    return A_LogNum(! (v1.signBit xor v2.signBit), v1.logval + v2.logval);
}

/*
 * Top level function for synthesis - array init lives here for this reason
 */
void A_LogNum::MAC(A_LogNum A, A_LogNum B) {
    arrayInit();
    A_LogNum mult = multiplyReals(A,B);
    (*this)=(addReals(*this,mult));
}

// Q: why are these functions defined?
// A: for use in copy ctor
bool A_LogNum::getSignBit() const  {
    return signBit;
}

double A_LogNum::getLogval() {
    return logval.to_double();
}

/*
 * the bitshift will be performed after
 * d is implicitly cast to an integer
 * using the FLOOR round scheme.
 * Note that d >= 0 from how we call this func
 * */
fixedtype deltaPlus(fixedtype d) {
    int d_int = getIndex(d);
    fixedtype deltaP_result = dPLUSvals[d_int];
    return deltaP_result;
}

fixedtype deltaMinus(fixedtype d) {
    int d_int = getIndex(d);
    fixedtype deltaM_result = dMINUSvals[d_int];
    return deltaM_result;
}


// get the index on delta tables corresponding to this value of d
// does it matter if we use array vs. map & key, CAM vs BRAM?
uint32_t getIndex(fixedtype d) {
    int x = (((fixedtype2)(d)) << FRACBITS);
    x += OFFSET;
    return (x);
}

// how do we make sure this gets executed exactly once, and before deltaPlus()?
// also, need to know how to index into this (vals[0] refers to vals[1000.0000]
//                                            vals[1] refers to vals[1000.0001]  )
//  OR...
//          we could design it so that vals[0] refers to vals[0000.0000]
//                                     vals[1] refers to vals[0000.0001]
// OR something
// just need to find a cast that will never over/underflow when we get
// d as a fixedtype, and must convert it to an int to index the (ROM?) thingy
void arrayInit() {
    fixedtype base(MIN_VAL);
    for (int i = 0; i < TABLEROWS; ++i) {
        double dp,dm;
        double d = (double)(base);
        dp = log2(1+pow(2,-d));
        dm = log2(1-pow(2,-d));
        dPLUSvals[i] = fixedtype(dp);
        dMINUSvals[i] = fixedtype(dm);
        base += RESOLUTION;
    }
}
