#include "TFile.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TString.h"
#include <sstream>
#include "TH1.h"
#include "TF1.h"
#include "TString.h"
#include <iostream>
#include <fstream>
#include <TMath.h>
#include "TKey.h"
#include "TLegend.h"
#include "TString.h"
#include "TGraphErrors.h"
#include "TIterator.h"
#include "TEnv.h"
#include "TEnv.h"

using namespace std;
bool debug = 0;

void reweightFile(std::string type, std::string inputFileName) {
  std::cout << "I am here " ;
  double cross_sec; 

  std::string FullFileName = "/net/scratch_cms/institut_3a/mukherjee/DibosonJobs/"+type+"/merged/"+inputFileName+".root" ;
  std::cout << FullFileName << std::endl;
  TFile *input   = new TFile(FullFileName.c_str());

  TEnv *xsec = new TEnv("INPUT");
  xsec->ReadFile("/user/mukherjee/limits_Diboson/xs.cfg", kEnvChange);
  std::string Name = inputFileName+".xs";
  cross_sec = xsec->GetValue(Name.c_str(), -1.);
  std::cout << "xsec=" << cross_sec << std::endl;

  double lumi=2119.50;
  TH1F* henv = (TH1F*)input->Get("h_counters");
  int nev = henv->Integral();
  std::cout << "Nevt=" << nev << std::endl;

  double weight = (lumi*cross_sec)/nev ;
  std::cout << "weight=" << weight << std::endl;

  TH1F* mass = (TH1F*)input->Get("SEL/h1_ZPrimemass_SEL_TauhTauh");
  if (mass->Integral()>0) mass->Scale(weight/mass->Integral());

  std::string outFileName = type+"_"+inputFileName+"_out.root";
  std::cout << outFileName << std::endl;
  TFile *outfile = new TFile(outFileName.c_str(),"recreate");
  outfile->cd();
  mass->Draw();
  mass->Write();
  outfile->Write();
}
