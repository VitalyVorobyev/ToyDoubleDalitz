/** Copyright 2016 Vitaly Vorobyev
 **
 **/

#include "/home/vitaly/B0toD0pipi/DDTatami/src/ddfcn.h"

#include <iostream>
#include <algorithm>

using Pdf = libTatami::AbsICPVPdf;
using Evt = libTatami::ICPVEvt;
using str = std::string;
using vectd = std::vector<double>;

using std::make_unique;

using std::cout;
using std::endl;
using std::max;
using std::min;

const str DDFcn::m_evt_conf("params/toyevt.txt");

DDFcn::DDFcn(TTree& tree, Pdf& pdf, double phi1, double wt,
             const str &dconf, const str &bconf):
    theErrorDef(1.), m_raw_pdf(pdf),
    m_evt(make_unique<Evt>(m_evt_conf)),
    m_pdf(make_unique<DDPdf>(*m_evt, m_raw_pdf, phi1, wt, dconf, bconf)),
    m_tuptool(make_unique<TupleTools>(tree)),
    m_fix_tau(true), m_fix_dm(true), m_fix_phase(true),
    m_fix_sin(false), m_fix_cos(false),
    m_fix_cp(false), m_N(tree.GetEntries()),
    m_use_dilut(false), m_angle(true) {
    m_tuptool->ConnectToTree(*m_evt);
    m_pdf->Set_phi1(phi1);
}

void DDFcn::SetCP(double sinv, double cosv) {
    m_pdf->Set_sin2beta(sinv);
    m_pdf->Set_cos2beta(cosv);
}

double DDFcn::operator()(const vectd& par) const {
    SetParams(par);
    double loglh = 0;
    for (uint32_t i = 0; i < m_N; i++) {
        m_tuptool->ReadEvent(i, *m_evt);
        auto pdf = (*m_pdf)(*m_evt);
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
                m_pdf->Set_D_bdlz(i+1, max(-1., min(1., par[4+i])));
            } else {
                m_pdf->Set_C_bdlz(i+1, max(-1., min(1., par[4+i])));
                m_pdf->Set_S_bdlz(i+1, max(-1., min(1., par[12+i])));
            }
        }
    }
    return 0;
}

void DDFcn::FixSin(bool x) { m_fix_sin = x;}
void DDFcn::FixCos(bool x) { m_fix_cos = x;}

void DDFcn::FixCP(bool x) {
    m_fix_sin = x;
    m_fix_cos = x;
    m_fix_cp  = x;
}

void DDFcn::FixPhases(bool x) { m_fix_phase = x;}
void DDFcn::FixTau(bool x) { m_fix_tau = x;}
void DDFcn::FixDm(bool x) { m_fix_dm = x;}
void DDFcn::UseAngle(bool x) { m_angle = x;}
