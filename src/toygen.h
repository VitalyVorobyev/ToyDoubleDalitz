/** Copyright 2016 Vitaly Vorobyev
 **
 **/

#ifndef _HOME_VITALY_B0TOD0PIPI_DDTATAMI_SRC_TOYGEN_H_
#define _HOME_VITALY_B0TOD0PIPI_DDTATAMI_SRC_TOYGEN_H_

#include <vector>
#include <string>
#include <memory>

#include "mylibs/libDD/ddlzbinstruct.h"
#include "mylibs/libTatami/absicpvpdf.h"
#include "mylibs/libTatami/icpvevent.h"
#include "mylibs/libTatami/toypdfgen.h"

///
/// \brief The ToyGen class.
///
class ToyGen {
 public:
    ///
    /// \brief ToyGen
    /// \param pdf
    /// \param phi1
    /// \param wt
    /// \param dconf
    /// \param bconf
    ///
    ToyGen(libTatami::AbsICPVPdf &pdf, double phi1, double wt,
           const std::string& dconf, const std::string& bconf);
    ///
    /// \brief GenFl
    /// \param N
    /// \param evec
    ///
    unsigned GenFl(unsigned N, std::vector<libTatami::ICPVEvt>* evec);
    ///
    /// \brief GenCP
    /// \param N
    /// \param evec
    ///
    unsigned GenCP(unsigned N, std::vector<libTatami::ICPVEvt>* evec);
    ///
    /// \brief GenDD
    /// \param N
    /// \param evec
    ///
    unsigned GenDD(unsigned N, std::vector<libTatami::ICPVEvt>* evec);
    ///
    /// \brief SetCP
    /// \param sinv
    /// \param cosv
    ///
    void SetCP(double sinv, double cosv);
    ///
    /// \brief Event
    /// \return
    ///
    const libTatami::ICPVEvt& Event(void) const { return *m_evt;}
    ///
    /// \brief SetSeed
    /// \param seed
    ///
    void SetSeed(unsigned seed) {m_gen->SetSeed(seed);}
    ///
    /// \brief SetSilent
    /// \param x
    ///
    void SetSilent(bool x) {m_silent = x;}
    ///
    /// \brief print_params
    ///
    void print_params(void) const;

 private:
    void SetNorm(unsigned N, std::vector<double> *v) const;
    void PrintDifference(unsigned requested, unsigned generated);

    libTatami::AbsICPVPdf& m_pdf;
    std::unique_ptr<libTatami::ICPVEvt> m_evt;
    std::unique_ptr<libTatami::ToyPdfGen> m_gen;
    std::unique_ptr<DDlz::DDlzBinStruct> m_dd;
    bool m_silent;
    static const std::string m_evt_conf;
};

#endif  // _HOME_VITALY_B0TOD0PIPI_DDTATAMI_SRC_TOYGEN_H_
