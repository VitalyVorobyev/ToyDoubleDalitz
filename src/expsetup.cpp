/** Copyright 2016-2017 Vitaly Vorobyev
 **
 **/

#include "/home/vitaly/B0toD0pipi/DDTatami/src/expsetup.h"

#include <cmath>
#include <iostream>

using std::cout;
using std::endl;

ExpSetup::ExpSetup():
    mean(0.),
    width(1.),
    fbkg(0.),
    wtag(0.),
    etag(1.),
    nCP(0),
    nflv(0),
    nDD(0),
    nCPh0(0),
    nflvh0(0) {
}

double ExpSetup::TagPow() const {
    return etag * std::pow(1. - 2.*wtag, 2);
}

void ExpSetup::Print() const {
    cout << "Experimental setup: " << endl;
    cout << "  time resolution:       " << width << " ps" << endl;
    cout << "  background frac:       " << fbkg << endl;
    cout << "  tagging efficiency:    " << etag << endl;
    cout << "  wrong tag probability: " << wtag << endl;
    cout << "  tagging power: " << TagPow() << endl;
    cout << "  events: " << nCP << " " << nflv << " " << nDD;
    cout << " " << nCPh0 << " " << nflvh0 << endl;
}

void ExpSetup::ScaleStat(double s) {
    nCP *= s;
    nflv *= s;
    nDD *= s;
    nCPh0 *= s;
    nflvh0 *= s;
}

void ExpSetup::AutoScale(void) {
    ScaleStat(1./(1. - fbkg));
}
