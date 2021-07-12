#include "TopLJets2015/TopAnalysis/interface/EfficiencyScaleFactorsWrapper.h"

#include "TFile.h"
#include "TSystem.h"
#include "TMath.h"

#include <iostream>


using namespace std;

//
EfficiencyScaleFactorsWrapper::EfficiencyScaleFactorsWrapper(bool isData,TString era,TString eleid)
{
  if(isData) return;
  init(era, eleid);
}

//
void EfficiencyScaleFactorsWrapper::init(TString era, TString eleid)
{
  if(era.Contains("era2017")) era_=2017;
  if(era.Contains("era2016")) era_=2016;

  cout << "[EfficiencyScaleFactorsWrapper]" << endl
       << "\tStarting efficiency scale factors for " << era << endl
       << "\tThe ID of electrons is " << eleid << endl
       << "\tWarnings: no trigger SFs for any object" << endl
       << "\t          uncertainties returned are of statistical nature only" << endl
       << "\tDon't forget to fix these and update these items!" << endl;

  //PHOTONS
  TString a_recoSF,a_idSF,a_psvSF;
  if(era_==2016){
    a_recoSF=era+"/EGM2D_BtoH_GT20GeV_RecoSF_Legacy2016.root";
    a_idSF=era+"/Fall17V2_2016_Tight_photons.root";
    a_psvSF=era+"/ScalingFactors_80X_Summer16.root";
  }else if(era_==2017){
    if(eleid.Contains("LOOSE"))a_idSF=era+"/egammaEffi.txt_EGM2D_Loose_UL17.root";
    if(eleid.Contains("MEDIUM"))a_idSF=era+"/egammaEffi.txt_EGM2D_Medium_UL17.root";
    if(eleid.Contains("TIGHT"))a_idSF=era+"/egammaEffi.txt_EGM2D_Tight_UL17.root";
    if(eleid.Contains("VETO"))a_idSF=era+"/egammaEffi.txt_EGM2D_Veto_UL17.root";
    if(eleid.Contains("MVA80ISO"))a_idSF=era+"/egammaEffi.txt_EGM2D_MVA80iso_UL17.root";
    if(eleid.Contains("MVA80NOISO"))a_idSF=era+"/egammaEffi.txt_EGM2D_MVA80noIso_UL17.root";
    if(eleid.Contains("MVA90ISO"))a_idSF=era+"/egammaEffi.txt_EGM2D_MVA90iso_UL17.root";
    if(eleid.Contains("MVA90NOISO"))a_idSF=era+"/egammaEffi.txt_EGM2D_MVA90noIso_UL17.root";
    a_recoSF=era+"/egammaEffi_ptAbove20.txt_EGM2D_UL2017.root";
//    a_recoSF=era+"/egammaEffi.txt_EGM2D_runBCDEF_passingRECO.root";
//    a_idSF=era+"/2017_PhotonsTight.root";
    a_psvSF=era+"/PixelSeed_ScaleFactors_2017.root";
  }

  gSystem->ExpandPathName(a_recoSF);
  TFile *fIn=TFile::Open(a_recoSF);
  if(fIn && !fIn->IsZombie()) {
    cout << "photons: reco SF from" << a_recoSF << endl;
    scaleFactorsH_["g_rec"]=(TH2 *)fIn->Get("EGamma_SF2D")->Clone();
    scaleFactorsH_["g_rec"]->SetDirectory(0);
    fIn->Close();
  }

  gSystem->ExpandPathName(a_psvSF);
  fIn=TFile::Open(a_psvSF);
  if(fIn && !fIn->IsZombie()) {
    cout << "photons: pixel-seed-veto SF from" << a_psvSF << endl;
    if(era_==2016) {
      scaleFactorsH_["g_psv"]=(TH2 *)fIn->Get("Scaling_Factors_HasPix_R9 Inclusive")->Clone();
      scaleFactorsH_["g_psv"]->SetDirectory(0);
    }else {
      scaleFactors1DH_["g_psv"]=(TH1 *)fIn->Get("Tight_ID")->Clone();
      scaleFactors1DH_["g_psv"]->SetDirectory(0);
    }
    fIn->Close();
  }

  gSystem->ExpandPathName(a_idSF);
  fIn=TFile::Open(a_idSF);
  if(fIn && !fIn->IsZombie()) {
    cout << "photons: id SF from" << a_psvSF << endl;
    scaleFactorsH_["g_id"]=(TH2 *)fIn->Get("EGamma_SF2D")->Clone();
    scaleFactorsH_["g_id"]->SetDirectory(0);
    fIn->Close();
  }

  //ELECTRONS
  TString e_recoSF,e_idSF;
  if(era_==2016){
    e_recoSF=era+"/EGM2D_BtoH_GT20GeV_RecoSF_Legacy2016.root";
    e_idSF=era+"/2016LegacyReReco_ElectronTight_Fall17V2.root";
  }else if(era_==2017){
    //e_recoSF=era+"/egammaEffi.txt_EGM2D_runBCDEF_passingRECO.root";
    e_recoSF=era+"/egammaEffi_ptAbove20.txt_EGM2D_UL2017.root";
    //e_idSF=era+"/2017_ElectronTight.root";
    if(eleid.Contains("LOOSE"))e_idSF=era+"/egammaEffi.txt_EGM2D_Loose_UL17.root";
    if(eleid.Contains("MEDIUM"))e_idSF=era+"/egammaEffi.txt_EGM2D_Medium_UL17.root";
    if(eleid.Contains("TIGHT"))e_idSF=era+"/egammaEffi.txt_EGM2D_Tight_UL17.root";
    if(eleid.Contains("VETO"))e_idSF=era+"/egammaEffi.txt_EGM2D_Veto_UL17.root";
    if(eleid.Contains("MVA80ISO"))e_idSF=era+"/egammaEffi.txt_EGM2D_MVA80iso_UL17.root";
    if(eleid.Contains("MVA80NOISO"))e_idSF=era+"/egammaEffi.txt_EGM2D_MVA80noIso_UL17.root";
    if(eleid.Contains("MVA90ISO"))e_idSF=era+"/egammaEffi.txt_EGM2D_MVA90iso_UL17.root";
    if(eleid.Contains("MVA90NOISO"))e_idSF=era+"/egammaEffi.txt_EGM2D_MVA90noIso_UL17.root";
  }

  gSystem->ExpandPathName(e_recoSF);
  fIn=TFile::Open(e_recoSF);
  if(fIn && !fIn->IsZombie()) {
    cout << "electrons: reco SF from" << e_recoSF << endl;
    scaleFactorsH_["e_rec"]=(TH2 *)fIn->Get("EGamma_SF2D")->Clone();
    scaleFactorsH_["e_rec"]->SetDirectory(0);
    fIn->Close();
  }

  gSystem->ExpandPathName(e_idSF);
  fIn=TFile::Open(e_idSF);
  if(fIn && !fIn->IsZombie()) {
    cout << "electrons: id SF from" << e_idSF << endl;
    scaleFactorsH_["e_id"]=(TH2 *)fIn->Get("EGamma_SF2D")->Clone();
    scaleFactorsH_["e_id"]->SetDirectory(0);
    fIn->Close();
  }


  //MUONS

  std::vector<float>   lumiWgts;
  std::vector<TString> m_tkSF, m_idSF,m_isoSF;
  if(era_==2016){
    m_tkSF.push_back( era+"/MuonTracking_EfficienciesAndSF_BCDEF.root");
    m_tkSF.push_back( era+"/MuonTracking_EfficienciesAndSF_GH.root");
    m_idSF.push_back( era+"/RunBCDEF_SF_ID.root");
    m_idSF.push_back( era+"/RunGH_SF_ID.root");
    m_isoSF.push_back( era+"/RunBCDEF_SF_ISO.root");
    m_isoSF.push_back( era+"/RunGH_SF_ISO.root");
    lumiWgts.push_back(0.5);
    lumiWgts.push_back(0.5);
  }
  else{
    //m_idSF.push_back( era+"/RunBCDEF_SF_ID.root");
    //m_isoSF.push_back( era+"/RunBCDEF_SF_ISO.root");
    m_idSF.push_back( era+"/RunBCDEF_SF_MuID.root");
    m_isoSF.push_back( era+"/RunBCDEF_SF_MuISO.root");
    lumiWgts.push_back(1.0);
  }

/*
  TString m_tkSF, m_idSF,m_isoSF;
  if(era_==2017){
    m_idSF=era+"/RunBCDEF_SF_MuID.root";
    m_isoSF=era+"/RunBCDEF_SF_MuISO.root";
  }
*/
///////////
/*
  for(size_t i=0; i<m_tkSF.size(); i++) {
    gSystem->ExpandPathName(m_tkSF[i]);
    fIn=TFile::Open(m_tkSF[i]);
    if(fIn && !fIn->IsZombie()) {
      cout << "muons: tk SF from" << m_tkSF[i] << " with weight " << lumiWgts[i] << endl;
      scaleFactorsGr_["m_tkSF"]=(TGraphAsymmErrors *)fIn->Get("ratio_eff_aeta_dr030e030_corr");
      fIn->Close();
    }
  }
*/

  for(size_t i=0; i<m_idSF.size(); i++) {
    gSystem->ExpandPathName(m_idSF[i]);
    fIn=TFile::Open(m_idSF[i]);
    if(fIn && !fIn->IsZombie()) {
      cout << "muons: id SF from" << m_idSF[i] << endl;
      scaleFactorsH_["m_id"]=(TH2F *)fIn->Get("NUM_TightID_DEN_genTracks_pt_abseta")->Clone();
      scaleFactorsH_["m_id"]->SetDirectory(0);
      fIn->Close();
    }
  }

  for(size_t i=0; i<m_isoSF.size(); i++) {
    gSystem->ExpandPathName(m_isoSF[i]);
    fIn=TFile::Open(m_isoSF[i]);
    if(fIn && !fIn->IsZombie()) {
      cout << "muons: iso SF from" << m_isoSF[i] << endl;
      scaleFactorsH_["m_iso"]=(TH2F *)fIn->Get("NUM_TightRelIso_DEN_TightIDandIPCut_pt_abseta")->Clone();
      scaleFactorsH_["m_iso"]->SetDirectory(0);
      fIn->Close();
    }
  }

  ////////////


  // trigger scale factors
  TString trg_SF;
  if (era_==2017){
    trg_SF = era+"/AN2019_140_TriggerSF_2017.root";
  }

  // mumu
//  for(size_t i=0; i<trg_SF.size(); i++) {
    gSystem->ExpandPathName(trg_SF);
    fIn=TFile::Open(trg_SF);
    if(fIn && !fIn->IsZombie()) {
      cout << "trigger SF from" << trg_SF << endl;
      scaleFactorsH_["mumu_pt_SF"]=(TH2F *)fIn->Get("h2D_SF_mumu_lepABpt")->Clone();
      scaleFactorsH_["mumu_eta_SF"]=(TH2F *)fIn->Get("h2D_SF_mumu_lepABeta")->Clone();
      scaleFactorsH_["mumu_pt_SF"]->SetDirectory(0);
      scaleFactorsH_["mumu_eta_SF"]->SetDirectory(0);

      scaleFactorsH_["emu_pt_SF"]=(TH2F *)fIn->Get("h2D_SF_emu_lepABpt")->Clone();
      scaleFactorsH_["emu_eta_SF"]=(TH2F *)fIn->Get("h2D_SF_emu_lepABeta")->Clone();
      scaleFactorsH_["emu_pt_SF"]->SetDirectory(0);
      scaleFactorsH_["emu_eta_SF"]->SetDirectory(0);

      scaleFactorsH_["ee_pt_SF"]=(TH2F *)fIn->Get("h2D_SF_ee_lepABpt")->Clone();
      scaleFactorsH_["ee_eta_SF"]=(TH2F *)fIn->Get("h2D_SF_ee_lepABeta")->Clone();
      scaleFactorsH_["ee_pt_SF"]->SetDirectory(0);
      scaleFactorsH_["ee_eta_SF"]->SetDirectory(0);
      fIn->Close();
    }
//  }

  //Charge Flip
  TString ChargeFlip_SF_chi2, ChargeFlip_SF_MLE;
  if(era_==2017){
      ChargeFlip_SF_chi2 = era+"/ChargeFlipProbability_2017_chi2_Fit.root";
      ChargeFlip_SF_MLE = era+"/ChargeFlipProbability_2017_MLE_Fit.root";
  }
  gSystem->ExpandPathName(ChargeFlip_SF_chi2);
  fIn=TFile::Open(ChargeFlip_SF_chi2);
  if(fIn && !fIn->IsZombie()) {
    cout << "electrons: charge flip SF from" << ChargeFlip_SF_chi2 << endl;
    scaleFactorsH_["ChargeFlip_chi2_data"]=(TH2F *)fIn->Get("data_CFRate");
    scaleFactorsH_["ChargeFlip_chi2_MC"]=(TH2F *)fIn->Get("MC_CFRate");
    scaleFactorsH_["ChargeFlip_chi2_data"]->SetDirectory(0);
    scaleFactorsH_["ChargeFlip_chi2_MC"]->SetDirectory(0);
  }
  fIn->Close();
  gSystem->ExpandPathName(ChargeFlip_SF_MLE);
  fIn=TFile::Open(ChargeFlip_SF_MLE);
  if(fIn && !fIn->IsZombie()) {
    cout << "electrons: charge flip SF from" << ChargeFlip_SF_MLE << endl;
    scaleFactorsH_["ChargeFlip_MLE_data"]=(TH2F *)fIn->Get("data_CFRate");
    scaleFactorsH_["ChargeFlip_MLE_MC"]=(TH2F *)fIn->Get("MC_CFRate");
    scaleFactorsH_["ChargeFlip_MLE_data"]->SetDirectory(0);
    scaleFactorsH_["ChargeFlip_MLE_MC"]->SetDirectory(0);
  }
  fIn->Close();
}


//
EffCorrection_t EfficiencyScaleFactorsWrapper::getPhotonSF(float pt,float eta)
{
  EffCorrection_t sf(1.0,0.01);

  if(scaleFactorsH_.find("g_rec") != scaleFactorsH_.end()) {
    TH2 *h=scaleFactorsH_["g_rec"];
    float etaForSF  = TMath::Min(h->GetXaxis()->GetXmax(),TMath::Max(eta,h->GetXaxis()->GetXmin()));
    int   xbinForSF = h->GetXaxis()->FindBin(etaForSF);
    float ptForSF   = TMath::Min(h->GetYaxis()->GetXmax(),TMath::Max(pt,h->GetYaxis()->GetXmin()));
    int   ybinForSF = h->GetYaxis()->FindBin(ptForSF);
    sf.first  *= h->GetBinContent(xbinForSF,ybinForSF);
    sf.second *= pow(h->GetBinError(xbinForSF,ybinForSF),2);
  }

  if(scaleFactorsH_.find("g_id") != scaleFactorsH_.end()) {
    TH2 *h=scaleFactorsH_["g_id"];
    float etaForSF  = TMath::Min(h->GetXaxis()->GetXmax(),TMath::Max(eta,h->GetXaxis()->GetXmin()));
    int   xbinForSF = h->GetXaxis()->FindBin(etaForSF);
    float ptForSF   = TMath::Min(h->GetYaxis()->GetXmax(),TMath::Max(pt,h->GetYaxis()->GetXmin()));
    int   ybinForSF = h->GetYaxis()->FindBin(ptForSF);
    sf.first  *= h->GetBinContent(xbinForSF,ybinForSF);
    sf.second *= pow(h->GetBinError(xbinForSF,ybinForSF),2);
  }

  if(scaleFactorsH_.find("g_psv") != scaleFactorsH_.end()) {
    if(era_==2016){
      TH2 *h=scaleFactorsH_["g_psv"];
      int   xbinForSF(fabs(eta)<1.5 ? 1 : 3);
      sf.first  *= h->GetBinContent(xbinForSF,1);
      sf.second *= pow(h->GetBinError(xbinForSF,1),2);
    }
    else if(era_==2017){
      TH1 *h=scaleFactors1DH_["g_psv"];
      int   xbinForSF(fabs(eta)<1.5 ? 1 : 4);
      sf.first  *= h->GetBinContent(xbinForSF);
      sf.second *= pow(h->GetBinError(xbinForSF),2);
    }
  }


  sf.second=sqrt(sf.second);

  return sf;
}

//
EffCorrection_t EfficiencyScaleFactorsWrapper::getElectronSF(float pt,float eta)
{
  EffCorrection_t sf(1.0,0.01);

  if(scaleFactorsH_.find("e_rec") != scaleFactorsH_.end()) {
    TH2 *h=scaleFactorsH_["e_rec"];
    float ptForSF = pt;
    float etaForSF = fabs(eta);
    if (pt > h->GetYaxis()->GetXmax()) ptForSF = h->GetYaxis()->GetXmax() - 0.01;
    if (pt < h->GetYaxis()->GetXmin()) ptForSF = h->GetYaxis()->GetXmin() + 0.01;
    if (fabs(eta) > h->GetXaxis()->GetXmax()) etaForSF = h->GetXaxis()->GetXmax() - 0.01;
    if (fabs(eta) < h->GetXaxis()->GetXmin()) etaForSF = h->GetXaxis()->GetXmin() + 0.01;
    int   ybinForSF = h->GetYaxis()->FindBin(ptForSF);
    int   xbinForSF = h->GetXaxis()->FindBin(etaForSF);
    sf.first  *= h->GetBinContent(xbinForSF,ybinForSF);
    sf.second *= pow(h->GetBinError(xbinForSF,ybinForSF),2);
  }

  if(scaleFactorsH_.find("e_id") != scaleFactorsH_.end()) {
    TH2 *h=scaleFactorsH_["e_id"];
    float ptForSF = pt;
    float etaForSF = fabs(eta);
    if (pt > h->GetYaxis()->GetXmax()) ptForSF = h->GetYaxis()->GetXmax() - 0.01;
    if (pt < h->GetYaxis()->GetXmin()) ptForSF = h->GetYaxis()->GetXmin() + 0.01;
    if (fabs(eta) > h->GetXaxis()->GetXmax()) etaForSF = h->GetXaxis()->GetXmax() - 0.01;
    if (fabs(eta) < h->GetXaxis()->GetXmin()) etaForSF = h->GetXaxis()->GetXmin() + 0.01;
    int   ybinForSF = h->GetYaxis()->FindBin(ptForSF);
    int   xbinForSF = h->GetXaxis()->FindBin(etaForSF);
    sf.first  *= h->GetBinContent(xbinForSF,ybinForSF);
    sf.second *= pow(h->GetBinError(xbinForSF,ybinForSF),2);
  }

  sf.second=sqrt(sf.second);

  return sf;
}

//
EffCorrection_t EfficiencyScaleFactorsWrapper::getMuonSF(float pt,float eta)
{
  EffCorrection_t sf(1.0,0.01);

  if(scaleFactorsH_.find("m_iso") != scaleFactorsH_.end()) {
    TH2 *h=scaleFactorsH_["m_iso"];
    float ptForSF = pt;
    float etaForSF = fabs(eta);
    if (pt > h->GetXaxis()->GetXmax()) ptForSF = h->GetXaxis()->GetXmax() - 0.01;
    if (pt < h->GetXaxis()->GetXmin()) ptForSF = h->GetXaxis()->GetXmin() + 0.01;
    if (fabs(eta) > h->GetYaxis()->GetXmax()) etaForSF = h->GetYaxis()->GetXmax() - 0.01;
    if (fabs(eta) < h->GetYaxis()->GetXmin()) etaForSF = h->GetYaxis()->GetXmin() + 0.01;
    int   ybinForSF = h->GetYaxis()->FindBin(etaForSF);
    int   xbinForSF = h->GetXaxis()->FindBin(ptForSF);
    sf.first  *= h->GetBinContent(xbinForSF,ybinForSF);
    sf.second *= pow(h->GetBinError(xbinForSF,ybinForSF),2);
  }

  if(scaleFactorsH_.find("m_id") != scaleFactorsH_.end()) {
    TH2 *h=scaleFactorsH_["m_id"];
    float ptForSF = pt;
    float etaForSF = fabs(eta);
    if (pt > h->GetXaxis()->GetXmax()) ptForSF = h->GetXaxis()->GetXmax() - 0.01;
    if (pt < h->GetXaxis()->GetXmin()) ptForSF = h->GetXaxis()->GetXmin() + 0.01;
    if (fabs(eta) > h->GetYaxis()->GetXmax()) etaForSF = h->GetYaxis()->GetXmax() - 0.01;
    if (fabs(eta) < h->GetYaxis()->GetXmin()) etaForSF = h->GetYaxis()->GetXmin() + 0.01;
    int   ybinForSF = h->GetYaxis()->FindBin(etaForSF);
    int   xbinForSF = h->GetXaxis()->FindBin(ptForSF);
    sf.first  *= h->GetBinContent(xbinForSF,ybinForSF);
    sf.second *= pow(h->GetBinError(xbinForSF,ybinForSF),2);
  }

  //cout<<"sf "<<sf.first<<"  "<<sf.second<<endl;
  sf.second=sqrt(sf.second);
  return sf;
}

//
EffCorrection_t EfficiencyScaleFactorsWrapper::getMuMuPtSF(float pt1,float pt2)
{
  EffCorrection_t sf(1.0,0.01);

  if(scaleFactorsH_.find("mumu_pt_SF") != scaleFactorsH_.end()) {
    TH2 *h=scaleFactorsH_["mumu_pt_SF"];
    float pt1ForSF = pt1;
    float pt2ForSF = pt2;
    if (pt1 > h->GetXaxis()->GetXmax()) pt1ForSF = h->GetXaxis()->GetXmax() - 0.01;
    if (pt1 < h->GetXaxis()->GetXmin()) pt1ForSF = h->GetXaxis()->GetXmin() + 0.01;
    if (pt2 > h->GetYaxis()->GetXmax()) pt2ForSF = h->GetYaxis()->GetXmax() - 0.01;
    if (pt2 < h->GetYaxis()->GetXmin()) pt2ForSF = h->GetYaxis()->GetXmin() + 0.01;
    int   ybinForSF = h->GetYaxis()->FindBin(pt2ForSF);
    int   xbinForSF = h->GetXaxis()->FindBin(pt1ForSF);
    sf.first  *= h->GetBinContent(xbinForSF,ybinForSF);
    sf.second *= h->GetBinError(xbinForSF,ybinForSF);
  }

  //cout<<"sf mumu pt"<<sf.first<<"  "<<sf.second<<endl;
  return sf;
}

EffCorrection_t EfficiencyScaleFactorsWrapper::getMuMuEtaSF(float eta1,float eta2)
{
  EffCorrection_t sf(1.0,0.01);

  if(scaleFactorsH_.find("mumu_eta_SF") != scaleFactorsH_.end()) {
    TH2 *h=scaleFactorsH_["mumu_eta_SF"];
    float eta1ForSF= eta1;
    float eta2ForSF = eta2;
    if (eta1 > h->GetXaxis()->GetXmax()) eta1ForSF = h->GetXaxis()->GetXmax() - 0.01;
    if (eta1 < h->GetXaxis()->GetXmin()) eta1ForSF = h->GetXaxis()->GetXmin() + 0.01;
    if (eta2 > h->GetYaxis()->GetXmax()) eta2ForSF = h->GetYaxis()->GetXmax() - 0.01;
    if (eta2 < h->GetYaxis()->GetXmin()) eta2ForSF = h->GetYaxis()->GetXmin() + 0.01;
    int   ybinForSF = h->GetYaxis()->FindBin(eta2ForSF);
    int   xbinForSF = h->GetXaxis()->FindBin(eta1ForSF);
    sf.first  *= h->GetBinContent(xbinForSF,ybinForSF);
    sf.second *= h->GetBinError(xbinForSF,ybinForSF);
  }

  //cout<<"sf mumu eta"<<sf.first<<"  "<<sf.second<<endl;
  return sf;
}

EffCorrection_t EfficiencyScaleFactorsWrapper::getEMuPtSF(float pt1,float pt2)
{
  EffCorrection_t sf(1.0,0.01);

  if(scaleFactorsH_.find("emu_pt_SF") != scaleFactorsH_.end()) {
    TH2 *h=scaleFactorsH_["emu_pt_SF"];
    float pt1ForSF = pt1;
    float pt2ForSF = pt2;
    if (pt1 > h->GetXaxis()->GetXmax()) pt1ForSF = h->GetXaxis()->GetXmax() - 0.01;
    if (pt1 < h->GetXaxis()->GetXmin()) pt1ForSF = h->GetXaxis()->GetXmin() + 0.01;
    if (pt2 > h->GetYaxis()->GetXmax()) pt2ForSF = h->GetYaxis()->GetXmax() - 0.01;
    if (pt2 < h->GetYaxis()->GetXmin()) pt2ForSF = h->GetYaxis()->GetXmin() + 0.01;
    int   ybinForSF = h->GetYaxis()->FindBin(pt2ForSF);
    int   xbinForSF = h->GetXaxis()->FindBin(pt1ForSF);
    sf.first  *= h->GetBinContent(xbinForSF,ybinForSF);
    sf.second *= h->GetBinError(xbinForSF,ybinForSF);
  }

  //cout<<"sf emu pt"<<sf.first<<"  "<<sf.second<<endl;
  return sf;
}

EffCorrection_t EfficiencyScaleFactorsWrapper::getEMuEtaSF(float eta1,float eta2)
{
  EffCorrection_t sf(1.0,0.01);

  if(scaleFactorsH_.find("emu_eta_SF") != scaleFactorsH_.end()) {
    TH2 *h=scaleFactorsH_["emu_eta_SF"];
    float eta1ForSF = eta1;
    float eta2ForSF = eta2;
    if (eta1 > h->GetXaxis()->GetXmax()) eta1ForSF = h->GetXaxis()->GetXmax() - 0.01;
    if (eta1 < h->GetXaxis()->GetXmin()) eta1ForSF = h->GetXaxis()->GetXmin() + 0.01;
    if (eta2 > h->GetYaxis()->GetXmax()) eta2ForSF = h->GetYaxis()->GetXmax() - 0.01;
    if (eta2 < h->GetYaxis()->GetXmin()) eta2ForSF = h->GetYaxis()->GetXmin() + 0.01;
    int   ybinForSF = h->GetYaxis()->FindBin(eta2ForSF);
    int   xbinForSF = h->GetXaxis()->FindBin(eta1ForSF);
    sf.first  *= h->GetBinContent(xbinForSF,ybinForSF);
    sf.second *= h->GetBinError(xbinForSF,ybinForSF);
  }

  //cout<<"sf emu eta"<<sf.first<<"  "<<sf.second<<endl;
  return sf;
}

EffCorrection_t EfficiencyScaleFactorsWrapper::getEEPtSF(float pt1,float pt2)
{
  EffCorrection_t sf(1.0,0.01);

  if(scaleFactorsH_.find("ee_pt_SF") != scaleFactorsH_.end()) {
    TH2 *h=scaleFactorsH_["ee_pt_SF"];
    float pt1ForSF = pt1;
    float pt2ForSF = pt2;
    if (pt1 > h->GetXaxis()->GetXmax()) pt1ForSF = h->GetXaxis()->GetXmax() - 0.01;
    if (pt1 < h->GetXaxis()->GetXmin()) pt1ForSF = h->GetXaxis()->GetXmin() + 0.01;
    if (pt2 > h->GetYaxis()->GetXmax()) pt2ForSF = h->GetYaxis()->GetXmax() - 0.01;
    if (pt2 < h->GetYaxis()->GetXmin()) pt2ForSF = h->GetYaxis()->GetXmin() + 0.01;
    int   ybinForSF = h->GetYaxis()->FindBin(pt2ForSF);
    int   xbinForSF = h->GetXaxis()->FindBin(pt1ForSF);
    sf.first  *= h->GetBinContent(xbinForSF,ybinForSF);
    sf.second *= h->GetBinError(xbinForSF,ybinForSF);
  }

  //cout<<"sf ee pt"<<sf.first<<"  "<<sf.second<<endl;
  return sf;
}

EffCorrection_t EfficiencyScaleFactorsWrapper::getEEEtaSF(float eta1,float eta2)
{
  EffCorrection_t sf(1.0,0.01);

  if(scaleFactorsH_.find("ee_eta_SF") != scaleFactorsH_.end()) {
    TH2 *h=scaleFactorsH_["ee_eta_SF"];
    float eta1ForSF = eta1;
    float eta2ForSF = eta2;
    if (eta1 > h->GetXaxis()->GetXmax()) eta1ForSF = h->GetXaxis()->GetXmax() - 0.01;
    if (eta1 < h->GetXaxis()->GetXmin()) eta1ForSF = h->GetXaxis()->GetXmin() + 0.01;
    if (eta2 > h->GetYaxis()->GetXmax()) eta2ForSF = h->GetYaxis()->GetXmax() - 0.01;
    if (eta2 < h->GetYaxis()->GetXmin()) eta2ForSF = h->GetYaxis()->GetXmin() + 0.01;
    int   ybinForSF = h->GetYaxis()->FindBin(eta2ForSF);
    int   xbinForSF = h->GetXaxis()->FindBin(eta1ForSF);
    sf.first  *= h->GetBinContent(xbinForSF,ybinForSF);
    sf.second *= h->GetBinError(xbinForSF,ybinForSF);
  }

  //cout<<"sf ee eta"<<sf.first<<"  "<<sf.second<<endl;
  return sf;
}

EffCorrection_t EfficiencyScaleFactorsWrapper::getChargeFlipSF(TString File_name, TString Fit_type, float pt1,float eta1, int charge1, float pt2, float eta2, int charge2)
{
  EffCorrection_t sf(1.0,0.01); // Uncertainty part not yet be done.
  if(File_name.Contains("TTTo") || File_name.Contains("DY")){ // Only apply to ttbar and DY process
    TH2 *h_data=scaleFactorsH_["ChargeFlip_chi2_data"];
    TH2 *h_MC  =scaleFactorsH_["ChargeFlip_chi2_MC"];
    if(Fit_type.Contains("MLE")){
      TH2 *h_data_MLE = scaleFactorsH_["ChargeFlip_MLE_data"];
      TH2 *h_MC_MLE = scaleFactorsH_["ChargeFlip_MLE_MC"];
      h_data = h_data_MLE;
      h_MC = h_MC_MLE;
    }
    float pt1ForSF = pt1;
    float pt2ForSF = pt2;
    float eta1ForSF = fabs(eta1);
    float eta2ForSF = fabs(eta2);
    if (pt1 > h_data->GetXaxis()->GetXmax()) pt1ForSF = h_data->GetXaxis()->GetXmax() - 0.01;
    if (pt1 < h_data->GetXaxis()->GetXmin()) pt1ForSF = h_data->GetXaxis()->GetXmin() + 0.01;
    if (pt2 > h_data->GetXaxis()->GetXmax()) pt2ForSF = h_data->GetXaxis()->GetXmax() - 0.01;
    if (pt2 < h_data->GetXaxis()->GetXmin()) pt2ForSF = h_data->GetXaxis()->GetXmin() + 0.01;
    if (eta1 > h_data->GetYaxis()->GetXmax()) eta1ForSF = h_data->GetYaxis()->GetXmax() - 0.01;
    if (eta1 < h_data->GetYaxis()->GetXmin()) eta1ForSF = h_data->GetYaxis()->GetXmin() + 0.01;
    if (eta2 > h_data->GetYaxis()->GetXmax()) eta2ForSF = h_data->GetYaxis()->GetXmax() - 0.01;
    if (eta2 < h_data->GetYaxis()->GetXmin()) eta2ForSF = h_data->GetYaxis()->GetXmin() + 0.01;
    int   y1binForSF = h_data->GetYaxis()->FindBin(eta1ForSF);
    int   x1binForSF = h_data->GetXaxis()->FindBin(pt1ForSF);
    int   y2binForSF = h_data->GetYaxis()->FindBin(eta2ForSF);
    int   x2binForSF = h_data->GetXaxis()->FindBin(pt2ForSF);
    float data_CF,MC_CF,P1_data,P2_data,P1_MC,P2_MC;
    P1_data = h_data->GetBinContent(x1binForSF,y1binForSF);
    P2_data = h_data->GetBinContent(x2binForSF,y2binForSF);
    P1_MC = h_MC->GetBinContent(x1binForSF, y1binForSF);
    P2_MC = h_MC->GetBinContent(x2binForSF, y2binForSF);
    data_CF = P1_data + P2_data - 2.*P1_data*P2_data;
    MC_CF = P1_MC + P2_MC -2.*P1_MC*P2_MC;
    if(charge1*charge2>0) sf.first = data_CF/MC_CF;
    else if(charge1*charge2<0) sf.first = (1.-data_CF)/(1.-MC_CF);
  }

  return sf;
}
//
EfficiencyScaleFactorsWrapper::~EfficiencyScaleFactorsWrapper()
{
}
