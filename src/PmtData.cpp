#include"PmtData.h"
#include<iostream>
#include<queue>
#include<cstdlib>
#include<TH1D.h>
#include<TGraph.h>
#include<limits>
#include<string>
#include<TTree.h>
using namespace std;
PmtData::PmtData()
{
	
}
PmtData :: PmtData(const char *filename): RawData(filename)
{
	cerr<<" 100    PmtData const char* "<<filename<<endl;
	for(int cha=0;cha<NCHA;++cha)
	{
	    cerr<<" 101   PmtData cha "<<cha<<endl;

	    DC[cha]=new vector<double>();
	    time[cha]=new vector<int>();
	    pulseIntegral[cha]=new vector<int>();
	}
}

PmtData :: PmtData(const char* filename,const char* setup): PmtData(filename)
{
    cerr<<" 200   PmtData const char* filename  "<<filename;
    cerr<<"   const setup char* "<<setup<<endl;
    cout<<"using setup file "<<setup<<endl;
    TFile file(setup);
    cerr<<" 201   PmtData  TFILE  "<<&file<<endl;
    TTree* setTree=(TTree*)file.Get(setup);
    cerr<<" 202   PmtData  TTree* "<<setTree<<endl;
    vector<double>* dc=new vector<double>();
    cerr<<" 203   PmtData  vectot<double> * "<<dc<<endl;
    vector <int>* pulse=new vector<int>();
    cerr<<" 204   PmtData  vectot<int>*  "<<pulse<<endl;
    vector<int>* t=new vector<int>();
    cerr<<" 205   PmtData  vectot<int>*  "<<t<<endl;
    cout<<"setting up branch DC\n";
    setTree->SetBranchAddress("DC",&dc);
    cout<<"setting up branch pulseIntegral\n";
    setTree->SetBranchAddress("pulseIntegral",&pulse);
    cout<<"setting up branch time\n";
    setTree->SetBranchAddress("time",&t);
    for(int cha=0;cha<NCHA;++cha)
    {
	cerr<<"  202   PmtData cha "<<cha<<endl;
	setTree->GetEntry(cha);
	DC[cha]=dc;
	pulseIntegral[cha]=pulse;
	time[cha]=t;
    }
    delete setTree;

}



TH1D PmtData:: GetIntegralHist(int cha)
{
	cerr<<" 301 PmtData GetIntegral hist\n";
	int num=GetEntries();
	double top=0;
	double bot=numeric_limits<int>::max();
	if(pulseIntegral[cha]->size()==0)
	{
	    CalIntegral(cha);
	}
	for(int i=0;i<num;++i)
	{
	    cerr<<" 301 PmtData GetIntegral pulseIntegral.size "<<pulseIntegral[cha]->size()<<endl;
	    int val=pulseIntegral[cha]->at(i);
	    if(val>top)
	    {

		top=val;
	    }
	    if(val<bot)
	    {
		bot=val;
	    }

	}
	
	string name="hist_";
	string chan=to_string(cha);
	name+=chan;
	TH1D charge(name.c_str(),"Pulse Integrals",100,bot,top);
	if(cha>=NCHA)
	{
	    cout<<"channel higher than "<< NCHA-1<<" will be set to 0";
	    cha=0;
	}
	for(int i=0;i<num;++i)
	{
	    charge.Fill(pulseIntegral[cha]->at(i));	    
	}
	
	return charge;
}

int PmtData :: GetPulseIntegral(int cha,int event)
{
    cerr<<" 000 PmtData Get Pulse Integral int "<<cha;
    cerr<<" int "<<event<<endl;
    int num=GetEntries();
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
    cerr<<"  001 PmtData pulseIntegral[cha= "<<cha;
    cerr<<"] <<.size() "<<pulseIntegral[cha]->size()<<endl;
    if(pulseIntegral[cha]->size()==0)
    {
	CalIntegral(cha);
    }
    return pulseIntegral[cha]->at(event);
}
void PmtData::CalIntegral(int cha)
{
    DC[cha]->clear();
    pulseIntegral[cha]->clear();
    time[cha]->clear();
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

	cout<<"writing  "<<fileName<<endl;
	TFile  file(fileName,"RECREATE");
	vector<double>* dc=new vector<double>();
	vector <int>* pulse=new vector<int>();
	vector<int>* t=new vector<int>();
	TTree treeOut(fileName,fileName);
	for(int cha=0;cha<NCHA;++cha)
	{
	    cout<<"writing channel "<<cha<<endl;
	    this->GetIntegralHist(cha).Write();

	    dc=DC[cha];
	    pulse=pulseIntegral[cha];
	    t=time[cha];
	    
	    string dcName("DC");
	    int bufDC=dc->size();//*sizeof(int);
	    int bufPul=pulse->size();//*sizeof(int);
	    int bufT=t->size();//*sizeof(int);
	    treeOut.Branch("DC",&dc);
	    treeOut.Branch("pulseIntegral",&pulse);
	    treeOut.Branch("time",&t);
	    treeOut.Fill();
	
	}
	treeOut.Write();
	file.Close();	
}
