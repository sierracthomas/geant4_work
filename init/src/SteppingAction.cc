//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B1/src/SteppingAction.cc
/// \brief Implementation of the B1::SteppingAction class

#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"

#include "G4CsvAnalysisManager.hh"

namespace B1
{

  
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(EventAction* eventAction)
: fEventAction(eventAction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  //track length 
  G4ThreeVector x = step->GetDeltaPosition();
  using G4AnalysisManager = G4CsvAnalysisManager;
  G4AnalysisManager *man = G4AnalysisManager::Instance();
  man->FillNtupleDColumn(0,0, sqrt(x[0]*x[0]+x[1]*x[1]));
  man->AddNtupleRow(0);

  G4int track_id = step->GetTrack()->GetTrackID();
  man->FillNtupleIColumn(1,track_id);

  //Event ID
  G4int event_id = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
  man->FillNtupleIColumn(2,event_id);

  //Delta Z distance  
  man->FillNtupleDColumn(3, x[2]);

  //Change in energy
  G4double delta_E = step->GetDeltaEnergy();
  man->FillNtupleDColumn(4,delta_E);

  //Type of particle
  G4String name = step->GetTrack()->GetParticleDefinition()->GetParticleName();
  G4int type = 0;
  
  if (name=="gamma"){type = 0;}
  else if (name=="e-"){type = 1;}
  else {type = 2;}
  man->FillNtupleIColumn(5,type);

  man->AddNtupleRow(0);

  if (!fScoringVolume) {
    const auto detConstruction = static_cast<const DetectorConstruction*>(
      G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fScoringVolume = detConstruction->GetScoringVolume();
  }

  // get volume of the current step
  G4LogicalVolume* volume
    = step->GetPreStepPoint()->GetTouchableHandle()
      ->GetVolume()->GetLogicalVolume();

  // check if we are in scoring volume
  if (volume != fScoringVolume) return;

  // collect energy deposited in this step
  G4double edepStep = step->GetTotalEnergyDeposit();
  fEventAction->AddEdep(edepStep);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}
