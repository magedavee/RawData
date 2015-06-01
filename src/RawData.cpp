#include"RawData.h"
#include<string>
#include<iostream>
#include<TH1D.h>
RawData::RawData()
{
}

RawData::RawData(char *fileName)
{
	file=new TFile(fileName);
	tree=(TTree*)file->Get("RawEventTree");
	ch0=new vector<int>();
	tree->SetBranchAddress("channelData0",&ch0);
	entry=0;
	tree->GetEntry(entry);
}

RawData::RawData(string *fileName)
{
	file=new TFile(fileName->c_str());
	tree=(TTree*)file->Get("RawEventTree");
	ch0=new vector<int>();
	tree->SetBranchAddress("channelData0",&ch0);
	entry=0;
	tree->GetEntry(entry);
}

RawData::~RawData()
{
}

TGraph* RawData::GetTrace()
{
	int x[500];
	int y[500];
	for(int i=0;i<500&&i<ch0->size();++i)
	{
		x[i]=i;
		y[i]=ch0->at(i);
	}
	TGraph* trace=new TGraph(500,x,y);
	return trace;

}

