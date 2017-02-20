/** Copyright 2016 Vitaly Vorobyev
 **
 **/

#include "./ddpdf.h"

#include <iostream>

typedef libTatami::AbsICPVPdf Pdf;
typedef libTatami::ICPVEvt Evt;
typedef std::string str;
using DDlz::DDlzBinStruct;

using std::cout;
using std::endl;
using DDlz::DlzBinStruct;

const int DDPdf::m_fl_type = 1;
const int DDPdf::m_cp_type = 2;
const int DDPdf::m_dd_type = 3;

DDPdf::DDPdf(const Evt& evt, Pdf *pdf, const double &phi1, const double& wt,
             const str& dconf, const str& bconf) :
    DDlzBinStruct(pdf->tau(), pdf->dm(), phi1, wt, dconf, bconf),
    m_pdf(pdf),
    m_dt(0.), m_cp(0), m_flv(0), m_binb(0), m_bind(0),
    m_use_dilut(false) {
    init_indices(evt);
    print_params();
}

void DDPdf::init_indices(const Evt& evt) {
    m_dt_ind   = evt.FindDVar("dt");
    m_cp_ind   = evt.FindIVar("cp");
    m_flv_ind  = evt.FindIVar("flv");
    m_binb_ind = evt.FindIVar("binb");
    m_bind_ind = evt.FindIVar("bind");
}

void DDPdf::read_event(const Evt& evt) {
    m_dt   = evt.DVar(m_dt_ind);
    m_cp   = evt.IVar(m_cp_ind);
    m_flv  = evt.IVar(m_flv_ind);
    m_binb = evt.IVar(m_binb_ind);
    m_bind = evt.IVar(m_bind_ind);
}

int DDPdf::get_type(void) const {
    if (m_cp != 0) {          return m_cp_type;
    } else if (m_bind == 0) { return m_fl_type;}
                              return m_dd_type;
}

void DDPdf::set_coefs(const int type) {
    switch (type) {
    case m_fl_type:
        m_pdf->SetC(CosCoefFlv(m_flv));
        m_pdf->SetS(SinCoefFlv());
        break;
    case m_cp_type:
        if (m_use_dilut) {
            m_pdf->SetC(DCosCoefCP());
            m_pdf->SetS(DSinCoefCP(m_flv, m_cp, m_binb));
        } else {
            m_pdf->SetC(CosCoefCP(m_flv, m_binb));
            m_pdf->SetS(SinCoefCP(m_flv, m_cp, m_binb));
        }
        break;
    case m_dd_type:
        if (m_use_dilut) {
            m_pdf->SetC(DCosCoefDD(m_flv, m_bind));
            m_pdf->SetS(DSinCoefDD(m_flv, m_bind, m_binb));
        } else {
            m_pdf->SetC(CosCoefDD(m_flv, m_bind, m_binb));
            m_pdf->SetS(SinCoefDD(m_flv, m_bind, m_binb));
        }
        break;
    default:
        break;
    }
}

void DDPdf::Set_tau(const double& x) {
    DDlzBinStruct::Set_tau(x);
    update_pdf();
}

void DDPdf::Set_dm(const double& x) {
    DDlzBinStruct::Set_dm(x);
    update_pdf();
}

void DDPdf::update_pdf(void) {
    m_pdf->SetTauDm(m_tau, m_dm);
}

double DDPdf::operator() (const Evt& evt) {
    read_event(evt);
    set_coefs(get_type());
    return (*m_pdf)(evt);
}
