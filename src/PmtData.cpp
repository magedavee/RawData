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
	    time[i]=new vector<int>();
	    pulseIntegral[i]=new vector<int>();
	}
}




TH1D* PmtData::GetIntegralHist(int cha)
{
	TH1D* charge;
	if(cha>NCHA)
	{
	    cout<<"channel higher than "<< NCHA<<" will be set to 0";
	    cha=0;
	}

	for(int i=0;i<num;++i)
	{
	    charge->Fill(pulseIntegral[cha]->at(i));	    
	}
	
	return charge;
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
	int val=0;
	while(!lebCal->empty())
	{
	    
	    val=lebCal->top()-dc;
	    pulseInt-=4*val;
	    lebCal->pop();
	}


	pulseIntegral[cha]->push_back(pulseInt);
	int peak=val;
	TGraph* gr=GetTrace(cha);
	for(int j=0;j<1200;++j)
	{
	    int check=gr->Eval(j);
	    if(check<val/2)
	    {
		time[cha]->push_back(j);
		break;
	    }
	}
	delete gr;	
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
	for(int i=0;i<NCHA;++i)
	{
	    this->GetIntegralHist(i)->Write();
	}
	file->Close();	
}
