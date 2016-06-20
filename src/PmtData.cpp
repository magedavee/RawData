#include"PmtData.h"
#include<iostream>
#include<queue>
#include <cstdlib>
#include<TH1D.h>
#include<TGraph.h>
using namespace std;
PmtData::PmtData()
{
	
}
PmtData::PmtData(char *filename):RawData(filename)
{
	entry=0;
	num=GetEntries();
	sub=false;
	for(int i=0;i<NCHA;++i)
	{

	    DC[i]=new vector<double>();
	}
	cutoff=3980;
	charge[0]=new TH1D("chargeLeft","charge Left",10000,0,10000);
	charge[1]=new TH1D("chargeRight","charge Right",10000,0,10000);
	pulseIntegral[0]=new vector<int>();
	pulseIntegral[1]=new vector<int>();
}




TH1D* PmtData::GetIntegral(int cha)
{
	if(cha>NCHA)
	{
	    cout<<"channel higher than NCHA will be set to 0";
	    cha=0;
	}
	
	return charge[cha];
}

int PmtData::GetPulseIntegral(int cha,int event)
{
    if(cha>NCHA)
    {
	cout<<"channel is greater then NCHA.";
	return 0;
    }
    
    if(event>=num)
    {
	cout<<"event is greater then "<< num;
	return 0;
    }
    return pulseIntegral[cha]->at(event);
}
void PmtData::CalIntegral(int cha)
{
    int ent=GetEntries();
    for(int i=0;i<ent;++i)
    {
	//this order the values so that everything in pulse 
	//which is less then 0 is pushed to the end
	priority_queue<int> *lebCal=new priority_queue<int>();
	SetEntry(i);
	if(i%10000==0)
	{
	    cout<<"event "<<i<<" out of "<<ent<<" for channel "<<cha<<endl;
	}
	for(int j=0;j<300;++j)
	{
	    int val=ch[cha]->at(j);
	    lebCal->push(val);
	}
	double dc=0;
	//calculate the DC for the trace by taking the 
	//highest values average
	for(int j=0;j<250;++j)
	{
	    int val=lebCal->top();
	    dc+=(double)val/250.00;
	    lebCal->pop();
	}
	DC[cha]->push_back(dc);
	//calcualte pulse intergral with whats left
	int pulseInt=0;
	while(!lebCal->empty())
	{
	    
	    int val=lebCal->top()-dc;
	    pulseInt-=4*val;
	    lebCal->pop();
	}
	pulseIntegral[cha]->push_back(pulseInt);
	delete lebCal;
    }

		
}


TGraph* PmtData::GetTrace(int cha)
{
	float x[300];
        float y[300];
	if(cha>NCHA)
	{
	    cout<<"channel higher than NCHA will be set to 0";
	    cha=0;
	}
        for(int i=0;i<300&&i<ch[cha]->size();++i)
        {
                x[i]=i*4;
                y[i]=(float)ch[cha]->at(i)-DC[cha]->at(this->GetEntry());
        }
        TGraph* trace=new TGraph(300,x,y);
        return trace;

}
array<int,300> PmtData::GetPulse(int cha,int event)
{
	array<int,300> x;
	if(cha>NCHA)
	{
	    cout<<"channel higher than NCHA will be set to 0";
	    cha=0;
	}
	for(int i=0;i<300&&i<ch[cha]->size();++i)
	{
		x[i]=ch[cha]->at(i)-DC[cha]->at(event);
	}
        return x;

}


void PmtData::Write(char *fileName)
{

	cout<<fileName<<endl;
	TFile * file=new TFile(fileName,"RECREATE");
	this->charge[0]->Write();
	this->charge[1]->Write();
	file->Close();	
}
