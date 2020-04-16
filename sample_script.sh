##This is the sample script to run any other circuit.
##
## Make sure hgr and dim files are copied to input directory.
##

##Set below variables to respective hgr and dim file#########
input_hgr_file="biomedP.hgr"
input_dim_file="biomedP.dim"
###########################################################

make all
./min_cut.out -hgr_file $input_hgr_file -dim_file $input_dim_file -mode 0 -partition_type 1 -area_constraint 0.05 -verbose 0
