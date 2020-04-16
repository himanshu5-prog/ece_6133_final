//--------------
/*
 * Name: Himanshu Yadav
 * Date: April 16, 2020
 * Description: Main function for min cut placement
*/
//--------------
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<stdlib.h>
#include<bits/stdc++.h>
#include<algorithm>
#include<map>
#include "cell.h" 
#include "fileRead.h"
//#include "print_functions.h"
#include "create_data_structure.h"
#include "fm_algo_2.h"
//#include "kl_algo.h"
#include<vector>
#include<string>
#include<ctime>
#include "row.h"
#include "placementStrategy.h"
//#include "fm_algo.h"
const double cellHeight = 1.4;
const double rowHeight = 1.4;

using namespace std;
using namespace std::chrono;
string file_name = "structP.hgr";
string dimFileName = "structP.dim";
const string input_location ="input/";

//---------------------------------------------------------------
//Mode: 0 (Min cut placement)
//Mode: 1 (Mincut placement with 50% window terminal propagation)
//Mode: 2 (Mincut placement with all terminal propagation)
int Mode = 1;
//---------------------------------------------------------------

//------------------------------------------------------------
//partitionStrategy: true -> BFS
//partitionStrategy: false -> DFS
bool partitionStrategy = true;
//-------------------------------------------------------------
bool debugMode = false;
double areaConstraint = 0.05;
int main(int argc, char* argv[])
{
	string arg_dim_file, arg_hgr_file;
	bool arg_dim =false;
	bool arg_hgr = false;
	bool arg_mode_1 = false;
	bool arg_ptype = false;
	bool arg_verbose = false;
        bool arg_area_constraint = false;
	bool verbose = false;
	int arg_mode=0;
	int arg_partition_type = 0;
	double area_constraint = 0.05;
	//--------------------------------------------------
	if(argc == 2 &&  (string(argv[1]) == "-h" || string(argv[1]) == "-help"   ))
	{
		cout<<"Following arguments could be provided: \n"<<
		"\n1) -hgr_file : Provide hgr file (Default: structP.hgr)\n"<<
		"\n2) -dim_file : Provide dim file (Default: structP.dim)\n"<<
		"\n3) -mode (0,1,2)  : Provide placement mode (Defalult: 1) (0: Mincut | 1: Terminal propagation with 50% window | 2: Terminal propagation with 0% window)\n"<<
		"\n4) -partition_type (0,1) : Provide how partitioning should be done (Default: 1)(0: DFS | 1: BFS) \n"<<
		"\n5) -area_constraint ( any value between 0 and 0.4 (excluding 0.4) (Default: 0.05) Difference in partition cell_count = area_constraint*total cell count )\n"<<
		"\n6) -verbose (0,1): When set to 1, detailed log file is generated. (Default: 0 (brief log file is generated))\n";
		return 10;
	}
	else if (argc % 2 == 0 )
	{
		cout<<"Incorrect argument provided!!"<<endl;
		return 5;	
	}
	else
	{
		for(int ii = 1; ii < argc; ii++)
		{
			if(string(argv[ii]) == "-hgr_file")
			{
				arg_hgr_file = argv[ii+1];
				arg_hgr = true;
				ii++;
			}
			else if(string(argv[ii]) == "-dim_file")
			{
				arg_dim_file = argv[ii+1];
				arg_dim = true;
				ii++;		
			}
			else if(string(argv[ii]) == "-mode")
			{
				arg_mode = stoi(argv[ii+1]);
				arg_mode_1 = true;
				ii++;
			}
			else if(string(argv[ii]) == "-partition_type")
			{
				arg_partition_type = stoi(argv[ii+1]);
				arg_ptype = true;	
				ii++;		
			}
			else if(string(argv[ii]) == "-area_constraint" )
			{
				area_constraint = stod(argv[ii+1]);
				arg_area_constraint = true;
				ii++;			
			}
			else if(string(argv[ii]) == "-verbose")
			{
				verbose =  stoi(argv[ii+1]);
				arg_verbose = true;
				ii++;	
			}
		}
	}
	cout<<"------------------------------------------------------------------------------------------------------------------------------------------\n";
	//cout<<"After receiving argument "<<"dim_file: "<<arg_dim_file<<" hgr file: "<<arg_hgr_file<<" mode: "<<arg_mode<<" arg_partition_type: "<<arg_partition_type<<endl; 
	
	if(arg_hgr == true)
	{
		file_name = arg_hgr_file;		
	}
	 if (arg_dim == true)
	{
		dimFileName = arg_dim_file;	
	}
	if(arg_mode_1 == true)
	{
		Mode = arg_mode; 
	}	
	if(arg_ptype == true)
	{
		if(arg_partition_type == 0)
			partitionStrategy = false;
		else if (arg_partition_type ==1)
			partitionStrategy = true;					
	}
	if(arg_area_constraint == true)
	{
		areaConstraint = area_constraint;
		//cout<<"Area constraint: "<<areaConstraint<<endl;
	}
	if  (verbose == true)
	{
		cout<<"Note: Verbose Log file will be generated"<<endl;
	}
	//-------------------------------------------------
	//cout<<"Argument count: "<<argc<<endl;
	auto start = high_resolution_clock::now();
	string circuit = file_name; 
	circuit.erase(circuit.end() - 4, circuit.end());
	file_name = input_location + file_name;
	file_name =  file_name;
        dimFileName = input_location + dimFileName;	
        dimFileName =  dimFileName;	
	int net_count;
	int cell_count;
	int temp_net_count;
	
	get_netcount_from_file(file_name,temp_net_count);
	vector<int> net_list[temp_net_count];
	vector<string>v;
	vector<int> size1;
	bool hgrFileStatus= false;
	
	hgrFileStatus = fileRead(file_name,net_count, cell_count, net_list);
	
	if(hgrFileStatus == false)
	{
		cout<<"Problem processing hgr file!";
    		return 2;
	}
	//cout<<"Done reading hgr file\n";
	//-----------------------------------------------------
    
    double *widthCell;
    widthCell = (double*) calloc(cell_count,sizeof(double));
    
    double chipHeight=1000, chipWidth=1000;
    int rowCount = 0;
    bool dimFileStatus=false;
    
    dimFileStatus = getDimension(dimFileName,chipWidth,chipHeight,widthCell,cell_count, rowCount);
    
    if(dimFileStatus == false)
    {
    	cout<<"Problem processing dim file!";
    	return 3;
    }
    string bfs_dfs = "bfs";
    if(partitionStrategy == false)  bfs_dfs = "dfs";
    string logfile = "logfile/" + circuit + "_" + to_string(Mode) + "_" +  bfs_dfs + ".log";
    cout<<"log file name: "<<logfile<<endl;
	//string log2 = "circuit.log";
	ofstream output_logfile;
	output_logfile.open(logfile);
	assert(output_logfile.is_open() == true);
  auto timenow = 
      chrono::system_clock::to_time_t(chrono::system_clock::now());
    output_logfile << ctime(&timenow) << endl;  
    //cout<<"Done reading dim file: "<<dimFileName<<"\n";
    //output_logfile<<"Done reading dim file: "<<dimFileName<<"\n";
    cout<<"Width: "<<chipWidth<<" height: "<<chipHeight<<"| standard cell row count: "<<rowCount<<endl;
    output_logfile<<"Width: "<<chipWidth<<" height: "<<chipHeight<<"| standard cell row count: "<<rowCount<<endl;
   
 
    
	vector<Cell> detailed_net_list[net_count];
	vector<Cell> adjacent_list[cell_count];
	
	vector<int> *cell_net_list;
	cell_net_list = new vector<int>[cell_count];

	
	//Checking the value of net count, cell count and netlist 
	cout<<"Input file: "<<file_name<<endl;

	//--Creating detailed netlist----------------------------------
	create_detailed_netlist(detailed_net_list,net_list,net_count, chipWidth, chipHeight);
	cout<<"net_count: "<<net_count<<" cell_count: "<<cell_count<<endl;
	output_logfile<<"net_count: "<<net_count<<" cell_count: "<<cell_count<<endl;
	
	vector<Cell> cell_list;
	vector<Cell*> cell_ptr_list;
	vector<Cell*> global_cell_list;
	int cut_size=0;
	//-----------------------------
	create_cell_net_list(detailed_net_list,net_count,cell_net_list,cell_count);
	//cout<<"Hello"<<endl;
	int max_connect  = get_max_connectivity(cell_net_list,cell_count);
	//cout<<"max_connect: "<<max_connect<<endl;

	
	//Assigning partition id depending on whether cell id is odd or even-------------------------------------
	initialise(detailed_net_list,net_count,adjacent_list,cell_count);
	//create_cell_list(cell_list,cell_count,detailed_net_list,net_count);
	create_cell_list_2(cell_list,cell_count,detailed_net_list,net_count,cell_ptr_list);
	create_global_cell_list(cell_list,global_cell_list,cell_count);
	cut_size = calculate_cutsize(detailed_net_list,net_count);
	
	//cout<<"initial cut size: "<<cut_size<<endl;
	int threshold = (0.1*cell_count)+1;
	//cout<<" max cell diff allowed: "<<threshold<<endl;
	
	cout<<"PART1: Spreading of cells starts\n";
	output_logfile<<"PART1: Spreading of cells starts\n";
	vector<Cell> global_cell_list1(cell_list);
	vector<Cell> cell_ptr_p1;
	vector<Cell> cell_ptr_p2;
	bool p_type=0; //Horizontal
		
	Row *row = new Row [rowCount];
	for(int j=0; j< rowCount;j++)
	{
		row[j].y_coord = 2*rowHeight*j;
		row[j].rowPointer = 0;
		
	}
	double x_coord_init = chipWidth/2;
	double y_coord_init = chipHeight/2;
	double init_height = chipHeight;
	double init_width = chipWidth;
		
	int mode = 0;
	mode =Mode;
	assert(mode >=0 && mode <= 2);
	double delta=0;	
	bool strat = false;
	strat = partitionStrategy;
	bool debug_mode = false;
	debug_mode = debugMode;
        //cout<<"area constraint: "<<areaConstraint<<endl;
        output_logfile<<"area constraint: "<<areaConstraint<<endl;
	
	if(areaConstraint > 0.4)
	{
		cout<<"areaConstraint greater than 0.4. It should be less than 0.4. Exiting now";
		return 55;
	}		
	//cout<<"Mode: "<<mode<<" DebugMode: "<<debugMode<<endl;
	string p_strat;
		///*
	if(strat == false)
	{
		cout<<"Using DFS"<<endl;
		output_logfile<<"Using DFS"<<endl;
		p_strat = "dfs";		
		min_cut_placement(cell_count,cell_list,p_type,threshold,cell_net_list,detailed_net_list,net_count,cut_size,adjacent_list,
		max_connect,global_cell_list1,x_coord_init,y_coord_init,init_width,init_height, mode,delta,net_list, chipWidth, chipHeight, debug_mode, areaConstraint,
		output_logfile, verbose);
	}	  
	else if(strat == true)
	{
		cout<<"Using BFS"<<endl;
		output_logfile<<"Using BFS"<<endl;
		bool p_type_bfs=1;	
		p_strat = "bfs";
		min_cut_placement_bfs(cell_count,cell_list,p_type_bfs,threshold,cell_net_list,detailed_net_list,net_count,cut_size,adjacent_list,
		max_connect,global_cell_list1,x_coord_init,y_coord_init,init_width,init_height, mode,delta,net_list, chipWidth, chipHeight, debug_mode, areaConstraint,
		output_logfile, verbose);		
	}
		//-------------------------------------------------------------------------------------------------------------------------------			
	unsigned int wire_len = compute_wirelength(detailed_net_list,global_cell_list1,net_count);
	//cout<<"After spreading the cells, Wirelength: "<<wire_len<<endl;	
	output_logfile<<"After spreading the cells, Wirelength: "<<wire_len<<endl;	
	cout<<"PART1 ENDS: done spreading the cell\n";
	output_logfile<<"PART1 ENDS: done spreading the cell\n";
	cout<<"PART2 Starts: Now placing the cells\n";
	output_logfile<<"PART2 Starts: Now placing the cells\n";
		//------------------------------------------------------
	map<int,pair<double,double>> finalPlacement;
		
	performFinalPlacement(row,rowCount,rowHeight,global_cell_list1,finalPlacement, cell_count, chipWidth, widthCell, output_logfile);
	
	//performFinalPlacement_2(row,rowCount,rowHeight,global_cell_list1,finalPlacement, cell_count, chipWidth, widthCell);
	cout<<"PART2 Ends: Done placing the cells\n";	
	output_logfile<<"PART2 Ends: Done placing the cells\n";	
	unsigned int wl2 = compute_wirelength_2(detailed_net_list,finalPlacement,net_count);
	cout<<"After placing the cells, wirelength: "<<wl2<<endl;
	output_logfile<<"After placing the cells, wirelength: "<<wl2<<endl;
	
	//Generating CSV file that has coordinate of each cell
	string csv_file = circuit + "_" + p_strat + "_" + to_string(mode) + ".csv";
        //cout<<"csv file name: "<<csv_file<<endl;	 
        output_logfile<<"csv file name: ./output/"<<csv_file<<endl;	 
	generateCSV(finalPlacement,widthCell, csv_file);
	//---------------------------------------------------
	if(verbose)
		generateRowData(row,rowCount, output_logfile);
	
	auto stop = high_resolution_clock::now(); 
	auto duration = duration_cast<microseconds>(stop - start); 
	
	cout <<"( mode: "<<mode<< ") Time taken : "<< duration.count()/1000000 << " seconds" <<" ( "<< duration.count()<<" microseconds) Final WL: "<<wl2<< endl;
	output_logfile <<"( mode: "<<mode<< ") Time taken : "<< duration.count()/1000000 << " seconds" <<" ( "<< duration.count()<<" microseconds) Final WL: "<<wl2<< endl;
        output_logfile.close(); 
	return 0;
}
