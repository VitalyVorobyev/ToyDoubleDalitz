/** Copyright 2016 Vitaly Vorobyev
 **
 **/

#ifndef _HOME_VITALY_B0TOD0PIPI_DDTATAMI_SRC_DDFCN_H_
#define _HOME_VITALY_B0TOD0PIPI_DDTATAMI_SRC_DDFCN_H_

#include <vector>
#include <string>
#include <cstdint>

#include "Minuit2/FCNBase.h"
#include <TTree.h>

#include "mylibs/libTatami/toypdf.h"

#include "./ddpdf.h"
#include "./tupletools.h"

/**
 * @brief The DDFcn class
 */
class DDFcn : public ROOT::Minuit2::FCNBase {
 public:
    DDFcn(TTree& tree, libTatami::AbsICPVPdf& pdf, double phi1,
          double wt, const std::string &dconf, const std::string &bconf);

    double Up() const {return theErrorDef;}
    void SetErrorDef(double x) {theErrorDef = x;}

    double operator()(const std::vector<double>& par) const;

    void FixCP(bool x);
    void FixSin(bool x);
    void FixCos(bool x);
    void FixPhases(bool x);
    void FixTau(bool x);
    void FixDm(bool x);
    void SetCP(double sinv, double cosv);
    void UseAngle(bool x);

    double C_bdlz(int bin) const { return m_pdf->C_bdlz(bin);}
    double S_bdlz(int bin) const { return m_pdf->S_bdlz(bin);}
    double D_bdlz(int bin) const { return m_pdf->D_bdlz(bin);}

    void UseDilut(bool x) {m_use_dilut = x; m_pdf->UseDilut(m_use_dilut);}

 private:
    int SetParams(const std::vector<double>& par) const;

    double theErrorDef;
    libTatami::AbsICPVPdf& m_raw_pdf;
    std::unique_ptr<libTatami::ICPVEvt> m_evt;
    std::unique_ptr<DDPdf> m_pdf;
    std::unique_ptr<TupleTools> m_tuptool;

    bool m_fix_tau;
    bool m_fix_dm;
    bool m_fix_phase;
    bool m_fix_sin;
    bool m_fix_cos;
    bool m_fix_cp;
    uint32_t  m_N;

    bool m_use_dilut;
    bool m_angle;

    const static std::string m_evt_conf;
};

#endif  // _HOME_VITALY_B0TOD0PIPI_DDTATAMI_SRC_DDFCN_H_
