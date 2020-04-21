//----------------------------------------------------------------------------------
//Name: Himanshu Yadav
//Date: April 16th, 2020
//Description: This file contains function to create data structure used in project.
//----------------------------------------------------------------------------------

#include<iostream>
using namespace std;



int 
calculate_cutsize(  vector<Cell> detailed_net_list[], int detailed_netlist_size)
{
	int cut_size=0;
	int p1=0, p2=0;
	for(int i=0;i<detailed_netlist_size;i++)
	{
		for(int j=0;j<detailed_net_list[i].size();j++)
		{
			if(j==0)
			{
				p1 = detailed_net_list[i][j].partition_id;
			}
			else 
			{
				p2 = detailed_net_list[i][j].partition_id;
				
				if(p1!= p2)
				{
					cut_size++;
					break;
				}	
			}
		}
	}
	return cut_size;
}

void create_detailed_netlist(vector<Cell> detailed_net_list[], vector<int> net_list[], int net_count, double chip_width, double chip_height)
{
	Cell *temp;
	temp = new Cell;
	//temp->weight=0;
	double w;
	for(int i=0;i<net_count;i++)
	{
		for(int j=0;j<net_list[i].size();j++)
		{
			temp->cell_id=net_list[i][j];
			temp->locked=false;
			temp->partition_id = 5 ;
			//cout<<"size: "<<net_list[i].size()<<endl;
			//w= 1/ ((double (net_list[i].size() ))-1 );
			//	cout<<"w: "<<w<<endl;
//			temp->weight = 0;
			temp->x_dim=chip_width/2;
			temp->y_dim=chip_height/2;
//			temp->all_same_partition_net_count= 0;
//			temp->only_cell_net_count = 0;
			temp->active = false;
			detailed_net_list[i].push_back(*temp);
		//	cout<<detailed_net_list[i].size()<<endl;
		}
	}
}


void post_processing_weight (vector< pair<int,int> > pair_list, map< pair<int,int>, double> &weight)
{
	vector< pair<int,int> >::iterator itr;
	itr = pair_list.begin();
	
	while(itr!= pair_list.end())
	{	
		weight[{itr->first, itr->second}] /=2;
		itr++;
	}
}
bool is_cell_present(vector<Cell> net_list, int net_list_size, int node)
{
	for(int i=0; i<net_list_size;i++)
	{
		if(node== net_list[i].cell_id)
			return true;
	}
	return false;
}

bool legalWidth(double *widthCell, int cellCount)
{
	for(int i=0; i<cellCount; i++)
	{
		if(widthCell[i] == 0)
			return false;
	}
	return true;
}

void create_cell_list(vector<Cell> &cell_list, int cell_count, vector<Cell> detailed_net_list[], int detailed_netlist_size)
{
	int cell_id=0;
	bool present=false;
	bool loop_exit=false;
	cout<<"inside create cell list: cell_count: "<<cell_count<<endl;
	for(int i=0;i<cell_count;i++)
	{
		cell_id = i+1;
		loop_exit=false;
		for(int j=0; j<detailed_netlist_size;j++)
		{
			present = is_cell_present(detailed_net_list[j], detailed_net_list[j].size(),cell_id);
			if(!present )
				continue;
			
			else if(present)
			{
				for(int k=0;k<detailed_net_list[j].size();k++)
				{
					if(cell_id == detailed_net_list[j][k].cell_id)
					{
						cell_list.push_back(detailed_net_list[j][k]);
						loop_exit=true;
						break;
					}
				}
			}
			if(loop_exit==true)
				break;
		}
	}
}

void create_cell_list_2(vector<Cell> &cell_list, int cell_count, vector<Cell> detailed_net_list[], int detailed_netlist_size, vector<Cell*> &cell_ptr_list)
{
	int cell_id=0;
	bool present=false;
	bool loop_exit=false;
	Cell *temp;
	temp = new Cell;
	
	temp->cell_id=0;
	temp->locked=true;
	temp->active = false;
	//temp->weight=0;
	//temp->weight = 0;
	temp->x_dim=0;
	temp->y_dim=0;
	//temp->all_same_partition_net_count= 0;
	//temp->only_cell_net_count = 0;
	//cout<<"inside create cell list: cell_count: "<<cell_count<<endl;
	for(int i=0;i<cell_count;i++)
	{
		cell_id = i+1;
		loop_exit=false;
		for(int j=0; j<detailed_netlist_size;j++)
		{
			present = is_cell_present(detailed_net_list[j], detailed_net_list[j].size(),cell_id);
			if(!present )
				continue;
			
			else if(present)
			{
				for(int k=0;k<detailed_net_list[j].size();k++)
				{
					if(cell_id == detailed_net_list[j][k].cell_id)
					{
						cell_list.push_back(detailed_net_list[j][k]);
						cell_ptr_list.push_back(&detailed_net_list[j][k]);
						loop_exit=true;
						break;
					}
				}
			}
			if(loop_exit==true)
				break;
		}
		if(loop_exit==false)
		{
	//		cout<<"inserting dummy cell!! ";
	//		cout<<"cell_id not connected: "<<cell_id<<endl;
			cell_list.push_back(*temp);
		}
	}
	
	//cout<<"size of cell_list: "<<cell_list.size()<<endl;
}
vector<Cell> report_neighbors(int cell_id, vector<Cell> reduced_adjacent_list[], int cell_count)
{
	vector <Cell> neighbor;
	int index = cell_id - 1;
	int n_cell_id = 0;
	for(int i=0; i< reduced_adjacent_list[index].size();i++)
	{
		neighbor.push_back(reduced_adjacent_list[index][i]);
	}
	return neighbor;
}

void update_detailed_netlist(vector<Cell> cell_list, vector<Cell> detailed_net_list[], int net_count)
{
	//bool present=false;
	int cell_id,index;
	for(int i=0;i<net_count;i++)
	{
		for(int j=0;j<detailed_net_list[i].size();j++)
		{
				cell_id = detailed_net_list[i][j].cell_id;
				index = cell_id - 1;
				detailed_net_list[i][j].partition_id = cell_list[index].partition_id;
				detailed_net_list[i][j].x_dim = cell_list[index].x_dim;
				detailed_net_list[i][j].y_dim = cell_list[index].y_dim;
		}
	}
}

void create_cell_net_list(vector<Cell> detailed_net_list[], int net_count, vector<int> cell_net_list[], int cell_count)
{
	int cell_id;
	bool present;
	for(int i=0;i<cell_count;i++)
	{
		cell_id = i + 1;
		present=false;
		for(int j=0;j<net_count;j++)
		{
			present = is_cell_present(detailed_net_list[j], detailed_net_list[j].size(),cell_id);
			
			if(present)
			{
				cell_net_list[i].push_back(j);
			}
		}
	}
}
void initialise_cell_list(vector<Cell> &cell_list)
{
	for(int i=0;i<cell_list.size();i++)
	{
		cell_list[i].partition_id = (i%2) + 1;
		//cell_list[i].only_cell_net_count = 0;
		//cell_list[i].all_same_partition_net_count = 0;
		cell_list[i].active = true;
	}
}

void initialise_cell_list_2(vector<Cell> &cell_list, vector<Cell> &global_cell_list)
{
	int cell_id;
	for(int i=0;i<cell_list.size();i++)
	{
		cell_id = cell_list[i].cell_id;
		
		cell_list[i].partition_id = (i%2) + 1;
	//	cell_list[i].only_cell_net_count = 0;
	//	cell_list[i].all_same_partition_net_count = 0;
		cell_list[i].active = true;
		cell_list[i].locked = false;
		
		if(cell_id != 0)
		{
			global_cell_list[cell_id - 1].partition_id = (i%2) + 1;
			//global_cell_list[cell_id - 1].only_cell_net_count = 0;
			//global_cell_list[cell_id - 1].all_same_partition_net_count = 0;
			global_cell_list[cell_id - 1].active = true;
			global_cell_list[cell_id - 1].locked = false;
		}
	}
}

void reset_global_cell_list(vector<Cell> cell_list, vector<Cell> &global_cell_list)
{
	int cell_id;
	for(int i=0;i<cell_list.size();i++)
	{
		cell_id = cell_list[i].cell_id;
		if(cell_id != 0)
		{
			global_cell_list[cell_id - 1].active = false;
			global_cell_list[cell_id - 1].locked = false;
		}
	}
}

void create_global_cell_list(vector<Cell> cell_list, vector<Cell*> &global_cell_list, int cell_count)
{
	for(int i=0; i< cell_list.size();i++)
	{
		global_cell_list.push_back(&cell_list[i]);
	}
}

unsigned int compute_wirelength(vector<Cell> detailed_net_list[], vector<Cell> global_cell_list, int net_count)
{
	unsigned int w = 0;
	int cell_id;
	int max_x, max_y;
	int min_x, min_y;
	for(int i=0;i<net_count;i++)
	{
		max_x = 0;
		max_y = 0;
		min_x = 0;
		min_y = 0;
		for(int j=0;j < detailed_net_list[i].size();j++)
		{
		//	max_x = 0;
		//	max_y = 0;
			cell_id = detailed_net_list[i][j].cell_id;
			assert(cell_id > 0);	
			if(cell_id > 0)
			{
				if(j==0)
				{
					max_x = global_cell_list[cell_id - 1].x_dim;
					min_x = global_cell_list[cell_id - 1].x_dim;
					max_y = global_cell_list[cell_id - 1].y_dim;
					min_y = global_cell_list[cell_id - 1].y_dim;
				}
				else
				{
					if(max_x < global_cell_list[cell_id - 1].x_dim)
					{
						max_x = global_cell_list[cell_id - 1].x_dim;
					}
					
					if(min_x > global_cell_list[cell_id - 1].x_dim)
                                        {
                                                min_x = global_cell_list[cell_id - 1].x_dim;
                                        }
	
					if( max_y < global_cell_list[cell_id - 1].y_dim)
					{
						max_y = global_cell_list[cell_id - 1].y_dim;
					}
					
					if( min_y > global_cell_list[cell_id - 1].y_dim)
                    {
                                                min_y = global_cell_list[cell_id - 1].y_dim;
                    }

				}
			}
			
			//ax_x + max_y;
			
		}
//		cout<<"For net# "<<i<<" max_x: "<<max_x<<" max_y: "<<max_y<<" min_x: "<<min_x<<" min_y: "<<min_y<<endl;
		w = w + max_x + max_y - min_x - min_y;
	}
	return w;
}


unsigned int compute_wirelength_2(vector<Cell> detailed_net_list[], map<int, pair<double,double> > finalPlacement, int net_count, string wire_coord_file, double chipWidth)
{
	unsigned int w = 0;
	int cell_id;
	int max_x, max_y;
	int min_x, min_y;
	ofstream output_file;
	string op_file = "output/" + wire_coord_file;
	output_file.open(op_file);
	double x_coord_min = 0,y_coord_min = 0;
	double x_coord_max = 0, y_coord_max = 0;	
	for(int i=0;i<net_count;i++)
	{
		max_x = 0;
		max_y = 0;
		min_x = 0;
		min_y = 0;
		x_coord_min = 0;
		y_coord_min = 0;
		x_coord_max = 0;
		y_coord_max = 0;

		double x_coord;
		double y_coord;
		for(int j=0;j < detailed_net_list[i].size();j++)
		{
		//	max_x = 0;
		//	max_y = 0;
			cell_id = detailed_net_list[i][j].cell_id;
			assert(cell_id > 0);	
			if(cell_id > 0)
			{
				x_coord = finalPlacement[cell_id].first;
				y_coord = finalPlacement[cell_id].second;
				if(j==0)
				{
					max_x = x_coord;
					min_x = x_coord;
					max_y = y_coord;
					min_y = y_coord;
					x_coord_min = x_coord;
                			y_coord_min = y_coord;
                			x_coord_max = x_coord;
                			y_coord_max = y_coord;

				}
				else
				{
					if(max_x < x_coord)
					{
						max_x = x_coord;
						x_coord_max = x_coord;
						y_coord_max = y_coord;
					}
					
					if(min_x > x_coord)
                    			{
                        			min_x = x_coord;
						x_coord_min = x_coord;
                        			y_coord_min = y_coord;
                    			}
	
					if( max_y < y_coord)
					{
						max_y = y_coord;
					}
					
					if( min_y > y_coord)
                   			{
                        			min_y = y_coord;
                    			}

				}
			}
			
			//ax_x + max_y;
			
		}
//		cout<<"For net# "<<i<<" max_x: "<<max_x<<" max_y: "<<max_y<<" min_x: "<<min_x<<" min_y: "<<min_y<<endl;
		if( max_x + max_y - min_x - min_y > chipWidth/2)
		{
			output_file<<min_x<<","<<max_y<<","<<max_x<<","<<min_y<<endl;
		}
		w = w + max_x + max_y - min_x - min_y;
	}
	output_file.close();
	return w;
}
double compute_abs_diff(double a, double b)
{
	double diff = a - b;
	
	if(diff >= 0)
		return diff;
	else
		return -diff;
}
