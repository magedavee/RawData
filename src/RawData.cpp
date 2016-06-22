#include"RawData.h"
#include<string>
#include<iostream>
#include<TH1D.h>
RawData::RawData()
{
}

RawData::RawData(const char *fileName)
{
	cerr<<" 100   RawData const char "<<fileName<<endl;
	file=new TFile(fileName);
	tree=(TTree*)file->Get("RawEventTree");
	cerr<<" 101   tree->GetEntries "<<tree->GetEntries()<<endl;
	for(int cha=0;cha<NCHA;++cha)
	{
	    ch[cha]=new vector<int>();
	    string name("channelData");
	    string chan=to_string(cha);
	    name+=chan;

	    cerr<<" 102   RawData cha "<<cha<<endl;
	    cout<<"making branch "<<name<<endl;
	    tree->SetBranchAddress(name.c_str(),&ch[cha]);
	}
	cout<<"make branch nChannels\n";
	tree->SetBranchAddress("nChannels",&nCha);
	entry=0;
	cerr<<" 103   RawData tree-> GetEntry \n";
	tree->GetEntry(entry);
}

RawData::RawData(string *fileName):RawData(fileName->c_str())
{
    cerr<<"RawData string* "<<fileName<<endl;
}

RawData::RawData(string fileName):RawData(fileName.c_str())
{
    cerr<<"RawData string "<<fileName<<endl;
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
	}
	TGraph* trace=new TGraph(300,x,y);
	return trace;

}

