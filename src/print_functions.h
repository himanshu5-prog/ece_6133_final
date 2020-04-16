//------------------------------------------------------------------------------------------------
//Name: Himanshu Yadav
//Date: April 16th, 2020
//Description: This file contains print function to print various data structure/data for debugging
//-------------------------------------------------------------------------------------------------
#include<iostream>
#include<vector>
using namespace std;

void print_bucket_ds (map<int,unordered_set<int>> bucket_ds, int max_connect)
{
	for(int p= max_connect; p>= -max_connect; p--)
	{	
		cout<<p<<" -> ";
		for(auto it = bucket_ds[p].begin(); it!= bucket_ds[p].end(); it++)
		{
			cout<<*it<<" ";
		}
		cout<<endl;
	}
}
void print_func (vector<int> net_list[], int net_size)
{
	for(int i=0;i<net_size;i++)
	{
		for(auto x:net_list[i])
		{
			cout<<x<<" ";
		}
		cout<<endl;
	}
}
void print_func_2 (vector<int> net_list[], int net_size)
{
	for(int i=0;i<net_size;i++)
	{
		for(int j=0;j<net_list[i].size();j++)
		{
			cout<<net_list[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
}

void print_detailed_netlist(vector<Cell> detailed_net_list[], int net_count)
{
	cout<<"Detailed Netlist:"<<endl;
	for(int i=0;i<net_count;i++)
	{
		for(auto x: detailed_net_list[i])
		{
			cout<<x.cell_id<<" ("<<x.partition_id<<") ";
		}
		cout<<endl;
	}
	cout<<endl;
}

void print_adj_list(vector<Cell> adj_list[], int size)
{
	cout<<"Adjacent list: "<<endl;
	for(int i=0;i<size;i++)
	{
		if(!adj_list[i].empty())
		{
			for(auto x: adj_list[i])
			{
				cout<<x.cell_id<< " ("<<x.partition_id<<") ";
			}
		}
		cout<<endl;
	}
	cout<<endl;
}


void print_pair_list( vector< pair<int,int> > pair_list )
{
	vector< pair<int,int> >::iterator itr;
	itr = pair_list.begin();
	
	while(itr != pair_list.end())
	{
		cout<<itr->first<< " "<<itr->second;
		itr++;
		cout<<endl;
	}
	
}

void print_weight_list (vector< pair<int,int> > pair_list, map< pair<int,int>, double> weight)
{
	vector< pair<int,int> >::iterator itr;
	itr = pair_list.begin();
	
	while(itr!= pair_list.end())
	{	
		cout<<"( "<<itr->first<<" "<<itr->second<<" ): ";
		cout<<weight[{itr->first, itr->second}]<<endl;
		itr++;
	}
}

void print_cell_list(vector<Cell> cell_list)
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

void print_partition_list(vector<int> v)
{
	for(int i=0;i<v.size();i++)
	{
		cout<<v[i]<<" ";
	}
	cout<<endl;
}

void print_cell_netlist(vector<int> cell_net_list[], int cell_count)
{
	cout<<"Cell Netlist: "<<endl;
	for(int i=0;i<cell_count;i++)
	{
		for(int j=0;j<cell_net_list[i].size();j++)
		{
			cout<<cell_net_list[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
}

void print_netDistr(int **netDistr, int net_count)
{
	for(int i=0;i<net_count;i++)
	{
		cout<<netDistr[i][0]<<" , "<<netDistr[i][1]<<endl;
	}
}

void print_gain(map<int,int> gain, int cell_count)
{
	cout<<"Gain: \n";
	cout<<"size: "<<gain.size()<<endl;
	
	map<int,int>::iterator itr;
	for(auto itr=gain.begin();itr!=gain.end();itr++)
	{
		cout<<itr->first<<"-> ";
		cout<<itr->second<<endl;
	}
	
}

void print_bucket_ds( vector<int> bucket_ds[], int bucket_size, int max_connect)
{
	cout<<"Bucket data structure:\n";
	for(int i=0; i<bucket_size;i++)
	{
		cout<<i-max_connect<<" : ";
		for(int j=0;j<bucket_ds[i].size();j++)
		{
			cout<<bucket_ds[i][j]<<" ";
		}
		cout<<endl;
	}
}

void print_vector(vector<int> vec1, vector<int> vec2, vector<int> vec3)
{
	for(int i=0;i<vec1.size();i++)
	{
		cout<<vec1[i]<<" "<<vec2[i]<<" "<<vec3[i]<<endl;
	}
}
