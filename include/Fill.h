#ifndef FILL
#define FILL
#include "Constants.h"
#include "Util_func.h"
#include "Container.h"
#include "Message.h"
#include "LogSettings.h"
#include "Settings.h"
#include "CentralityCut.h"
#include "Rndom.h"

class Fill{

private:

	int N_iCentEv;
	int NsuspiciousParticle;
	double x_edge_min;
	double x_edge_max;
	double y_edge_min;
	double y_edge_max;

	shared_ptr<Message> ms;  
		Options options;
		shared_ptr<Util_func> uf;
		shared_ptr<Rndom> rndom;
		vector<double> xMin_cstm, xMax_cstm;


			bool fix_ax(const int id, int &nx, double m);
			int get_cell_index(const double x_val_);
			int get_cell_index_y(const double y_val_);
			int get_cell_index_logplot(const double x_val_);
			double getDeltaPhi(const double phi1, const double phi2);
			double getDeltaPhi_twopc1D(const double phi1, const double phi2);
			void SetCustomBin(){
                              this->xMax_cstm=options.xMax_cstm;
                              this->xMin_cstm=options.xMin_cstm;
			}
			int get_cell_index_cstm(const double val);

			vector<Container::ParticleInfo> select_N_RndomEv (const vector<EbyeInfo>& eBye_All);

public:

Fill(shared_ptr<Message> ms_in, Options options_in, shared_ptr<Util_func> uf);
~Fill();


void nextCent(shared_ptr<Rndom> rndom);
void fill_jets(shared_ptr<Container>& ct);
			void fill_twopc_B_CMS(shared_ptr<Container>& ct, const vector<EbyeInfo>& eBye_All);
			void fill_twopc(shared_ptr<Container>& ct);
			void fill_2D(shared_ptr<Container>& ct);
			void fill_Rt(shared_ptr<Container>& ct);
			void fill_RtYield(shared_ptr<Container>& ct);
			void fill_twopc1D_taggedInteg(shared_ptr<Container>& ct);
			void fill_twopc1D_tagged(shared_ptr<Container>& ct);
			void fill_twopc1D_tagged_1particle(shared_ptr<Container>& ct);
			void fill_twopc1D(shared_ptr<Container>& ct);
			void fill(shared_ptr<Container>& ct, const double dNdeta, const int bin);
			void fill_TimeLapse(shared_ptr<Container>& ct);
			void fill_vn4multi(shared_ptr<Container>& ct, const double Nch);
			void fill_vnmulti(shared_ptr<Container>& ct);
			void fill_vneta(shared_ptr<Container>& ct);
			void fill_vnpt(shared_ptr<Container>& ct);
			void fill_vnpt_2sub(shared_ptr<Container>& ct);
			void fill_vnmulti_2sub(shared_ptr<Container>& ct);
			void fill_vn4multi_2sub(shared_ptr<Container>& ct);
			void fill_vn4multi_3sub(shared_ptr<Container>& ct);

};
#endif
