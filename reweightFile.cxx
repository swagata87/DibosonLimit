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

void reweightFile(std::string type) {
  std::cout << "I am here " ;
  double cross_sec; 
  
  if (type=="SIGNAL") {
    std::string inputFileName[] = {
      "ZprimeToZhToZlephtata_narrow_M-1000_13TeV_MG-skimid3200",
      "ZprimeToZhToZlephtata_narrow_M-1600_13TeV_MG-skimid3201",
      "ZprimeToZhToZlephtata_narrow_M-1200_13TeV_MG-skimid3193",
      "ZprimeToZhToZlephtata_narrow_M-1800_13TeV_MG-skimid3197",
      "ZprimeToZhToZlephtata_narrow_M-1400_13TeV_MG-skimid3196",
      "ZprimeToZhToZlephtata_narrow_M-800_13TeV_MG-skimid3194"
    };
    
    const int arraySize = sizeof(inputFileName)/sizeof(inputFileName[0]);
    std::cout << "arraySize " << arraySize << std::endl;
    for (int i = 0; i < arraySize; ++i){
      
      std::string FullFileName = "/net/scratch_cms/institut_3a/mukherjee/DibosonJobs/"+type+"/merged/"+inputFileName[i]+".root" ;
      std::cout << FullFileName << std::endl;
      TFile *input   = new TFile(FullFileName.c_str());
      
      TEnv *xsec = new TEnv("INPUT");
      xsec->ReadFile("/user/mukherjee/limits_Diboson/xs.cfg", kEnvChange);
      std::string Name = inputFileName[i]+".xs";
      cross_sec = xsec->GetValue(Name.c_str(), -1.);
      std::cout << "xsec=" << cross_sec << std::endl;
      
      double lumi=2119.50; // in pb-1
      TH1F* henv = (TH1F*)input->Get("h_counters");
      int nev = henv->Integral();
      std::cout << "Nevt=" << nev << std::endl;
      
      double weight = (lumi*cross_sec)/nev ;
      std::cout << "weight=" << weight << std::endl;
      
      TH1F* mass = (TH1F*)input->Get("SEL/h1_ZPrimemass_SEL_TauhTauh");
      if (mass->Integral()>0) mass->Scale(weight/mass->Integral());
      
      std::string outFileName = type+"_"+inputFileName[i]+"_out.root";
      std::cout << outFileName << std::endl;
      TFile *outfile = new TFile(outFileName.c_str(),"recreate");
      outfile->cd();
      //mass->Draw();
      mass->Write();
      outfile->Write();
    }
  }



  if (type=="BACKGROUND") {
    std::string inputFileName[] = {
      "DYJetsToLL_M-50_13TeV_MG-skimid3253",       
      "QCD_Pt_2400to3200_13TeV_P8-skimid3360",                     
      "TT_13TeV_PH-skimid3233",
      "DYJetsToLL_M-5to50_13TeV_MG-skimid3260",    
      "QCD_Pt_300to470_13TeV_P8-skimid3282",                       
      "VBFHToTauTau_M125_13TeV_PH-skimid3413",
      "GJets_HT-100To200_13TeV_MG-skimid3385",     
      "QCD_Pt_30to50_13TeV_P8-skimid3269",                         
      "WJetsToLNu_13TeV_MG-skimid3238",
      "GJets_HT-200To400_13TeV_MG-skimid3388",     
      "QCD_Pt_3200toInf_13TeV_P8-skimid3379",                      
      "WWTo2L2Nu_13TeV_PH-skimid3314",
      "GJets_HT-400To600_13TeV_MG-skimid3400",     
      "QCD_Pt_470to600_13TeV_P8-skimid3397",                       
      "WWTo4Q_13TeV-v2_PH-skimid2871",
      "GJets_HT-40To100_13TeV_MG-skimid3393",      
      "QCD_Pt_50to80_13TeV_P8-skimid3292",                         
      "WWToLNuQQ_13TeV_PH-skimid3392",
      "GJets_HT-600ToInf_13TeV_MG-skimid3259",     
      "QCD_Pt_5to10_13TeV_P8-skimid3356",                          
      "WZTo3LNu_13TeV_PH-skimid3032",
      "GluGluHToTauTau_M125_13TeV_PH-skimid3057",  
      "QCD_Pt_600to800_13TeV_P8-skimid3275",                       
      "WHToTauTau_M125_13TeV_PH",
      "QCD_Pt_10to15_13TeV_P8-skimid3307",        
      "QCD_Pt_800to1000_13TeV_P8-skimid3398",                      
      "QCD_Pt_120to170_13TeV_P8-skimid3371",       
      "QCD_Pt_80to120_13TeV_P8-skimid3361",                        
      "ZHToTauTau_M125_13TeV_PH-skimid3412",
      "QCD_Pt_1400to1800_13TeV_P8-skimid3340",     
      "ST_t-channel_antitop_4f_leptonDecays_13TeV_PH-skimid3302",  
      "ZZTo2L2Nu_13TeV_PH-skimid3046",
      "QCD_Pt_15to30_13TeV_P8-skimid3386",         
      "ST_t-channel_top_4f_leptonDecays_13TeV_PH-skimid2845",      
      "ZZTo4L_13TeV_PH-skimid3045",
      "QCD_Pt_170to300_13TeV_P8-skimid3353",       
      "ST_tW_antitop_5f_inclusiveDecays_13TeV_PH-skimid2858",     
      "QCD_Pt_1800to2400_13TeV_P8-skimid3326",     
      "ST_tW_top_5f_inclusiveDecays_13TeV-v2_PH-skimid2861"      
    };
    
    const int arraySize = sizeof(inputFileName)/sizeof(inputFileName[0]);
    for (int i = 0; i < arraySize; ++i){
      
      std::string FullFileName = "/net/scratch_cms/institut_3a/mukherjee/DibosonJobs/"+type+"/merged/"+inputFileName[i]+".root" ;
      std::cout << FullFileName << std::endl;
      TFile *input   = new TFile(FullFileName.c_str());
      
      TEnv *xsec = new TEnv("INPUT");
      xsec->ReadFile("/user/mukherjee/limits_Diboson/xs.cfg", kEnvChange);
      std::string Name = inputFileName[i]+".xs";
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
      
      std::string outFileName = type+"_"+inputFileName[i]+"_out.root";
      std::cout << outFileName << std::endl;
      TFile *outfile = new TFile(outFileName.c_str(),"recreate");
      outfile->cd();
      //mass->Draw();
      mass->Write();
      outfile->Write();
    }
  }



}
