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
	vector<int>* ch0;
	int entry;
public:
	RawData();
	RawData(string *fileName);
	RawData(char *fileName);
	~RawData();
	int GetEntry(){return entry;};
	void SetEntry(int entry){this->entry=entry; this->tree->GetEntry(entry);};
	vector<int>* GetCh0(){return ch0;};
	TGraph* GetTrace();
		
};


#endif 
