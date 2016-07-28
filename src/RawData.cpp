#include"RawData.h"
#include<string>
#include<iostream>
#include<TH1D.h>
RawData::RawData()
{
}

RawData::RawData(const char *fileName)
{
	//cerr<<" 100   RawData const char "<<fileName<<endl;
	file=new TFile(fileName);
	tree=(TTree*)file->Get("RawEventTree");
	nCha=new uint32_t();
	//cerr<<" 104 GetNCha ptr "<<nCha<<endl;
	//cerr<<" *nCha  "<<*nCha<<endl;
	//cerr<<" 101   tree->GetEntries "<<tree->GetEntries()<<endl;
	for(int cha=0;cha<NCHA;++cha)
	{
	    ch[cha]=new vector<int>();
	    string name("channelData");
	    string chan=to_string(cha+CHAOFF);
	    name+=chan;

	    //cerr<<" 102   RawData cha "<<cha<<endl;
	    cout<<"making branch on "<<name<<" using "<<cha<<endl;
	    tree->SetBranchAddress(name.c_str(),&ch[cha]);
	    //cerr<<&ch[cha]<<" "<<ch[cha]->size()<<endl;
	}
	
	cout<<"make branch nChannels\n";
	//cerr<<"RawData &nCha "<<nCha<<endl;
	tree->SetBranchAddress("nChannels",nCha);
	entry=0;
	//cerr<<" 103   RawData tree-> GetEntry \n";
	tree->GetEntry(entry);
	//cerr<<" 105 GetNCha ptr "<<nCha<<endl;
	//cerr<<" *nCha  "<<*nCha<<endl;
}

RawData::RawData(string *fileName):RawData(fileName->c_str())
{
    //cerr<<"RawData string* "<<fileName<<endl;
}

RawData::RawData(string fileName):RawData(fileName.c_str())
{
    //cerr<<"RawData string "<<fileName<<endl;
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

