/** Copyright 2016 Vitaly Vorobyev
 **
 **/

#ifndef _HOME_VITALY_B0TOD0PIPI_DDTATAMI_SRC_DDPDF_H_
#define _HOME_VITALY_B0TOD0PIPI_DDTATAMI_SRC_DDPDF_H_

#include <string>
#include <vector>

#include "mylibs/libTatami/absicpvpdf.h"
#include "mylibs/libDD/ddlzbinstruct.h"

///
/// \brief The DDPdf class
///
class DDPdf: public DDlz::DDlzBinStruct {
 public:
    ///
    /// \brief DDPdf
    /// \param evt
    /// \param pdf
    /// \param phi1
    /// \param wt
    /// \param dconf
    /// \param bconf
    ///
    DDPdf(const libTatami::ICPVEvt& evt, libTatami::AbsICPVPdf& pdf,
          double phi1, double wt, const std::string &dconf,
          const std::string &bconf);
    ///
    /// \brief operator ()
    /// \param evt
    /// \return
    ///
    double operator() (const libTatami::ICPVEvt& evt);
    ///
    /// \brief UseDilut
    /// \param x
    ///
    void UseDilut(bool x) {m_use_dilut = x;}

    // Extend methods
    ///
    /// \brief Set_tau
    /// \param x
    ///
    void Set_tau(double x);
    ///
    /// \brief Set_dm
    /// \param x
    ///
    void Set_dm(double x);

 private:
    void init_indices(const libTatami::ICPVEvt& evt);
    void read_event(const libTatami::ICPVEvt& evt);
    int  get_type(void) const;
    void set_coefs(const int type);
    void update_pdf(void);

    libTatami::AbsICPVPdf& m_pdf;

    double m_dt;
    int m_cp;
    int m_flv;
    int m_binb;
    int m_bind;

    int m_dt_ind;
    int m_cp_ind;
    int m_flv_ind;
    int m_binb_ind;
    int m_bind_ind;

    static const int m_fl_type;
    static const int m_cp_type;
    static const int m_dd_type;

    bool m_use_dilut;
};

#endif  // _HOME_VITALY_B0TOD0PIPI_DDTATAMI_SRC_DDPDF_H_
