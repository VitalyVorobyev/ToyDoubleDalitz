/** Copyright 2016 Vitaly Vorobyev
 **
 **/

#include "/home/vitaly/B0toD0pipi/DDTatami/src/tupletools.h"

typedef libTatami::ICPVEvt Evt;
typedef std::string         str;
typedef std::vector<double> vectd;
typedef std::vector<int>    vecti;

TupleTools::TupleTools() :
    nIvar(0), nDvar(0)
{}

// Output TTree tools //
void TupleTools::FillTree(const Evt& evt) {
    fill_idv(evt);
    m_tree->Fill();
}

TTree* TupleTools::InitTree(const Evt& evt, const str& tname) {
    m_tree = new TTree(tname.c_str(), tname.c_str());
    InitTree(evt, m_tree);
    return m_tree;
}

void TupleTools::InitTree(const Evt& evt, TTree* tree) {
    m_tree = tree;
    init_idv(evt);
    for (unsigned i = 0; i < nIvar; i++) {
        const str name = evt.IName(i);
        const str nametype = name + "/I";
        m_tree->Branch(name.c_str(), &m_iv[i], nametype.c_str());
    }
    for (unsigned i = 0; i < nDvar; i++) {
        const str name = evt.DName(i);
        const str nametype = name + "/D";
        m_tree->Branch(name.c_str(), &m_dv[i], nametype.c_str());
    }
}

// Input TTree tools //
void TupleTools::ConnectToTree(TTree* tree, const Evt& evt) {
    m_tree = tree; init_idv(evt);
    for (unsigned i = 0; i < nIvar; i++)
        m_tree->SetBranchAddress(evt.IName(i).c_str(), &m_iv[i]);
    for (unsigned i = 0; i < nDvar; i++)
        m_tree->SetBranchAddress(evt.DName(i).c_str(), &m_dv[i]);
}

void TupleTools::ReadEvent(const int nev, Evt* evt) {
    m_tree->GetEvent(nev);
    for (unsigned i = 0; i < m_iv.size(); i++) evt->SetIVar(i, m_iv[i]);
    for (unsigned i = 0; i < m_dv.size(); i++) evt->SetDVar(i, m_dv[i]);
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
