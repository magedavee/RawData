#include"PmtData.h"
#include<iostream>
#include<queue>
#include<cstdlib>
#include<TH1D.h>
#include<TGraph.h>
#include<limits>
#include<string>
#include<TTree.h>
#include <unistd.h>
using namespace std;
PmtData::PmtData()
{
	
}
PmtData :: PmtData(const char *filename): RawData(filename)
{
	//cerr<<"db"<<" 100    PmtData const char* "<<filename<<endl;
	for(int cha=0;cha<NCHA;++cha)
	{
	    //cerr<<"db"<<" 101   PmtData cha "<<cha<<endl;

	    DC[cha]=new vector<double>();
	    time[cha]=new vector<float>();
	    pulseIntegral[cha]=new vector<int>();
	//cerr<<"db"<<" 100    PmtData DC "<<DC[cha]<<endl;
	}
	SetEntry(0);
	//cerr<<"db"<<&ch[0]<<" "<<ch[0]->at(0)<<endl;
}

PmtData :: PmtData(const char* filename,const char* setup): PmtData(filename)
{
    //cerr<<"db"<<" 200   PmtData const char* filename  "<<filename;
    //cerr<<"db"<<"   const setup char* "<<setup<<endl;
    cout<<"using setup file "<<setup<<endl;
    TFile file(setup);
    //cerr<<"db"<<" 201   PmtData  TFILE  "<<&file<<endl;
    TTree* setTree=(TTree*)file.Get(setup);
    int chan,pulse;
    float dc;
    float t;
    cout<<"setting up branch DC\n";
    //cerr<<"db"<<" 203   PmtData  dc  "<<&dc<<endl;
    setTree->SetBranchAddress("DC",&dc);
    cout<<"setting up branch pulseIntegral\n";
    setTree->SetBranchAddress("pulseIntegral",&pulse);
    cout<<"setting up branch time\n";
    setTree->SetBranchAddress("time",&t);
    cout<<"setting up branch channel\n";
    setTree->SetBranchAddress("channel",&chan);
    int ent=setTree->GetEntries();
    for(int i=0;i<ent;++i)
    {
	setTree->GetEntry(i);
	//cerr<<"db"<<" 208  PmtData "<<i<<endl;
	//cerr<<"db"<<"  206   PmtData cha "<<chan<<endl;
	//cerr<<"db"<<"  207   PmtData dc "<<dc<<endl;
	DC[chan]->push_back((double)dc);
	//cerr<<"db"<<"  207   PmtData dc "<<pulse<<endl;
	pulseIntegral[chan]->push_back(pulse);
	//cerr<<"db"<<"  207   PmtData dc "<<t<<endl;
	time[chan]->push_back(t);
	//cerr<<"db"<<"  209   PmtData filling "<<t<<endl;
    }


    delete setTree;

}



TH1D PmtData:: GetIntegralHist(int cha)
{
	//cerr<<"db"<<" 301 PmtData GetIntegral hist\n";
	int num=GetEntries();
	double top=0;
	double bot=numeric_limits<int>::max();
	if(pulseIntegral[cha]->size()==0)
	{
	    CalIntegral();
	}
	for(int i=0;i<num;++i)
	{
	    //cerr<<"db"<<" 301 PmtData GetIntegral pulseIntegral.size "<<pulseIntegral[cha]->size()<<endl;
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
	    cout<<"channel higher than "<< NCHA-1<<" will be set to 0\n";
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
    //cerr<<"db"<<" 000 PmtData Get Pulse Integral int "<<cha;
    //cerr<<"db"<<" int "<<event<<endl;
    int num=GetEntries();
    if(cha>NCHA)
    {
	//cerr<<"db"<<"channel is greater then NCHA.";
	return 0;
    }
    
    if(event>=num)
    {
	//cerr<<"db"<<"event is greater then "<< num;
	return 0;
    }
    //cerr<<"db"<<" 001 checking pulseIntegral size "<<endl;
    if(pulseIntegral[cha]->size()==0)
    {
	//cerr<<"db"<<"  011 PmtData size"<<pulseIntegral[cha]->size()<<endl;
	//cerr<<"db"<<"  012 PmtData CalIntegral next\n";
	CalIntegral();
    }
	//cerr<<"db"<<"  002 PmtData pulseIntegral size "<<pulseIntegral[cha]->size()<<" event  "<<event<<endl;
	//cerr<<"db"<<"  003 PmtData pulseIntegral->at "<<pulseIntegral[cha]->at(event)<<endl;
    return pulseIntegral[cha]->at(event);
}
void PmtData::CalTime()
{
    int ent=GetEntries();
    for(int cha=0;cha<NCHA;++cha)
    {
	time[cha]->clear();
	for(int i=0;i<ent;++i)
	{
	    if(i%10000==0)
	    {
		cout<<"Timing event "<<i<<" out of "<<ent<<" for channel "<<cha<<endl;
	    }
	    SetEntry(i);
	    float val=999999;
	    int index=0;
	    TGraph* gr=GetTrace(cha);
	    for(int j=0;j<1200;++j)
	    {
		int check=gr->Eval(j);
		if(check < val)
		{
		    index=j;
		    val =check;
		}
	    }
	    //cerr<<"db"<<"event val index "<<i<<" "<<val<<" "<<index<<endl;
	    float t=0;
	    int j=index;
	    int stop=0;
	    int old=j;
	    while(j>stop)
	    {

		float check=gr->Eval(j);
		//cerr<<"db"<<"cal time check j check val/2  "<<j<<" "<<check<<" "<<val/2<<" "<<endl;
		if(check > val/2)
		{
		    //cerr<<"db"<<"event "<<i<<endl;
		    //cerr<<"db"<<"cal time before j stop old  "<<j<<" "<<stop<<" "<<old<<" "<<endl;
		    stop=j;
		    j=old;
		    //cerr<<"db"<<"cal time after j stop old  "<<j<<" "<<stop<<" "<<old<<" "<<endl;


		}
		else
		{
		    old=j;
		    j=(j+stop)/2;
		}
	    }
	    
	    //cerr<<"db"<<"j is  "<<j<<endl;
	    for(float k=(float)j ; k<j+2 ; k+=.1)
	    {
		int check=gr->Eval(k);
		//cerr<<"db"<<"time is: "<<k<<" "<<j<<endl;
		if(check<val/2)
		{
		    t=k;
		    stop=true;
		    break;
		}
	    }
	    time[cha]->push_back(t);
	    //cerr<<"db"<<"event : "<<i<<" cha: "<<cha<<" peak: "<<val<<" time: "<<t<<endl;
	    delete gr;	
	}
    }
}


void PmtData::CalIntegral()
{
    //cerr<<"db"<<" 600  Calintegral\n ";
    int ent=GetEntries();
    for(int cha=0;cha<NCHA;++cha)
    {
	DC[cha]->clear();
	pulseIntegral[cha]->clear();
	time[cha]->clear();
	//cerr<<"db"<<"605 Calintergral chan "<<cha<<endl;
	for(int i=0;i<ent;++i)
	{
	    //cerr<<"db"<<" 601  event "<<i<<" chan "<<cha<<endl;
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
		//cerr<<"db"<<" 602  lebCal push "<<i<<" chan "<<cha<< " at  "<<j<<endl;
		//cerr<<"db"<<" 602 channel "<<cha<<" is size "<<ch[cha]->size()<<endl;
		int val=ch[cha]->at(j);
		//cerr<<"db"<<" 602 pushing val "<<val<<endl;
		lebCal->push(val);
	    }
	    double dc=0;
	    //calculate the DC for the trace by taking the 
	    //highest values average
	    for(int j=0;j<200;++j)
	    {
		//cerr<<"db"<<" 603  lebCal Dc "<<i<<" chan "<<cha<< " at  "<<j<<endl;
		int val=lebCal->top();
		dc+=(double)val/200.00;
		lebCal->pop();
	    }
	    //cerr<<"db"<<" 6031   Dc is  "<<dc<<endl;
	    DC[cha]->push_back(dc);
	    //calcualte pulse intergral with whats left
	    int pulseInt=0;
	    int val=0;
	    while(!lebCal->empty())
	    {
		//cerr<<"db"<<" 604  lebCal  "<<i<<" chan "<<cha<<endl;
		
		val=lebCal->top()-dc;
		//cerr<<"db"<<" 604 poping val "<<val<<endl;
		pulseInt-=4*val;
		lebCal->pop();
	    }

	    //cerr<<"db"<<" 6041 pulseInt "<<pulseInt<<endl;

	    pulseIntegral[cha]->push_back(pulseInt);
	    TGraph* gr=GetTrace(cha);
	    delete gr;	
	    delete lebCal;
	}
    }
    CalTime();
    //cerr<<"db"<<" 600END  Calintegral end \n";
		
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

float PmtData::DeltaT(int cha1,int cha2)
{
    //cerr<<"db"<<" 500  DeltaT cha1 "<<cha1<<" cha2  "<<cha2<<endl;
    //cerr<<"db"<<" 500  DeltaT size1 "<<time[cha1]->size()<<" cha2  "<<time[cha1]->size()<<endl;
    int ent=GetEntry();
    //cerr<<"db"<<" 501 DeltaT Checking Size cha1\n"; 
    //if(time[cha1]->size()==0)
    //{
	////cerr<<"db"<<" 501 CalIntegral cha1\n"; 
	//CalIntegral();
    //}
    
    ////cerr<<"db"<<" 501 DeltaT Checking Size cha2\n"; 
    //if(time[cha2]->size()==0)
    //{
	////cerr<<"db"<<" 501 CalIntegral cha2\n"; 
	//CalIntegral();
    //}
    CheckCha(cha1);
    CheckCha(cha2);
    float t1=time[cha1]->at(ent);
    float t2=time[cha2]->at(ent);
    //cerr<<"db"<<" 502  t1 "<<t1<<" t2 "<<t2<<endl;
    float dt=t1-t2;
    //cerr<<"db"<<" 502  Delta "<<dt<<endl;
    return dt;
}


float PmtData::AveT(int cha1,int cha2)
{
    //cerr<<"db"<<" 500  DeltaT cha1 "<<cha1<<" cha2  "<<cha2<<endl;
    //cerr<<"db"<<" 500  DeltaT size1 "<<time[cha1]->size()<<" cha2  "<<time[cha1]->size()<<endl;
    int ent=GetEntry();
    //cerr<<"db"<<" 501 DeltaT Checking Size cha1\n"; 
    CheckCha(cha1);
    CheckCha(cha2);
    float t1=time[cha1]->at(ent);
    float t2=time[cha2]->at(ent);
    //cerr<<"db"<<" 502  t1 "<<t1<<" t2 "<<t2<<endl;
    float dt=(t1+t2/2.0);
    //cerr<<"db"<<" 502  Delta "<<dt<<endl;
    return dt;
}


void PmtData::Write(char *fileName)
{

	cout<<"writing  "<<fileName<<endl;
	TFile  file(fileName,"RECREATE");
	//vector<double>* dc=new vector<double>();
	//vector <int>* pulse=new vector<int>();
	//vector<int>* t=new vector<int>();
	//vector<uint32_t*>* ncl  =new vector<uint32_t*>();
	int pulse,chan;
	float dc,t;
	TTree treeOut(fileName,fileName);
	treeOut.Branch("DC",&dc,"dc/F");
	treeOut.Branch("pulseIntegral",&pulse,"pulse/I");
	treeOut.Branch("time",&t,"t/F");
	treeOut.Branch("channel",&chan,"chan/I");
	for(int cha=0;cha<NCHA;++cha)
	{
	    if(DC[cha]->size()==0)
	    {
		CalIntegral();
	    }

	    if(pulseIntegral[cha]->size()==0)
	    {
		CalIntegral();
	    }
	    if(time[cha]->size()==0)
	    {
		CalIntegral();
	    }
	    cout<<"writing channel "<<cha<<endl;
	    this->GetIntegralHist(cha).Write();
	 

	    for(int i=0;i<DC[cha]->size();++i)
	    {
		//cerr<<"db"<<"for event: "<<i<<endl;
		dc=(int)DC[cha]->at(i);
		//cerr<<"db"<<" dc:  "<<dc<<endl;
		pulse=pulseIntegral[cha]->at(i);
		//cerr<<"db"<<" pulse:  "<<pulse<<endl;
		t=time[cha]->at(i);
		//cerr<<"db"<<"time:  "<<t<<endl;
		chan=cha;
		//cerr<<"db"<<"chan: "<<chan<<endl;
		//cerr<<"db"<<"Writing dc pulse time chan i ";
		//cerr<<"db"<<dc<<" "<<pulse<<" "<<t<<" "<<chan<<i<<endl;
		treeOut.Fill();
	    }
	    
	
	}
	treeOut.Write();
	file.Close();	
}
