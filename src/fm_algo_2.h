//--------------------------------------------------------------------------------
//Name: Himanshu Yadav
//Date: April 16th, 2020
//Description: This file contains functions used for FM Algorithm for partitioning
//---------------------------------------------------------------------------------
#include<iostream>
#include<algorithm>
using namespace std;
//extern bool debugMode;

int get_max_connectivity(vector<int> cell_net_list[], int cell_count)
{
	int size_max=0;
	int size;
	for(int i=0;i<cell_count;i++)
	{
	//	if(cell_list[i].cell_id == 0)
	//		continue;
		
		size= cell_net_list[i].size();
		if(size > size_max)
		{
			size_max = size;
		}
	}
	return size_max;
}

void initialise(vector<Cell> detailed_net_list[], int detailed_netlist_size, vector<Cell> adjacent_list[], int adj_size )
{
	for(int i=0;i<detailed_netlist_size;i++)
	{
		for(int j=0;j<detailed_net_list[i].size();j++)
		{
			detailed_net_list[i][j].partition_id = (detailed_net_list[i][j].cell_id % 2) + 1;
		}
	}
	
	for(int i=0; i< adj_size; i++)
	{
		if(!adjacent_list[i].empty())
		{
			for(int j=0;j<adjacent_list[i].size();j++)
			{
				adjacent_list[i][j].partition_id = (adjacent_list[i][j].cell_id % 2) + 1;
			}
		}
		/*	
		if(!reduced_adjacent_list[i].empty())
		{
			for(int j=0;j<reduced_adjacent_list[i].size();j++)
			{
				reduced_adjacent_list[i][j].partition_id = (reduced_adjacent_list[i][j].cell_id % 2) + 1;
			}
		}
		*/
	}
}
void FM_Algorithm_wrapper_2 (vector<Cell> &cell_list, int cell_count, vector<int> cell_net_list[], vector<Cell> detailed_net_list[], int net_count , int initial_cutsize,
int threshold,vector<Cell> adjacent_list[], int max_connect, vector<Cell> &global_cell_list, vector<int> net_list[], double width, double height,
double x_coord, double y_coord, int mode,bool p_type, bool debugMode, const double area_constraint);

void create_cellid2index(vector<Cell> cell_list, map<int,int> &cellid2index);

bool search_cell(vector<Cell> cell_list, int cell_id)
{
	for(int i=0;i<cell_list.size();i++)
	{
		if(cell_list[i].cell_id == cell_id)
		{
			return true;
		}
	}
	return false;
}
int get_partition_count_5(vector<Cell> cell_list, int partition_id)
{
	int num=0;
	
	for(int i=0;i< cell_list.size();i++)
	{
		if(cell_list[i].cell_id ==0)
			continue;
		
		if(cell_list[i].partition_id == partition_id)
			num++;
	}
	return num;
}

int get_unlocked_cell_id_5(vector<Cell> cell_list)
{
		for(int i=0;i<cell_list.size();i++)
		{
			if(cell_list[i].cell_id ==0)
				continue;
			
			//int cell_id = i+1;
			if(cell_list[i].locked == false)
			{
				return cell_list[i].cell_id;
			}

		}
	return -1;
}

//----------------------------------------------
bool check_area_violation(int cell_id, int current_p_id, vector<Cell> cell_list, int threshold)
{
	int p1 = 1;
	int p2 = 2;
	
	int p1_count = get_partition_count_5(cell_list, p1);
	int p2_count = get_partition_count_5(cell_list, p2);
	
	if(current_p_id == p1)
	{
		p1_count--;
		p2_count++;
	}
	
	else if(current_p_id == p2)
	{
		p1_count++;
		p2_count--;
	}
		
	int diff = p1_count - p2_count;
	
	if(diff < 0 )
		diff = -diff;
	
//	cout<<"diff: "<<diff<<" threshold: "<<threshold<<endl;
	if(diff <= threshold)
		return false;
	
	else 
		return true;
}


bool check_area_violation_2(int current_p_id, vector<Cell> cell_list, int threshold)
{
	int p1 = 1;
	int p2 = 2;
	
	int p1_count = get_partition_count_5(cell_list, p1);
	int p2_count = get_partition_count_5(cell_list, p2);
	
	if(current_p_id == p1)
	{
		p1_count--;
		p2_count++;
	}
	
	else if(current_p_id == p2)
	{
		p1_count++;
		p2_count--;
	}
		
	int diff = p1_count - p2_count;
	
	if(diff < 0 )
		diff = -diff;
	
//	cout<<"diff: "<<diff<<" threshold: "<<threshold<<endl;
	if(diff <= threshold)
		return false;
	
	else 
		return true;
}

//------------------------------------------------
void unlock_all_cells_5 (vector <Cell> &cell_list)
{
	for(int i=0;i<cell_list.size();i++)
	{
		if(cell_list[i].cell_id == 0)
		{
			continue;
		}
		cell_list[i].locked=false;
	}
}
//--------------------------------------------------
void FM_Algorithm_single_pass_5(vector<Cell> cell_list, int cell_count, vector<int> cell_net_list[], vector<Cell> detailed_net_list[], int net_count, 
vector<Cell> adjacent_list[], int max_connect, vector<Cell> global_cell_list, map<int,int> &gain, int **netDistr,
vector<int> &movement, vector<int> &movementGain, vector<int> &movementGainSum, map<int,int> cellid2index, bool debugMode, const double area_constraint)
{
	//vector<int> bucket_ds[2*max_connect + 1];
//	cout<<"starting FM Algorithm single pass\n";	
	map<int, unordered_set<int> > bucket_ds_2;
	//vector<int> movement;
	//vector<int> movementGain;
	//vector<int> movementGainSum;
	map<int,int> :: iterator itr;
	int size_bucket = 2*max_connect + 1;
	//int threshold1 = (0.05 * cell_count) + 2;
	int threshold1 = (area_constraint * cell_count) + 2;
	int cell_id_g, gainCell;
	//---------
	//for(int i=0; i<gain.size();i++)
	
	for(int y= max_connect; y>= -max_connect; y--)
        {
		bucket_ds_2[y].insert(-1);
	}
	//cout<<"done initialising bucket ds\n";	
	int index2;
	//print_gain(gain,cell_count);
	for(itr=gain.begin(); itr != gain.end(); itr++)
	{
	//	bucket_ds[gain[i+1] + max_connect].push_back(i+1);
		index2 = itr->first;
		gainCell = itr->second;
	//	if(cell_id_g < 0)
	//		cout<<"cell_id_g: "<<cell_id_g<<endl;
			
	//	if(cell_id_g == 1907)
	//		cout<<"Found 1907\n";
		assert(index2 < cell_count);	
		assert(index2 >=0);
		assert(gainCell <= max_connect && max_connect >= -max_connect);
		bucket_ds_2[gainCell].insert(index2);
	}
	
//	print_bucket_ds(bucket_ds_2,max_connect);	
	//-------------------
	
	//print_bucket_ds(bucket_ds,size_bucket,max_connect);
	//cout<<"Hello"<<endl;
	int cell_id;
	int current_p_id;
	bool AreaViolation;
	int initial_sum_gain=0;
	int max_gain;
	bool valid=false;
	int net_id;
	int cell_id1;
	int ts, fs;
	int base_cell;
	int old_gain;
	int dest_p_id;
	unlock_all_cells_5(cell_list);
	bool exit=false;
	int count=0;
	int index, index1;
	int base_index;
	while(get_unlocked_cell_id_5(cell_list)!=-1)
	{
			
			//for(int h = size_bucket - 1; h>=0 ;h--)
			for(int h= max_connect; h >= -max_connect; h--)
			{
				exit = false;
			//	cout<<bucket_ds_2[h].size()<<endl;
				if(bucket_ds_2[h].size() > 0)
				{
					//cout<<"h: "<<h<<endl;
				//	or(int y=0; y < bucket_ds[h].size();y++)
				    for(auto it = bucket_ds_2[h].begin(); it != bucket_ds_2[h].end(); it++ )
					{
					//	cout<<"h: "<<h<<endl;
						
					//	cell_id = *it;
						index = *it;	
						//if(cell_id == 0 || cell_id == -1)
						//	continue;
						
						if(index == -1)
							continue;
						
						assert(index >= 0);
						
						current_p_id = cell_list[index].partition_id;
					//	AreaViolation = check_area_violation(cell_id,current_p_id,cell_list,threshold1);
						AreaViolation = check_area_violation_2(current_p_id,cell_list,threshold1);
						if(AreaViolation)
						{
						//	cout<<"Area violation"<<endl;
							continue;
						}
						//max_gain = h - max_connect;
						max_gain = h;
						cell_id = cell_list[index].cell_id;
						if(cell_id < 1)
						{
							cout<<"cell_id: "<<cell_id<<"index: "<<index<<endl;
						//	print_cell_list(cell_list);
						}
						assert(cell_id >= 1);
						//movement.push_back(cell_id);
						movement.push_back(index);
						base_cell = cell_id;
						base_index = index;
			//			cout<<"Active cell: "<<base_cell<<" gain: "<<max_gain<<endl;
						assert(base_cell > 0);
						movementGain.push_back(max_gain);
						initial_sum_gain += max_gain;
						movementGainSum.push_back(initial_sum_gain);
						cell_list[index].locked=true;
						global_cell_list[cell_id - 1].locked = true;
						//cout<<"h: "<<h<<endl;
					//	cout<<"locked active cell\n";
						
						//vector<int>::iterator it;
						//it = find(bucket_ds[max_gain+max_connect].begin(), bucket_ds[max_gain+max_connect].end(),base_cell );
						//assert(it != bucket_ds[max_gain + max_connect].end());
						//bucket_ds[max_gain + max_connect].erase(it);
						assert(bucket_ds_2[h].find(base_index) != bucket_ds_2[h].end());
						
						bucket_ds_2[h].erase(base_index);
						//if(cell_id == 1907)
						//	cout<<"deleted 1907\n";
						//cout<<"deleted the active cell from bucket data structure\n";
						//--------------------------
						//before making the move
						if(current_p_id ==1)
							dest_p_id = 2;
						else if(current_p_id == 2)
							dest_p_id = 1;	
						
						assert(base_cell >=1);	
						for(int x = 0; x < cell_net_list[base_cell - 1].size(); x++)
						{
							net_id = cell_net_list[base_cell - 1][x];
						
							//if(cell_list[base_cell - 1].partition_id == 1)
							if(cell_list[index].partition_id == 1)
							{
								ts = 1; 
								fs = 0;	
							}
							else if (cell_list[index].partition_id ==2)
							{
								ts = 0;
								fs = 1;
							}
							
							if(netDistr[net_id][ts] == 0)
							{
								for(int p = 0; p<detailed_net_list[net_id].size(); p++)
								{
									cell_id1 = detailed_net_list[net_id][p].cell_id;
									//int index3 = cellid2index[cell_id1];
									if(global_cell_list[cell_id1 - 1].locked == false && global_cell_list[cell_id1-1].active == true)
									{
										
										if(debugMode==true)
										{
											cout<<"Checking whether the cell is in cell list or not\n";
											assert(search_cell(cell_list,cell_id1) ==true);
										}
										
										int index3 = cellid2index[cell_id1];
										
									/*	
										if(gain[index3] > max_connect)
										{
											cout<<"greater:: index3: "<<index3<<" gain: "<<gain[index3]<<endl;
										}
										else if (gain[index3] < -max_connect)
										{
											cout<<"less:: index3: "<<index3<<" gain: "<<gain[index3]<<endl;
										}
										assert(gain[index3] <= max_connect && gain[index3] >= -max_connect);
									*/	
										
										old_gain = gain[index3];
										//assert(bucket_ds_2[old_gain].find(index3) != bucket_ds_2[old_gain].end());
										if(old_gain <= max_connect && old_gain >= -max_connect)
										{
											if(bucket_ds_2[old_gain].find(index3) != bucket_ds_2[old_gain].end())
												bucket_ds_2[old_gain].erase(index3);
											
										}
										//vector<int>::iterator it;
										//it = find(bucket_ds[old_gain+max_connect].begin(), bucket_ds[old_gain+max_connect].end(),cell_id1 );
																					
										
										//assert(it != bucket_ds[old_gain + max_connect].end());
										//bucket_ds[old_gain + max_connect].erase(it);
										
										gain[index3]++;
										//if(gain[index3] > max_connect)
										//	gain[index3] = max_connect;
					//					cout<<"ts: Incrementing gain of "<<cell_id1<<" initial gain: "<<old_gain<<" to "<<gain[cell_id1]<<endl;
										//bucket_ds[gain[cell_id1] + max_connect ].push_back(cell_id1);
									
										bucket_ds_2[old_gain + 1].insert(index3);
									}
								}	
							}
							else if (netDistr[net_id][ts] == 1)
							{
								for(int p = 0; p<detailed_net_list[net_id].size(); p++)
								{
									cell_id1 = detailed_net_list[net_id][p].cell_id;
									
									
									if(global_cell_list[cell_id1-1].active == true){
									int index3 = cellid2index[cell_id1];
										
										
										if(debugMode == true)
										{
											cout<<"Checking whether the cell is in cell list or not\n";
											assert(search_cell(cell_list,cell_id1)==true);
										}
										
										if(global_cell_list[cell_id1 - 1].locked == false && cell_list[index3].partition_id == dest_p_id)
										{
										//	int index3 = cellid2index[cell_id1];
										
										//	assert(gain[index3] <= max_connect && gain[index3] >= -max_connect);
											old_gain = gain[index3];
											//vector<int>::iterator it;
											//it = find(bucket_ds[old_gain+max_connect].begin(), bucket_ds[old_gain+max_connect].end(),cell_id1 );
											//assert(it != bucket_ds[old_gain + max_connect].end());
											//bucket_ds[old_gain + max_connect].erase(it);
										//	assert(bucket_ds_2[old_gain].find(index3) != bucket_ds_2[old_gain].end());
											
											if(old_gain <= max_connect && old_gain >= -max_connect)
											{
												if(bucket_ds_2[old_gain].find(index3) != bucket_ds_2[old_gain].end())
													bucket_ds_2[old_gain].erase(index3);
											}
										
											gain[index3]--;
										//	if(gain[index3] < -max_connect)
										//	gain[index3] = -max_connect;
						//					cout<<"ts: decrementing gain of "<<cell_id1<<" initial gain: "<<old_gain<<" to "<<gain[cell_id1]<<endl;
											//bucket_ds[gain[cell_id1] + max_connect ].push_back(cell_id1);
											bucket_ds_2[old_gain - 1].insert(index3);
										}
									}
								}	
							}
							
							//Moving cell to other partition-----------------------
							if(current_p_id == 1)
							{
								netDistr[net_id][0]--;
								netDistr[net_id][1]++;
							}
							else if( current_p_id == 2)
							{
								netDistr[net_id][0]++;
								netDistr[net_id][1]--;
							}
							
							cell_list[base_index].partition_id = dest_p_id;
							//------------------------------------------------------
							
							if(netDistr[net_id][fs] == 0)
							{
								for(int p = 0; p<detailed_net_list[net_id].size(); p++)
								{
										
									cell_id1 = detailed_net_list[net_id][p].cell_id;
									
									if(global_cell_list[cell_id1 - 1].locked == false && global_cell_list[cell_id1-1].active == true)
									{
										
										if(debugMode)
										{
											cout<<"Checking whether the cell is in cell list or not\n";
											assert(search_cell(cell_list,cell_id1)==true);
										}
										
										int index3 = cellid2index[cell_id1];
										
										//assert(gain[index3] <= max_connect && gain[index3] >= -max_connect);
										old_gain = gain[index3];
										
										//vector<int>::iterator it;
										//it = find(bucket_ds[old_gain+max_connect].begin(), bucket_ds[old_gain+max_connect].end(),cell_id1 );
										//assert(it != bucket_ds[old_gain + max_connect].end());
										//bucket_ds[old_gain + max_connect].erase(it);
										
										//assert(bucket_ds_2[old_gain].find(index3) != bucket_ds_2[old_gain].end());
										
										if(old_gain <= max_connect && old_gain >= -max_connect)
										{
										
												if(bucket_ds_2[old_gain].find(index3) != bucket_ds_2[old_gain].end())
													bucket_ds_2[old_gain].erase(index3);
										}
										//bucket_ds_2[old_gain].erase(index3);
											
										gain[index3]--;
									//	if(gain[index3] < -max_connect)
									//		gain[index3] = -max_connect;
					//					cout<<"ts: decrementing gain of "<<cell_id1<<" initial gain: "<<old_gain<<" to "<<gain[cell_id1]<<endl;
										//bucket_ds[gain[cell_id1] + max_connect ].push_back(cell_id1);
										bucket_ds_2[old_gain - 1].insert(index3);
									}
								}	
							}
							else if( netDistr[net_id][fs] == 1)
							{
								for(int p = 0; p<detailed_net_list[net_id].size(); p++)
								{
									cell_id1 = detailed_net_list[net_id][p].cell_id;
									int index3 = cellid2index[cell_id1];
									
									if(global_cell_list[cell_id1-1].active == true) 
									{
										int index3 = cellid2index[cell_id1];
										
										
										if(debugMode == true)
										{
											cout<<"Checking whether the cell is in cell list or not\n";
											assert(search_cell(cell_list,cell_id1)==true);
										}
										
									if(global_cell_list[cell_id1 - 1].locked == false && cell_list[index3].partition_id == current_p_id)
									{
										//int index3 = cellid2index[cell_id1];
										
										//assert(gain[index3] <= max_connect && gain[index3] >= -max_connect);
										old_gain = gain[index3];
										//vector<int>::iterator it;
										//it = find(bucket_ds[old_gain+max_connect].begin(), bucket_ds[old_gain+max_connect].end(),cell_id1 );
										//assert(it != bucket_ds[old_gain + max_connect].end());
										//bucket_ds[old_gain + max_connect].erase(it);
										if(bucket_ds_2[old_gain].find(index3) == bucket_ds_2[old_gain].end())
										{
											cout<<"gain: "<<gain[index3]<<" size: "<<bucket_ds_2[old_gain].size()<<endl;
											
											//print_gain(gain,cell_count);	
											//print_bucket_ds(bucket_ds_2,max_connect);	
											cout<<"active state of cell_id1: "<<cell_id1<<" "<<global_cell_list[cell_id1-1].active<<endl;
											cout<<"cell list state of cell_id1: "<<cell_id1<<" "<<search_cell(cell_list,cell_id1)<<endl;
										}
										//assert(bucket_ds_2[old_gain].find(index3) != bucket_ds_2[old_gain].end());
										
										if(old_gain <= max_connect && old_gain >= -max_connect)
										{
												if(bucket_ds_2[old_gain].find(index3) != bucket_ds_2[old_gain].end())
													bucket_ds_2[old_gain].erase(index3);
										}
										//bucket_ds_2[old_gain].erase(index3);
										
										gain[index3]++;
									//	if(gain[index3] > max_connect)
									//		gain[index3] = max_connect;
					//					cout<<"fs: Incrementing gain of "<<cell_id1<<" initial gain: "<<old_gain<<" to "<<gain[cell_id1]<<endl;
										//bucket_ds[gain[cell_id1] + max_connect ].push_back(cell_id1);
										bucket_ds_2[old_gain + 1].insert(index3);
									}
								}
								}	
							}
							
							
							
						}
						exit = true;
						break;
						//-------------------------
									 	
					}
				}
				if(exit==true)
					break;
			}
			count++;
			if(count > cell_count/2 || initial_sum_gain < 0)
				break;
		//	cout<<"count: "<<count<<endl;
			
			
	}
	bucket_ds_2.clear();
	
}



template<typename KeyType, typename ValueType> 
std::pair<KeyType,ValueType> get_max( const std::map<KeyType,ValueType>& x ) {
  using pairtype=std::pair<KeyType,ValueType>; 
  return *std::max_element(x.begin(), x.end(), [] (const pairtype & p1, const pairtype & p2) {
        return p1.second < p2.second;
  }); 
}

void update_detailed_netlist_55(vector<Cell> cell_list, vector<Cell> detailed_net_list[], int net_count)
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

void update_detailed_netlist_555(vector<Cell> cell_list, vector<Cell> detailed_net_list[], int net_count,vector<Cell> global_cell_list)
{
	//bool present=false;
	int cell_id,index;
	for(int i=0;i<net_count;i++)
	{
		for(int j=0;j<detailed_net_list[i].size();j++)
		{
				cell_id = detailed_net_list[i][j].cell_id;
				assert(cell_id >= 1);
				
				if(global_cell_list[cell_id - 1].active == true)
				{
					index = cell_id - 1;
					detailed_net_list[i][j].partition_id = global_cell_list[index].partition_id;
					detailed_net_list[i][j].x_dim = global_cell_list[index].x_dim;
					detailed_net_list[i][j].y_dim = global_cell_list[index].y_dim;
					//detailed_net_list[i][j].active = true;
				}
		}
	}
}

int 
calculate_cutsize_5(  vector<Cell> detailed_net_list[], int detailed_netlist_size)
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

int 
calculate_cutsize_55(  vector<Cell> detailed_net_list[], int detailed_netlist_size, vector<Cell> global_cell_list)
{
	int cut_size=0;
	int p1=0, p2=0;
	bool start=true;
	int cell_id;
	for(int i=0;i<detailed_netlist_size;i++)
	{
		start=true;
		for(int j=0;j<detailed_net_list[i].size();j++)
		{
			cell_id = detailed_net_list[i][j].cell_id;
			assert(cell_id >= 1);
			if(global_cell_list[cell_id - 1].active == true)
			{
			
				if(start==true)
				{
					p1 = detailed_net_list[i][j].partition_id;
					start=false;
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
	}
	return cut_size;
}

void print_cell_list_5(vector<Cell> cell_list)
{
	int p1_count=0, p2_count=0;
	cout<<"Cell_list: "<<endl;
	for(int i=0;i<cell_list.size();i++)
	{
		cout<<cell_list[i].cell_id<< "->"<<cell_list[i].partition_id<<endl;
		
		if(cell_list[i].partition_id == 1)
			p1_count++;
		
		if(cell_list[i].partition_id == 2)
			p2_count++;
		
	}
	cout<<"Partition1 count: "<<p1_count<<endl;
	cout<<"Partition2 count: "<<p2_count<<endl;
}






void create_net_distribution(vector<int> net_list[], int **netDistr, int net_count)
{
	for(int i=0; i< net_count; i++)
	{
		netDistr[i][0]=0;
		netDistr[i][1]=0;
		for(int j=0; j<net_list[i].size(); j++)
		{
			if(net_list[i][j] % 2 == 0)
			{
				netDistr[i][0]++;
			}
			else
				netDistr[i][1]++;
		}
	}
}

void create_net_distribution_2(vector<Cell> cell_list, vector<int> net_list[], int **netDistr, int net_count, vector<Cell> global_cell_list, 
bool p_type, double x_coord, double y_coord, double width, double height, int mode)
{
	int cell_id;
	for(int i=0; i< net_count; i++)
	{
		netDistr[i][0]=0;
		netDistr[i][1]=0;
		for(int j=0; j<net_list[i].size(); j++)
		{
			cell_id = net_list[i][j];
			assert(cell_id >= 1);
			if(mode == 0) // No terminal propagation
			{
				if(global_cell_list[cell_id - 1].active == true)
				{
					if(global_cell_list[cell_id - 1].partition_id == 1)
					{
						netDistr[i][0]++;	
					}
					else if(global_cell_list[cell_id - 1].partition_id == 2)
					{
						netDistr[i][1]++;
					}
				}
			}
			else if(mode == 1) // 50% window
			{
				if(p_type == 0 ) //Horizontal partitioning
				{
					if(global_cell_list[cell_id - 1].active == true)
					{
						if(global_cell_list[cell_id - 1].partition_id == 1)
						{
							netDistr[i][0]++;	
						}
						else if(global_cell_list[cell_id - 1].partition_id == 2)
						{
							netDistr[i][1]++;
						}
					}
					
					else if(global_cell_list[cell_id - 1].active == false)
					{
						if( global_cell_list[cell_id - 1].y_dim >= y_coord + height/4  )
						{
							netDistr[i][0]++;
						}
						else if (global_cell_list[cell_id - 1].y_dim <= y_coord - height/4 )
						{
							netDistr[i][1]++;
						}
					}
				}
				else if(p_type == 1) //Vertical cut
				{
					if(global_cell_list[cell_id - 1].active == true)
					{
						if(global_cell_list[cell_id - 1].partition_id == 1)
						{
							netDistr[i][0]++;	
						}
						else if(global_cell_list[cell_id - 1].partition_id == 2)
						{
							netDistr[i][1]++;
						}
					}
					
					else if(global_cell_list[cell_id - 1].active == false)
					{
						if( global_cell_list[cell_id - 1].x_dim >= x_coord + width/4  )
						{
							netDistr[i][1]++;
						}
						else if (global_cell_list[cell_id - 1].x_dim <= x_coord - width/4 )
						{
							netDistr[i][0]++;
						}
					}
				}
				
			}
			else if(mode == 2) //All terminal propagated
			{
				if(p_type == 0 ) //Horizontal partitioning
				{
					if(global_cell_list[cell_id - 1].active == true)
					{
						if(global_cell_list[cell_id - 1].partition_id == 1)
						{
							netDistr[i][0]++;	
						}
						else if(global_cell_list[cell_id - 1].partition_id == 2)
						{
							netDistr[i][1]++;
						}
					}
					
					else if(global_cell_list[cell_id - 1].active == false)
					{
						if( global_cell_list[cell_id - 1].y_dim >= y_coord  )
						{
							netDistr[i][0]++;
						}
						else if (global_cell_list[cell_id - 1].y_dim < y_coord )
						{
							netDistr[i][1]++;
						}
					}
				}
				else if(p_type == 1) //Vertical cut
				{
					if(global_cell_list[cell_id - 1].active == true)
					{
						if(global_cell_list[cell_id - 1].partition_id == 1)
						{
							netDistr[i][0]++;	
						}
						else if(global_cell_list[cell_id - 1].partition_id == 2)
						{
							netDistr[i][1]++;
						}
					}
					
					else if(global_cell_list[cell_id - 1].active == false)
					{
						if( global_cell_list[cell_id - 1].x_dim >= x_coord)
						{
							netDistr[i][1]++;
						}
						else if (global_cell_list[cell_id - 1].x_dim < x_coord )
						{
							netDistr[i][0]++;
						}
					}
				}
			}
			
		}
	}
}

void update_net_distribution(vector<int> cell_net_list[], int **netDistr, int net_count, int cell_count, vector<Cell> cell_list)
{
	int cell_id;
	int net_id;
	int p_id;
	
	for(int i=0;i<net_count;i++)
	{
		netDistr[i][0]=0;
		netDistr[i][1]=0;
	}
	//cout<<"Hello\n";
	for(int i=0; i< cell_count; i++)
	{
		cell_id = i + 1;
		
		p_id = cell_list[i].partition_id;
		
		for(int j=0; j<cell_net_list[i].size();j++)
		{
			net_id = cell_net_list[i][j];
			assert(net_id >=0);	
			
			
			if(p_id == 1)
			{
				netDistr[net_id][0]++;
			}
			else if(p_id == 2)
			{
				netDistr[net_id][1]++;
			}
		}
	}
}
void calculate_initial_gain(int **netDistr, int net_count, map<int,int> &gain, int cell_count, vector<int> cell_net_list[], vector<Cell> global_cell_list, vector<Cell> cell_list)
{
	int cell_id;
	int net_id;
	int gain1 = 0;
	int ts,fs;
	for(int i=0; i<cell_count;i++)
	{
		cell_id = cell_list[i].cell_id;
		//if(global_cell_list[cell_id - 1].cell_id ==0)
		//	continue;
		
		if(cell_id == 0)
			continue;
			//cout<<cell_id<<endl;
		assert(cell_id >= 1);
	
		
		
		//cout<<"cell_id: "<<cell_id<<endl;
		
		gain1 = 0;
		ts =0;
		fs =0;
		for(int j=0; j< cell_net_list[cell_id - 1].size();j++)
		{
			net_id = cell_net_list[cell_id - 1][j];
			
			if(cell_list[i].partition_id ==1)
			{
				ts = 1;
				fs = 0;
				
				if(netDistr[net_id][fs] == 1)
					gain1++;
				
				if(netDistr[net_id][ts] == 0)
					gain1--;
					
			}
			else if (cell_list[i].partition_id ==2)
			{
				ts = 0;
				fs = 1;
				
				if(netDistr[net_id][fs] == 1)
					gain1++;
				
				if(netDistr[net_id][ts] == 0)
					gain1--;
			}
		
		}
		gain.insert({cell_id,gain1});
	}
}


void calculate_initial_gain_5(int **netDistr, int net_count, map<int,int> &gain, int cell_count, vector<int> cell_net_list[], vector<Cell> global_cell_list, vector<Cell> cell_list)
{
	int cell_id;
	int net_id;
	int gain1 = 0;
	int ts,fs;
	for(int i=0; i<cell_count;i++)
	{
		cell_id = cell_list[i].cell_id;
		//if(global_cell_list[cell_id - 1].cell_id ==0)
		//	continue;
		
		if(cell_id == 0)
			continue;
			//cout<<cell_id<<endl;
		assert(cell_id >= 1);
	
		
		
		//cout<<"cell_id: "<<cell_id<<endl;
		
		gain1 = 0;
		ts =0;
		fs =0;
		for(int j=0; j< cell_net_list[cell_id - 1].size();j++)
		{
			net_id = cell_net_list[cell_id - 1][j];
			
			if(cell_list[i].partition_id ==1)
			{
				ts = 1;
				fs = 0;
				
				if(netDistr[net_id][fs] == 1)
					gain1++;
				
				if(netDistr[net_id][ts] == 0)
					gain1--;
					
			}
			else if (cell_list[i].partition_id ==2)
			{
				ts = 0;
				fs = 1;
				
				if(netDistr[net_id][fs] == 1)
					gain1++;
				
				if(netDistr[net_id][ts] == 0)
					gain1--;
			}
		
		}
		gain.insert({i,gain1});
	}
}
//------------------------------------------------
void calculate_initial_gain_2(int **netDistr, int net_count, map<int,int> &gain, int cell_count, vector<int> cell_net_list[], vector<Cell> global_cell_list,
 vector<Cell> cell_list)
{
	int cell_id;
	int net_id;
	int gain1 = 0;
	int ts,fs;
	for(int i=0; i<cell_count;i++)
	{
		
		if(global_cell_list[i].cell_id ==0)
			continue;
		
		cell_id = cell_list[i].cell_id;
		//cout<<"cell_id: "<<cell_id<<endl;
		assert(cell_id >= 1);
		gain1 = 0;
		ts =0;
		fs =0;
		for(int j=0; j< cell_net_list[cell_id - 1].size();j++)
		{
			net_id = cell_net_list[cell_id - 1][j];
			
			if(cell_list[cell_id - 1].partition_id ==1)
			{
				ts = 1;
				fs = 0;
				
				if(netDistr[net_id][fs] == 1)
					gain1++;
				
				if(netDistr[net_id][ts] == 0)
					gain1--;
					
			}
			else if (cell_list[cell_id - 1].partition_id ==2)
			{
				ts = 0;
				fs = 1;
				
				if(netDistr[net_id][fs] == 1)
					gain1++;
				
				if(netDistr[net_id][ts] == 0)
					gain1--;
			}
		
		}
		gain.insert({cell_id,gain1});
	}
}

void apply_FM_Algorithm_5(vector<Cell> &cell_list, int cell_count, vector<int> cell_net_list[], vector<Cell> detailed_net_list[], int net_count , int initial_cutsize
,vector<Cell> adjacent_list[], int max_connect, vector<Cell> &cell_ptr_p1, vector<Cell> &cell_ptr_p2, bool p_type,
vector<Cell> &global_cell_list, double x_coord, double y_coord, double width, double height, int mode, double delta, vector<int> net_list[], double chip_width,
double chip_height, bool debugMode, const double area_constraint)
{
	int p1 = 1;
	int p2 = 2;
	
	initialise_cell_list_2(cell_list,global_cell_list);
//	cout<<"Initial :: P1 count: "<<get_partition_count_5(cell_list,p1)<<" P2 count: "<<get_partition_count_5(cell_list, p2)<<endl;

	//cout<<"In apply_FM_algorithm function : cell count: "<<cell_count<<endl;
	
	int threshold = (0.1*cell_count) + 2;
	FM_Algorithm_wrapper_2(cell_list,cell_count,cell_net_list,detailed_net_list,net_count,initial_cutsize,threshold,adjacent_list,
	max_connect,global_cell_list,net_list, width, height, x_coord, y_coord, mode, p_type, debugMode, area_constraint);
	
	reset_global_cell_list(cell_list, global_cell_list);
	
	//---------------------------------------------------------------------------------------------------
	for(int i=0;i<cell_count;i++)
		{
			
			int cell_id;
			
			if(cell_list[i].partition_id == p1)
			{
				//Partition 1-----------------------------
				//cell belongs to partition 1
				cell_id = cell_list[i].cell_id;	
				
				
				if(p_type == 0) //Horizontal
				{
			
					cell_list[i].y_dim =  y_coord + height/4;
					assert(cell_list[i].y_dim <= chip_height);
					
					if(cell_id != 0)
					{
						assert(cell_id > 0);
						global_cell_list[cell_id - 1].y_dim = y_coord + height/4;
						global_cell_list[cell_id - 1].partition_id = 3;
					}
					
				}
				else if(p_type == 1) //vertical
				{
				//	x_coord = cell_list[i].x_dim;
			//		assert(x_coord <= chip_width);
					//cell_list[i].x_dim = x_coord/2;
					cell_list[i].x_dim = x_coord - width/4;
					assert(cell_list[i].x_dim <= chip_width);
					
					
					if(cell_id != 0)
					{
						assert(cell_id > 0);
						global_cell_list[cell_id - 1].x_dim = x_coord - width/4;
						global_cell_list[cell_id - 1].partition_id = 3;
					}
						
						
				}
				cell_ptr_p1.push_back(cell_list[i]);
				
			
			}
			else if (cell_list[i].partition_id == p2)
			{
				//Cell belongs to partition 2
				cell_id = cell_list[i].cell_id;
				if(p_type == 0) //horizontal cut
				{
					
				//	y_coord = cell_list[i].y_dim;
			//		assert(y_coord <= chip_height);
		//			if(cell_count < 1000)
		//				cout<<"i: "<<i<<endl;
				//	cell_list[i].y_dim =  (y_coord/2);
					cell_list[i].y_dim =  y_coord - height/4;
			//		assert(cell_list[i].y_dim <= chip_height);
					
					
					if(cell_id != 0)
					{
						assert(cell_id > 0);
						global_cell_list[cell_id - 1].y_dim = y_coord - height/4;
						global_cell_list[cell_id - 1].partition_id = 4;
					}
					
				}
				else if(p_type == 1) //vertical cut
				{
					
					
				//	x_coord = cell_list[i].x_dim;
					
		//			assert(x_coord <= chip_width);
				//	cell_list[i].x_dim = 3*x_coord/2;
					cell_list[i].x_dim = x_coord + width/4;
		//			assert(cell_list[i].x_dim <= chip_width);
		//			if(cell_count< 1000) cout<<"hello"<<" "<<x_coord<<endl;
					if(cell_id != 0)
					{
						assert(cell_id > 0);
						global_cell_list[cell_id - 1].x_dim = x_coord + width/4 ;
						global_cell_list[cell_id - 1].partition_id = 4;
					}
					
				}
				cell_ptr_p2.push_back(cell_list[i]);
			}
		}
	//---------------------------------------------------------------------------------------------------
	
	
	
}


void create_cellid2index(vector<Cell> cell_list, map<int,int> &cellid2index)
{
	int cell_id;
	for(int i=0; i< cell_list.size(); i++)
	{
		cell_id = cell_list[i].cell_id;
		if(cell_id !=0)
		{
			cellid2index.insert({cell_id,i} );
		}
	}
}
void FM_Algorithm_wrapper_2 (vector<Cell> &cell_list, int cell_count, vector<int> cell_net_list[], vector<Cell> detailed_net_list[], int net_count , int initial_cutsize,
int threshold,vector<Cell> adjacent_list[], int max_connect, vector<Cell> &global_cell_list, vector<int> net_list[], double width, double height,
double x_coord, double y_coord, int mode,bool p_type, bool debugMode, const double area_constraint)
{
	map<int,int> gain;
	//int old_cutsize = initial_cutsize;
	map<int, int> cellid2index;
	//Creating the net distribution--------
	int **netDistribution = new int*[net_count];
	for(int i=0;i<net_count;i++)
	{
		netDistribution[i] = new int [2];
		netDistribution[i][0] = 0;
		netDistribution[i][1] = 0;
	}
//	cout<<"mode: "<<mode<<endl;
//	cout<<"Debugmode: "<<debugMode<<endl;
	//create_net_distribution(net_list,netDistribution,net_count);
	//void create_net_distribution_2(vector<Cell> cell_list, vector<int> net_list[], int **netDistr, int net_count, vector<Cell> global_cell_list, 
//bool p_type, double x_coord, double y_coord, double width, double height, int mode)
	create_net_distribution_2(cell_list,net_list,netDistribution,net_count,global_cell_list,p_type,x_coord,y_coord,width,height,mode);
	//cout<<"net distribution created\n";
	calculate_initial_gain_5(netDistribution,net_count,gain,cell_count,cell_net_list,global_cell_list, cell_list);
	
//	int cut_size_3 =  
	//cout<<"initial gain computed\n";
	create_cellid2index(cell_list, cellid2index);
	int old_cutsize = calculate_cutsize_55(detailed_net_list,net_count,global_cell_list);
	//cout<<"cell2index map created. size: "<<cellid2index.size()<<endl;
	//------------------------------------------
	
	int max_gain=0,max_pos=0;
	vector<int> movement;
	vector<int> movementGain;
	vector<int> movementGainSum;
	
	vector<Cell> cell_list_old;
	int diff;
	int new_cut_size;
	int pass=0;
	do
		{
			//cout<<"pass: "<<pass<<endl;
			pass++;
			cell_list_old = cell_list;
		FM_Algorithm_single_pass_5(cell_list,cell_count,cell_net_list,detailed_net_list ,net_count,adjacent_list,max_connect, 
		global_cell_list, gain, netDistribution,movement,movementGain,movementGainSum, cellid2index, debugMode, area_constraint);
	//	cout<<"done with FM single pass\n";	
		//print_cell_list_5(cell_list);
		for(int i=0;i<movementGainSum.size();i++)
		{
			if(i==0)
			{
				max_pos=0;
				max_gain = movementGainSum[0];
			}
			else
			{
				if(max_gain < movementGainSum[i])
				{
					max_gain = movementGainSum[i];
					max_pos = i;
				}
			}
		}
		
		//cout<<"max_gain: "<<max_gain<<" max_pos: "<<max_pos<<endl;
		
		if(max_gain < 0)
			max_pos = 0;
			
		
		int cell_id2;
		int cell_id22;
		int dest_p_id;
		int index;
		for(int i=0; i<= max_pos;i++)
		{
			index = movement[i];
			assert(index >= 0);
			cell_id22 = cell_list[index].cell_id;
			if(cell_list[index].partition_id == 1 )
			{
				cell_list[index].partition_id =2;
				global_cell_list[cell_id22 - 1].partition_id =2;
			}
			else if(cell_list[index].partition_id == 2 )
			{
				cell_list[index].partition_id = 1;
				global_cell_list[cell_id22 - 1].partition_id =2;
			}
		}
		
		update_detailed_netlist_555(cell_list,detailed_net_list,net_count,global_cell_list);
		//cout<<"Updated detailed netlist\n";
		new_cut_size = calculate_cutsize_55(detailed_net_list,net_count,global_cell_list);
		//cout<<"calculated new cutsize: "<<new_cut_size<<endl;
	//	cout<<"new cut size: "<<new_cut_size<<endl;
	//	cout<<"p1 count: "<<get_partition_count_5(cell_list,1)<<" p2_count: "<<get_partition_count_5(cell_list,2)<<endl;
		
		update_net_distribution(cell_net_list,netDistribution,net_count,cell_count,cell_list);
		//create_net_distribution_2(cell_list,net_list,netDistribution,net_count,global_cell_list,p_type,x_coord,y_coord,width,height,mode);
		gain.clear();
		//cout<<"updated net distribution\n";
		calculate_initial_gain_5(netDistribution,net_count,gain,cell_count,cell_net_list,global_cell_list, cell_list);
		
		//print_gain(gain,cell_count);
		
		
		diff = old_cutsize - new_cut_size;
	//	cout<<"diff: "<<diff<<endl;
		old_cutsize = new_cut_size;
		movement.clear();
		movementGain.clear();
		movementGainSum.clear();
	}while(diff > 0);
	
	if(diff < 0)
	{
		//cout<<"Negative diff!!\n";
		update_net_distribution(cell_net_list,netDistribution,net_count,cell_count,cell_list_old);
		update_detailed_netlist_555(cell_list_old,detailed_net_list,net_count, global_cell_list);
	//	int new_cut_size = calculate_cutsize_55(detailed_net_list,net_count,global_cell_list);
	//	cout<<"new cut size: "<<new_cut_size<<endl;
	//	cout<<"p1 count: "<<get_partition_count_5(cell_list_old,1)<<" p2_count: "<<get_partition_count_5(cell_list_old,2)<<endl;
	}
	
	for(int i=0; i< net_count; i++)
		delete [] netDistribution[i];
	
	delete[] netDistribution;
	
	
	gain.clear();
	
	movement.clear();
	movement.shrink_to_fit();
	movementGain.clear();
	movementGain.shrink_to_fit();
	movementGainSum.clear();
	movementGainSum.shrink_to_fit();
	
	
}
