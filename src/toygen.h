/** Copyright 2016 Vitaly Vorobyev
 **
 **/

#ifndef _HOME_VITALY_B0TOD0PIPI_DDTATAMI_SRC_TOYGEN_H_
#define _HOME_VITALY_B0TOD0PIPI_DDTATAMI_SRC_TOYGEN_H_

#include <vector>
#include <string>

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
    ToyGen(libTatami::AbsICPVPdf *pdf, const double &phi1, const double &wt,
           const std::string& dconf, const std::string& bconf);
    ///
    /// \brief GenFl
    /// \param N
    /// \param evec
    ///
    unsigned GenFl(const unsigned N, std::vector<libTatami::ICPVEvt>* evec);
    ///
    /// \brief GenCP
    /// \param N
    /// \param evec
    ///
    unsigned GenCP(const unsigned N, std::vector<libTatami::ICPVEvt>* evec);
    ///
    /// \brief GenDD
    /// \param N
    /// \param evec
    ///
    unsigned GenDD(const unsigned N, std::vector<libTatami::ICPVEvt>* evec);
    ///
    /// \brief SetCP
    /// \param sinv
    /// \param cosv
    ///
    void SetCP(const double& sinv, const double& cosv);
    ///
    /// \brief Event
    /// \return
    ///
    const libTatami::ICPVEvt& Event(void) const { return *m_evt;}
    ///
    /// \brief SetSeed
    /// \param seed
    ///
    void SetSeed(const unsigned seed) {m_gen->SetSeed(seed);}
    ///
    /// \brief SetSilent
    /// \param x
    ///
    void SetSilent(const bool x) {m_silent = x;}
    ///
    /// \brief print_params
    ///
    void print_params(void) const;

 private:
    void SetNorm(const unsigned N, std::vector<double> *v) const;
    void PrintDifference(const unsigned requested, const unsigned generated);

    libTatami::AbsICPVPdf* m_pdf;
    libTatami::ICPVEvt* m_evt;
    libTatami::ToyPdfGen* m_gen;
    DDlz::DDlzBinStruct* m_dd;
    bool m_silent;
};

#endif  // _HOME_VITALY_B0TOD0PIPI_DDTATAMI_SRC_TOYGEN_H_
