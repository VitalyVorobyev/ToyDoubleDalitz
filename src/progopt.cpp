/** Copyright 2016-2017 Vitaly Vorobyev
 **
 **/

#include "/home/vitaly/B0toD0pipi/DDTatami/src/progopt.h"

#include <iostream>
#include <fstream>

#include <boost/program_options.hpp>

using str = std::string;

namespace po = boost::program_options;

using po::options_description;
using po::variables_map;
using po::store;
using po::notify;
using po::parse_command_line;
using po::parse_config_file;
using po::error;
using po::value;

using std::cout;
using std::endl;
using std::ifstream;
using std::to_string;
using std::cerr;

int m_belle_flg;
int m_lhcb_flg;

ProgOpt::ProgOpt(void):
    m_gen(false),  m_fit(false),  m_fl(false),  m_cp(false),  m_dd(false),
    m_sim(false),  m_dil(false),  m_nexp(0), m_fexp(0),
    m_belle(BelleV::ZERO), m_lhcb(LHCbV::ZERO),
    m_d0h0(false),  m_fix_sin(false),  m_fix_cos(false),  m_fix_pha(false),
    m_fix_tau(true), m_fix_dm(true), m_betafit(true),
    data_path("data/"), cfg_path("params/"),
    m_phi_gen(23.)
{}

int ProgOpt::Parse(int argc, char** argv) {
    str data_path_belle;
    str data_path_lhcb;

    options_description desc{"Allowed options"};
    desc.add_options()
    ("help",  "produce help message")
    ("gen,g", "generate events")
    ("fit,f", "perform fit")
    ("dd",    "use double Dalitz events")
    ("cp",    "use CP events")
    ("flv",   "use flavor tagged events")
    ("sim",   "simultaneous usage of CP and double Dalitz events")
    ("dil",   "configuration with dilution factor")
    ("toy",  value(&m_nexp),
     "perform toy MC. arg - number of experiments (int)")
    ("exp",  value(&m_fexp),
     "arg - indedx of the first toy experiment (int)")
    ("beta", value(&m_phi_gen),
     "set value of the angle beta. arg - value (double)")
    ("belle", value(&m_belle_flg),
     "configure for Belle conditions. arg: 1 (Belle), 2 (Belle II)")
    ("lhcb", value(&m_lhcb_flg),
     "configure for LHCb conditions. arg: 1 (run I), 2 (run II) or 3 (Upgrade)")
    ("d0h0", "configure for B0 -> D0h0 analysis")
    ("fsin", "fix sin(2beta)")
    ("fcos", "fix cos(2beta)")
    ("fpha", "fix the strong phase parameters")
    ("betafit", "fit the beta angle");

    options_description fileOptions{"Config file options"};
    fileOptions.add_options()
    ("Belle.res_mean",  value(&m_belle_setup.mean),
     "Mean value of the Belle resolution function")
    ("Belle.res_width", value(&m_belle_setup.width),
     "Width of the Belle resolution function")
    ("Belle.fbkg",      value(&m_belle_setup.fbkg),
     "Background fraction in Belle")
    ("Belle.wtag",      value(&m_belle_setup.wtag),
     "Wrong tag probability in Belle")
    ("Belle.etag",      value(&m_belle_setup.etag),
     "Tagging efficiency in Belle")
    ("Belle.nCP",       value(&m_belle_setup.nCP),
     "Number of CP tagged events at Belle")
    ("Belle.nCPh0",     value(&m_belle_setup.nCPh0),
     "Number of CP tagged events at Belle for D0h0")
    ("Belle.nflv",      value(&m_belle_setup.nflv),
     "Number of flavor tagged events at Belle")
    ("Belle.nflvh0",    value(&m_belle_setup.nflvh0),
     "Number of flavor tagged events at Belle for D0h0")
    ("Belle.nDD",       value(&m_belle_setup.nDD),
     "Number of double Dalitz events at Belle")
    ("Belle.coef",      value(&m_belleII_coef),
     "Belle II statistics scale")
    ("LHCb.res_mean",   value(&m_lhcb_setup.mean),
     "Mean value of the LHCb resolution function")
    ("LHCb.res_width",  value(&m_lhcb_setup.width),
     "Width of the LHCb resolution function")
    ("LHCb.fbkg",       value(&m_lhcb_setup.fbkg),
     "Background fraction in LHCb")
    ("LHCb.wtag",       value(&m_lhcb_setup.wtag),
     "Wrong tag probability in LHCb")
    ("LHCb.etag",       value(&m_lhcb_setup.etag),
     "Tagging efficiency in LHCb")
    ("LHCb.nCP",        value(&m_lhcb_setup.nCP),
     "Number of CP tagged events at LHCb")
    ("LHCb.nflv",       value(&m_lhcb_setup.nflv),
     "Number of flavor tagged events at LHCb")
    ("LHCb.nDD",        value(&m_lhcb_setup.nDD),
     "Number of double Dalitz events at LHCb")
    ("LHCb.coef1",      value(&m_lhcbII_coef),
     "LHCb run II statistics scale")
    ("LHCb.coef2",      value(&m_lhcbUp_coef),
     "LHCb Upgrade statistics scale")
    ("dataPathBelle",   value(&data_path_belle),
     "Data path for Belle")
    ("dataPathLHCb",    value(&data_path_lhcb),
     "Data path for LHCb")
    ("cfgPath",         value(&cfg_path),
     "Configuration path")
    ("d0pipi_path",     value(&d0pp_cfg),
     "B0 -> D0 pi+ pi- binned paraemters")
    ("d0h0_path",       value(&d0h0_cfg),
     "B0 -> D0 pi0 binned paraemters")
    ("kspipi_path",     value(&kspp_cfg),
     "D0 -> Ks0 pi+ pi- binned paraemters")
    ("tname",           value(&tname),
     "Title for TTree objects")
    ("dm",              value(&m_dm),
     "B0 mass difference")
    ("btau",            value(&m_btau),
     "B0 lifetime")
    ("dtmax",           value(&m_dtmax),
     "dt range");

    try {
        variables_map vm;
        store(parse_command_line(argc, argv, desc), vm);

        ifstream settings_file("params/params.cfg", ifstream::in);
        store(parse_config_file(settings_file, fileOptions), vm);

        notify(vm);

        if (vm.count("help")) { cout << desc << endl; return 1; }
        if (vm.count("gen")) m_gen = true;
        if (vm.count("fit")) m_fit = true;
        if (vm.count("dd"))  m_dd  = true;
        if (vm.count("cp"))  m_cp  = true;

        if (vm.count("flv")) {
            m_fl      = true;
            m_fix_sin = true;
            m_fix_cos = true;
            m_fix_pha = true;
            m_fix_tau = false;
            m_fix_dm  = true;
        }

        if (vm.count("sim")) {
            m_sim = true;
            m_cp  = true;
            m_dd  = true;
            m_fl  = false;
        }

        if (vm.count("dil")) m_dil = true;
        if (vm.count("belle")) { m_lhcb  = LHCbV::ZERO;
                                 m_belle = static_cast<BelleV>(m_belle_flg);}
        if (vm.count("lhcb"))  { m_belle = BelleV::ZERO;
                                 m_lhcb = static_cast<LHCbV>(m_lhcb_flg);}
        if (vm.count("d0h0"))  { m_d0h0 = true; m_fix_pha = true; }
        if (vm.count("fsin")) m_fix_sin = true;
        if (vm.count("fcos")) m_fix_cos = true;
        if (vm.count("fpha")) m_fix_pha = true;
        if (vm.count("betafit")) m_betafit = true;
    }  // try
    catch (const error &ex) { cerr << ex.what() << endl;}

  if (m_belle != BelleV::ZERO) {
      m_setup = m_belle_setup;
      data_path = data_path_belle;
      switch (m_belle) {
      case BelleV::I:  // Belle
          m_pref = "belle";
          break;
      case BelleV::II:  // Belle II
          m_pref = "belleII";
          m_setup.ScaleStat(m_belleII_coef);
          break;
      case BelleV::ZERO: break;
      }
  } else {
      m_setup = m_lhcb_setup;
      data_path = data_path_lhcb;
      switch (m_lhcb) {
      case LHCbV::I:  // LHCb
          m_pref = "lhcb";
          break;
      case LHCbV::II:  // LHCb RunII
          m_pref = "lhcbRII";
          m_setup.ScaleStat(m_lhcbII_coef);
          break;
      case LHCbV::Upg:  // LHCb Upgrade
          m_pref = "lhcbUpgr";
          m_setup.ScaleStat(m_lhcbUp_coef);
          break;
      case LHCbV::ZERO: break;
      }
  }

  if (m_d0h0) m_pref += "_d0h0";
  dconf = cfg_path + kspp_cfg;
  bconf = cfg_path + (m_d0h0 ? d0h0_cfg : d0pp_cfg);
  m_setup.AutoScale();
  return 0;
}

str ProgOpt::Postfix(int idx) const {
    str pfx = "_beta" + to_string(static_cast<int>(m_phi_gen));
    if (idx == -1) return pfx + ".root";
    return pfx + "_" + to_string(idx) + ".root";
}

str ProgOpt::FlFile(int idx) const {
    return data_path + "fl_" + m_pref + Postfix(idx);
}

str ProgOpt::CPFile(int idx) const {
    return data_path + "cp_" + m_pref + Postfix(idx);
}

str ProgOpt::DDFile(int idx) const {
    return data_path + "dd_" + m_pref + Postfix(idx);
}

str ProgOpt::ToyFitFile(void) const {
    return data_path + "fitres" + m_pref + "_" + to_string(m_nexp) + ".root";
}

str ProgOpt::File(DataTypes type, int idx) const {
    switch (type) {
    case DataTypes::FL: return FlFile(idx);
    case DataTypes::CP: return CPFile(idx);
    case DataTypes::DD: return DDFile(idx);
    default: throw std::invalid_argument("Wrong data type");
    }
    return str("");
}

uint32_t ProgOpt::Nev(DataTypes type) const {
    switch (type) {
    case DataTypes::FL: return m_setup.nflv;
    case DataTypes::CP: return m_setup.nCP;
    case DataTypes::DD: return m_setup.nDD;
    default: throw std::invalid_argument("Wrong data type");
    }
    return 0;
}

str ProgOpt::ToyEvtCfg(void) const {
    str evtcfg = cfg_path + "toyfitresevt";
    if      ( m_dil && m_betafit) evtcfg += "4.txt";
    else if (!m_dil && m_betafit) evtcfg += "3.txt";
    else if ( m_dil &&!m_betafit) evtcfg += "2.txt";
    else if (!m_dil &&!m_betafit) evtcfg += "1.txt";
    return evtcfg;
}
