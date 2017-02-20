/** Copyright 2016 Vitaly Vorobyev
 **
 **/

#ifndef _HOME_VITALY_B0TOD0PIPI_DDTATAMI_SRC_PROGOPT_H_
#define _HOME_VITALY_B0TOD0PIPI_DDTATAMI_SRC_PROGOPT_H_

#include <string>

#include "./expsetup.h"

///
/// \brief The ProgOpt class
///
class ProgOpt {
 public:
    ProgOpt(void);
    /**
     * @brief Parse
     * @param argc
     * @param argv
     * @return zero if there is no an error
     */
    int Parse(int argc, char **argv);

    int   IsBelle(void) const {return m_belle;}
    int   IsLHCb (void) const {return m_lhcb;}
    bool  IsD0h0 (void) const {return m_d0h0;}
    std::string DataPath (void) const {return data_path;}
    std::string DCfgPath(void)  const {return dconf;}
    std::string BCfgPath(void)  const {return bconf;}

    double dm(void)    const {return m_dm;}
    double btau(void)  const {return m_btau;}
    double dtmax(void) const {return m_dtmax;}

    std::string FlFile(const int idx = -1) const;
    std::string CPFile(const int idx = -1) const;
    std::string DDFile(const int idx = -1) const;
    std::string File(const int type, const int idx = -1) const;
    unsigned Nev(const unsigned type) const;

    std::string ToyFitFile(void) const;

    std::string TreeName(void) const {return tname;}

    double Beta(void) const {return m_phi_gen;}
    bool FPha(void)   const {return m_fix_pha;}
    bool FSin(void)   const {return m_fix_sin;}
    bool FCos(void)   const {return m_fix_cos;}
    bool FTau(void)   const {return m_fix_tau;}
    bool FDm(void)    const {return m_fix_dm;}

    bool Gen(void) const {return m_gen;}
    bool Fit(void) const {return m_fit;}

    bool FlFit(void)   const {return m_fl;}
    bool CPFit(void)   const {return m_cp;}
    bool DDFit(void)   const {return m_dd;}
    bool SimFit(void)  const {return m_sim;}
    bool BetaFit(void) const {return m_betafit;}

    bool Toy(void)    const {return m_nexp != 0;}
    int NToyExt(void) const {return m_nexp;}
    int FToyExt(void) const {return m_fexp;}

    bool Dilut(void) const {return m_dil;}

    std::string pref(void) const {return m_pref;}

    const ExpSetup& BelleSetup(void) const {return m_belle_setup;}
    const ExpSetup& LHCbSetup(void)  const {return m_lhcb_setup;}
    const ExpSetup& Setup(void)      const {return m_setup;}

    std::string ToyEvtCfg(void) const;

    static const bool LHCb;
    static const bool Belle;

    static const unsigned FL;
    static const unsigned CP;
    static const unsigned DD;

 private:
    std::string Postfix(const int idx = -1) const;
    void preprocess(void);
  // command line options
    bool m_gen;
    bool m_fit;
    bool m_fl;
    bool m_cp;
    bool m_dd;
    bool m_sim;
    bool m_dil;
    int  m_nexp;
    int  m_fexp;
    int  m_belle;
    int  m_lhcb;
    bool m_d0h0;
    bool m_fix_sin;
    bool m_fix_cos;
    bool m_fix_pha;
    bool m_fix_tau;
    bool m_fix_dm;
    bool m_betafit;

    std::string data_path;
    std::string cfg_path;
    std::string d0pp_cfg;
    std::string d0h0_cfg;
    std::string kspp_cfg;
    std::string bconf;
    std::string dconf;
    double m_phi_gen;
    int m_belleII_coef;
    int m_lhcbII_coef;
    int m_lhcbUp_coef;
    std::string m_pref;

  // config file options
    ExpSetup m_belle_setup;
    ExpSetup m_lhcb_setup;
    ExpSetup m_setup;
    std::string tname;

    double m_dm;
    double m_btau;
    double m_dtmax;
};

#endif  // _HOME_VITALY_B0TOD0PIPI_DDTATAMI_SRC_PROGOPT_H_
