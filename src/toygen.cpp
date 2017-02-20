/** Copyright 2016 Vitaly Vorobyev
 **
 **/

#include "/home/vitaly/B0toD0pipi/DDTatami/src/toygen.h"

using std::cout;
using std::endl;
using std::to_string;

using DDlz::DDlzBinStruct;

typedef libTatami::AbsICPVPdf Pdf;
typedef libTatami::ToyPdfGen ToyPdfGen;
typedef libTatami::ICPVEvt Evt;
typedef std::string str;
typedef std::vector<Evt> vectevt;
typedef std::vector<double> vectd;

ToyGen::ToyGen(Pdf* pdf, const double& phi1, const double& wt,
               const str& dconf, const str& bconf) :
    m_pdf(pdf),
    m_evt(new Evt("/home/vitaly/B0toD0pipi/DDTatami/params/toyevt.txt")),
    m_gen(new ToyPdfGen(m_pdf)),
    m_dd(new DDlzBinStruct(pdf->tau(), pdf->dm(), phi1, wt, dconf, bconf)),
    m_silent(false) {}

void ToyGen::SetCP(const double& sinv, const double& cosv) {
    m_dd->Set_sin2beta(sinv);
    m_dd->Set_cos2beta(cosv);
}

void ToyGen::PrintDifference(const unsigned requested,
                             const unsigned generated) {
    if (requested != generated) {
        cout << to_string(requested) << " events requested" << endl;
        cout << to_string(generated) << " events generated" << endl;
    }
}

unsigned ToyGen::GenFl(const unsigned N, vectevt* evec) {
    cout << "ToyGen::GenFl: " << N << " enents" << endl;
    evec->clear();
    m_evt->SetIVar("cp", 0);
    m_evt->SetIVar("bind", 0);
    for (int j = 0; j < 2; j++) {  // flv
        const int flv = 2 * j - 1;
        m_evt->SetIVar("flv", flv);
        for (int binb = -8; binb <= 8; binb++) {
            if (binb) {  // binb
                m_evt->SetIVar("binb", binb);
                const int nev = 0.5 * N * m_dd->FractionD0pipi(binb, flv) + 0.5;
                if (nev < 0) {
                    cout << "ToyGen::GenFlv not positive number of event "
                         << nev << " is requested";
                    cout << " for flv " << flv << ", binB "
                         << binb << "!" << endl;
                    continue;
                }
                m_pdf->SetS(m_dd->SinCoefFlv());
                m_pdf->SetC(m_dd->CosCoefFlv(flv));
                vectd dtv;
                m_gen->Generate(nev, &dtv, m_silent);
                for (auto& dt : dtv) {
                    m_evt->SetDVar("dt", dt);
                    evec->push_back(Evt(*m_evt));
                }
            }
        }
    }
    PrintDifference(N, evec->size());
    return evec->size();
}

unsigned ToyGen::GenCP(const unsigned N, vectevt* evec) {
    cout << "ToyGen::GenCP: " << N << " enents" << endl;
    evec->clear();
    m_evt->SetIVar("bind", 0);
    for (int i = 0; i < 2; i++) {  // cp
        const int cp = 2*i-1;
        m_evt->SetIVar("cp", cp);
        for (int j = 0; j < 2; j++) {  // flv
            const int flv = 2 * j - 1;
            m_evt->SetIVar("flv", flv);
            for (int binb = -8; binb <= 8; binb++) {
                if (binb) {  // binb
                    m_evt->SetIVar("binb", binb);
                    const unsigned nev = flv == 1 ?
                             0.25 * N * m_dd->FractionD0pipi(binb, flv) + 0.5:
                             0.25 * N * m_dd->FractionD0pipi(binb, flv) + 0.5;
                    if (nev < 0) {
                        cout << "ToyGen::GenCP not positive number of event "
                             << nev << " is requested";
                        cout << " for CP " << cp << ", flv " << flv
                             << ", Bbin " << binb << "!" << endl;
                        continue;
                    }
                    m_pdf->SetS(m_dd->SinCoefCP(flv, cp, binb));
                    m_pdf->SetC(m_dd->CosCoefCP(flv, binb));
                    vectd dtv;
                    m_gen->Generate(nev, &dtv, m_silent);
                    for (auto& dt : dtv) {
                        m_evt->SetDVar("dt", dt);
                        evec->push_back(Evt(*m_evt));
                    }
                }
            }
        }
    }
    PrintDifference(N, evec->size());
    return evec->size();
}

void ToyGen::SetNorm(const unsigned N, vectd* v) const {
    double norm0 = 0;
    for (auto& value : *v) { norm0 += value;}
    for (auto& value : *v) { value /= norm0 * N;}
    for (unsigned i = 0; i < v->size(); i++) norm0 += (*v)[i];
    for (unsigned i = 0; i < v->size(); i++) (*v)[i] /= norm0 * N;
}

unsigned ToyGen::GenDD(const unsigned N, vectevt* evec) {
    cout << "ToyGen::GenDD: " << N << " enents" << endl;
    evec->clear();
    m_evt->SetIVar("cp", 0);
    for (int binb = -8; binb <= 8; binb++) {
        if (binb != 0) {
            m_evt->SetIVar("binb", binb);
            for (int bind = -8; bind <= 8; bind++) {
                if (bind != 0) {
                    m_evt->SetIVar("bind", bind);
                    for (unsigned j = 0; j < 2; j++) {
                        const unsigned flv = 2 * j - 1;
                        m_evt->SetIVar("flv", flv);
                        const unsigned nev = 0.5 * N *
                                m_dd->FractionDblDlz(binb, bind, flv) + 0.5;
                        if (nev < 0) {
                            cout << "ToyDblDalitzGen::GenerateDblDtz "
                                 << nev << " events is requested";
                            cout << "for Bbin " << binb << ", Dbin "
                                 << bind << ", flv " << flv << "!" << endl;
                            continue;
                        }
                        m_pdf->SetS(m_dd->SinCoefDD(flv, bind, binb));
                        m_pdf->SetC(m_dd->CosCoefDD(flv, bind, binb));
                        vectd dtv;
                        m_gen->Generate(nev, &dtv, m_silent);
                        for (auto& dt : dtv) {
                            m_evt->SetDVar("dt", dt);
                            evec->push_back(Evt(*m_evt));
                        }
                    }
                }
            }
        }
    }
    PrintDifference(N, evec->size());
    return evec->size();
}

void ToyGen::print_params(void) const {
    cout << "ToyGen init parameters:" << endl;
    cout << "  Pdf:" << endl;
    m_pdf->print_params();
}
