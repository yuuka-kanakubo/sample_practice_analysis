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
#include "Fill.h"

using namespace std;


Fill::Fill(shared_ptr<Message> ms_in, Settings::Options options_in, shared_ptr<InfoHist> infohist_in, shared_ptr<Util_func> uf_in):ms(ms_in), options(options_in), infohist(infohist_in), uf(uf_in){};
Fill::~Fill(){};

			void Fill::fill_jets(shared_ptr<Container>& ct){


				Container::EventInfo& EVENT= ct->EVENTINFO;

				//Determine xbin
				//---------------
				double x_val=EVENT.Aj();
				double y_val=1.0;
				if(x_val<constants::x_min || x_val>this->infohist->x_max) return;
				int nx=this->get_cell_index(x_val);

				ct->Hist[nx]+=y_val;
				ct->Hist_x[nx]+=x_val;
				ct->Hist_weight[nx]+=1.0;
				if(ct->max_nx<nx) ct->max_nx=nx;

				ct->SumWeight+=1.0;

			}



			void Fill::fill_twopc_tagged(shared_ptr<Container>& ct){


				Container::EventInfo& EVENT= ct->EVENTINFO;

				//Count particle by particle.
				//----------------------------
				double **Hit1ev;
				Hit1ev = new double *[constants::x_cell_capa];
				for(int i_cell=0; i_cell<constants::x_cell_capa; i_cell++){
					Hit1ev[i_cell] = new double [constants::y_cell_capa];
				}
				for(int i=0; i<constants::x_cell_capa; i++){
					for(int j=0; j<constants::y_cell_capa; j++){
						Hit1ev[i][j]=0.0;
					}
				}
				int max_nx = 0, max_ny = 0;
				int NumPair=0;
				for(int i=0; i<(int)EVENT.part.size(); ++i){

					if(EVENT.part[i].pt<constants::twopc_tagged_leadingptmin) continue;

					for(int j=0; j<(int)EVENT.part.size(); ++j){
						if (i==j) continue;
						string TAG = EVENT.part[j].TAG;
						if(options.get_flag_only_core_associates() && TAG==constants::corona_tag) continue;
						if(options.get_flag_only_corona_associates() && TAG==constants::core_tag) continue;

						double x_val=EVENT.part[i].eta - EVENT.part[j].eta;
						if(x_val<constants::x_min || x_val>this->infohist->x_max) continue;
						int nx=(int)((x_val/this->infohist->d_x)+(std::fabs(constants::x_min)/this->infohist->d_x));

						double y_val=this->getDeltaPhi(EVENT.part[i].phi, EVENT.part[j].phi);
						if(y_val<constants::y_min || y_val>this->infohist->y_max) continue;
						int ny=(int)((y_val/this->infohist->d_y)+(std::fabs(constants::y_min)/this->infohist->d_y));

						if(max_nx<nx) max_nx = nx;
						if(max_ny<ny) max_ny = ny;
						Hit1ev[nx][ny]+=1.0;
						ct->Hist2D_x[nx][ny]+=x_val*EVENT.weight();
						ct->Hist2D_y[nx][ny]+=y_val*EVENT.weight();
						if(ct->max_nx<nx) ct->max_nx=nx;
						if(ct->max_ny<ny) ct->max_ny=ny;
						NumPair++;

					}
				}
				//---------------
				
				for(int nx = 0; nx<max_nx+1; nx++){
					for(int ny = 0; ny<max_ny+1; ny++){
						ct->Hist2D[nx][ny]+=Hit1ev[nx][ny]*EVENT.weight();
						ct->Hist2DPartHit[nx][ny]+=Hit1ev[nx][ny]*EVENT.weight();
					}
				}

				ct->SumWeight+=EVENT.weight();
				ct->SumPair+=((double)NumPair)*EVENT.weight();


				for(int i = 0; i < constants::x_cell_capa; i++) {
					delete[] Hit1ev[i];
				}
				delete[] Hit1ev;

			}
			void Fill::fill_twopc(shared_ptr<Container>& ct){


				Container::EventInfo& EVENT= ct->EVENTINFO;

				//Count particle by particle.
				//----------------------------
				double **Hit1ev;
				Hit1ev = new double *[constants::x_cell_capa];
				for(int i_cell=0; i_cell<constants::x_cell_capa; i_cell++){
					Hit1ev[i_cell] = new double [constants::y_cell_capa];
				}
				for(int i=0; i<constants::x_cell_capa; i++){
					for(int j=0; j<constants::y_cell_capa; j++){
						Hit1ev[i][j]=0.0;
					}
				}
				int max_nx = 0, max_ny = 0;
				int NumPair=0;
				for(int i=0; i<(int)EVENT.part.size(); ++i){
					for(int j=0; j<(int)EVENT.part.size(); ++j){
						if (i==j) continue;

						double x_val=EVENT.part[i].eta - EVENT.part[j].eta;
						if(x_val<constants::x_min || x_val>this->infohist->x_max) continue;
						int nx=(int)((x_val/this->infohist->d_x)+(std::fabs(constants::x_min)/this->infohist->d_x));

						double y_val=this->getDeltaPhi(EVENT.part[i].phi, EVENT.part[j].phi);
						if(y_val<constants::y_min || y_val>this->infohist->y_max) continue;
						int ny=(int)((y_val/this->infohist->d_y)+(std::fabs(constants::y_min)/this->infohist->d_y));

						if(max_nx<nx) max_nx = nx;
						if(max_ny<ny) max_ny = ny;
						Hit1ev[nx][ny]+=1.0;
						ct->Hist2D_x[nx][ny]+=x_val*EVENT.weight();
						ct->Hist2D_y[nx][ny]+=y_val*EVENT.weight();
						if(ct->max_nx<nx) ct->max_nx=nx;
						if(ct->max_ny<ny) ct->max_ny=ny;
						NumPair++;

					}
				}
				//---------------
				
				for(int nx = 0; nx<max_nx+1; nx++){
					for(int ny = 0; ny<max_ny+1; ny++){
						ct->Hist2D[nx][ny]+=Hit1ev[nx][ny]*EVENT.weight();
						ct->Hist2DPartHit[nx][ny]+=Hit1ev[nx][ny]*EVENT.weight();
					}
				}

				ct->SumWeight+=EVENT.weight();
				ct->SumPair+=((double)NumPair)*EVENT.weight();


				for(int i = 0; i < constants::x_cell_capa; i++) {
					delete[] Hit1ev[i];
				}
				delete[] Hit1ev;

			}

			void Fill::fill_Rt(shared_ptr<Container>& ct){

				Container::EventInfo& EVENT= ct->EVENTINFO;

				//Find max pt particle.
				//----------------------------
				double max_pt=-1.0;
				int itrig=-1;
				for(int i=0; i<(int)EVENT.part.size(); ++i){
					if(fabs(EVENT.part[i].pt)<constants::minpt_Rt) continue;
					int ID = EVENT.part[i].id;
					if(!(abs(ID)==constants::id_proton||abs(ID)==constants::id_ch_pion||abs(ID)==constants::id_ch_kaon)) continue;
					if(max_pt<EVENT.part[i].pt) {
						max_pt = EVENT.part[i].pt;
						itrig=i;
					}
				}
				if(itrig<0) {
					return;
				}

				//Get Nt.
				//-------------------------------
				int Nt=0;
				for(int j=0; j<(int)EVENT.part.size(); ++j){
					//Count Nt
					//-----------
					int ID = EVENT.part[j].id;
					if(!(abs(ID)==constants::id_proton||abs(ID)==constants::id_ch_pion||abs(ID)==constants::id_ch_kaon)) continue;
					if(fabs(EVENT.part[j].phi-EVENT.part[itrig].phi)<constants::maxPhi_RtTrans 
							&& fabs(EVENT.part[j].phi-EVENT.part[itrig].phi)>constants::minPhi_RtTrans){
						Nt++;
					}
				}
				//---------------
				//Store 
				//--------
				ct->Nt_eBye.push_back(Nt);
				ct->dNdeta_eBye.push_back(EVENT.Nch());
				ct->weight_eBye.push_back(EVENT.weight());
				ct->SumWeight+=EVENT.weight();
				return;
			}

			void Fill::fill_RtYield(shared_ptr<Container>& ct){

				Container::EventInfo& EVENT= ct->EVENTINFO;

				//Find max pt particle.
				//----------------------------
				double max_pt=-1.0;
				int itrig=-1;
				for(int i=0; i<(int)EVENT.part.size(); ++i){
					int ID = EVENT.part[i].id;
					if(!(abs(ID)==constants::id_proton||abs(ID)==constants::id_ch_pion||abs(ID)==constants::id_ch_kaon)) continue;
					if(fabs(EVENT.part[i].pt)<constants::minpt_Rt) continue;
					if(max_pt<EVENT.part[i].pt) {
						max_pt = EVENT.part[i].pt;
						itrig=i;
					}
				}
				if(itrig<0) {
					return;
				}

				//Get Nt.
				//-------------------------------
				int Nt=0;
				int Ntmin=0;
				int Ntmax=0;
				int Ntcore=0;
				int Ntcorona=0;
				int Nncore=0;
				int Nncorona=0;
				Container::yield TransYield;
				Container::yield TowardYield;
				for(int j=0; j<(int)EVENT.part.size(); ++j){
					//Count Nt (multiplicity in transverse region)
					//----------------------------------------------
					int ID = EVENT.part[j].id;
					if(fabs(EVENT.part[j].phi-EVENT.part[itrig].phi)<constants::maxPhi_RtTrans && fabs(EVENT.part[j].phi-EVENT.part[itrig].phi)>constants::minPhi_RtTrans && (abs(ID)==constants::id_proton||abs(ID)==constants::id_ch_pion||abs(ID)==constants::id_ch_kaon)){
						Nt++;
						if(EVENT.part[j].phi-EVENT.part[itrig].phi>0.0) Ntmin++;
						else Ntmax++;
					}
					if(Ntmin>Ntmax){
						int Nttemp=Ntmin;
						Ntmin=Ntmax;
						Ntmax=Nttemp;
					}

					//Count Nth(yield in transverse region)
					//----------------------------------------------
					if(fabs(EVENT.part[j].phi-EVENT.part[itrig].phi)<constants::maxPhi_RtTrans && fabs(EVENT.part[j].phi-EVENT.part[itrig].phi)>constants::minPhi_RtTrans){
						int ID = EVENT.part[j].id;
						string TAG = EVENT.part[j].TAG;
						if(abs(ID)==constants::id_proton) TransYield.add_ppbar(1.0);
							if(abs(ID)==constants::id_ch_pion)TransYield.add_chpi(1.0); 
							if(abs(ID)==constants::id_ch_kaon)TransYield.add_chkaon(1.0); 
							if(abs(ID)==constants::id_phi)TransYield.add_phi(1.0); 
							if(abs(ID)==constants::id_lambda)TransYield.add_lambda(1.0); 
							if(abs(ID)==constants::id_cascade)TransYield.add_cascade(1.0); 
							if(abs(ID)==constants::id_omega)TransYield.add_omega(1.0); 

							if(TAG == constants::core_tag)Ntcore++;
							else if(TAG == constants::corona_tag)Ntcorona++;

					}
					//Count Nth(yield in towards region)
					//----------------------------------------------
					else if(fabs(EVENT.part[j].phi-EVENT.part[itrig].phi)<constants::minPhi_RtTrans){
						int ID = EVENT.part[j].id;
						string TAG = EVENT.part[j].TAG;
						if(abs(ID)==constants::id_proton) TowardYield.add_ppbar(1.0);
							if(abs(ID)==constants::id_ch_pion)TowardYield.add_chpi(1.0); 
							if(abs(ID)==constants::id_ch_kaon)TowardYield.add_chkaon(1.0); 
							if(abs(ID)==constants::id_phi)TowardYield.add_phi(1.0); 
							if(abs(ID)==constants::id_lambda)TowardYield.add_lambda(1.0); 
							if(abs(ID)==constants::id_cascade)TowardYield.add_cascade(1.0); 
							if(abs(ID)==constants::id_omega)TowardYield.add_omega(1.0); 

							if(TAG == constants::core_tag)Nncore++;
							else if(TAG == constants::corona_tag)Nncorona++;
					}else{

					}
				}
				//---------------
				//Store 
				//--------
				ct->TransYield_eBye.push_back(TransYield);
				ct->TowardYield_eBye.push_back(TowardYield);
				ct->Nt_eBye.push_back(Nt);
				ct->dNdeta_eBye.push_back(EVENT.Nch());
				ct->CoreT_eBye.push_back(Ntcorona);
				ct->CoreN_eBye.push_back(Nncorona);
				ct->Ntmin_eBye.push_back(Ntmin);
				ct->Ntmax_eBye.push_back(Ntmax);
				ct->weight_eBye.push_back(EVENT.weight());
				ct->SumWeight+=EVENT.weight();
				ct->TagEventNum.push_back(ct->CountEv-1);
				return;
			}

			void Fill::fill_twopc1D_taggedInteg(shared_ptr<Container>& ct){

				Container::EventInfo& EVENT= ct->EVENTINFO;

				//Count particle by particle.
				//----------------------------
				int NumPair=0;
				int NumTrig=0;
				double x_val=EVENT.Nch();
				if(x_val<constants::x_min || x_val>this->infohist->x_max) return;
				int nx=(int)((x_val/this->infohist->d_x)+(std::fabs(constants::x_min)/this->infohist->d_x));
				for(int i=0; i<(int)EVENT.part.size(); ++i){
					if(EVENT.part[i].pt<constants::twopc_tagged_leadingptmin) continue;
					NumTrig++;

					//Seeing associates.
					//-------------------------------
					for(int j=0; j<(int)EVENT.part.size(); ++j){
						if(i==j) continue;
						string TAG = EVENT.part[j].TAG;
						if(options.get_flag_only_core_associates() && TAG==constants::corona_tag) continue;
						if(options.get_flag_only_corona_associates() && TAG==constants::core_tag) continue;

						double DeltaPhi=this->getDeltaPhi_twopc1D(EVENT.part[i].phi, EVENT.part[j].phi);
						double DeltaEta=fabs(EVENT.part[i].eta - EVENT.part[j].eta);
						if(options.get_flag__2PCfull() && DeltaEta> constants::DeltaEtaFULL ) continue;
						else if(options.get_flag__2PCnearside() && (DeltaEta>constants::DeltaEtaNS || fabs(DeltaPhi)>constants::DeltaPhiNS)) continue;
						else if(options.get_flag__2PCout() && (DeltaEta<constants::DeltaEtaNS || DeltaEta>constants::DeltaEtaFULL || DeltaPhi<constants::DeltaPhiNS || DeltaPhi>constants::DeltaPhiOUT)) continue;


						ct->Hist[nx]+=1.0*EVENT.weight();
						NumPair++;

					}
				}
				//---------------
				ct->HistHit[nx]+=((double)NumTrig)*EVENT.weight();
				ct->Hist_x[nx]+=x_val*EVENT.weight();
				ct->Hist_weight[nx]+=EVENT.weight();
				ct->SumWeight+=EVENT.weight();
				if(ct->max_nx<nx) ct->max_nx=nx;
				return;
			}

			void Fill::fill_twopc1D_tagged(shared_ptr<Container>& ct){

				Container::EventInfo& EVENT= ct->EVENTINFO;

				//Count particle by particle.
				//----------------------------
				int NumPair=0;
				int NumTrig=0;
				for(int i=0; i<(int)EVENT.part.size(); ++i){
					if(EVENT.part[i].pt<constants::twopc_tagged_leadingptmin) continue;
					NumTrig++;

					//Seeing associates.
					//-------------------------------
					for(int j=0; j<(int)EVENT.part.size(); ++j){
						if(i==j) continue;
						string TAG = EVENT.part[j].TAG;
						if(options.get_flag_only_core_associates() && TAG==constants::corona_tag) continue;
						if(options.get_flag_only_corona_associates() && TAG==constants::core_tag) continue;

						double x_val=this->getDeltaPhi_twopc1D(EVENT.part[i].phi, EVENT.part[j].phi);
						double DeltaEta=fabs(EVENT.part[i].eta - EVENT.part[j].eta);
						if(options.get_flag__2PCfull() && DeltaEta> constants::DeltaEtaFULL) continue;
						else if(options.get_flag__2PCnearside() && DeltaEta>constants::DeltaEtaNS) continue;
						else if(options.get_flag__2PCout() && (DeltaEta<constants::DeltaEtaNS || DeltaEta>constants::DeltaEtaFULL)) continue;

						if(x_val<constants::x_min || x_val>this->infohist->x_max) continue;
						int nx=(int)((x_val/this->infohist->d_x)+(std::fabs(constants::x_min)/this->infohist->d_x));

						ct->Hist[nx]+=1.0*EVENT.weight();
						ct->Hist_x[nx]+=x_val*EVENT.weight();
						ct->Hist_weight[nx]+=EVENT.weight();
						ct->HistHit[nx]++;
						if(ct->max_nx<nx) ct->max_nx=nx;
						NumPair++;

					}
				}
				//---------------
				ct->SumWeight+=EVENT.weight();
				ct->SumPair+=((double)NumPair)*EVENT.weight();
				ct->SumTrig+=((double)NumTrig)*EVENT.weight();
				return;
			}


			void Fill::fill_twopc1D_tagged_1particle(shared_ptr<Container>& ct){

				Container::EventInfo& EVENT= ct->EVENTINFO;

				//Count particle by particle.
				//----------------------------
				int NumPair=0;
				int NumTrig=0;
				int itrig=-1;
				double max_pt=-1.0;
				for(int i=0; i<(int)EVENT.part.size(); ++i){
					if(abs(EVENT.part[i].id)==constants::id_K0S) continue;
					if(max_pt<EVENT.part[i].pt) {
						max_pt = EVENT.part[i].pt;
						itrig=i;
					}
				}
				if(itrig<0) {
					NumTrig++;
				}

				//Seeing K0S.
				//-------------------------------
				for(int j=0; j<(int)EVENT.part.size(); ++j){
					if(itrig<0) {
						break;
					}
					if(abs(EVENT.part[j].id)!=constants::id_K0S) continue;

					double x_val=this->getDeltaPhi_twopc1D(EVENT.part[itrig].phi, EVENT.part[j].phi);
					double DeltaEta=fabs(EVENT.part[itrig].eta - EVENT.part[j].eta);
					if(options.get_flag__2PCfull() && DeltaEta> constants::DeltaEtaFULL) continue;
					else if(options.get_flag__2PCnearside() && DeltaEta>constants::DeltaEtaNS) continue;
					else if(options.get_flag__2PCout() && (DeltaEta<constants::DeltaEtaNS || DeltaEta>constants::DeltaEtaFULL)) continue;

					if(x_val<constants::x_min || x_val>this->infohist->x_max) continue;
					int nx=(int)((x_val/this->infohist->d_x)+(std::fabs(constants::x_min)/this->infohist->d_x));

					ct->Hist[nx]+=1.0*EVENT.weight();
					ct->Hist_x[nx]+=x_val*EVENT.weight();
					ct->Hist_weight[nx]+=EVENT.weight();
					ct->HistHit[nx]++;
					if(ct->max_nx<nx) ct->max_nx=nx;
					NumPair++;

				}
				//---------------
				ct->SumWeight+=EVENT.weight();
				ct->SumPair+=((double)NumPair)*EVENT.weight();
				ct->SumTrig+=((double)NumTrig)*EVENT.weight();
				return;
			}

			void Fill::fill_twopc1D(shared_ptr<Container>& ct){

				Container::EventInfo& EVENT= ct->EVENTINFO;

				//Count particle by particle.
				//----------------------------
				int NumPair=0;
				for(int i=0; i<(int)EVENT.part.size(); ++i){

					for(int j=0; j<(int)EVENT.part.size(); ++j){
						if(i==j) continue;

						double x_val=this->getDeltaPhi_twopc1D(EVENT.part[i].phi, EVENT.part[j].phi);
						double DeltaEta=fabs(EVENT.part[i].eta - EVENT.part[j].eta);
						if(options.get_flag__2PCfull() && DeltaEta>constants::DeltaEtaFULL) continue;
						else if(options.get_flag__2PCnearside() && DeltaEta>constants::DeltaEtaNS) continue;
						else if(options.get_flag__2PCout() && (DeltaEta<constants::DeltaEtaNS || DeltaEta>constants::DeltaEtaFULL)) continue;

						if(x_val<constants::x_min || x_val>this->infohist->x_max) continue;
						int nx=(int)((x_val/this->infohist->d_x)+(std::fabs(constants::x_min)/this->infohist->d_x));

						ct->Hist[nx]+=1.0*EVENT.weight();
						ct->Hist_x[nx]+=x_val*EVENT.weight();
						ct->Hist_weight[nx]+=EVENT.weight();
						ct->HistHit[nx]++;
						if(ct->max_nx<nx) ct->max_nx=nx;
						NumPair++;

					}
				}
				//---------------

				ct->SumWeight+=EVENT.weight();
				ct->SumPair+=((double)NumPair)*EVENT.weight();
				return;
			}


			double Fill::getDeltaPhi(const double phi1, const double phi2){
				double deltaPhi = phi1 - phi2;
				if(deltaPhi<0.0) deltaPhi += 2.0*M_PI;

				if(deltaPhi>=constants::y_min && deltaPhi<=this->infohist->y_max){
					return deltaPhi;
				}else if(deltaPhi<constants::y_min){
					return deltaPhi + 2.0*M_PI;
				}else if(deltaPhi>this->infohist->y_max){
					return deltaPhi-2.0*M_PI;
				}else{
					cout << ":( Error. Something wrong in double getDeltaPhi." << deltaPhi << endl;
					exit(1);
				}

			}
			double Fill::getDeltaPhi_twopc1D(const double phi1, const double phi2){
				//1. Fix range from -2pi<phi<2pi to 0<phi<2pi
				//----------------------------
				double deltaPhi = phi1-phi2;
				if(deltaPhi<0.0) deltaPhi += 2.0*M_PI;

				if(deltaPhi>=constants::x_min && deltaPhi<=this->infohist->x_max){
					return deltaPhi;
				}else if(deltaPhi<constants::x_min){
					return deltaPhi + 2.0*M_PI;
				}else if(deltaPhi>this->infohist->x_max){
					return deltaPhi-2.0*M_PI;
				}else{
					cout << ":( Error. Something wrong in double getDeltaPhi." << deltaPhi << endl;
					exit(1);
				}

			}


			void Fill::fill(shared_ptr<Container>& ct){


				Container::EventInfo& EVENT= ct->EVENTINFO;

				//Determine xbin
				//---------------
				double x_val=constants::dummy;
				if(constants::MODE.find("meanpt")!=string::npos || constants::MODE.find("meanmt")!=string::npos){
					x_val=EVENT.Nch();
					if(x_val<constants::x_min || x_val>this->infohist->x_max) return;
				}
				int nx=(!options.get_flag_HI())? this->get_cell_index(x_val):this->get_cell_index_logplot(x_val);
				//For mean pt PID, assign 1 bin for each centrality.
				//--------------------------------------------------
				if(constants::MODE.find("MeanptPID")!=string::npos) nx=0;


				//Count particle by particle.
				//----------------------------
				for(int j=0; j<(int)EVENT.part.size(); ++j){
					double y_val=constants::dummy;
					if(constants::MODE.find("meanpt")!=string::npos || constants::MODE.find("MeanptPID")!=string::npos){
						y_val = EVENT.part[j].pt;
					}else if(constants::MODE.find("meanmt")!=string::npos){
						y_val = EVENT.part[j].mt - EVENT.part[j].m;
					}else if(constants::MODE.find("mtscaling")!=string::npos){
						x_val=EVENT.part[j].m;
						if(x_val<constants::x_min || x_val>this->infohist->x_max) continue;
						nx=(int)((x_val/this->infohist->d_x)+(std::fabs(constants::x_min)/this->infohist->d_x));

			 			if(EVENT.part[j].id==constants::id_phi){
							uf->checkMassOnShell(EVENT.part[j].m, EVENT.part[j].e, EVENT.part[j].px, EVENT.part[j].py, EVENT.part[j].pz);
						}

						if(!this->fix_ax(EVENT.part[j].id, nx, x_val)) continue;
						y_val = EVENT.part[j].mt - EVENT.part[j].m;
					}
					ct->Hist[nx]+=y_val*EVENT.weight();
					ct->Hist_x[nx]+=x_val*EVENT.weight();
					ct->HistHit[nx]++;
					ct->HistHist[nx]+=pow(y_val,2)*EVENT.weight();
					ct->Hist_weight[nx]+=EVENT.weight();
					if(ct->max_nx<nx) ct->max_nx=nx;

					ct->SumWeight+=EVENT.weight();

				}


			}


			bool Fill::fix_ax(const int id, int &nx, double m){

					//Current 
					//----------
					if(abs(id)==constants::id_ch_pion) nx=0;
					else if(abs(id)==constants::id_ch_kaon) nx=1;
					else if(abs(id)==constants::id_proton) nx=2;
					else if(abs(id)==constants::id_phi) nx=3;
					else if(abs(id)==constants::id_lambda) nx=4;
					else if(abs(id)==constants::id_cascade) nx=5;
					else if(abs(id)==constants::id_omega) nx=6;
					else {
						cout << "continue " << endl;
						return false;
					}
					return true;
			}


			int Fill::get_cell_index(const double x_val_){
				int ncell = (int)((x_val_/this->infohist->d_x)+(fabs(constants::x_min)/this->infohist->d_x));
				//if(constants::MODE.find("cumulant_multi")!=string::npos){
					//if(x_val_>constants::maxNchPP) ncell=6;
				//}
				return  ncell;
			}



			int Fill::get_cell_index_logplot(const double x_val_){

				double x_val=x_val_;
				int ncell=0;


				if(x_val<constants::switchBin_x){
					int ncell=(int) floor((x_val-constants::x_min)/constants::binSize_small);

					//Put small multiplicity events into one bin.
					//---------------------------------------------
					if(constants::MODE.find("cumulant_multi")!=string::npos && x_val<constants::minNchHI) ncell=0;

					return ncell;
				}else{
					//Put small multiplicity events into one bin.
					//---------------------------------------------
					if(constants::MODE.find("cumulant_multi")!=string::npos && x_val<constants::minNchHI){
						ncell=0;
						return ncell;
					}

					int ncell_log=1;
					int ncell_start=floor(constants::switchBin_x/constants::binSize_small)-1;
					while(ncell_log<constants::x_cell_capa){

						if(x_val<pow(constants::binSize_log,ncell_log)+constants::switchBin_x){

							ncell=ncell_start+ncell_log;
							break;

						}
						ncell_log++;

					}
					return ncell;
				}
			}



			void Fill::fill_vn4multi(shared_ptr<Container>& ct){


				Container::EventInfo& EVENT= ct->EVENTINFO;

				if((int)EVENT.part.size()<4) return;

				//Determine xbin
				//---------------
				double x_val=EVENT.Nch();
				if(x_val<constants::x_min || x_val>this->infohist->x_max) return;
				int nx=(!options.get_flag_HI())? this->get_cell_index(x_val):this->get_cell_index_logplot(x_val);


				//Count particle by particle.
				//----------------------------
				std::complex<double> Qvec_tot=constants::initialval_comp;
				std::complex<double> Qvec2_tot=constants::initialval_comp;
				std::complex<double> n_coeff (this->infohist->N_coeff, 0.0);
				std::complex<double> n2_coeff (this->infohist->N_coeff*2.0, 0.0);
				for(int j=0; j<(int)EVENT.part.size(); ++j){
					std::complex<double> phi_ (EVENT.part[j].phi,0.0);
					std::complex<double> Qvec=exp(constants::i_img*n_coeff*phi_);
					std::complex<double> Qvec2=exp(constants::i_img*n2_coeff*phi_);
					Qvec_tot += Qvec;
					Qvec2_tot += Qvec2;
				}

				double totN = (double)EVENT.part.size();
				double corr4 = real(Qvec_tot*Qvec_tot*conj(Qvec_tot)*conj(Qvec_tot) 
						- Qvec_tot * conj(Qvec_tot)*(4.0*totN-8.0)
						-conj(Qvec2_tot)* pow(Qvec_tot,2) - Qvec2_tot* conj(pow(Qvec_tot,2)) 
						+  Qvec2_tot*conj(Qvec2_tot) - 6.0*totN +2.0*pow(totN,2))/(pow(totN,4)-6.0*pow(totN,3)+11.0*pow(totN,2)-6.0*totN);

				double corr4_img = imag(Qvec_tot*Qvec_tot*conj(Qvec_tot)*conj(Qvec_tot) 
						- Qvec_tot * conj(Qvec_tot)*(4.0*totN-8.0)
						-conj(Qvec2_tot)* pow(Qvec_tot,2) - Qvec2_tot* conj(pow(Qvec_tot,2)) 
						+  Qvec2_tot*conj(Qvec2_tot) - 6.0*totN +2.0*pow(totN,2))/(pow(totN,4)-6.0*pow(totN,3)+11.0*pow(totN,2)-6.0*totN);

				//Obtain 2particle correlation
				//-------------------------------
				double squared_Qvec = real(Qvec_tot * conj(Qvec_tot));
				double corr2 = (squared_Qvec-totN)/(totN*(totN-1.0));


				ct->Hist[nx]+=corr4*EVENT.weight();
				ct->Hist_sub[nx]+=corr2*EVENT.weight();
				ct->Hist_x[nx]+=x_val*EVENT.weight();
				ct->HistHit[nx]++;
				ct->HistHist[nx]+=corr4*corr4*EVENT.weight();
				ct->HistHist_sub[nx]+=corr2*corr2*EVENT.weight();
				ct->Hist_weight[nx]+=EVENT.weight();
				ct->Hist_img_Qvec[nx]+=corr4_img*EVENT.weight();
				if(ct->max_nx<nx) ct->max_nx=nx;

				ct->SumWeight+=EVENT.weight();

			}



			void Fill::fill_vnmulti(shared_ptr<Container>& ct){


				Container::EventInfo& EVENT= ct->EVENTINFO;

				if((int)EVENT.part.size()<2) return;

				//Determine xbin
				//---------------
				double x_val=EVENT.Nch();
				if(x_val<constants::x_min || x_val>this->infohist->x_max) return;
				int nx=(!options.get_flag_HI())? this->get_cell_index(x_val):this->get_cell_index_logplot(x_val);

				//Count particle by particle.
				//----------------------------
				std::complex<double> Qvec_tot=constants::initialval_comp;
				std::complex<double> n_coeff (this->infohist->N_coeff, 0.0);
				for(int j=0; j<(int)EVENT.part.size(); ++j){
					std::complex<double> phi_ (EVENT.part[j].phi,0.0);
					std::complex<double> Qvec=exp(constants::i_img*n_coeff*phi_);
					Qvec_tot += Qvec;
				}
				double squared_Qvec = real(Qvec_tot * conj(Qvec_tot));
				double squared_Qvec_img = imag(Qvec_tot * conj(Qvec_tot));
				double totN = (double)EVENT.part.size();
				double corr = (squared_Qvec-totN)/(totN*(totN-1.0));

				ct->Hist[nx]+=corr*EVENT.weight();
				ct->Hist_x[nx]+=x_val*EVENT.weight();
				ct->HistHit[nx]++;
				ct->HistHist[nx]+=corr*corr*EVENT.weight();
				ct->Hist_weight[nx]+=EVENT.weight();
				ct->Hist_img_Qvec[nx]+=squared_Qvec_img*EVENT.weight();
				if(ct->max_nx<nx) ct->max_nx=nx;

				ct->SumWeight+=EVENT.weight();

			}





			void Fill::fill_vneta(shared_ptr<Container>& ct){


				Container::EventInfo& EVENT= ct->EVENTINFO;

				//Count particle by particle.
				//----------------------------
				std::complex<double> Qvec_tot[constants::x_cell_capa]={};
				double hit[constants::x_cell_capa]={};
				for(int i=0; i<constants::x_cell_capa; i++){
					Qvec_tot[i]=constants::initialval_comp;
					hit[i]=0.0;
				}
				std::complex<double> n_coeff (this->infohist->N_coeff, 0.0);
				for(int j=0; j<(int)EVENT.part.size(); ++j){

					//Determine xbin
					//---------------
					double x_val=EVENT.part[j].eta;
					if(x_val<constants::x_min || x_val>this->infohist->x_max) continue;
					int nx=(int)((x_val/this->infohist->d_x)+(fabs(constants::x_min)/this->infohist->d_x));

					std::complex<double> phi_ (EVENT.part[j].phi,0.0);
					std::complex<double> Qvec=exp(constants::i_img*n_coeff*phi_);
					Qvec_tot[nx] += Qvec;
					//      cout << "Qvec " << Qvec << endl;
					hit[nx]++;
					ct->Hist_x[nx]+=x_val;
					ct->HistPartHit[nx]++;

					if(ct->max_nx<nx) ct->max_nx=nx;

				}

				for(int nx=0; nx<ct->max_nx+constants::margin; nx++){
					double squared_Qvec = real(Qvec_tot[nx] * conj(Qvec_tot[nx]));
					double squared_Qvec_img = imag(Qvec_tot[nx] * conj(Qvec_tot[nx]));
					double totN = hit[nx];
					if(totN<2) continue;
					double corr = (squared_Qvec-totN)/(totN*(totN-1.0));

					ct->Hist[nx]+=corr*EVENT.weight();
					ct->HistHit[nx]++;
					ct->HistHist[nx]+=corr*corr*EVENT.weight();
					ct->Hist_weight[nx]+=EVENT.weight();
					ct->Hist_img_Qvec[nx]+=squared_Qvec_img*EVENT.weight();

					ct->SumWeight+=EVENT.weight();
				}


			}


			void Fill::fill_vnpt(shared_ptr<Container>& ct){


				Container::EventInfo& EVENT= ct->EVENTINFO;



				//Count particle by particle.
				//----------------------------
				std::complex<double> Qvec_tot[constants::x_cell_capa]={};
				double hit[constants::x_cell_capa]={};
				for(int i=0; i<constants::x_cell_capa; i++){
					Qvec_tot[i]=constants::initialval_comp;
					hit[i]=0.0;
				}
				std::complex<double> n_coeff (this->infohist->N_coeff, 0.0);
				for(int j=0; j<(int)EVENT.part.size(); ++j){

					//Determine xbin
					//---------------
					double x_val=EVENT.part[j].pt;
					if(x_val<constants::x_min || x_val>this->infohist->x_max) continue;
					int nx=(int)((x_val/this->infohist->d_x)+(fabs(constants::x_min)/this->infohist->d_x));

					std::complex<double> phi_ (EVENT.part[j].phi,0.0);
					std::complex<double> Qvec=exp(constants::i_img*n_coeff*phi_);
					Qvec_tot[nx] += Qvec;
					//      cout << "Qvec " << Qvec << endl;
					hit[nx]++;
					ct->Hist_x[nx]+=x_val;
					ct->HistPartHit[nx]++;

					if(ct->max_nx<nx) ct->max_nx=nx;

				}

				for(int nx=0; nx<ct->max_nx+constants::margin; nx++){
					double squared_Qvec = real(Qvec_tot[nx] * conj(Qvec_tot[nx]));
					double squared_Qvec_img = imag(Qvec_tot[nx] * conj(Qvec_tot[nx]));
					double totN = hit[nx];
					if(totN<2) continue;
					double corr = (squared_Qvec-totN)/(totN*(totN-1.0));

					ct->Hist[nx]+=corr*EVENT.weight();
					ct->HistHit[nx]++;
					ct->HistHist[nx]+=corr*corr*EVENT.weight();
					ct->Hist_weight[nx]+=EVENT.weight();
					ct->Hist_img_Qvec[nx]+=squared_Qvec_img*EVENT.weight();

					ct->SumWeight+=EVENT.weight();
				}


			}




			//-------2sub


			void Fill::fill_vnpt_2sub(shared_ptr<Container>& ct){


				Container::EventInfo& EVENT= ct->EVENTINFO;

				if((int)EVENT.part.size()<2) return;


				//Count particle by particle.
				//----------------------------
				std::complex<double> Qvec_tot_A[constants::x_cell_capa]={};
				std::complex<double> Qvec_tot_B[constants::x_cell_capa]={};
				double hit_A[constants::x_cell_capa]={};
				double hit_B[constants::x_cell_capa]={};
				for(int i=0; i<constants::x_cell_capa; i++){
					Qvec_tot_A[i]=constants::initialval_comp;
					Qvec_tot_B[i]=constants::initialval_comp;
					hit_A[i]=0.0;
					hit_B[i]=0.0;
				}
				std::complex<double> n_coeff (this->infohist->N_coeff, 0.0);
				for(int j=0; j<(int)EVENT.part.size(); ++j){

					//Determine xbin
					//---------------
					double x_val=EVENT.part[j].pt;
					if(x_val<constants::x_min || x_val>this->infohist->x_max) continue;
					int nx=(int)((x_val/this->infohist->d_x)+(fabs(constants::x_min)/this->infohist->d_x));

					std::complex<double> phi_ (EVENT.part[j].phi,0.0);
					std::complex<double> Qvec=exp(constants::i_img*n_coeff*phi_);
					if(EVENT.part[j].eta<-0.0){
						Qvec_tot_A[nx] += Qvec;
						hit_A[nx]++;
					}else if(EVENT.part[j].eta>0.0){
						Qvec_tot_B[nx] += Qvec;
						hit_B[nx]++;
					}

					if(ct->max_nx<nx) ct->max_nx=nx;

				}

				for(int nx=0; nx<ct->max_nx+constants::margin; nx++){
					double squared_Qvec = real(Qvec_tot_A[nx] * conj(Qvec_tot_B[nx]));
					double squared_Qvec_img = imag(Qvec_tot_A[nx] * conj(Qvec_tot_B[nx]));
					if(hit_A[nx]==0.0 || hit_B[nx]==0.0) continue;
					double corr = (squared_Qvec)/(hit_A[nx]*hit_B[nx]);

					ct->Hist[nx]+=corr*EVENT.weight();
					ct->HistHit[nx]++;
					ct->HistHist[nx]+=corr*corr*EVENT.weight();
					ct->Hist_weight[nx]+=EVENT.weight();
					ct->Hist_img_Qvec[nx]+=squared_Qvec_img*EVENT.weight();

					ct->SumWeight+=EVENT.weight();
				}


			}





			void Fill::fill_vnmulti_2sub(shared_ptr<Container>& ct){


				Container::EventInfo& EVENT= ct->EVENTINFO;


				//Determine xbin
				//---------------
				double x_val=EVENT.Nch();
				if(x_val<constants::x_min || x_val>this->infohist->x_max) return;
				int nx=(!options.get_flag_HI())? this->get_cell_index(x_val):this->get_cell_index_logplot(x_val);

				//Count particle by particle.
				//----------------------------
				std::complex<double> Qvec_tot_A=constants::initialval_comp;
				std::complex<double> Qvec_tot_B=constants::initialval_comp;
				std::complex<double> n_coeff (this->infohist->N_coeff, 0.0);
				double hit_A=0.0, hit_B=0.0;
				for(int j=0; j<(int)EVENT.part.size(); ++j){
					std::complex<double> phi_ (EVENT.part[j].phi,0.0);
					std::complex<double> Qvec=exp(constants::i_img*n_coeff*phi_);
					if(EVENT.part[j].eta<-(constants::etaGap/2.0)){
						Qvec_tot_A += Qvec;
						hit_A++;
					}else if(EVENT.part[j].eta>(constants::etaGap/2.0)){
						Qvec_tot_B += Qvec;
						hit_B++;
					}
				}
				double squared_Qvec = real(Qvec_tot_A * conj(Qvec_tot_B));
				double squared_Qvec_img = imag(Qvec_tot_A * conj(Qvec_tot_B));

				if(hit_A==0.0 || hit_B==0.0) return;

				double corr = (squared_Qvec)/(hit_A*hit_B);

				ct->Hist[nx]+=corr*EVENT.weight();
				ct->Hist_x[nx]+=x_val*EVENT.weight();
				ct->HistHit[nx]++;
				ct->HistHist[nx]+=corr*corr*EVENT.weight();
				ct->Hist_weight[nx]+=EVENT.weight();
				ct->Hist_img_Qvec[nx]+=squared_Qvec_img*EVENT.weight();
				if(ct->max_nx<nx) ct->max_nx=nx;

				ct->SumWeight+=EVENT.weight();

			}


			void Fill::fill_vn4multi_2sub(shared_ptr<Container>& ct){


				Container::EventInfo& EVENT= ct->EVENTINFO;


				//Determine xbin
				//---------------
				double x_val=EVENT.Nch();
				if(x_val<constants::x_min || x_val>this->infohist->x_max) return;
				int nx=(!options.get_flag_HI())? this->get_cell_index(x_val):this->get_cell_index_logplot(x_val);

				//Count particle by particle.
				//----------------------------
				std::complex<double> Qvec_tot_A=constants::initialval_comp;
				std::complex<double> Qvec_tot_B=constants::initialval_comp;
				std::complex<double> Qvec2_tot_A=constants::initialval_comp;
				std::complex<double> Qvec2_tot_B=constants::initialval_comp;
				std::complex<double> n_coeff (this->infohist->N_coeff, 0.0);
				std::complex<double> n2_coeff (this->infohist->N_coeff*2.0, 0.0);
				double hit_A=0.0, hit_B=0.0;
				for(int j=0; j<(int)EVENT.part.size(); ++j){
					std::complex<double> phi_ (EVENT.part[j].phi,0.0);
					std::complex<double> Qvec=exp(constants::i_img*n_coeff*phi_);
					std::complex<double> Qvec2=exp(constants::i_img*n2_coeff*phi_);
					if(EVENT.part[j].eta<-(constants::etaGap/2.0)){
						Qvec_tot_A += Qvec;
						Qvec2_tot_A += Qvec2;
						hit_A++;
					}else if(EVENT.part[j].eta>(constants::etaGap/2.0)){
						Qvec_tot_B += Qvec;
						Qvec2_tot_B += Qvec2;
						hit_B++;
					}
				}
				double squared_Qvec = real((pow(Qvec_tot_A,2) -  Qvec2_tot_A)* conj(pow(Qvec_tot_B,2) - Qvec2_tot_B));
				double squared_Qvec_img = imag((pow(Qvec_tot_A,2) -  Qvec2_tot_A)* conj(pow(Qvec_tot_B,2) - Qvec2_tot_B));

				if(hit_A<=1.0 || hit_B<=1.0) return;

				double corr4 = (squared_Qvec)/(hit_A*(hit_A-1)*hit_B*(hit_B-1));

				//Obtain 2particle correlation
				//------------------------------
				double squared_Qvec_2part = real(Qvec_tot_A * conj(Qvec_tot_B));
				double corr2 = (squared_Qvec_2part)/(hit_A*hit_B);


				ct->Hist[nx]+=corr4*EVENT.weight();
				ct->Hist_sub[nx]+=corr2*EVENT.weight();
				ct->Hist_x[nx]+=x_val*EVENT.weight();
				ct->HistHit[nx]++;
				ct->HistHist[nx]+=corr4*corr4*EVENT.weight();
				ct->HistHist_sub[nx]+=corr2*corr2*EVENT.weight();
				ct->Hist_weight[nx]+=EVENT.weight();
				ct->Hist_img_Qvec[nx]+=squared_Qvec_img*EVENT.weight();
				if(ct->max_nx<nx) ct->max_nx=nx;

				ct->SumWeight+=EVENT.weight();

			}



			//----------3sub

			void Fill::fill_vn4multi_3sub(shared_ptr<Container>& ct){


				Container::EventInfo& EVENT= ct->EVENTINFO;


				//Determine xbin
				//---------------
				double x_val=EVENT.Nch();
				if(x_val<constants::x_min || x_val>this->infohist->x_max) return;
				int nx=(!options.get_flag_HI())? this->get_cell_index(x_val):this->get_cell_index_logplot(x_val);

				//Count particle by particle.
				//----------------------------
				std::complex<double> Qvec_tot_A=constants::initialval_comp;
				std::complex<double> Qvec_tot_B=constants::initialval_comp;
				std::complex<double> Qvec_tot_C=constants::initialval_comp;
				std::complex<double> Qvec2_tot_A=constants::initialval_comp;
				std::complex<double> Qvec2_tot_B=constants::initialval_comp;
				std::complex<double> Qvec2_tot_C=constants::initialval_comp;
				std::complex<double> n_coeff (this->infohist->N_coeff, 0.0);
				std::complex<double> n2_coeff (this->infohist->N_coeff*2.0, 0.0);
				double hit_A=0.0, hit_B=0.0, hit_C=0.0;
				for(int j=0; j<(int)EVENT.part.size(); ++j){
					std::complex<double> phi_ (EVENT.part[j].phi,0.0);
					std::complex<double> Qvec=exp(constants::i_img*n_coeff*phi_);
					std::complex<double> Qvec2=exp(constants::i_img*n2_coeff*phi_);
					if(EVENT.part[j].eta<constants::etaA_3sub){
						Qvec_tot_B += Qvec;
						Qvec2_tot_B += Qvec2;
						hit_B++;
					}else if(EVENT.part[j].eta<=constants::etaB_3sub){
						Qvec_tot_A += Qvec;
						Qvec2_tot_A += Qvec2;
						hit_A++;
					}else{
						Qvec_tot_C += Qvec;
						Qvec2_tot_C += Qvec2;
						hit_C++;
					}
				}
				double squared_Qvec = real((pow(Qvec_tot_A,2)-Qvec2_tot_A)*conj(Qvec_tot_B)*conj(Qvec_tot_C));
				double squared_Qvec_img = imag((pow(Qvec_tot_A,2)-Qvec2_tot_A)*conj(Qvec_tot_B)*conj(Qvec_tot_C));

				if(hit_A<=1.0 || hit_B<=0.0|| hit_C<=0.0) return;

				double corr4 = (squared_Qvec)/(hit_A*(hit_A-1)*hit_B*hit_C);

				//Obtain 2particle correlation btw A and B.
				//----------------------------------------
				double squared_Qvec_2partAB = real(Qvec_tot_A * conj(Qvec_tot_B));
				double corr2_AB = (squared_Qvec_2partAB)/(hit_A*hit_B);


				//Obtain 2particle correlation btw A and C.
				//----------------------------------------
				double squared_Qvec_2partAC = real(Qvec_tot_A * conj(Qvec_tot_C));
				double corr2_AC = (squared_Qvec_2partAC)/(hit_A*hit_C);


				ct->Hist[nx]+=corr4*EVENT.weight();
				ct->Hist_sub[nx]+=corr2_AB*EVENT.weight();
				ct->Hist_subsub[nx]+=corr2_AC*EVENT.weight();
				ct->Hist_x[nx]+=x_val*EVENT.weight();
				ct->HistHit[nx]++;
				ct->HistHist[nx]+=corr4*corr4*EVENT.weight();
				ct->HistHist_sub[nx]+=corr2_AB*corr2_AB*EVENT.weight();
				ct->HistHist_subsub[nx]+=corr2_AC*corr2_AC*EVENT.weight();
				ct->Hist_weight[nx]+=EVENT.weight();
				ct->Hist_img_Qvec[nx]+=squared_Qvec_img*EVENT.weight();
				if(ct->max_nx<nx) ct->max_nx=nx;

				ct->SumWeight+=EVENT.weight();

			}








