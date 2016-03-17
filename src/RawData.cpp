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
	ch[0]=new vector<int>();
	ch[1]=new vector<int>();
	tree->SetBranchAddress("channelData0",&ch[0]);
	tree->SetBranchAddress("channelData1",&ch[1]);
	tree->SetBranchAddress("nChannels",&nCha);
	entry=0;
	tree->GetEntry(entry);
}

RawData::RawData(string *fileName)
{
	file=new TFile(fileName->c_str());
	tree=(TTree*)file->Get("RawEventTree");
	ch[0]=new vector<int>();
	tree->SetBranchAddress("channelData0",&ch[0]);
	entry=0;
	tree->GetEntry(entry);
}

RawData::~RawData()
{
}

TGraph* RawData::GetTrace(int cha)
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
		y[i]=ch[cha]->at(i);
		cout<<i<<endl;
	}
	TGraph* trace=new TGraph(300,x,y);
	return trace;

}

