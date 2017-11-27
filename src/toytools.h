/** Copyright 2016 Vitaly Vorobyev
 **
 **/

#ifndef _HOME_VITALY_B0TOD0PIPI_DDTATAMI_SRC_TOYTOOLS_H_
#define _HOME_VITALY_B0TOD0PIPI_DDTATAMI_SRC_TOYTOOLS_H_

#include "./toygen.h"
#include "./tupletools.h"
#include "./ddfitter.h"
#include "./progopt.h"

///
/// \brief The ToyTools class. Provides interface for all tools for
/// the feasibility study of phi1 measurement with binned Dalitz plots
/// analysis of B0 -> D0 pi+ pi-, D0 -> Ks0 pi+ pi- decays.
///
class ToyTools {
 public:
    ///
    /// \brief ToyTools
    /// \param p
    ///
    explicit ToyTools(const ProgOpt& p);
    ///
    /// \brief Run
    ///
    void Run(void);
 private:
    ///
    /// \brief Generate
    /// \return
    ///
    int Generate();
    ///
    /// \brief Fit
    /// \param idx
    /// \return
    ///
    ROOT::Minuit2::MnUserParameterState Fit(int idx = -1);
    ///
    /// \brief Generate
    /// \param type
    /// \param idx
    /// \param silent
    /// \return
    ///
    int Generate(ProgOpt::DataTypes type, int idx = -1, bool silent = false);
    std::vector<libTatami::ICPVEvt> Generate(ToyGen& gen,
                                             ProgOpt::DataTypes type,
                                             uint32_t Nev);
    ///
    /// \brief ToySim
    /// \return
    ///
    int ToySim();
    ///
    /// \brief GetPdf
    /// \return
    ///
    std::unique_ptr<libTatami::AbsICPVPdf> GetPdf(void);
    ///
    /// \brief FillEvt
    /// \param evt
    /// \param ps
    /// \return
    ///
    int FillEvt(libTatami::ICPVEvt& evt,
                const ROOT::Minuit2::MnUserParameterState& ps);
    ///
    /// \brief init
    ///
    void init();
    ///
    /// \brief popt. An instance of the ProgOpt class keeps configuration
    ///
    const ProgOpt& popt;

    std::string dconf;
    std::string bconf;
    double phi1;
    double wrtag;
    bool fix_phases;
    std::string tree_name;
    bool betafit;
    std::unique_ptr<libTatami::AbsICPVPdf> pdf;

    ROOT::Minuit2::MnUserParameterState nullstate;
};

#endif  // _HOME_VITALY_B0TOD0PIPI_DDTATAMI_SRC_TOYTOOLS_H_
