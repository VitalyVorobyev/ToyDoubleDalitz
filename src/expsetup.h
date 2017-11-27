/** Copyright 2016-2017 Vitaly Vorobyev
 **
 **/

#ifndef _HOME_VITALY_B0TOD0PIPI_DDTATAMI_SRC_EXPSETUP_H_
#define _HOME_VITALY_B0TOD0PIPI_DDTATAMI_SRC_EXPSETUP_H_

#include <cstdint>

///
/// \brief The ExpSetup class
///
class ExpSetup {
 public:
    ///
    /// \brief ExpSetup
    ///
    ExpSetup();
    ///
    /// \brief TagPow
    /// \return
    ///
    double TagPow() const;
    ///
    /// \brief Print
    ///
    void Print() const;
    ///
    /// \brief ScaleStat
    /// \param s
    ///
    void ScaleStat(double s);
    ///
    /// \brief AutoScale
    ///
    void AutoScale(void);

    double mean;
    double width;
    double fbkg;
    double wtag;
    double etag;
    uint32_t nCP;
    uint32_t nflv;
    uint32_t nDD;
    uint32_t nCPh0;
    uint32_t nflvh0;
    uint32_t nDDh0;
};

#endif  // _HOME_VITALY_B0TOD0PIPI_DDTATAMI_SRC_EXPSETUP_H_
