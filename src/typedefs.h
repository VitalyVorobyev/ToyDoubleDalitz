/** Copyright 2016 Vitaly Vorobyev
 **
 **/

#ifndef _HOME_VITALY_B0TOD0PIPI_DDTATAMI_SRC_TYPEDEFS_H_
#define _HOME_VITALY_B0TOD0PIPI_DDTATAMI_SRC_TYPEDEFS_H_

#include <vector>
#include <string>

#include "mylibs/libTatami/toypdfgen.h"
#include "mylibs/libTatami/absicpvpdf.h"
#include "mylibs/libTatami/icpvevent.h"

typedef libTatami::AbsICPVPdf Pdf;
typedef libTatami::ToyPdfGen ToyPdfGen;
typedef libTatami::ICPVEvt Evt;
typedef std::string str;
typedef std::vector<Evt> vectevt;
typedef std::vector<double> vectd;

#endif  // _HOME_VITALY_B0TOD0PIPI_DDTATAMI_SRC_TYPEDEFS_H_
