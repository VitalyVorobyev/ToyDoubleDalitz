/** Copyright 2016 Vitaly Vorobyev
 **
 **/

#ifndef _HOME_VITALY_B0TOD0PIPI_DDTATAMI_SRC_EXPSETUP_H_
#define _HOME_VITALY_B0TOD0PIPI_DDTATAMI_SRC_EXPSETUP_H_

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
    void ScaleStat(const double& s);
    ///
    /// \brief AutoScale
    ///
    void AutoScale(void);

    double mean;
    double width;
    double fbkg;
    double wtag;
    double etag;
    int nCP;
    int nflv;
    int nDD;
    int nCPh0;
    int nflvh0;
    int nDDh0;
};

#endif  // _HOME_VITALY_B0TOD0PIPI_DDTATAMI_SRC_EXPSETUP_H_
