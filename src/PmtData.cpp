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
	num=50000;
	sub=false;
	DC=0;
	GetDC();
	cutoff=3980;
	peak0=new vector<array<int,500>>();
	peak1=new vector<array<int,500>>();
	charge=new TH1D("charge","charge",10000,0,10000);
}

double PmtData::GetDC()
{
	if(DC!=0)
	{
		return DC;
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
				for(auto it=ch0->begin();it!=ch0->end();++it)
				{
					if(*it>cutoff)
					{
						hist.Fill(*it);	
					}
				}
			}
			if(kurt>hist.GetKurtosis())
			{
				kurt=hist.GetKurtosis();
				index=j;
			}
			mean[j]=hist.GetMean();
		this->SetEntry(ent);
		DC=mean[index];
		}
	}
	cout<<"DC "<<DC<<endl;
	return DC;
}



TH1D* PmtData::GetIntegral()
{
	
	return charge;
}

void PmtData::CalIntegral()
{	
	FindPeaks();
	int ent=GetEntry();
	for(int i=0;i<num;++i)
	{
	
		SetEntry(i);	
		for(int j=5;j<480;++j)
		{
			if(peak1->at(i)[j]<0)
			{
			//	cout<<"i "<<i<<" j "<<j<<endl;
				int integral=0;
				for(int k=j-5;k<j+20;++k)
				{
					integral-=ch0->at(k)-DC;	
				}
			//	cout<<integral<<endl;
				charge->Fill(integral);
			}
		}		
		
	}
	SetEntry(ent);	
		
}

void PmtData::FindPeaks()
{
	cout<<"Finding Peaks\n";
	bool peak=false;
	array <int,500> peakBuffer0;
	array <int,500> peakBuffer1;
	bool find=false;
	int ent=GetEntry();
	for(int j=0;j<num;++j)
	{
		SetEntry(j);
		peakBuffer0.fill(0);
		peakBuffer1.fill(0);
		
		for(int i=0;i<ch0->size();++i)
		{
			if(cutoff>=ch0->at(i))
			{
				peakBuffer0[i]=ch0->at(i)-DC;
				
			}
			else
			{	
				peakBuffer0[i]=0;
			}
	//		cout<<peakBuffer0[i]<<endl;
		}
		peak0->push_back(peakBuffer0);
		vector<int> indexBuffer;
		for(int i=5;i<480;++i)
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

}

TGraph* PmtData::GetTrace()
{
	int x[500];
        int y[500];
        for(int i=0;i<500&&i<ch0->size();++i)
        {
                x[i]=i;
                y[i]=ch0->at(i)-DC;
        }
        TGraph* trace=new TGraph(500,x,y);
        return trace;

}

TGraph* PmtData::GetTraceSubNoise()
{
	int x[500];
	int y[500];
	for(int i=0;i<500;++i)
	{
		x[i]=i;
		y[i]=peak0->at(entry)[i];
	}
	return new TGraph(500,x,y);
}

TGraph* PmtData::GetTracePeak()
{
	int x[500];
	int y[500];
	for(int i=0;i<500;++i)
	{
		x[i]=i;
		y[i]=peak1->at(entry)[i];
	}
	return new TGraph(500,x,y);
}

void PmtData::Write(char *fileName)
{

	cout<<fileName<<endl;
	TFile * file=new TFile(fileName,"RECREATE");
	this->charge->Write();
	file->Close();	
}
