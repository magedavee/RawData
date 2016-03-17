#include<TFile.h>
#include<TTree.h>
#include<TGraph.h>
#include<string>
#include<vector>
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
	void SetEntry(int entry){this->entry=entry; this->tree->GetEntry(entry);};
	vector<int>* GetCh(int cha){return ch[cha];};
	TGraph* GetTrace(int);
		
};


#endif 
