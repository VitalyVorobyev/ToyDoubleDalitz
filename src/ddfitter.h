/** Copyright 2016 Vitaly Vorobyev
 **
 **/

#ifndef _HOME_VITALY_B0TOD0PIPI_DDTATAMI_SRC_DDFITTER_H_
#define _HOME_VITALY_B0TOD0PIPI_DDTATAMI_SRC_DDFITTER_H_

#include <string>
#include <vector>

#include "Minuit2/MnUserParameterState.h"
#include "Minuit2/MnUserParameters.h"
#include "Minuit2/MnMigrad.h"

#include "mylibs/libTatami/absicpvpdf.h"

#include "./ddfcn.h"

///
/// \brief The DDFitter class
///
class DDFitter {
 public:
    ///
    /// \brief DDFitter. Constructor
    /// \param tree. Toy MC data
    /// \param pdf. AbsICPVPdf
    /// \param phi1. The angle beta in rad
    /// \param wt. Wrong tagging probability
    /// \param dconf. Parameters of the D0 -> Ks0 pi+ pi- decay binned Dalitz plot
    /// \param bconf. Parameters of the B0 -> D0 pi+ pi- decay binned Dalitz plot
    /// \param dil. Symmetrize the B0 -> D0 pi+ pi- decay Dalitz plot
    ///
    DDFitter(TTree* tree, libTatami::AbsICPVPdf* pdf, const double& phi1,
             const double& wt, const std::string& dconf,
             const std::string& bconf, const bool dil);
    ///
    /// \brief Fit
    /// \return
    ///
    ROOT::Minuit2::MnUserParameterState Fit(void);
    ///
    /// \brief FixCP
    /// \param x
    ///
    void FixCP(const bool x = true);
    ///
    /// \brief FixSin
    /// \param x
    ///
    void FixSin(const bool x = true);
    ///
    /// \brief FixCos
    /// \param x
    ///
    void FixCos(const bool x = true);
    ///
    /// \brief FixPhases
    /// \param x
    ///
    void FixPhases(const bool x = true);
    ///
    /// \brief FixTau
    /// \param x
    ///
    void FixTau(const bool x = true);
    ///
    /// \brief FixDm
    /// \param x
    ///
    void FixDm(const bool x = true);
    ///
    /// \brief FixR
    /// \param x
    ///
    void FixR(const bool x = true);
    ///
    /// \brief FixPhi
    /// \param x
    ///
    void FixPhi(const bool x = true);
    ///
    /// \brief SetCP
    /// \param sinv
    /// \param cosv
    ///
    void SetCP(const double& sinv, const double& cosv) {
        m_fcn->SetCP(sinv, cosv);
    }
    ///
    /// \brief UseAngle
    /// \param x
    ///
    void UseAngle(const bool x) { m_angle = x;}

 private:
    void define_params(const libTatami::AbsICPVPdf &pdf, const double &phi1);
    void setup_params(ROOT::Minuit2::MnMigrad* migrad);
    void ConfigFcn(void);

    DDFcn* m_fcn;
    ROOT::Minuit2::MnUserParameters upar;

    bool m_fix_tau;
    bool m_fix_dm;
    bool m_fix_sin;
    bool m_fix_cos;
    bool m_fix_phase;
    bool m_fix_r;
    bool m_fix_phi;
    bool m_fix_cp;
    bool m_use_dilut;

    bool m_angle;
};

#endif  // _HOME_VITALY_B0TOD0PIPI_DDTATAMI_SRC_DDFITTER_H_
