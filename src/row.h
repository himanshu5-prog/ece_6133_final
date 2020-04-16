//----------------------------------------------------------------
//Name: Himanshu Yadav
//Date: April 16th, 2020
//Description: This file contains data structure for placement row
//----------------------------------------------------------------
#include<iostream>
using namespace std;

struct Row
{
	double rowPointer;
	double y_coord;
	vector<int> cell_list;
	map<int,double> x_coordinate;
	vector<int> cell_order;
};
