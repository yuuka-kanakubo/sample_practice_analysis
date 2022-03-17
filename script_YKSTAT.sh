#!/bin/sh

DIR="../DCCI/data/PBPB2760GEV_transSmear0.6_sigma0.3_pT01.0"
EV="ev"
EXT="hadronFinal_corecorona_weakStop.txt"
#EXT="default.txt"
outputdir="PBPB_V0Mcut_WOCOL_CORE_CORONAtrigger"
n=130000


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
# --CentralityCut 9 \
# --CentralityCut_ext hadronFinal_corecorona_weakStop.txt \
# --HI \
# --only_corona \
# --INEL_lg_0 \
# --twosub  \
# --threesub  \
# --4particle \
# --tagged \
# --2PCfull \
# --2PCnearside \
# --2PCout \
# --setNcoeff 3 \
# --only_corona_associates \

./YKSTAT \
 -n $n -outdir ${outputdir} -dir ${DIR} -f ${EV} -ext ${EXT} \
 --SB_CMS \
 --CentralityCut 4 \
 --only_corona_triggers \
 --only_core_associates \
 > ${log_dname}${log_fname}.log  2>&1  &
