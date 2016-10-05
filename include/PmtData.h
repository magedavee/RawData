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
	vector<float>* time[NCHA];
	void CheckCha(int cha)
	{
	    //cerr<<"db"<<" 100 CalIntegral check\n"; 
	    if(time[cha]->size()==0)
	    {
		//cerr<<"db"<<" 101 CalIntegral cha1\n"; 
		CalIntegral();
	    }
	}
public:
	void CalIntegral();
	void CalTime();
	PmtData(const char*);
	PmtData(const char*,const char* );
	PmtData();
	void SetEntry(int entry)
	{
	    //cerr<<"db"<<" 000 PmtData.h SetEntry\n";
	    int num=GetEntries();
	    if(entry<num&&num>=0)
	    {
		//cerr<<"db"<<" 001 PmtData.h SetEntry to  "<<entry<<endl;
		this->entry=entry; 
		this->tree->GetEntry(entry);	
	    }
	    else
	    {
		    //cerr<<"db"<<entry<<" is out range [0,"<<num <<"). Entry not changed.\n";
	    }
    };
	vector <int>* GetTimeList(int);
	float GetTime();
	TH1D GetIntegralHist(int);
	int GetPulseIntegral(int,int);
	TGraph* GetTrace(int);
	void Write(char* file);
	array<int,300> GetPulse(int,int);
	float DeltaT(int,int);
	float GetT(int cha)
	{
	    if(cha<NCHA && cha>=0)
	    {
		int ent=GetEntry();
		return time[cha]->at(ent);
	    }
	    else
		cerr<<"channel "<<cha<<" not in range\n";
	    return 0.0;
    
	};
	float AveT(int,int);

};


#endif
