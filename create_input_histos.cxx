#include "TFile.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TString.h"
#include <sstream>
#include "TH1.h"
#include "TF1.h"
#include "TString.h"
//#include "Environment.h"
#include <iostream>
#include <fstream>
#include <TMath.h>
#include "TKey.h"
#include "TLegend.h"
#include "TString.h"
#include "TGraphErrors.h"
#include "TIterator.h"

//using namespace RooFit ;
using namespace std;
bool debug = 0;

void create_input_histos() {

  TFile* infile; 

  TString rootfilenames[] = {
    "/user/mukherjee/limits_Diboson/bkg/QCD.root",
    "/user/mukherjee/limits_Diboson/bkg/GJets.root",
    "/user/mukherjee/limits_Diboson/bkg/SingleT.root",
    "/user/mukherjee/limits_Diboson/bkg/DY.root",
    "/user/mukherjee/limits_Diboson/bkg/ttbar.root",
    "/user/mukherjee/limits_Diboson/bkg/Higgs.root",
    "/user/mukherjee/limits_Diboson/bkg/Diboson.root",
    "/user/mukherjee/limits_Diboson/bkg/wjets.root"
};

  TString sample_names[] = {"QCD","GJets","singleTop","DY","ttbar","Higgs","Diboson","Wjets"};
  const int arraySize = sizeof(sample_names)/sizeof(sample_names[0]);  

  Char_t file_title[400];
  Char_t dir_title[400];  
  Char_t dir_title_syst[400];  

  //outfile for all histos
  TFile *outfile = new TFile("root_out/out.root","recreate");

  //outfile for all text to be used as input for the limit cfgs
  ofstream myfile;
  myfile.open ("txt_out/normalization.txt");

  double mass_min=800;
  double mass_max=1800;

  int step_size=200;

  int N_points=(int)(mass_max-mass_min)/(double)step_size;

  double masses[400];
  double num_total[400];

  double num_signal[400];
  double num_observed[400];

  if (debug) std::cout << "will start initializer loop" << std::endl;

  for(int n=0; n<400; n++)
  {
    masses[n]=0.;
    num_total[n]=0.;
    num_signal[n]=0.;
    num_observed[n]=0.;
  }  

  //###############
  //loop over backgrounds
  //###############  
  if (debug) std::cout << "will start background loop"<< std::endl;
  double Lumi_bkg = 1;
  std::cout << "Lumi scale factor for bkg : " << Lumi_bkg << std::endl;

  for (int i = 0; i < arraySize; ++i){
    if (debug) std::cout << "will get the bkg root file"<< std::endl;
    std::cout << "\n\nrootfilename " << rootfilenames[i] << "  sample_name " << sample_names[i] << std::endl;
    TFile* infile = new TFile(rootfilenames[i]);
    TH1F* hist_ori;
    if (debug) std::cout << "will get the mass hist "<< std::endl;
    hist_ori=(TH1F*)infile->Get("h1_ZPrimemass_SEL_TauhTauh");
    hist_ori->SetName(sample_names[i]);
    outfile->cd();
    hist_ori->Write(sample_names[i]);
    std::cout  << "bkg " << sample_names[i] << " " << hist_ori->Integral(1,4000) << "\n";
    myfile << "bkg " << sample_names[i] << " " << hist_ori->Integral(1,4000) << "\n";
    
    if (debug) std::cout << "Will delete hist_ori "<< std::endl;
    delete hist_ori;
    if (debug) std::cout << "Successfully deleted hist_ori "<< std::endl;
    
  }    
  //###############
  //end loop over backgrounds
  //###############    

  if (debug) std::cout << "will get data root file"<< std::endl;
  TFile* data_file = new TFile("/net/scratch_cms/institut_3a/mukherjee/DibosonJobs/DATA/merged/allData.root");
  TH1F* data;
  if (debug) std::cout << "will get data hist"<< std::endl;
  data=(TH1F*)data_file->Get("SEL/h1_ZPrimemass_SEL_TauhTauh"); 
  outfile->cd();
  data->SetName("data_obs");
  data->Write();
  myfile << "data " << "data" << " " << data->Integral(1,4000) << "\n";

  //  const int num_samples=(int)((mass_max-mass_min)/(double)step_size+1);
  // double mass_sig=(double)mass_min;

  TFile* infile_sig;
  TString rootfilenames_sig[] = {
    "/user/mukherjee/limits_Diboson/sig/SIGNAL_ZprimeToZhToZlephtata_narrow_M-800_13TeV_MG-skimid3194_out.root",
    "/user/mukherjee/limits_Diboson/sig/SIGNAL_ZprimeToZhToZlephtata_narrow_M-1000_13TeV_MG-skimid3200_out.root",
    "/user/mukherjee/limits_Diboson/sig/SIGNAL_ZprimeToZhToZlephtata_narrow_M-1200_13TeV_MG-skimid3193_out.root",
    "/user/mukherjee/limits_Diboson/sig/SIGNAL_ZprimeToZhToZlephtata_narrow_M-1400_13TeV_MG-skimid3196_out.root",
    "/user/mukherjee/limits_Diboson/sig/SIGNAL_ZprimeToZhToZlephtata_narrow_M-1600_13TeV_MG-skimid3201_out.root",
    "/user/mukherjee/limits_Diboson/sig/SIGNAL_ZprimeToZhToZlephtata_narrow_M-1800_13TeV_MG-skimid3197_out.root"
  };

  int sample_names_sig[] = {800, 1000, 1200, 1400, 1600, 1800};
  const int arraySize_sig = sizeof(sample_names_sig)/sizeof(sample_names_sig[0]);

  for (int k = 0; k < arraySize_sig; ++k){
    TFile* infile_sig = new TFile(rootfilenames_sig[k]);
    TH1F* signal_temp;
    sprintf(file_title,"root_out/out_mass_%d.root",sample_names_sig[k]);
    sprintf(dir_title,"txt_out/normalization_Mass_%d_input_histos.txt",sample_names_sig[k]);
    ofstream myfile_temp;
    myfile_temp.open(dir_title);
    TFile* outfile_signal = new TFile(file_title,"recreate");
    outfile_signal->cd();
    
    int counter_histos=0;
    TKey *key;
    TIter next(outfile->GetListOfKeys());
    while ((key = (TKey*)next())) {
      TH1 *h1 = (TH1*)key->ReadObj();
      TString *name = new TString(h1->GetName());
      if(strcmp(h1->GetName(),"data_obs") ==0 )
	{
	  myfile_temp << "data " << h1->GetName() << " " << h1->Integral(1,4000) << "\n";	    
	}	
      else
	{
	  myfile_temp << "bkg " << h1->GetName() << " " << h1->Integral(1,4000) << "\n"; 
	}
      h1->Write();
      counter_histos++;
      delete name;
      delete h1;
      
    }
    signal_temp=(TH1F*)infile_sig->Get("h1_ZPrimemass_SEL_TauhTauh");
    myfile << "signal " << sample_names_sig[k] << " " << signal_temp->Integral(1,4000) << "\n";
    cout << "signal mass: " << sample_names_sig[k] << " N events: " << signal_temp->Integral(1,4000) << endl; 
    outfile_signal->cd();
    signal_temp->Write("signal");
    myfile_temp << "signal " << sample_names_sig[k] << " " << signal_temp->Integral(1,4000) << "\n";
    delete signal_temp;
    myfile_temp.close();
  }
  //close the output txt file:
  myfile.close();
  //  delete gauss;
  
}
