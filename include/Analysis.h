// -*- mode:c++ -*-
#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <complex>
#include <math.h>
#include "Constants.h"
#include "Util_func.h"
#include "Container.h"
#include "Message.h"
#include "LogSettings.h"
#include "Settings.h"
#include "CentralityCut.h"
#include "ReadIn.h"
#include "Fill.h"
#include "Stat.h"
#include "Write.h"
#include "InfoHist.h"

using namespace std;


class Analysis{

	private:


		shared_ptr<Util_func> uf;
		shared_ptr<Message> ms;  
		shared_ptr<ReadIn> read;
		shared_ptr<Fill> fill;
		shared_ptr<Stat> stat;
		shared_ptr<Write> write;

		Settings::Options options;
		LogSettings log;


		//Maximum value for histgram
		//-------------------------
		shared_ptr<InfoHist> infohist;

	public:

		Analysis(const Settings::Options options_in, LogSettings log_in): options(options_in), log(log_in){
			infohist = make_shared<InfoHist>(constants::x_max, constants::y_max, constants::d_x, constants::d_y, 2.0);
			ms = make_shared<Message>();
			uf = make_shared<Util_func>();
			if(options.get_flag_HI()){
				cout << ":) HI option is called.\n  Maximum value of xaxis is adjusted to HI data." << endl;
				infohist->x_max=constants::x_max_HI;
				infohist->d_x=constants::d_x_HI;
				infohist->y_max=constants::y_max_HI;
				infohist->d_y=constants::d_y_HI;
			}
			if(options.get_flag_set_Ncoeff()){
				infohist->N_coeff = options.get_Ncoeff();
			}
			read = make_shared<ReadIn>(this->ms, this->options);
			fill = make_shared<Fill>(this->ms, this->options, this->infohist, this->uf);
			stat = make_shared<Stat>(this->ms, this->options, this->infohist, this->uf);
			write = make_shared<Write>(this->ms, this->options, this->infohist, this->uf);

			this->ana();
		};
		~Analysis(){};






			int ana(){

				//Start Centrality Cut.
				//-----------------------
				int nCent=1;
				vector<EbyeInfo> eBye_CentCut;
				if(options.get_flag_CentralityCut()){
					CentralityCut CentCut(eBye_CentCut, options);
					nCent=(int)options.name_cent.size();
				}


				//Centrality Loop
				for(int iCent=0; iCent<nCent; iCent++){

					if(options.get_flag_CentralityCut())cout << ":)Start analyzing centrality " << options.name_cent[iCent] << "." << endl;



					auto ct = make_shared<Container>();

					//Event Loop
					for(int i=0; i<options.get_nfile(); ++i){
						if(!(i%1000)) ms->read(i);
						ct->CountEv++;
						std::stringstream ss;
						ss << options.get_dir_name() << "/" << options.get_f_name() << setw(9) << setfill('0') << i << "/" << options.get_ext_name();
						std::string inputpath = ss.str();


						//Centrality cut.
						//---------------
						if(options.get_flag_CentralityCut()){
							if(eBye_CentCut[i].get_V0M_class()!=iCent) continue;
						}


						if(!options.get_flag_CentralityCut()){
							if(options.get_flag_INEL_lg_0()){
								eByeInSettings ebe;
								eByeInSettings::eByeMulti Multi(options, inputpath);
								if(!Multi.ebye.multiplicity_INEL_lg_0) continue;
								ebe.print_eByeInfo(i,Multi);
							}else if(options.get_flag_3outof3_trigger()){
								eByeInSettings ebe;
								eByeInSettings::eByeMulti Multi(options, inputpath);
								if(!Multi.ebye.trig_3outof3) continue;
								ebe.print_eByeInfo(i,Multi);
							}else if(options.get_flag_2outof3_trigger()){
								eByeInSettings ebe;
								eByeInSettings::eByeMulti Multi(options, inputpath);
								if(!Multi.ebye.trig_2outof3) continue;
								ebe.print_eByeInfo(i,Multi);
							}else if(options.get_flag_ATLAS_cut()){
								eByeInSettings ebe;
								eByeInSettings::eByeMulti Multi(options, inputpath);
								if(!Multi.ebye.ATLAS_cut) continue;
								ebe.print_eByeInfo(i,Multi);
							}else if(options.get_flag_VZEROAND_trigger()){
								eByeInSettings ebe;
								eByeInSettings::eByeMulti Multi(options, inputpath);
								if(!Multi.ebye.trig_VZEROAND) continue;
								ebe.print_eByeInfo(i,Multi);
							}else{
								eByeInSettings ebe;
								eByeInSettings::eByeMulti Multi(options, inputpath);
								ebe.print_eByeInfo(i,Multi);
							}
						}



						//Read events
						//---------------
						if(constants::MODE.find("JET_PRAC")!=std::string::npos){
							if(!read->read_jetinfo(inputpath, ct)) continue;
						}else{
							if(!read->read(inputpath, ct)) continue;
						}
						if(constants::MODE.find("cumulant_multi")!=std::string::npos){
							if(options.get_flag_2subevent()){ 
								if(options.get_flag__4particle()){
									fill->fill_vn4multi_2sub(ct); 
								}else{		
									fill->fill_vnmulti_2sub(ct); 
								}              
							}else if(options.get_flag_3subevent()){
								if(options.get_flag__4particle()){
									fill->fill_vn4multi_3sub(ct); 
								}else{		
									cout << "ERROR :( Option is wrong. --threesub should be used with --4particle." << endl;
									exit(1);
								}              
							}else{
								if(options.get_flag__4particle()){
									fill->fill_vn4multi(ct);
								}else{
									fill->fill_vnmulti(ct);
								}
							}
						}else if(constants::MODE.find("cumulant_pt")!=std::string::npos){
							if(options.get_flag_2subevent()) fill->fill_vnpt_2sub(ct); 
							else fill->fill_vnpt(ct);
						}else if(constants::MODE.find("cumulant_eta")!=std::string::npos){
							fill->fill_vneta(ct);
						}else if(constants::MODE.find("twopc2D")!=std::string::npos){
							if(options.get_flag_tagged()) fill->fill_twopc_tagged(ct); 
							else fill->fill_twopc(ct);
						}else if(constants::MODE.find("twopc1D")!=std::string::npos){
							if(options.get_flag_tagged()) fill->fill_twopc1D_tagged(ct);
							//this->fill_twopc1D_tagged_1particle(ct); 
							else fill->fill_twopc1D(ct);
						}else if(constants::MODE.find("twopcInteg")!=string::npos){
							fill->fill_twopc1D_taggedInteg(ct);
						}else if(constants::MODE.find("JET_PRAC")!=std::string::npos){
							fill->fill_jets(ct);
						}else if(constants::MODE.find("Rt_spectra")!=string::npos){
							fill->fill_Rt(ct);
						}else if(constants::MODE.find("Rt_yield")!=string::npos){
							fill->fill_RtYield(ct);
						}else{
							fill->fill(ct);
						}

					}//Event loop

					if(constants::MODE.find("cumulant_eta")!=string::npos || constants::MODE.find("cumulant_multi")!=string::npos || constants::MODE.find("cumulant_pt")!=string::npos){ 
						stat->stat_flow(ct);
					}else if(constants::MODE.find("JET_PRAC")!=string::npos){
						stat->stat_jets(ct);
					}else if(constants::MODE.find("twopc2D")!=string::npos){
						stat->stat_twopc(ct);
					}else if(constants::MODE.find("twopc1D")!=string::npos){
						stat->stat_twopc1D(ct);
					}else if(constants::MODE.find("Rt_spectra")!=string::npos){
						stat->stat_Rt(ct);
					}else if(constants::MODE.find("Rt_yield")!=string::npos){
						stat->stat_RtYield(ct);
					}else{
						stat->stat(ct);
					}


					//Making output directory name
					//-----------------------------
					std::string generated_directory_name;
					if(options.get_flag_CentralityCut()){
						if(iCent==0){
							generated_directory_name=uf->get_output_directory(options.get_out_directory_name());
							uf->make_output_directory(generated_directory_name);
						}
						generated_directory_name=uf->get_output_directory(options.get_out_directory_name()+="/Cent"+options.name_cent[iCent]);
					}else generated_directory_name=uf->get_output_directory(options.get_out_directory_name());
					uf->make_output_directory(generated_directory_name);


					//Output
					//------
					if(constants::MODE.find("Rt_yield")!=string::npos){
						if(!write->write_RtYield(generated_directory_name, ct)) return 1;
					}else {
						if(!write->write(generated_directory_name, ct)) return 1;
					}
					log.archive_settings(generated_directory_name);

				}//Centrality loop


				return 0;
			}



		};