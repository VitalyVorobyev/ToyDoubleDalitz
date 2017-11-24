/** Copyright 2016 Vitaly Vorobyev
 **
 **/

#include "/home/vitaly/B0toD0pipi/DDTatami/src/tupletools.h"

using Evt = libTatami::ICPVEvt;
using str = std::string;
using vectd = std::vector<double>;
using vecti = std::vector<int>;

//using std::make_shared;

TupleTools::TupleTools(TTree& tree) : m_tree(tree), nIvar(0), nDvar(0) {}

TupleTools::TupleTools(TTree& tree, const Evt& evt) :
    TupleTools(tree) {InitTree(evt);}

// Output TTree tools //
void TupleTools::FillTree(const Evt& evt) {
    fill_idv(evt);
    m_tree.Fill();
}

//TTree* TupleTools::InitTree(const Evt& evt, const str& tname) {
//    m_tree = make_shared<TTree>(tname.c_str(), tname.c_str());
//    InitTree(evt, m_tree);
//    return m_tree;
//}

void TupleTools::InitTree(const Evt& evt) {
    init_idv(evt);
    for (unsigned i = 0; i < nIvar; i++) {
        const str name = evt.IName(i);
        const str nametype = name + "/I";
        m_tree.Branch(name.c_str(), &m_iv[i], nametype.c_str());
    }
    for (unsigned i = 0; i < nDvar; i++) {
        const str name = evt.DName(i);
        const str nametype = name + "/D";
        m_tree.Branch(name.c_str(), &m_dv[i], nametype.c_str());
    }
}

// Input TTree tools //
void TupleTools::ConnectToTree(const Evt& evt) {
//    m_tree = make_shared<TTree>(tree);
    init_idv(evt);
    for (unsigned i = 0; i < nIvar; i++)
        m_tree.SetBranchAddress(evt.IName(i).c_str(), &m_iv[i]);
    for (unsigned i = 0; i < nDvar; i++)
        m_tree.SetBranchAddress(evt.DName(i).c_str(), &m_dv[i]);
}

void TupleTools::ReadEvent(int nev, Evt& evt) {
//    if (!m_tree) throw("TupleTools::ReadEvent: TTree is not initialized");
    m_tree.GetEvent(nev);
    for (unsigned i = 0; i < m_iv.size(); i++) evt.SetIVar(i, m_iv[i]);
    for (unsigned i = 0; i < m_dv.size(); i++) evt.SetDVar(i, m_dv[i]);
}

// Private methods //
void TupleTools::init_idv(const Evt& evt) {
    nIvar = evt.IVars().size();
    nDvar = evt.DVars().size();
    m_iv.resize(nIvar, 0);
    m_dv.resize(nDvar, 0);
}

void TupleTools::fill_idv(const Evt& evt) {
    for (unsigned i = 0; i < nIvar; i++) m_iv[i] = evt.IVar(i);
    for (unsigned i = 0; i < nDvar; i++) m_dv[i] = evt.DVar(i);
}
