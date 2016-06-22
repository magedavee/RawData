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
	vector<double> *DC[NCHA];
	vector <int>* pulseIntegral[NCHA];
	vector<int>* time[NCHA];
public:
	PmtData(const char*);
	PmtData(const char*,const char* );
	PmtData();
	void SetEntry(int entry)
	{
	    cerr<<" 000 PmtData.h SetEntry\n";
	    int num=GetEntries();
	    if(entry<num&&num>=0)
	    {
		cerr<<" 001 PmtData.h if entry "<<entry<<endl;
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
	TH1D GetIntegralHist(int);
	void CalIntegral(int);
	int GetPulseIntegral(int,int);
	TGraph* GetTrace(int);
	void Write(char* file);
	array<int,300> GetPulse(int,int);

};


#endif
