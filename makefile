#ifneq [ -d "output"] ; then echo "directory doesn't exist\n"; 
#if [ ! -d "./output" ];then     \
#                mkdir output;           \
#        fi
#       #       if [  -d ./output/*.csv ];then     \
                rm ./output/*.csv;           \
        fi

all:	
	@g++ -std=c++11 src/min_cut.cpp -O3 -o min_cut.out
	@if [ ! -d "./output" ];then     \
                mkdir output;           \
        fi
	@if [ ! -d "./logfile" ];then     \
                mkdir logfile;           \
        fi

clear:
	@if [  -a "min_cut.out" ];then     \
               rm  min_cut.out;          \
        fi
	@if [  -d "output" ]; then     \
                rm -r output;           \
        fi
	@if [  -d "logfile" ]; then     \
                rm -r logfile;           \
        fi

help:
	@echo "-------------------------------------------------Argument list----------------------------------------------------------------------------------------------"
	@echo ""
	@echo "Following arguments can be provided:"
	@echo ""
	@echo  "1) -hgr_file : Provide hgr file (Default: structP.hgr)"
	@echo ""
	@echo "2) -dim_file : Provide dim file (Default: structP.dim)"
	@echo ""
	@echo "3) -mode (0,1,2)  : Provide placement mode (Defalult: 1) (0: Mincut | 1: Terminal propagation with 50% window | 2: Terminal propagation with 0% window)"
	@echo ""
	@echo "4) -partition_type (0,1) : Provide how partitioning should be done (Default: 1)(0: DFS | 1: BFS)"
	@echo ""
	@echo "5) -area_constraint ( any value between 0 and 0.4 (excluding 0.4) (Default: 0.05) Difference in partition cell_count = area_constraint*total cell count )"
	@echo ""
	@echo "6) -verbose (0,1): When set to 1, detailed log file is generated. (Default: 0 (brief log file is generated))";
	@echo ""
	@echo "------------------------------------------------------------------------------------------------------------------------------------------------------------"


