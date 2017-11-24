/** Copyright 2016 Vitaly Vorobyev
 **
 **/

#ifndef _HOME_VITALY_B0TOD0PIPI_DDTATAMI_SRC_TUPLETOOLS_H_
#define _HOME_VITALY_B0TOD0PIPI_DDTATAMI_SRC_TUPLETOOLS_H_

#include <string>
#include <vector>
//#include <memory>

#include "mylibs/libTatami/icpvevent.h"

#include "TTree.h"

///
/// \brief The TupleTools class
///
class TupleTools {
 public:
    TupleTools(TTree& tree);
    TupleTools(TTree& tree, const libTatami::ICPVEvt& evt);

  // Output TTree tools //
    ///
    /// \brief InitTree
    /// \param evt
    /// \param tname
    /// \return
    ///
//    TTree* InitTree(const libTatami::ICPVEvt& evt, const std::string& tname);
    ///
    /// \brief InitTree
    /// \param evt
    ///
    void InitTree(const libTatami::ICPVEvt& evt);
    ///
    /// \brief FillTree
    /// \param evt
    ///
    void FillTree(const libTatami::ICPVEvt& evt);

  // Input TTree tools //
    ///
    /// \brief ConnectToTree
    /// \param tree
    /// \param evt
    ///
    void ConnectToTree(const libTatami::ICPVEvt& evt);
    ///
    /// \brief ReadEvent
    /// \param nev
    /// \param evt
    ///
    void ReadEvent(int nev, libTatami::ICPVEvt& evt);

 private:
    ///
    /// \brief init_idv
    /// \param evt
    ///
    void init_idv(const libTatami::ICPVEvt& evt);
    ///
    /// \brief fill_idv
    /// \param evt
    ///
    void fill_idv(const libTatami::ICPVEvt& evt);
    ///
    /// \brief m_tree
    ///
//    std::shared_ptr<TTree> m_tree;
    TTree& m_tree;
    ///
    /// \brief m_iv
    ///
    std::vector<int> m_iv;
    ///
    /// \brief m_dv
    ///
    std::vector<double> m_dv;
    ///
    /// \brief nIvar
    ///
    unsigned nIvar;
    ///
    /// \brief nDvar
    ///
    unsigned nDvar;
};

#endif  // _HOME_VITALY_B0TOD0PIPI_DDTATAMI_SRC_TUPLETOOLS_H_
