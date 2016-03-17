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
	double DC[2];
	bool sub;
	TH1D* charge[2];
	vector <int>* pulseIntegral[2];
	char* histName;
	int cutoff;
	void FindPeaks(int);
	vector<array<int,300>>* peak0;
	vector<array<int,300>>* peak1;
public:
	//void SetHistName(char * name){this->charge->SetName(name);};
	PmtData(char* filename);
	PmtData();
	double GetDC(int);
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
	TH1D* GetIntegral(int);
	void CalIntegral(int);
	int GetPulseIntegral(int,int);
	TGraph* GetTrace(int);
	TGraph* GetTraceSubNoise();
	TGraph* GetTracePeak();
	void Write(char* file);
	array<int,300> GetPulse(int);

};


#endif
