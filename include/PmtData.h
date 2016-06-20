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
	vector<double> *DC[NCHA];
	bool sub;
	vector <int>* pulseIntegral[NCHA];
	char* histName;
	vector<int>* time[NCHA];
public:
	PmtData(char* filename);
	PmtData();
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
	vector <int>* GetTimeList(int);
	int GetTime();
	void SetNum(int num){this->num=num;};
	int GetNum(){return this->num;};
	TH1D* GetIntegralHist(int);
	void CalIntegral(int);
	int GetPulseIntegral(int,int);
	TGraph* GetTrace(int);
	void Write(char* file);
	array<int,300> GetPulse(int,int);

};


#endif
