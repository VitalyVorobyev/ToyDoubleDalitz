/** Copyright 2016 Vitaly Vorobyev
 **
 **/

#include "/home/vitaly/B0toD0pipi/DDTatami/src/ddfcn.h"

#include <iostream>

typedef libTatami::AbsICPVPdf Pdf;
typedef libTatami::ICPVEvt Evt;
typedef std::string str;
typedef std::vector<double> vectd;

using std::cout;
using std::endl;

DDFcn::DDFcn(TTree* tree, Pdf* pdf, const double &phi1, const double &wt,
             const str &dconf, const str &bconf):
    theErrorDef(1.), m_tree(tree), m_raw_pdf(pdf),
    m_evt(new Evt("params/toyevt.txt")),
    m_pdf(new DDPdf(*m_evt, m_raw_pdf, phi1, wt, dconf, bconf)),
    m_tuptool(new TupleTools()),
    m_fix_tau(true), m_fix_dm(true), m_fix_phase(true),
    m_fix_sin(false), m_fix_cos(false),
    m_fix_cp(false), m_use_dilut(false), m_angle(true) {
    m_tuptool = new TupleTools();
    m_tuptool->ConnectToTree(m_tree, *m_evt);
    m_N = m_tree->GetEntries();
    m_pdf->Set_phi1(phi1);
}

DDFcn::~DDFcn() {
    delete m_evt;
    delete m_pdf;
    delete m_tuptool;
}

void DDFcn::SetCP(const double& sinv, const double& cosv) {
    m_pdf->Set_sin2beta(sinv);
    m_pdf->Set_cos2beta(cosv);
}

double DDFcn::operator()(const vectd& par) const {
    SetParams(par);
    double loglh = 0;
    for (int i = 0; i < m_N; i++) {
        m_tuptool->ReadEvent(i, m_evt);
        const double pdf = (*m_pdf)(*m_evt);
        if (!std::isnan(pdf) && pdf > 0) { loglh += -2 * log(pdf);
        } else { cout << "DDFcn: bad pdf :(" << endl;}
    }
    cout << "lh: " << loglh;
    cout << ", tau " << par[0] << ", dm " << par[1];
    if (m_angle) {
        cout << ", phi1 " << par[3];
    } else {
        cout << ", sin " << par[2] << ", cos " << par[3];
    }
    cout << endl;
    return loglh;
}

int DDFcn::SetParams(const vectd &par) const {
    if (!m_fix_tau) m_pdf->Set_tau(par[0]);
    if (!m_fix_dm)  m_pdf->Set_dm(par[1]);
    if (m_angle) {
        if (!m_fix_cp) m_pdf->Set_phi1(par[3]);
    } else {
        if (!m_fix_sin) m_pdf->Set_sin2beta(par[2]);
        if (!m_fix_cos) m_pdf->Set_cos2beta(par[3]);
    }
    if (!m_fix_phase) {
        for (int i = 0; i < 8; i++) {
            if (m_use_dilut) {
                m_pdf->Set_D_bdlz(i+1, par[ 4+i]);
            } else {
                m_pdf->Set_C_bdlz(i+1, par[ 4+i]);
                m_pdf->Set_S_bdlz(i+1, par[12+i]);
            }
        }
    }
    return 0;
}

void DDFcn::FixSin(const bool x) { m_fix_sin = x;}
void DDFcn::FixCos(const bool x) { m_fix_cos = x;}

void DDFcn::FixCP(const bool x) {
    m_fix_sin = x;
    m_fix_cos = x;
    m_fix_cp  = x;
}

void DDFcn::FixPhases(const bool x) { m_fix_phase = x;}
void DDFcn::FixTau(const bool x) { m_fix_tau = x;}
void DDFcn::FixDm(const bool x) { m_fix_dm = x;}
void DDFcn::UseAngle(const bool x) { m_angle = x;}
