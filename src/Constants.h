#ifndef CONSTANTS
#define CONSTANTS
#include <sstream>
#include <vector>

using std::string;
using std::vector;

namespace constants{
  


#ifdef DNDETA_PROTON
  const std::string MODE = "dndeta_proton";
  const string default_out_directory_name="dndeta_proton";
  const double x_max = 20.0;
  const double x_min = -20.0;
  const double d_x = 0.5;
  const int x_cell_capa=1000;
  const string default_ext="/hadronised.txt";
#elif defined VERTICES
  const std::string MODE = "vertices";
  const string default_out_directory_name="vertices";
  const double x_max = 20.0;
  const double x_min = -20.0;
  const double d_x = 0.5;
  const int x_cell_capa=1000;
  const string default_ext="/hadronised.txt";
#elif defined DNDPT
  const std::string MODE = "dndpt";
  const string default_out_directory_name="dndpt";
  const double x_max = 2000.0;
  const double x_min = 0.0;
  const double d_x = 1.0;
  const int x_cell_capa=1000;
  const string default_ext="/hadronised.txt";
#elif defined JET_PRAC
  const std::string MODE = "JET_PRAC";
  const string default_out_directory_name="JET_PRAC";
  const double x_max = 10.0;
  const double x_min = -10.0;
  const double d_x = 0.5;
  const int x_cell_capa=1000;
  const string default_ext="/jetinfo.txt";
#elif defined DEF
  const std::string MODE = "default";
  const string default_out_directory_name="output";
  const double x_max = 20.0;
  const double x_min = -20.0;
  const double d_x = 0.5;
  const int x_cell_capa=1000;
  const string default_ext="/hadronised.txt";
#else
  const std::string MODE = "default";
  const string default_out_directory_name="output";
  const double x_max = 20.0;
  const double x_min = -20.0;
  const double d_x = 0.5;
  const int x_cell_capa=1000;
  const string default_ext="/hadronised.txt";
#endif





  const int id_nucleus=1000000000;
  const int id_gluon=21;
  const int id_ch_pion=211;
  const int id_ch_kaon=321;
  const int id_proton=2212;

 const double w_eta_multiplicity=0.5;
 const double w_eta_multiplicity_INEL_lg_0=1.0;
 const double w_eta_ATLAS_cut=2.5;
 const double V0M_fwd1=2.8;
 const double V0M_fwd2=5.1;
 const double V0M_bkw1=-3.7;
 const double V0M_bkw2=-1.7;
 const double outer_SPD=1.4;
 
 const double delta_phi_SMALL = 0.5;
 const double delta_eta = 0.5;


 const double pPb_mid_rapidity__fwd=0.50;
 const double pPb_mid_rapidity__bkw=0.0;
 const double pPb_rap_shift_from_lab_to_cm=0.465;
 const double default_midy_pm = 0.5;
 const double LARGE=1.0e+16;
 const double multip_cut_more_than=-LARGE;
 const double multip_cut_less_than=LARGE;

 const double wBin_pp_1[7]={61.049758987000004, 0.92521, 0.052516, 0.010366, 0.00092042, 0.00019170, 0.000036893};//2.76TeV
 const double wBin_pp_2[7]={67.78102915, 3.3182, 0.23192, 0.053267, 0.0057856, 0.0014325, 0.00036575};//7TeV
 const double wBin_pp_1tot = 62.039;
 const double wBin_pp_2tot = 71.392;
 const int nBin=7;
 const double val_cent_pp[10]={0.0095,0.047,0.095,0.14,0.19,0.28,0.38,0.48,0.68,1.0};
 const double val_cent_original[4]={0.1,0.3,0.5,1.0};
 const double val_cent_pPb[7]={0.05,0.1,0.2,0.4,0.6,0.8,1.0};
 const double val_cent_PbPb[11]={0.05,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0};
 const std::string name_cent_pp[10]={"0-0.95pct","0.95-4.7pct", "4.7-9.5pct", "9.5-14pct","14-19pct","19-28pct","28-38pct", "38-48pct", "48-68pct", "68-100pct"};
 const std::string name_cent_original[4]={"0-10pct","10-30pct", "30-50pct", "50-100pct"};
 const std::string name_cent_pPb[7]={"0-5pct", "5-10pct", "10-20pct", "20-40pct", "40-60pct", "60-80pct", "80-100pct"};
 const std::string name_cent_PbPb[11]={"0-5pct","5-10pct", "10-20pct","20-30pct", "30-40pct","40-50pct","50-60pct","60-70pct","70-80pct","80-90pct","90-100pct"};
 const double binSize_small=0.25;
 const double binSize_log=1.5;
 const double switchBin_x = 1.0;
 const double ptMax = 50.0;
 const double etaMax = 10.0;
 const double yMax = 10.0;

 const std::string core_tag="H";
 const std::string corona_tag="S";



  const int margin = 10;
  const std::string data_directory="data";

  const string default_inputfname = "input";
  const string default_out_fname="hist.txt";
  const int default_nfiles=10000;
  const vector<string> save_settings_fname={"src/Constants.h"};


 const std::string settings_outputfname="settings_yuuka_analysis.txt";
 const std::string default_subdir_name="centrality_cut";
 const std::string fname_eByeInfo="eByeInfo.txt";

 const double dummy = -100.0;




}
#endif