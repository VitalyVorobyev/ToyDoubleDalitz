/** Copyright 2016 Vitaly Vorobyev
 **
 **/

#ifndef _HOME_VITALY_B0TOD0PIPI_DDTATAMI_SRC_DDTOOLS_H_
#define _HOME_VITALY_B0TOD0PIPI_DDTATAMI_SRC_DDTOOLS_H_

#include "mylibs/libDD/ddlzbinstruct.h"

using DDlz::DDlzBinStruct;

class DDTools {
 public:
    DDTools(void);

 private:
    DDlzBinStruct* m_dd;
};

#endif  // _HOME_VITALY_B0TOD0PIPI_DDTATAMI_SRC_DDTOOLS_H_
