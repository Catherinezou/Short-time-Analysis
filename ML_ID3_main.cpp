#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <ctype.h>
#include <conio.h>
#include <sstream>
#include <math.h>
#include <algorithm>

using namespace std; //Ziyan Zou

//definition of variables-training
static string dir_trainingdata;
vector<string> titles;
vector<vector<bool>>trainingdata;
static int featurenum;
static int result_index;
vector<bool> prunenode_indicates;
vector<int> testfeature;
vector<int> result_without_node;
static int node = 0;
static int leaf = 0;

//definition of variables-validation
static string dir_validationdata;
vector<vector<bool>>validationdata;
vector<string> validation_titles;

// definition of variables-testing
vector<string> totalcmd;
static string dir_testingdata;
vector<string> testing_titles;
vector<vector<bool>>testingdata;

//definition of functions
void read_trainingdata();
void read_testingdata();
void read_validationdata();
int testing(vector<int>&  input_indicates, int start_index);
void newnode(vector<int>& input_indicates, int node_index);
bool usedfeature(vector<int>&  input_indicates);
float initialentropy(vector<int>&  input_indicates);
float entropy_calculation(int a, int b);
vector<bool> comparison(vector<int>&  input_indicates);
float conditionalentropy(vector<int>&  input_indicates, int attribute_index);
int countusedfutures(vector<int>&  input_indicates);


int main()
{
	// read training data
	cout<< "Please input the name of training data (eg. training_set.csv)";
	cin >> dir_trainingdata;
	ifstream input_trainingdata;
	input_trainingdata.open(dir_trainingdata, ios::in);
	if (!input_trainingdata.is_open())
	{
		cout << "It is a invalid name, please input again\n";
	}
	else
	{
		input_trainingdata.close();
	}
	read_trainingdata();

	// training model
	vector<int> indicates(featurenum, -1);
	vector<int> tmp(pow(2, featurenum + 1) - 1, -1);
	vector<bool> bool_tmp(pow(2, featurenum + 1) - 1, false);
	prunenode_indicates = bool_tmp;
	testfeature = tmp;
	result_without_node = tmp;

	cout << "Node Size is : " << result_without_node.size() << "\n";
	newnode(indicates, 1);
	cout << "\n=====summary=====\n";
	int right = 0;
	int wrong = 0;
	for (int j = 0; j < trainingdata.size(); j++)
	{
		vector<int> test;
		for (int n = 0; n < featurenum; n++)
		{
			if (trainingdata[j][n])
			{
				test.push_back(1);
			}
			else
			{
				test.push_back(0);
			}
		}
		
		if ((testing(test, 1) == 1 && trainingdata[j][result_index]))
		{
			right++;
		}
		else if (testing(test, 1) == 0 && (!trainingdata[j][result_index]))
		{
			right++;
		}
		else
		{
			wrong++;
		}
	}
	float accuracy = (float)right / (float)(right + wrong);

	
	cout << "Number of training instances = " << trainingdata.size() << "\n";
	cout << "Number of training attributes = " << featurenum << "\n";
	cout << "Total number of nodes in the tree = " << node << "\n";
	cout << "Number of leaf nodes in the tree = " << leaf << "\n";
	cout << "Accuracy of the model on the training dataset = ";
	cout.precision(3);
	cout << 100 * accuracy << "%\n\n";
	

	cout << "please prune the tree" << "\n";
	node = 0;
	leaf = 0;

	cout << "Please input the name of validation data to prune the tree (eg. validation_set.csv)";
	cin >> dir_validationdata;
	ifstream input_validationdata;
	input_validationdata.open(dir_validationdata, ios::in);
	if (!input_validationdata.is_open())
	{
		cout << "It is a invalid name, please input again\n";
	}
	else
	{
		input_validationdata.close();
	}
	read_validationdata();

	//prune the model
	for (int i = 0; i<totalcmd.size(); i++)
	{
		cout << totalcmd[i];
	}
	cout << "\n";
	vector<int> prunednode;
	bool prune_node = false;
	while (!prune_node)
	{
		cout << "\nPlease select the node that you want to delete?(Please divide numbers by ',' .ie 1,2,3,4)\n";
		stringstream  number;
		string  input;
		string before_int;
		int is_int;
		cin >> input;
		number << input;
		prunednode.clear();
		prune_node = true;
		while (getline(number, before_int, ','))
		{
			if (!(stringstream(before_int) >> is_int))
			{
				cout << "Please input valid number\n";
				prune_node = false;
				break;
			}
			if (is_int <= result_without_node.size() && is_int > 0)
			{
				prunednode.push_back(is_int);
			}
			else
			{
				cout << "The number exceeds maximun number of nodes!";
				prune_node = false;
				break;
			}
		}
		vector<int> new_feature_indicates(featurenum, -1);

		testfeature = tmp;
		result_without_node = tmp;
		for (int n = 0; n < prunednode.size(); n++)
		{
			prunenode_indicates[prunednode[n] - 1] = true;
		}
		newnode(new_feature_indicates, 1);
	}

	cout << "\n=====Above is the new model after pruning=====\n";
	right = 0;
	wrong = 0;
	for (int j = 0; j<validationdata.size(); j++)
	{
		vector<int>totest;
		for (int n = 0; n<featurenum; n++)
		{
			if (validationdata[j][n])
			{
				totest.push_back(1);
			}
			else
			{
				totest.push_back(0);
			}
		}


		if ((testing(totest, 1) == 1 && validationdata[j][result_index]))
		{
			right++;
		}
		else if (testing(totest, 1) == 0 && (!validationdata[j][result_index]))
		{
			right++;
		}
		else
		{
			wrong++;
		}
	}
	accuracy = (float)right / (float)(right + wrong);

	cout << "Number of validation instances = " << validationdata.size() << "\n";
	cout << "Number of validation attributes = " << featurenum << "\n";
	cout << "Total number of nodes in the tree = " << node << "\n";
	cout << "Number of leaf nodes in the tree = " << leaf << "\n";
	cout << "Accuracy of the model on the validation dataset = ";
	cout.precision(3);
	cout << 100 * accuracy << "%\n\n";

	
  // testing model
	bool file = false;
	while (1)
	{
		dir_testingdata = "";
		cout << "Please input the name of testing data (eg. test_set.csv): ";
		cin >> dir_testingdata;
		//if (dir_testingdata == "Q" || dir_testingdata == "q") { break; }
		ifstream input_testingdata;
		input_testingdata.open(dir_testingdata, ios::in);
		if (!input_testingdata.is_open())
		{
			cout << "It is a invalid name, please input again!\n";
		}
		else
		{
			testing_titles.clear();
			testingdata.clear();
			input_testingdata.close();
			file = true;
			read_testingdata();
		//if (testing_titles.size() != titles.size()) 	{ cout << "Testing data does not fit the model!!!\n"; continue;	}

			right = 0;
			wrong = 0;
			for (int j = 0; j < testingdata.size(); j++)
			{
				vector<int>totest;
				for (int n = 0; n < featurenum; n++)
				{
					if (testingdata[j][n])
					{
						totest.push_back(1);
					}
					else
					{
						totest.push_back(0);
					}
				}


				if ((testing(totest, 1) == 1 && testingdata[j][result_index]))
				{
					right++;
				}
				else if (testing(totest, 1) == 0 && (!testingdata[j][result_index]))
				{
					right++;
				}
				else
				{
					wrong++;
				}
			}
			float accuracy = (float)right / (float)(right + wrong);
			
			cout << "\n=======Summary of Testing=======\n";
			cout << "Number of testing instances = " << testingdata.size() << "\n";
			cout << "Number of training attributes = " << featurenum << "\n";
			cout << "Accuracy of the model on the training dataset = ";
			cout.precision(3);
			cout << 100 * accuracy << "%\n\n";
		}
	}
}

////////////////////////////////////////////////////////////////////
//functions
//function of read training data
void read_trainingdata()
{
	ifstream input_trainingdata;
	string each_line;
	input_trainingdata.open(dir_trainingdata, ios::in);

	// get feature title
	getline(input_trainingdata, each_line, '\n');
	int i = 0;
	int j = 0;
	string tmp;
	for (int n = 0; n < each_line.size(); n++)
	{
		if ((each_line[n] >= '0'&& each_line[n] <= 'z') && (each_line[n + 1]<'0' || each_line[n + 1]>'z'))
		{
			tmp = each_line.substr(i, n - i + 1);
			titles.push_back(tmp);
		}
		if ((each_line[n + 1] >= '0'&&each_line[n + 1] <= 'z') && (each_line[n]<'0' || each_line[n]>'z'))
		{
			i = n + 1;
		}
	}
	// get the values 
	while (getline(input_trainingdata, each_line, '\n'))
	{
		vector<bool> line_boolean;
		for (int n = 0; n < each_line.size(); n++)
		{
			if (each_line[n] == '0')
			{
				line_boolean.push_back(false);
			}
			else if (each_line[n] == '1')
			{
				line_boolean.push_back(true);
			}
		}
		if (line_boolean.size() == titles.size())
		{
			trainingdata.push_back(line_boolean);
		}
	}
	featurenum = titles.size() - 1;
	result_index = titles.size() - 1;
	
	
	input_trainingdata.close();
}


// function of reading testing data
void read_testingdata()
{
	ifstream input_testingdata;
	string each_line;
	input_testingdata.open(dir_testingdata, ios::in);
	getline(input_testingdata, each_line, '\n');
	int i, j;
	i = 0; j = 0;
	string tmp;
	for (int n = 0; n<each_line.size(); n++)
	{
		if ((each_line[n] >= '0'&&each_line[n] <= 'z') && (each_line[n + 1]<'0' || each_line[n + 1]>'z'))
		{
			tmp = each_line.substr(i, n - i + 1);
			testing_titles.push_back(tmp);
		}
		if ((each_line[n + 1] >= '0'&&each_line[n + 1] <= 'z') && (each_line[n]<'0' || each_line[n]>'z'))
		{
			i = n + 1;
		}
	}
	//get the values
	while (getline(input_testingdata, each_line, '\n'))
	{
		vector<bool> line_boolean;
		for (int n = 0; n<each_line.size(); n++)
		{
			if (each_line[n] == '0') { line_boolean.push_back(false); }
			else if (each_line[n] == '1') { line_boolean.push_back(true); }
		}
		if (line_boolean.size() == titles.size())
		{
			testingdata.push_back(line_boolean);
		}
	}
}


//function of reading validation data
void read_validationdata()
{
	ifstream input_validationdata;
	string each_line;
	input_validationdata.open(dir_validationdata, ios::in);
	getline(input_validationdata, each_line, '\n');
	int i, j;
	i = 0; j = 0;
	string tmp;
	for (int n = 0; n<each_line.size(); n++)
	{
		if ((each_line[n] >= '0'&&each_line[n] <= 'z') && (each_line[n + 1]<'0' || each_line[n + 1]>'z'))
		{
			tmp = each_line.substr(i, n - i + 1);
			validation_titles.push_back(tmp);
		}
		if ((each_line[n + 1] >= '0'&&each_line[n + 1] <= 'z') && (each_line[n]<'0' || each_line[n]>'z'))
		{
			i = n + 1;
		}
	}
	//get the values
	while (getline(input_validationdata, each_line, '\n'))
	{
		vector<bool> line_boolean;
		for (int n = 0; n<each_line.size(); n++)
		{
			if (each_line[n] == '0') { line_boolean.push_back(false); }
			else if (each_line[n] == '1') { line_boolean.push_back(true); }
		}
		if (line_boolean.size() == titles.size())
		{
			validationdata.push_back(line_boolean);
		}
	}
}


//function of testing
int testing(vector<int>&input_indicates, int start_index)
{
	int return_value;
	if (testfeature[start_index - 1] == -1)
	{
		return result_without_node[start_index - 1];
	}
	if (input_indicates[testfeature[start_index - 1]] == 0) 
	{ 
		return_value = testing(input_indicates, 2 * start_index);
	}
	if (input_indicates[testfeature[start_index - 1]] == 1) 
	{ 
		return_value = testing(input_indicates, 2 * start_index + 1); 
	}
	return return_value;
}


//function of which features are used
bool usedfeature(vector<int>&  input_indicates)
{
	for (int i = 0; i<input_indicates.size(); i++)
	{
		if (input_indicates[i] == -1) 
		{ 
			return false; 
		}
	}
	return true;
}


//function of calculation entropy
float entropy_calculation(int a, int b)
{
	if (a == 0 || b == 0)return 0.0;
	float p1; 
	float p2;
	p1 = (float)a / ((float)b + (float)a);
	p2 = 1-p1;
	float entropy;
	entropy = -1.0*p1*(log10f(p1) / log10f(2.0)) - p2*(log10f(p2) / log10f(2.0));
	return entropy;
}


//function of getting initial entropy
float initialentropy(vector<int>& input_indicates)
{
	int featurepos = 0;
	int featureneg = 0;

	for (int i = 0; i<trainingdata.size(); i++)
	{
		bool flag = true;
		for (int j = 0; j<featurenum; j++)
		{
			if (input_indicates[j] == -1)
			continue;
			if ((input_indicates[j] == 1 && trainingdata[i][j]) || (input_indicates[j] == 0 && (!trainingdata[i][j])))
			{
			}
			else
			{
				flag = false;
				break;
			}
		}
		if (flag)
		{
			if (trainingdata[i][result_index])
			{
				featurepos++;
			}
			else
			{
				featureneg++;
			}
		}	
	}
	return entropy_calculation(featurepos, featureneg);
}

// function of calculating the conditional entropy
float conditionalentropy(vector<int>&  input_indicates, int attribute_index)
{
		int featurepos = 0;
		int featureneg = 0;
		int featurepos_pos = 0;	
		int featurepos_neg = 0;
		int featureneg_pos = 0;
		int featureneg_neg = 0;

		for (int i = 0; i<trainingdata.size(); i++)
		{
			bool flag = true;
			for (int j = 0; j<featurenum; j++)
			{
				if (input_indicates[j] == -1)continue;
				if ((input_indicates[j] == 1 && trainingdata[i][j]) || (input_indicates[j] == 0 && (!trainingdata[i][j])))
				{
				}
				else
				{
					flag = false;
					break;
				}
			}
			if (flag)
			{
				if (trainingdata[i][attribute_index])
				{
					featurepos++;
					if (trainingdata[i][result_index])
					{
						featurepos_pos++;
					}
					else
					{
						featurepos_neg++;
					}
				}
				else
				{
					featureneg++;
					if (trainingdata[i][result_index])
					{
						featureneg_pos++;
					}
					else
					{
						featureneg_neg++;
					}
				}
			}
		}
		
		float ratio_p = (float)featurepos / ((float)featurepos + (float)featureneg);
		float ratio_n = 1 - ratio_p;
		return ratio_p*entropy_calculation(featurepos_pos, featurepos_neg) + ratio_n*entropy_calculation(featureneg_neg, featureneg_pos);
}


// function of comparing whose entropy is more
vector<bool> comparison(vector<int>&  input_indicates)
{
	bool b0 = true;	bool b1 = false;
	vector<bool> return_value;
	int respos = 0;
	int resneg = 0;

	for (int i = 0; i<trainingdata.size(); i++)
	{
		bool flag = true;
		for (int j = 0; j<featurenum; j++)
		{
			if (input_indicates[j] == -1)continue;
			if ((input_indicates[j] == 1 && trainingdata[i][j]) || (input_indicates[j] == 0 && (!trainingdata[i][j])))
			{
			}
			else
			{
				flag = false;
				break;
			}
		}
		if (flag)
		{
			if (trainingdata[i][result_index])	{	respos++;	}
			else   { resneg++;	}
		}
	}
	if (respos >= resneg) { b0 = true; }
	else	{ b0 = false; }

	if (resneg == 0 || respos == 0) { b1 = true; }
	else
	{
		b1 = false;
	}
	return_value.push_back(b0);
	return_value.push_back(b1);
	return return_value;

}

// function of counting the number of features that are used
int countusedfutures(vector<int>&  input_indicates)
{
	int n = 0;
	for (int i = 0; i < input_indicates.size(); i++)
	{
		if (input_indicates[i] == 1 || input_indicates[i] == 0)
			n++;
	}
	return n;
}

// function of creating new node
void newnode(vector<int>&  input_indicates, int node_index)
{
	node++;
	int save_index = node_index - 1;
	int tmp;
	bool nonfeature = usedfeature(input_indicates);
	vector<bool> resultcon = comparison(input_indicates);
	if (!resultcon[0])	{ tmp = 0; }
	else  { tmp = 1; }
	
	result_without_node[save_index] = tmp;

	if (nonfeature)
	{
		cout << ":" << resultcon[0];
		if (resultcon[0])
		{
			totalcmd.push_back(":1");
		}
		else
		{
			totalcmd.push_back(":0");
		}
		leaf++;
		return;
	}
	if (resultcon[1])
	{
		cout << ":" << resultcon[0];
		if (resultcon[0])
		{
			totalcmd.push_back(":1");
		}
		else
		{
			totalcmd.push_back(":0");
		}
		leaf++;
		return;
	}


	if (prunenode_indicates[save_index])
	{
		cout << ":" << resultcon[0];
		if (resultcon[0])
		{
			totalcmd.push_back(":1");
		}
		else
		{
			totalcmd.push_back(":0");
		}
		leaf++;
		return;
	}



	float ini_entropy = initialentropy(input_indicates);
	float IG_max = -0.1;
	float IG = 0.0;
	int max_index = 0;
	for (int i = 0; i<featurenum; i++)
	{
		if (input_indicates[i] != -1) {
			continue;
		}
		else
		{
			IG = ini_entropy - conditionalentropy(input_indicates, i);
			if (IG>IG_max)
			{
				IG_max = IG;
				max_index = i;
			}
		}
	}

	testfeature[save_index] = max_index;
	int symbol_number = countusedfutures(input_indicates);
	cout << "\n";
	totalcmd.push_back("\n");
	for (int j = 0; j<symbol_number; j++)
	{
		cout << "| ";
		totalcmd.push_back("| ");
	}
	cout << titles[max_index] << " = 0";
	totalcmd.push_back(titles[max_index] + "(" + std::to_string(node_index) + ") = 0");
	vector<int> mask_neg = input_indicates;
	mask_neg[max_index] = 0;
	newnode(mask_neg, 2 * node_index);

	cout << "\n";
	totalcmd.push_back("\n");
	for (int j = 0; j<symbol_number; j++)
	{
		cout << "| ";
		totalcmd.push_back("| ");
	}
	cout << titles[max_index] << " = 1";
	totalcmd.push_back(titles[max_index] + "(" + std::to_string(node_index) + ") = 1");
	vector<int> mask_pos = input_indicates;
	mask_pos[max_index] = 1;
	newnode(mask_pos, 2 * node_index + 1);
}
