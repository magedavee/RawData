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
	vector<int>* ch[2];
	int entry;
	uint32_t nCha;
public:
	RawData();
	RawData(string *fileName);
	RawData(char *fileName);
	~RawData();
	int GetEntry(){return entry;};
	int GetNCha(){return nCha;};
	int GetEntries	(){return tree->GetEntries  ();	};   
	vector<int>* GetCh(int cha){return ch[cha];};
	TGraph* GetTrace(int);
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
	vector<int>* GetCh0(int cha){return ch[cha];};
	TGraph* GetTrace();
		
};


#endif 
