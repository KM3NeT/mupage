#include "km3net-dataformat/offline/Evt.hh"
#include "km3net-dataformat/definitions/root.hh"
#include "TFile.h"
#include "TTree.h"
#include <iostream>

int main(int argc, char* argv[]){
using namespace std;
	if (argc != 2) {
		cout << "usage: readEvtFile file.root" << endl;
		return 1;
	} 
	//TFile::Open works with local and remote files (xrootd)
	TFile *f1 = TFile::Open(argv[1]);
	//check that file is open
	if (f1 == nullptr) {
		cerr << "File can't be open." << endl;
		return 1;
	}
	//TTree *EvtTree = (TTree*)f1->Get("E");
	TTree *EvtTree = (TTree*)f1->Get(TTREE_OFFLINE_EVENT);  // TTREE_OFFLINE_EVENT is in km3net-dataformat/definitions/root.hh
	if (EvtTree == 0) {
		cerr << "File has no " << TTREE_OFFLINE_EVENT << " tree." << endl;
		return 1;
	}
	Evt *evt = nullptr;
	EvtTree->SetBranchAddress("Evt",&evt);
	Long64_t nentries = EvtTree->GetEntries();
	for (Long64_t i=0; i<nentries; i++) {
		EvtTree->GetEntry(i);
		cout << "Event ID: " <<  evt->id << endl; //an example of accessing Evt struct object
		evt->print();  //an example of using Evt struct function
	}
	f1->Close();
	return 0;
}
