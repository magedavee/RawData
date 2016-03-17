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
<<<<<<< HEAD
	int GetNCha(){return nCha;};
	int GetEntries	(){return tree->GetEntries  ();	};   
	void SetEntry(int entry){this->entry=entry; this->tree->GetEntry(entry);};
	vector<int>* GetCh(int cha){return ch[cha];};
	TGraph* GetTrace(int);
=======
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
>>>>>>> fbf93376856ef18ade416f0b69026336b41c0742
		
};


#endif 
