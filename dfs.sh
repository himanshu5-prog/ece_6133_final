make all

##Note: All circuit input files are in input directory#############################################################
echo "Placement using DFS partitioning starts"
### Running placement on biomedP ###################################################################################
echo "------------------Processing biomedP-------------------------------------------------------------------------"
./min_cut.out -hgr_file biomedP.hgr -dim_file biomedP.dim -mode 0 -partition_type 0 -area_constraint 0.05 -verbose 0
./min_cut.out -hgr_file biomedP.hgr -dim_file biomedP.dim -mode 1 -partition_type 0 -area_constraint 0.05 -verbose 0
./min_cut.out -hgr_file biomedP.hgr -dim_file biomedP.dim -mode 2 -partition_type 0 -area_constraint 0.05 -verbose 0
echo "-----------------Done processing biomedP---------------------------------------------------------------------"
####################################################################################################################

### Running placement on p2 ###############################################################################
echo "------------------Processing p2-------------------------------------------------------------------------"
./min_cut.out -hgr_file p2.hgr -dim_file p2.dim -mode 0 -partition_type 0 -area_constraint 0.045 -verbose 0
./min_cut.out -hgr_file p2.hgr -dim_file p2.dim -mode 1 -partition_type 0 -area_constraint 0.045 -verbose 0
./min_cut.out -hgr_file p2.hgr -dim_file p2.dim -mode 2 -partition_type 0 -area_constraint 0.045 -verbose 0
echo "------------------Done processing p2--------------------------------------------------------------------"
###########################################################################################################

### Running placement on structP ###################################################################################
echo "------------------Processing structP-------------------------------------------------------------------------"
./min_cut.out -hgr_file structP.hgr -dim_file structP.dim -mode 0 -partition_type 0 -area_constraint 0.04 -verbose 0
./min_cut.out -hgr_file structP.hgr -dim_file structP.dim -mode 1 -partition_type 0 -area_constraint 0.04 -verbose 0
./min_cut.out -hgr_file structP.hgr -dim_file structP.dim -mode 2 -partition_type 0 -area_constraint 0.04 -verbose 0
echo "-------------------Done processing structP-------------------------------------------------------------------"
####################################################################################################################

### Running placement on industry2 #####################################################################################
echo "-------------------Processing industry2--------------------------------------------------------------------------"
./min_cut.out -hgr_file industry2.hgr -dim_file industry2.dim -mode 0 -partition_type 0 -area_constraint 0.04 -verbose 0
./min_cut.out -hgr_file industry2.hgr -dim_file industry2.dim -mode 1 -partition_type 0 -area_constraint 0.04 -verbose 0
./min_cut.out -hgr_file industry2.hgr -dim_file industry2.dim -mode 2 -partition_type 0 -area_constraint 0.04 -verbose 0
echo "-------------------Done Processing industry2---------------------------------------------------------------------"
########################################################################################################################

### Running placement on industry3 ######################################################################################
echo "-------------------Processing industry3--------------------------------------------------------------------------"
./min_cut.out -hgr_file industry3.hgr -dim_file industry3.dim -mode 0 -partition_type 0 -area_constraint 0.045 -verbose 0
./min_cut.out -hgr_file industry3.hgr -dim_file industry3.dim -mode 1 -partition_type 0 -area_constraint 0.045 -verbose 0
./min_cut.out -hgr_file industry3.hgr -dim_file industry3.dim -mode 2 -partition_type 0 -area_constraint 0.045 -verbose 0
echo "----------Done Processing industry3------------------------------------------------------------------------------"
#########################################################################################################################

echo "Placement using DFS partitioning completed"
echo "Refer to output directory for placement csv and logfile directory for log files"
