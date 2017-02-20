/** Copyright 2016 Vitaly Vorobyev
 **
 **/

#ifndef _HOME_VITALY_B0TOD0PIPI_DDTATAMI_SRC_DDFCN_H_
#define _HOME_VITALY_B0TOD0PIPI_DDTATAMI_SRC_DDFCN_H_

#include <vector>
#include <string>

#include "Minuit2/FCNBase.h"
#include <TTree.h>

#include "mylibs/libTatami/toypdf.h"

#include "./ddpdf.h"
#include "./tupletools.h"

///
/// \brief The DDFcn class
///
class DDFcn : public ROOT::Minuit2::FCNBase {
 public:
    DDFcn(TTree* tree, libTatami::AbsICPVPdf* pdf, const double &phi1,
          const double &wt, const std::string &dconf,
          const std::string &bconf);
    ~DDFcn();

    double Up() const {return theErrorDef;}
    void SetErrorDef(double x) {theErrorDef = x;}

    double operator()(const std::vector<double>& par) const;

    void FixCP(const bool x);
    void FixSin(const bool x);
    void FixCos(const bool x);
    void FixPhases(const bool x);
    void FixTau(const bool x);
    void FixDm(const bool x);
    void SetCP(const double& sinv, const double& cosv);
    void UseAngle(const bool x);

    double C_bdlz(const int bin) const { return m_pdf->C_bdlz(bin);}
    double S_bdlz(const int bin) const { return m_pdf->S_bdlz(bin);}
    double D_bdlz(const int bin) const { return m_pdf->D_bdlz(bin);}

    void UseDilut(const bool x) {m_use_dilut = x; m_pdf->UseDilut(m_use_dilut);}

 private:
    int SetParams(const std::vector<double>& par) const;

    double theErrorDef;
    TTree* m_tree;
    libTatami::AbsICPVPdf* m_raw_pdf;
    libTatami::ICPVEvt* m_evt;
    DDPdf* m_pdf;
    TupleTools* m_tuptool;

    bool m_fix_tau;
    bool m_fix_dm;
    bool m_fix_phase;
    bool m_fix_sin;
    bool m_fix_cos;
    bool m_fix_cp;
    int  m_N;

    bool m_use_dilut;
    bool m_angle;
};

#endif  // _HOME_VITALY_B0TOD0PIPI_DDTATAMI_SRC_DDFCN_H_
