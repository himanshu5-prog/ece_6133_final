//--------------------------------------------------------------------------
//Name: Himanshu Yadav
//Date: April 16th, 2020
//Description: This file contains function to implement final cell placement 
//--------------------------------------------------------------------------
#include<iostream>
#include<cmath>
using namespace std;

void min_cut_placement_bfs(int cell_count, vector<Cell> &cell_list,bool p_type, int count_threshold, vector<int> cell_net_list[], vector<Cell> detailed_net_list[],
int net_count, int cut_size, vector<Cell> adjacent_list[], int max_connect, vector<Cell> &global_cell_list,
double x_coord, double y_coord, double width, double height, int mode, double delta, vector<int> net_list[], double chipWidth, double chipHeight, bool debug, const double areaConstraint, ofstream& output_logfile, bool verbose)
{
	queue< vector<Cell> > q_cell_list;
	
	q_cell_list.push(cell_list);
	vector<Cell> cell_list_present;
	
	vector<Cell> cell_list_p1;
	vector<Cell> cell_list_p2;
	int cell_count_present;
	int count_p_type=0;
	int limit=1;
	bool start=true;
	double x_coord1 ,y_coord1 ,width1, height1;
	x_coord1 = x_coord;
	y_coord1 = y_coord;
	width1 = width;
	height1 = height;
	int partition_id_h = 1;	
	int partition_id_v = 1;	
	int cell_count_p1, cell_count_p2;
	double x_temp,y_temp;

	double h_temp=height;
	double w_temp=width;
	while(!q_cell_list.empty())
	{
			cell_list_present = q_cell_list.front();
			q_cell_list.pop();
			
			y_temp = y_coord1;
			x_temp = x_coord1;
				
			if(count_p_type == limit)
			{
				limit = limit<<1;
				count_p_type = 0;
								
				if(p_type == 0)
				{
					p_type = 1;
					h_temp = h_temp/2;
				}
				else if(p_type == 1)
				{
					w_temp = w_temp/2;
					p_type = 0;
				}
				//cout<<"limit: "<<limit<<endl;
			}
			
			
			if(cell_list_present.size() < 4)
			{
				//cout<<"Threshold reached"<<" height: "<<h_temp<<" width: "<<w_temp<<endl;	
			} 
			else
			{
				//if(count_p_type == 1)
				cell_count_present = cell_list_present.size();
				if(verbose == true)
				{
					output_logfile<<"---------------------------------------------------------------------\n";		
				if(p_type == 0)
				{
					output_logfile<<"Performing Horizontal cut: \n";
				} 
				else if(p_type == 1)
				{
					output_logfile<<"Performing Vertical cut: \n";
				}
				}
					
				
				//else if(p_type == 1)
				//cout<<"Performing Vertival cut\n";
				if(start==true)
				{
				apply_FM_Algorithm_5(cell_list_present,cell_count_present,cell_net_list,detailed_net_list ,net_count,cut_size,adjacent_list,max_connect,cell_list_p1,
		cell_list_p2,p_type,global_cell_list,
		x_coord1,y_coord1,width,height, mode, delta, net_list, chipWidth, chipHeight, debug, areaConstraint);
				//	apply_FM_Algorithm(cell_list_present,cell_count_present,cell_net_list,detailed_net_list ,net_count,cut_size,adjacent_list,cut_size,
				//max_connect,cell_list_p1,cell_list_p2,p_type,global_cell_list,x_coord1,y_coord1,width,height, mode, delta);
					start=false;
				}
				else if (p_type == 0)
				{
					//assert(cell_list_present[0].cell_id > 0);
					int p;
                                         for(int i=0;i<cell_list_present.size();i++)
                                        {
                                                if(cell_list_present[i].cell_id > 0)
                                                {
                                                        p = i;
                                                        break;
                                                }
                                        }
				//	h_temp = h_temp/2;		
					apply_FM_Algorithm_5(cell_list_present,cell_count_present,cell_net_list,detailed_net_list ,net_count,cut_size,adjacent_list,
					max_connect,cell_list_p1,
		cell_list_p2,p_type,global_cell_list,
		cell_list_present[p].x_dim,  cell_list_present[p].y_dim,width,h_temp, mode, delta, net_list, chipWidth, chipHeight, debug, areaConstraint);
					//apply_FM_Algorithm(cell_list_present,cell_count_present,cell_net_list,detailed_net_list ,net_count,cut_size,adjacent_list,
					//cut_size,max_connect,cell_list_p1,cell_list_p2,p_type,global_cell_list, cell_list_present[p].x_dim, cell_list_present[p].y_dim,
					// width, h_temp, mode, delta);
				}
				else if (p_type == 1)
				{
					int z;
					 for(int i=0;i<cell_list_present.size();i++)
					{
						if(cell_list_present[i].cell_id > 0)
						{
							z = i;
							break;
						}
					}
				//	w_temp = w_temp/2;
				apply_FM_Algorithm_5(cell_list_present,cell_count_present,cell_net_list,detailed_net_list ,net_count,cut_size,adjacent_list,max_connect,cell_list_p1,
		cell_list_p2,p_type,global_cell_list,
		cell_list_present[z].x_dim,cell_list_present[z].y_dim, w_temp, height, mode, delta, net_list,chipWidth, chipHeight, debug, areaConstraint);
				//	apply_FM_Algorithm(cell_list_present,cell_count_present,cell_net_list,detailed_net_list ,net_count,cut_size,adjacent_list,cut_size,
				//max_connect,cell_list_p1,cell_list_p2,p_type,global_cell_list, cell_list_present[z].x_dim, cell_list_present[z].y_dim, w_temp, height, mode, delta);
				}
				cell_count_p1 = cell_list_p1.size();
				cell_count_p2 = cell_list_p2.size();
				if(verbose == true)
				{
					output_logfile<<"sub cell list p1 size: "<<cell_count_p1<<" sub cell list p2 size: "<<cell_count_p2<<endl;
					if(cell_count_p1)
					{
						output_logfile<<"partition1 x coord: "<<cell_list_p1[0].x_dim<<" partition2 y coord: "<<cell_list_p1[0].y_dim<<endl;
					}
	
					if(cell_count_p2)
					{
						output_logfile<<"partition1 x coord: "<<cell_list_p2[0].x_dim<<" partition2 y coord: "<<cell_list_p2[0].y_dim<<endl;
					}
						output_logfile<<"-----------------------------------------------------------------------\n";
				}			
					q_cell_list.push(cell_list_p1);
					q_cell_list.push(cell_list_p2);
					
					cell_list_p1.clear();
					cell_list_p1.shrink_to_fit();
					
					cell_list_p2.clear();
					cell_list_p2.shrink_to_fit();
					
					count_p_type++;
				
			}
	} 
	
}

void min_cut_placement(int cell_count, vector<Cell> &cell_list,bool p_type, int count_threshold, vector<int> cell_net_list[], vector<Cell> detailed_net_list[],
int net_count, int cut_size, vector<Cell> adjacent_list[], int max_connect, vector<Cell> &global_cell_list,
double x_coord, double y_coord, double width, double height, int mode, double delta, vector<int> net_list[], double chipWidth, double chipHeight, bool debug, const double areaConstraint, ofstream& output_logfile, bool verbose)
{
	int cell_count_p1;
	int cell_count_p2;
	vector<Cell> cell_list_p1;
	vector<Cell> cell_list_p2;
	if(cell_count < 5)
	{
	//	cell_count_p1 = cell_list_p1.size();
	//	cell_count_p2 = cell_list_p2.size();
	//	cout<<"threshold sub cell list p1 size: "<<cell_count_p1<<" sub cell list p2 size: "<<cell_count_p2<<endl;
	//	cout<<"Threshold reached!!!!"<<"width: "<<width<<" height: "<<height<<endl;
		return;
	}
	
//	vector<Cell> cell_list_p1;
//	vector<Cell> cell_list_p2;
	//cout<<"min cut Cell count: "<<cell_count<<endl;
//	cout<<"initial p_type: "<<p_type<<endl;
	if(p_type == 0) p_type=1;
	else p_type = 0;
	int threshold = (0.1*cell_count)+1;
	
	double x_coord1, x_coord2, y_coord1, y_coord2;
	int h1, w1;
	
//	cout<<"final p_type: "<<p_type<<endl;
//	if(p_type == 0)
//		cout<<"Performing horizontal cut\n";
//	else 
//		cout<<"Performing vertical cut\n";
		
		apply_FM_Algorithm_5(cell_list,cell_count,cell_net_list,detailed_net_list ,net_count,cut_size,adjacent_list,max_connect,cell_list_p1,
		cell_list_p2,p_type,global_cell_list,
		x_coord,y_coord,width,height, mode, delta, net_list,chipWidth, chipHeight, debug, areaConstraint);
		
	//apply_FM_Algorithm(cell_list,cell_count,cell_net_list,detailed_net_list ,net_count,cut_size,adjacent_list,cut_size,max_connect,
	//cell_list_p1,cell_list_p2,p_type,global_cell_list,x_coord,y_coord,width,height, mode , delta);
	
	//int cell_count_p1;
	//int cell_count_p2;
	
	cell_count_p1 = cell_list_p1.size();
	cell_count_p2 = cell_list_p2.size();
	
	if(verbose == true)
	{
		output_logfile<<"sub cell list partition1 size: "<<cell_count_p1<<" sub cell list partition2 size: "<<cell_count_p2<<endl;
		if(cell_count_p1)
		{
			output_logfile<<"partition1 x coord: "<<cell_list_p1[0].x_dim<<"partition1 y coord: "<<cell_list_p1[0].y_dim<<endl;
		}
	
		if(cell_count_p2)
		{
			output_logfile<<"partition2 x coord: "<<cell_list_p2[0].x_dim<<"partition2 y coord: "<<cell_list_p2[0].y_dim<<endl;
		}	
	}
	if(p_type ==1)
	{
		//vertical cut
		//width = width/2;
		x_coord1 = x_coord - width/4;
	//	width = width/2;
		x_coord2 = x_coord + width/4; 
		y_coord1 = y_coord;
		width = width/2;
		y_coord2 = y_coord;
	}
	else if (p_type == 0)
	{
		//Horizontal cut
		y_coord1 = y_coord - height/4;

		//height = height/2;
		y_coord2 = y_coord + height/4;
		height = height/2;
		x_coord1 = x_coord;
		x_coord2 = x_coord;
	}
	
	min_cut_placement(cell_count_p1, cell_list_p1, p_type, count_threshold, cell_net_list,detailed_net_list,net_count,cut_size,adjacent_list,
	max_connect,global_cell_list, x_coord1,y_coord1, width, height, mode, delta, net_list, chipWidth, chipHeight, debug, areaConstraint, output_logfile, verbose);
	cell_list_p1.clear();
	cell_list_p1.shrink_to_fit();
	
	min_cut_placement(cell_count_p2, cell_list_p2, p_type, count_threshold, cell_net_list,detailed_net_list,net_count,cut_size,adjacent_list,
	max_connect,global_cell_list,x_coord2,y_coord2, width, height, mode, delta, net_list, chipWidth, chipHeight, debug, areaConstraint, output_logfile, verbose);
	cell_list_p2.clear();
	cell_list_p2.shrink_to_fit();
}

bool sortByVal(const pair<int, double> &a, 
               const pair<int, double> &b) 
{ 
    return (a.second < b.second); 
} 

void performFinalPlacement(Row *row, const int rowCount, const double rowHeight, vector<Cell> global_cell_list1, map<int, pair<double,double>> &finalPlacement, int cell_count,
double chipWidth, const double *widthCell, ofstream &output_logfile)
{
	map<int,double> yCoordinate;
	map<int,double> xCoordinate;
		
	int cell_id;
	double y_coordinate;
	double x_coordinate;
	for(int x = 0; x< cell_count; x++)
	{
		cell_id = global_cell_list1[x].cell_id;
			
		if(cell_id == 0)
			continue;
			
		y_coordinate = global_cell_list1[x].y_dim;
		x_coordinate = global_cell_list1[x].x_dim;
		yCoordinate.insert({cell_id,y_coordinate});
		xCoordinate.insert({cell_id,x_coordinate});
	}
	
	//cout<<"Done with x coordinate and ycoordinate map"<<endl;
	
	int row_id=0;
		vector< pair<int, double> > vec_y;
		map<int,double> :: iterator itr;
		
		for(itr = yCoordinate.begin(); itr != yCoordinate.end(); itr++)
		{
			vec_y.push_back( make_pair(  itr ->first, itr->second)  );
			
		}
		sort(vec_y.begin(), vec_y.end(), sortByVal);
		
		double width_cell=0;
		double width_cell_sum=0;
		double x_coord2;
		int sum_cell=0;
		double floorplan_height;
		int index_row = row_id;
		for(int i=0; i<vec_y.size(); i++)
		{
		//	cout<<vec_y[i].first<<" -> "<<vec_y[i].second<<endl;
			//row[row_id].rowPointer = 0;
			
			//row[row_id].y_coord = 2*row_id*rowHeight;
			//cout<<2*row_id*rowHeight;
			cell_id = vec_y[i].first;
			assert(cell_id >= 1);
			width_cell = widthCell[cell_id - 1];
			width_cell_sum += width_cell;
			
			
			if(width_cell_sum <= chipWidth - 5)
			{
				row[row_id].rowPointer = 0;
				row[row_id].cell_list.push_back(cell_id);
				row[row_id].y_coord = 2*row_id*rowHeight;
				//cell_id = vec_y[i].first;
				
				sum_cell++;
				x_coord2 = xCoordinate[cell_id];
				row[row_id].x_coordinate.insert({cell_id,x_coord2});
			}
			else
			{
				//cout<<"row #"<<row_id<<" : "<<row[row_id].cell_list.size()<<" width_cell_sum: "<<width_cell_sum<<"y coord: "<<row[row_id].y_coord<<endl;
			//	sum_cell += row[row_id].cell_list.size();
				row_id++;
				assert(row_id < rowCount);
				row[row_id].rowPointer = 0;
				row[row_id].cell_list.push_back(cell_id);
				sum_cell++;
				row[row_id].y_coord = 2*row_id*rowHeight;
				//cell_id = vec_y[i].first;
				x_coord2 = xCoordinate[cell_id];
				row[row_id].x_coordinate.insert({cell_id,x_coord2});
				
			
				width_cell_sum = 0;	
				
			}	
		}
		floorplan_height = (row_id + 1)* 2.8  - 1.4;
		cout<<"cell count: "<<sum_cell<<" row used: "<<row_id + 1<<" out of "<<rowCount<<endl;
		output_logfile<<"cell count: "<<sum_cell<<" row used: "<<row_id + 1<<" out of "<<rowCount<<endl;
		cout<<"Effective cell height: "<<floorplan_height<<" aspect-ratio: "<<floorplan_height/chipWidth<<endl; 
		output_logfile<<"Effective cell height: "<<floorplan_height<<" aspect-ratio: "<<floorplan_height/chipWidth<<endl; 
		vector< pair<int, double> > vec_x;
		int cell_id_row;
		for(int i=0; i<rowCount; i++)
		{
				if( row[i].cell_list.size() > 0)
				{
						map<int,double> :: iterator itr;
						
						for(itr = row[i].x_coordinate.begin(); itr!= row[i].x_coordinate.end();itr++)
						{
							assert(itr->first >= 1);
							vec_x.push_back(make_pair(itr->first,itr->second));
						}
						
						sort(vec_x.begin(), vec_x.end(), sortByVal);
						
					//	if(i==0)
					//	{
						double w=0;
							for(int j=0; j< vec_x.size(); j++)
							{
								
							//	if(i==43)
							//		cout<<vec_x[j].first<<" -> "<<vec_x[j].second<<endl;
								cell_id_row = vec_x[j].first;
								if(cell_id_row < 1)
									cout<<"cell_id: "<<cell_id_row<<" i: "<<i<<endl;
								assert(cell_id_row >= 1);
								row[i].cell_order.push_back(cell_id_row);
								
								pair<double,double> p1 (w,row[i].y_coord);
								
								//if(w > chipWidth)
								//{
								//	cout<<"w is beyond chip_width: "<<w<<endl;
								//}
								w= w + widthCell[cell_id_row - 1];
								/*
								if(w > chipWidth)
								{
									cout<<"overflow in row #"<<i<<" by "<<w - chipWidth<<endl;
								}
								*/
								finalPlacement.insert({cell_id_row, p1});
							}
							row[i].rowPointer = w;
							
								
					//	}
					//	cout<<"size of vec_x: "<<vec_x.size()<<" final cell x coord: "<< finalPlacement[row[i].cell_order.back()].first<<" final cell ycoord: "
					//	<<finalPlacement[row[i].cell_order.back()].second<<endl;
						vec_x.clear();				
				}
				
		}
	
	
}

void performFinalPlacement_2(Row *row, const int rowCount, const double rowHeight, vector<Cell> global_cell_list1, map<int, pair<double,double>> &finalPlacement, int cell_count,
double chipWidth, const double *widthCell)
{
	//--------------------------------------------
	map<int,double> yCoordinate;
	map<int,double> xCoordinate;
		
	int cell_id;
	double y_coordinate;
	double x_coordinate;
	for(int x = 0; x< cell_count; x++)
	{
		cell_id = global_cell_list1[x].cell_id;
			
		if(cell_id == 0)
			continue;
			
		y_coordinate = global_cell_list1[x].y_dim;
		x_coordinate = global_cell_list1[x].x_dim;
		yCoordinate.insert({cell_id,y_coordinate});
		xCoordinate.insert({cell_id,x_coordinate});
	}
	
	//----------------------------------------------
	
	
		int row_id=0;
		vector< pair<int, double> > vec_x;
		map<int,double> :: iterator itr;
		
		for(itr = xCoordinate.begin(); itr != xCoordinate.end(); itr++)
		{
			vec_x.push_back( make_pair(  itr ->first, itr->second)  );
			
		}
		sort(vec_x.begin(), vec_x.end(), sortByVal);
		//--- 
		
		double width_cell=0;
		double width_cell_sum=0;
		double x_coord2;
		int sum_cell=0;
		int index_row = row_id;
		int row_target = 0;
		double place_pointer;
		for(int i=0; i < vec_x.size();i++)
		{
			cell_id = vec_x[i].first;
			assert(cell_id >= 1);
			width_cell = widthCell[cell_id - 1];
			place_pointer = row[row_target].rowPointer;
			row_target = i % rowCount;
			row[row_target].x_coordinate.insert({cell_id, place_pointer});
			pair<double,double> p1(place_pointer, row[row_target].y_coord);
			finalPlacement.insert({cell_id , p1});
			row[row_target].rowPointer += width_cell;
			row[row_target].cell_list.push_back(cell_id);
			row[row_target].cell_order.push_back(cell_id);
		}
		
		
}

void generateCSV(map<int, pair<double,double>> finalPlacement, double *widthCell, string file_name)
{
	ofstream output_file;
	string op_file = "output/"  + file_name;
	//output_file.open("data1.csv");
	output_file.open(op_file);
	map<int, pair<double,double> > :: iterator itr3;
	//output_file<<"x,y,cell_id\n";
//	output_file<<"x,y\n";
	double width;
	int cell_id;
	for(itr3 = finalPlacement.begin(); itr3 != finalPlacement.end(); itr3++ )
	{
		cell_id = itr3->first;
		assert(cell_id >= 1);
		width = widthCell[cell_id - 1];
		//output_file<<itr3->second.first<<","<<itr3->second.second<<","<<itr3->first<<endl;
		output_file<<itr3->second.first<<","<<itr3->second.second<<","<<width<<endl;
		//cout<<itr3->second.first<<","<<itr3->second.second<<endl;
	}
		
	output_file.close();
	//cout<<"CSV file generated\n";
}

void generateRowData(Row *row, int rowCount, ofstream& output_logfile)
{
	int sum=0;
	for(int i=0; i<rowCount; i++)
	{
		output_logfile<<"Row id #"<<i+1<<" ";
		output_logfile<<" rowPointer: "<<row[i].rowPointer<<" ";
		output_logfile<<" Row y_coord "<<row[i].y_coord<<" ";
		output_logfile<<" cell count: "<<row[i].cell_list.size()<<endl;
		sum += row[i].cell_list.size();
	}
	output_logfile<<"Total cell placed: "<<sum<<endl;
}

void generateData (Row *row, int rowCount, double chipWidth, ofstream& output_logfile)
{
	double mean_occupancy = 0;
	double sum_occupancy=0;
	int valid_row = 0;

	for(int i=0; i < rowCount; i++)
	{
		if(row[i].cell_list.size() == 0)
			break;
		
		sum_occupancy += row[i].rowPointer/chipWidth;
		valid_row++;
	}
	assert(valid_row > 1);
	cout<<"last occupancy: "<<row[valid_row - 1].rowPointer/chipWidth<<endl;
	cout<<"valid_row: "<<valid_row<<endl;
	sum_occupancy = sum_occupancy - ((row[valid_row - 1].rowPointer)/chipWidth );
	mean_occupancy = sum_occupancy/(valid_row - 1);
	cout<<"Mean occupancy: "<<mean_occupancy<<endl;
	output_logfile<<"Mean occupancy: "<<mean_occupancy<<endl;
	double square_diff = 0;
	double occupancy = 0;
	double var=0;
	double standardDeviation = 0;
	for(int i=0; i<valid_row - 1; i++)
	{
		if(row[i].cell_list.size() == 0)
                        break;
		
		occupancy = row[i].rowPointer/chipWidth;
		//cout<<"occupancy: "<<occupancy<<endl;
		//if (occupancy > 1)
		//	cout<<"out of chip!!   occupancy: "<<occupancy<<endl;
		square_diff += ((occupancy - mean_occupancy)*(occupancy - mean_occupancy));
		
	}
	var = square_diff/(valid_row - 1);
	standardDeviation = sqrt(var);
	cout<<"standard deviation: "<<standardDeviation<<endl;			
	output_logfile<<"standard deviation: "<<standardDeviation<<endl;			
}
