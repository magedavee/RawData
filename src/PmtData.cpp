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
	cerr<<" 100    PmtData DC "<<DC[cha]<<endl;
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
    int chan,pulse,t;
    int dc;
    cout<<"setting up branch DC\n";
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
	cerr<<" 208  PmtData "<<i<<endl;
	cerr<<"  206   PmtData cha "<<chan<<endl;
	cerr<<"  207   PmtData dc "<<dc<<endl;
	DC[chan]->push_back((double)dc);
	cerr<<"  207   PmtData dc "<<pulse<<endl;
	pulseIntegral[chan]->push_back(pulse);
	cerr<<"  207   PmtData dc "<<t<<endl;
	time[chan]->push_back(t);
	cerr<<"  209   PmtData filling "<<t<<endl;
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
    cerr<<" 000 PmtData Get Pulse Integral int "<<cha;
    cerr<<" int "<<event<<endl;
    int num=GetEntries();
    if(cha>NCHA)
    {
	cerr<<"channel is greater then NCHA.";
	return 0;
    }
    
    if(event>=num)
    {
	cerr<<"event is greater then "<< num;
	return 0;
    }
    cerr<<" 001 checking pulseIntegral size "<<endl;
    if(pulseIntegral[cha]->size()==0)
    {
	cerr<<"  011 PmtData size"<<pulseIntegral[cha]->size()<<endl;
	cerr<<"  012 PmtData CalIntegral next\n";
	CalIntegral(cha);
    }
	cerr<<"  002 PmtData pulseIntegral size "<<pulseIntegral[cha]->size()<<" event  "<<event<<endl;
	cerr<<"  003 PmtData pulseIntegral->at "<<pulseIntegral[cha]->at(event)<<endl;
    return pulseIntegral[cha]->at(event);
}
void PmtData::CalIntegral(int cha)
{
    cerr<<" 600  Calintegral\n ";
    DC[cha]->clear();
    pulseIntegral[cha]->clear();
    time[cha]->clear();
    int ent=GetEntries();
    for(int cha=0;cha<NCHA;++cha)
    {
	cerr<<"605 Calintergral chan "<<cha<<endl;
	for(int i=0;i<ent;++i)
	{
	    cerr<<" 601  event "<<i<<" chan "<<cha<<endl;
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
		    cerr<<"0602 CalIntegral time "<<j<<endl;
		    time[cha]->push_back(j);
		    break;
		}
	    }
	    delete gr;	
	    delete lebCal;
	}
    }
    cerr<<" 600END  Calintegral end \n";
		
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

int PmtData::DeltaT(int cha1,int cha2)
{
    cerr<<" 500  DeltaT cha1 "<<cha1<<" cha2  "<<cha2<<endl;
    cerr<<" 500  DeltaT size1 "<<time[cha1]->size()<<" cha2  "<<time[cha1]->size()<<endl;
    int ent=GetEntry();
    cerr<<" 501 DeltaT Checking Size cha1\n"; 
    if(time[cha1]->size()==0)
    {
	cerr<<" 501 CalIntegral cha1\n"; 
	CalIntegral(cha1);
    }
    
    cerr<<" 501 DeltaT Checking Size cha2\n"; 
    if(time[cha2]->size()==0)
    {
	cerr<<" 501 CalIntegral cha2\n"; 
	CalIntegral(cha1);
    } 
    int t1=time[cha1]->at(ent);
    int t2=time[cha2]->at(ent);
    cerr<<" 502  t1 "<<t1<<" t2 "<<t2<<endl;
    int dt=t1-t2;
    cerr<<" 502  Delta "<<dt<<endl;
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
	int dc,t,pulse,chan;
	TTree treeOut(fileName,fileName);
	treeOut.Branch("DC",&dc,"dc/I");
	treeOut.Branch("pulseIntegral",&pulse,"pulse/I");
	treeOut.Branch("time",&t,"t/I");
	treeOut.Branch("channel",&chan,"chan/I");
	for(int cha=0;cha<NCHA;++cha)
	{
	    if(DC[cha]->size()==0)
	    {
		CalIntegral(cha);
	    }

	    if(pulseIntegral[cha]->size()==0)
	    {
		CalIntegral(cha);
	    }
	    if(time[cha]->size()==0)
	    {
		CalIntegral(cha);
	    }
	    cout<<"writing channel "<<cha<<endl;
	    this->GetIntegralHist(cha).Write();
	 

	    for(int i=0;i<DC[cha]->size();++i)
	    {
		dc=(int)DC[cha]->at(i);
		pulse=pulseIntegral[cha]->at(i);
		t=time[cha]->at(i);
		chan=cha;
		cerr<<"Writing dc pulse time chan i ";
		cerr<<dc<<" "<<pulse<<" "<<t<<" "<<chan<<i<<endl;
		treeOut.Fill();
	    }
	    
	
	}
	treeOut.Write();
	file.Close();	
}
