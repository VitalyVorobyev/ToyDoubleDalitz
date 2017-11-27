/** Copyright 2016 Vitaly Vorobyev
 **
 **/

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <memory>
#include <stdexcept>

#include "TFile.h"
#include "TChain.h"

#include "/home/vitaly/B0toD0pipi/DDTatami/src/toytools.h"

#include "./ddfcn.h"

using pstate = ROOT::Minuit2::MnUserParameterState;
using Evt = libTatami::ICPVEvt;
using vectevt = std::vector<Evt>;
using Pdf = libTatami::AbsICPVPdf;
using str = std::string;
using TT = ToyTools;

using std::cout;
using std::endl;
using libTatami::ToyPdf;

TT::ToyTools(const ProgOpt& p) : popt(p) { init();}

void TT::init() {
    dconf     = popt.DCfgPath();
    bconf     = popt.BCfgPath();
    phi1      = popt.Beta() * M_PI / 180.;
    wrtag     = popt.Setup().wtag;
    tree_name = popt.TreeName();
    pdf       = GetPdf();
}

std::unique_ptr<Pdf> TT::GetPdf(void) {
    const ExpSetup& st = popt.Setup();
    auto pdf = std::make_unique<ToyPdf>(st.mean, st.width, st.fbkg);
    pdf->SetRange(popt.dtmax());
    pdf->SetTauDm(popt.btau(), popt.dm());
    return std::move(pdf);
}

void TT::Run(void) {
    Generate();
    Fit();
    ToySim();
}

int TT::Generate() {
    if (popt.Toy() || !popt.Gen()) return 0;
    cout << popt.DDFit() << endl;
    if (popt.FlFit()) Generate(ProgOpt::DataTypes::FL);
    if (popt.CPFit()) Generate(ProgOpt::DataTypes::CP);
    if (popt.DDFit()) Generate(ProgOpt::DataTypes::DD);
    return 0;
}

vectevt TT::Generate(ToyGen& gen, ProgOpt::DataTypes type, uint32_t Nev) {
    switch (type) {
    case ProgOpt::DataTypes::FL: return gen.GenFl(Nev);
    case ProgOpt::DataTypes::CP: return gen.GenCP(Nev);
    case ProgOpt::DataTypes::DD: return gen.GenDD(Nev);
    default: throw std::invalid_argument("Wrong data type");
    }
}

int TT::Generate(ProgOpt::DataTypes type, int idx, bool silent) {
    cout << "Angle beta equals " << popt.Beta() << endl;
    popt.Setup().Print();
    const str fname = popt.File(type, idx);
    cout << "Data file: " << fname << endl;
    TFile file(fname.c_str(), "RECREATE");

    // Prepare generator
    ToyGen tgen(*pdf, phi1, wrtag, dconf, bconf);
    tgen.SetSilent(silent || true);
    // Generate events
    vectevt evec = Generate(tgen, type, popt.Nev(type));
    if (evec.empty()) { throw;}
    // Connect generator with the TTree
    TTree tree(tree_name.c_str(), tree_name.c_str());
    TupleTools tuptool(tree, tgen.Event());
    // Fill the TTree
    for (auto event : evec) tuptool.FillTree(event);
    tree.Write();
    file.Close();
    cout << "tree is written to file " << popt.File(type, idx) << endl;
    return 0;
}

void AddToChain(TChain& ch, const str& fname) {
    ch.Add(fname.c_str());
    cout << "File " << fname << " added to chain" << endl;
}

pstate TT::Fit(int idx) {
    if (idx < 1 && !popt.Fit()) return nullstate;
    cout << "  ** Fit **" << ", " << idx << ", " << popt.Fit() << endl;
    cout << "Angle beta equals " << popt.Beta() << endl;
    popt.Setup().Print();
    // Open data
    TChain tree(tree_name.c_str());
    if (popt.FlFit()) AddToChain(tree, popt.FlFile(idx));
    if (popt.CPFit()) AddToChain(tree, popt.CPFile(idx));
    if (popt.DDFit()) AddToChain(tree, popt.DDFile(idx));
    const int Nev = tree.GetEntries();
    cout << "TTree " << tree_name << " contains " << Nev << " events" << endl;
    if (!Nev) throw("Tree doesn't contain an event");
    // Prepare the fitter
    DDFitter fitter(tree, *pdf, phi1, wrtag, dconf, bconf, popt.Dilut());
    fitter.FixSin(popt.FSin());
    fitter.FixCos(popt.FCos());
    fitter.FixPhases(popt.FPha());
    fitter.FixTau(popt.FTau());
    fitter.FixDm(popt.FDm());
    return fitter.Fit();
}

int TT::ToySim() {
    if (!popt.Toy()) return 0;
    cout << "Welcome to ToySim. We are going to perform ";
    cout << popt.NToyExt() << " pseudoexperiments." << endl;
    Evt evt(popt.ToyEvtCfg());
    TTree tree("toytree", "toytree");
    TupleTools ttool(tree, evt);
    for (int i = 1 + popt.FToyExt(); i <= popt.NToyExt() + popt.FToyExt(); i++) {
        cout << " Exp " << i+1 << endl;
        if (!popt.Dilut()) {
            if (popt.CPFit()) Generate(ProgOpt::DataTypes::CP, i);
            if (popt.DDFit()) Generate(ProgOpt::DataTypes::DD, i);
        }
        const pstate ps = Fit(i);
        FillEvt(evt, ps);
        ttool.FillTree(evt);
    }
    TFile file(popt.ToyFitFile().c_str(), "RECREATE");
    tree.Write();
    file.Close();
    return 0;
}

int TT::FillEvt(Evt& evt, const pstate& ps) {
    const unsigned Npar = ps.Params().size();
    for (unsigned i = 0; i < Npar; i++) {
        const int ind = evt.FindDVar(ps.GetName(i));
        if (ind < 0) {
            cout << "TT::FillEvt: can't find variable " << ps.GetName(i)
                 << endl;
            continue;
        }
        evt.SetDVar(ind  , ps.Value(i));
        evt.SetDVar(ind+1, ps.Error(i));
    }
    return 0;
}
