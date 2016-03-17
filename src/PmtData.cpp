#include"PmtData.h"
#include<iostream>
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
	DC[0]=0;
	DC[1]=0;
	GetDC(0);
	GetDC(1);
	cutoff=3980;
	peak0=new vector<array<int,300>>();
	peak1=new vector<array<int,300>>();
	charge[0]=new TH1D("chargeLeft","charge Left",10000,0,10000);
	charge[1]=new TH1D("chargeRight","charge Right",10000,0,10000);
	pulseIntegral[0]=new vector<int>();
	pulseIntegral[1]=new vector<int>();
}

double PmtData::GetDC(int cha)
{
	if(cha>2)
	{
	    cout<<"channel higher than 2 will be set to 0";
	    cha=0;
	}
	if(DC[cha]!=0)
	{
		return DC[cha];
	}
	else
	{
		TH1D hist("noise","ch0",10000,0,10000);
		int ent=this->GetEntry();
		int kurt=1000;
		int index=0;
		double mean[10];
		for(int j=0;j<10;++j)
		{
			unsigned int cutoff=4000-j;
			hist.Reset();
			for(int i=0;i<num;++i)
			{
				this->SetEntry(i);
				int q=0;
				for(auto it=ch[0]->begin();it!=ch[0]->end();++it)
				{
					if(*it>cutoff)
					{
						hist.Fill(*it);	
					}
					++q;
				}
			}
			if(kurt>hist.GetKurtosis())
			{
				kurt=hist.GetKurtosis();
				index=j;
			}
			mean[j]=hist.GetMean();
		this->SetEntry(ent);
		DC[cha]=mean[index];
		}
	}
	cout<<"DC "<<DC[cha]<<endl;
	return DC[cha];
}



TH1D* PmtData::GetIntegral(int cha)
{
	if(cha>2)
	{
	    cout<<"channel higher than 2 will be set to 0";
	    cha=0;
	}
	
	return charge[cha];
}

int PmtData::GetPulseIntegral(int cha,int event)
{
    if(cha>2)
    {
	cout<<"channel is greater then 2.";
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
	if(cha>2)
	{
	    cout<<"channel higher than 2 will be set to 0";
	    cha=0;
	}
	FindPeaks(cha);
	int ent=GetEntry();
	for(int i=0;i<num;++i)
	{
	
		SetEntry(i);	
		for(int j=5;j<280;++j)
		{
			if(peak1->at(i)[j]<0)
			{
			//	cout<<"i "<<i<<" j "<<j<<endl;
				int integral=0;
				for(int k=j-5;k<j+20;++k)
				{
					integral-=ch[0]->at(k)-DC[0];	
				}
			//	cout<<integral<<endl;
				charge[cha]->Fill(integral);
				pulseIntegral[cha]->push_back(integral);
			}
		}		
		
	}
	SetEntry(ent);	
		
}

void PmtData::FindPeaks(int cha)
{
	cout<<"Finding Peaks\n";
	bool peak=false;
	array <int,300> peakBuffer0;
	array <int,300> peakBuffer1;
	bool find=false;
	int ent=GetEntry();
	if(cha>2)
	{
	    cout<<"channel higher than 2 will be set to 0";
	    cha=0;
	}
	for(int j=0;j<num;++j)
	{
		SetEntry(j);
		peakBuffer0.fill(0);
		peakBuffer1.fill(0);
		
		for(int i=0;i<ch[0]->size();++i)
		{
			if(cutoff>=ch[0]->at(i))
			{
				peakBuffer0[i]=ch[0]->at(i)-DC[0];
				
			}
			else
			{	
				peakBuffer0[i]=0;
			}
	//		cout<<peakBuffer0[i]<<endl;
		}
		peak0->push_back(peakBuffer0);
		vector<int> indexBuffer;
		for(int i=5;i<280;++i)
		{
			if(peakBuffer0[i]<0)
			{	
				int index=0;
				int peak=0;
				for(int k=i-5;k<i+20;++k)
				{
					if(peak>peakBuffer0[k])
					{
						peak=peakBuffer0[k];
						index=k;
					}
				}
		//		cout<<"peak"<<peak<<" index "<<index<<endl;
				peakBuffer1[index]=peak;
				indexBuffer.push_back(index);
				i+=20;
			}			
		}
		peak1->push_back(peakBuffer1);
	}
	SetEntry(ent);
	cout<<"Finding Peak end\n";
}

TGraph* PmtData::GetTrace(int cha)
{
	int x[300];
        int y[300];
	if(cha>2)
	{
	    cout<<"channel higher than 2 will be set to 0";
	    cha=0;
	}
        for(int i=0;i<300&&i<ch[cha]->size();++i)
        {
                x[i]=i*4;
                y[i]=ch[cha]->at(i)-DC[cha];
        }
        TGraph* trace=new TGraph(300,x,y);
        return trace;

}
array<int,300> PmtData::GetPulse(int cha)
{
	array<int,300> x;
	if(cha>2)
	{
	    cout<<"channel higher than 2 will be set to 0";
	    cha=0;
	}
        for(int i=0;i<300&&i<ch[cha]->size();++i)
        {
                x[i]=ch[cha]->at(i)-DC[cha];
        }
        return x;

}
TGraph* PmtData::GetTraceSubNoise()
{
	int x[300];
	int y[300];
	for(int i=0;i<300;++i)
	{
		x[i]=i*4;
		y[i]=peak0->at(entry)[i];
	}
	return new TGraph(300,x,y);
}

TGraph* PmtData::GetTracePeak()
{
	int x[300];
	int y[300];
	for(int i=0;i<300;++i)
	{
		x[i]=i;
		y[i]=peak1->at(entry)[i];
	}
	return new TGraph(300,x,y);
}

void PmtData::Write(char *fileName)
{

	cout<<fileName<<endl;
	TFile * file=new TFile(fileName,"RECREATE");
	this->charge[0]->Write();
	this->charge[1]->Write();
	file->Close();	
}
