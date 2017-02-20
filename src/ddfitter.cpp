/** Copyright 2016 Vitaly Vorobyev
 **
 **/

#include "/home/vitaly/B0toD0pipi/DDTatami/src/ddfitter.h"

#include <cmath>
#include <iostream>
#include "Minuit2/FunctionMinimum.h"

typedef libTatami::AbsICPVPdf Pdf;
typedef std::string str;

using ROOT::Minuit2::FunctionMinimum;
using ROOT::Minuit2::MnMigrad;
using ROOT::Minuit2::MnUserParameterState;

using std::cout;
using std::endl;
using std::to_string;

DDFitter::DDFitter(TTree* tree, Pdf *pdf, const double& phi1, const double& wt,
                   const str& dconf, const str& bconf, const bool dil) :
    m_fcn(new DDFcn(tree, pdf, phi1, wt, dconf, bconf)),
    m_fix_tau(true), m_fix_dm(true), m_fix_sin(false), m_fix_cos(false),
    m_fix_phase(true),
    m_fix_r(true), m_fix_phi(false), m_fix_cp(false),
    m_use_dilut(dil), m_angle(true) {
    define_params(*pdf, phi1);
    m_fcn->UseDilut(m_use_dilut);
}

void DDFitter::define_params(const Pdf& pdf, const double& phi1) {
    upar.Add("tau", pdf.tau(), 0.01, 0.5, 2.5);
    upar.Add("dm", pdf.dm(), 0.01, 0.4, 0.6);
    if (m_angle) {
        upar.Add("r", 1., 0.1, 0.1, 10.);
        upar.Add("beta", phi1, 0.1, -5., 5.);
    } else {
        upar.Add("sin2beta", std::sin(2.*phi1), 0.1, -5., 5.);
        upar.Add("cos2beta", std::cos(2.*phi1), 0.1, -5., 5.);
    }
    if (m_use_dilut) {
        for (int i = 0; i < 8; i++)
            upar.Add("D" + to_string(i+1), m_fcn->D_bdlz(i+1), 0.1, -1.5, 1.5);
    } else {
        for (int i = 0; i < 8; i++)
            upar.Add("C" + to_string(i+1), m_fcn->C_bdlz(i+1), 0.1, -1.5, 1.5);
        for (int i = 0; i < 8; i++)
            upar.Add("S" + to_string(i+1), m_fcn->S_bdlz(i+1), 0.1, -1.5, 1.5);
    }
}

void DDFitter::FixCP(const bool x) {
    m_fix_sin = x;
    m_fix_cos = x;
    m_fix_phi = x;
    m_fix_cp  = x;
}

void DDFitter::FixSin(const bool x) {
    m_fix_sin = x;
    m_fix_phi = x;
}

void DDFitter::FixCos(const bool x) {
    m_fix_cos = x;
    m_fix_phi = x;
}

void DDFitter::FixR(const bool x) {
    m_fix_r = x;
}

void DDFitter::FixPhi(const bool x) {
    m_fix_phi = x;
    m_fix_sin = x;
    m_fix_cos = x;
}

void DDFitter::FixPhases(const bool x) {
    m_fix_phase = x;
}

void DDFitter::FixTau(const bool x) {
    m_fix_tau = x;
}

void DDFitter::FixDm(const bool x) {
    m_fix_dm = x;
}

void DDFitter::setup_params(MnMigrad* migrad) {
    if (m_fix_tau) migrad->Fix("tau");
    if (m_fix_dm)  migrad->Fix("dm");
    if (!m_angle && m_fix_sin) migrad->Fix("sin2beta");
    if (!m_angle && m_fix_cos) migrad->Fix("cos2beta");
    if ( m_angle && m_fix_r)   migrad->Fix("r");
    if ( m_angle && m_fix_phi) migrad->Fix("beta");
    if (m_fix_phase) {
        if (m_use_dilut) {
            for (int i = 0; i < 8; i++)
                migrad->Fix(("D" + to_string(i+1)).c_str());
        } else {
            for (int i = 0; i < 8; i++) {
                migrad->Fix(("C" + to_string(i+1)).c_str());
                migrad->Fix(("S" + to_string(i+1)).c_str());
            }
        }
    }
}

void DDFitter::ConfigFcn(void) {
    m_fcn->UseAngle(m_angle);
    if (m_angle) {
        m_fcn->FixSin(m_fix_r && m_fix_phi);
        m_fcn->FixCos(m_fix_r && m_fix_phi);
    } else {
        m_fcn->FixSin(m_fix_sin);
        m_fcn->FixCos(m_fix_cos);
    }
    m_fcn->FixTau(m_fix_tau);
    m_fcn->FixDm(m_fix_dm);
    m_fcn->FixPhases(m_fix_phase);
    m_fcn->FixCP(m_fix_cp);
}

MnUserParameterState DDFitter::Fit(void) {
    ConfigFcn();
    MnMigrad migrad(*m_fcn, upar);
    setup_params(&migrad);
    cout << "We are ready for minimization." << endl;
    FunctionMinimum min = migrad();
    MnUserParameterState pstate = min.UserState();
    cout << "Minimization completed!" << endl;
    const int NPars = m_use_dilut ? 12 : 20;
    for (int i = 0; i < NPars; i++) {
        if (!migrad.Parameter(i).IsFixed()) {
            cout << upar.Name(i) << ": " << upar.Value(i) << " -> ";
            cout << pstate.Value(i) << " +- " << pstate.Error(i) << endl;
        }
    }
    return pstate;
}
