#include<TFile.h>
#include<TTree.h>
#include<TGraph.h>
#include<string>
#include<vector>
#include<iostream>
#ifndef RAWDATA_HH
#define RAWDATA_HH
using namespace std;
class RawData
{
protected:
	TFile * file;
	TTree* tree;
	vector<int>* ch0;
	int entry;
public:
	RawData();
	RawData(string *fileName);
	RawData(char *fileName);
	~RawData();
	int GetEntry(){return entry;};
	int GetTotalEvents(){return tree->GetEntries();};
	void SetEntry(int num)
	{
	    if(num>=0&&num<this->GetTotalEvents())
	    {
		this->entry=num; 
		this->tree->GetEntry(num);
	    }
	    else
		cout<<num<<" is not in bounds\n";
	};
	vector<int>* GetCh0(){return ch0;};
	TGraph* GetTrace();
		
};


#endif 
