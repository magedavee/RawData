#include"RawData.h"
#include <vector>
#include <array>
#include<TH1D.h>
#include<TGraph.h>
#include<iostream>
#ifndef PMTDATA_HH
#define PMTDATA_HH
using namespace std;
class PmtData:public RawData
{
protected:
	int num;
	double DC;
	bool sub;
	TH1D* charge;
	char* histName;
	int cutoff;
	void FindPeaks();
	vector<array<int,500>>* peak0;
	vector<array<int,500>>* peak1;
public:
	void SetHistName(char * name){this->charge->SetName(name);};
	PmtData(char* filename);
	PmtData();
	double GetDC();
	void SetEntry(int entry)
	{
		if(entry<num&&num>=0)
		{
			this->entry=entry; 
			this->tree->GetEntry(entry);	
		}
		else
		{
			cout<<entry<<" is out range [0,"<<num <<"). Entry not changed.\n";
		}
	};
	void SetNum(int num){this->num=num;};
	int GetNum(){return this->num;};
	TH1D* GetIntegral();
	void CalIntegral();
	TGraph* GetTrace();
	TGraph* GetTraceSubNoise();
	TGraph* GetTracePeak();
	void Write(char* file);

};


#endif
