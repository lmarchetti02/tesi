#include <iostream>
#include <vector>

#include "TH1F.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TFile.h"

using std::cin;
using std::cout;
using std::endl;

TH1F *evn_vs_entry_t1;
TH1F *evn_vs_entry_t2;
TCanvas *c1;

int analisi(const char *rootfilename)
{
  TFile *ff = new TFile(rootfilename, "r");
  if (ff == NULL)
  {
    cout << "ERROR opening file " << rootfilename << endl;
    return 1;
  }

  TTree *t1 = (TTree *)ff->Get("Info");
  if (t1 == NULL)
  {
    cout << "ERROR loading TTree Info" << endl;
    return 1;
  }
  int n_t1 = t1->GetEntries();

  TTree *t2 = (TTree *)ff->Get("Event");
  if (t2 == NULL)
  {
    cout << "ERROR loaging TTree Event" << endl;
    return 1;
  }
  int n_t2 = t2->GetEntries();
  cout << "n.entries t2 (CS) = " << t2->GetEntries() << endl;

  Int_t n_pxl;
  Double_t pix_size[3];
  Int_t n_events;

  Int_t eventID;
  Int_t NnoCS;
  std::vector<Int_t> *id_pixel_nocs = 0;
  std::vector<Double_t> *ene_pixel_nocs = 0;
  Int_t NCS;
  std::vector<Int_t> *id_pixel_cs = 0;
  std::vector<Double_t> *ene_pixel_cs = 0;

  t1->SetBranchAddress("Pixel N", &n_pxl);
  t1->SetBranchAddress("Pixels x-dim", &pix_size[0]);
  t1->SetBranchAddress("Pixels y-dim", &pix_size[1]);
  t1->SetBranchAddress("Pixels z-dim", &pix_size[2]);
  t1->SetBranchAddress("Event N", &n_events);

  t2->SetBranchAddress("Event", &eventID);
  t2->SetBranchAddress("NnoCS", &NnoCS);
  t2->SetBranchAddress("Id_nocs", &id_pixel_nocs);
  t2->SetBranchAddress("Ene_nocs", &ene_pixel_nocs);
  t2->SetBranchAddress("NCS", &NCS);
  t2->SetBranchAddress("Id_cs", &id_pixel_cs);
  t2->SetBranchAddress("Ene_cs", &ene_pixel_cs);

  t1->GetEntry(0);
  cout << "Pixel N = " << n_pxl << endl;
  cout << "x-dim = " << pix_size[0] << endl;
  cout << "y-dim = " << pix_size[1] << endl;
  cout << "z-dim = " << pix_size[2] << endl;
  cout << "Event N = " << n_events << endl;

  for (u_int32_t i = 0; i < t2->GetEntries(); i++)
  {

    t2->GetEntry(i);
    cout << "entry n." << i << endl;
    cout << "EventID=" << eventID << endl;
    cout << endl
         << "n.pixels noCS = " << id_pixel_nocs->size() << " = " << NnoCS << endl;
    double ene_nocs_tot = 0;
    for (u_int32_t j = 0; j < id_pixel_nocs->size(); j++)
    {
      cout << j << " pixID=" << id_pixel_nocs->at(j) << " ene=" << ene_pixel_nocs->at(j) << endl;
      ene_nocs_tot += ene_pixel_nocs->at(j);
    }
    cout << "ene_nocs_tot=" << ene_nocs_tot << endl;
    cout << endl
         << "n.pixels CS = " << id_pixel_cs->size() << " = " << NCS << endl;
    double ene_cs_tot = 0;
    for (u_int32_t j = 0; j < id_pixel_cs->size(); j++)
    {
      cout << j << " pixID=" << id_pixel_cs->at(j) << " ene=" << ene_pixel_cs->at(j) << endl;
      ene_cs_tot += ene_pixel_cs->at(j);
    }
    cout << "ene_cs_tot=" << ene_cs_tot << endl;
    char cc;
    cout << endl;
    cout << "s) stop loop" << endl;
    cout << "else continue" << endl;
    cin >> cc;
    if (cc == 's')
      break;
  }

  return 0;
}
