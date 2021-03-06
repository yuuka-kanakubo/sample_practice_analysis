#!/bin/sh

#DIR="../DCCI/data/20210105_PBPB_PT0REF0.9_SIGMA0.5_tau0Unlimited_for_DCCI_CF_T165"
#DIR="../DCCI/data/20210302_PP_PT0REF1.8_SIGMA0.5_tau0Unlimited_for_DCCI_CF_T165_MB_0Kto100K_NOparticlization"
DIR="../pythia8244/default_pythia_mymain/data/20210305_DEFAULT_PP13TEV"
EV="ev"
#EXT="hadronFinal_corecorona_weakStop_wcol.txt"
EXT="default.txt"
outputdir="VNPT_PYTHIA13TeV_PP_etagap0.0"
n=1000000


#Do not modify this.
#---------------------
log_dname="log/"
data_dir="data/"

today=$(date "+%Y%m%d")
log_fname=${today}${outputdir}
if [ ! -d $log_dname ]
then
    echo "Directory "$log_dname" DOES NOT exists." 
    echo "mkdir "$log_dname
    mkdir $log_dname
fi
if [ ! -d $data_dir ]
then
    echo "Directory "$data_dir" DOES NOT exists." 
    mkdir $data_dir
fi
#-------------------------


#Options.
#--------------
#--CentralityCut 9 --CentralityCut_ext hadronFinal_corecorona_weakStop.txt 
# --HI 

./analysis -n $n -outdir ${outputdir} -dir ${DIR} -f ${EV} -ext ${EXT} --INEL_lg_0   --twosub  --vn > ${log_dname}${log_fname}.log  2>&1  &

