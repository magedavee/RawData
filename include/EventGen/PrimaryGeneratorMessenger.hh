// Unrestricted Use - Property of AECL
// 
// PrimaryGeneratorMessenger.hh
// GEANT4 - geant4.9.3.p01
//
// Header File for Initial Kinematics Messenger
//      Contains class functions/variables
//
// --------------------------------------------------------
//      Version 1.01 - 2011/04/29 - A. Ho
//      Edited 201407 M. P. Mendenhall
// --------------------------------------------------------

#ifndef PrimaryGeneratorMessenger_H
/// Assure header file is only loaded once
#define PrimaryGeneratorMessenger_H

#include "G4UImessenger.hh"

#include "globals.hh"

class PrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAnInteger;

/// UI messenger for primary event generator
class PrimaryGeneratorMessenger: public G4UImessenger {
public:
    /// Constructor
    PrimaryGeneratorMessenger(PrimaryGeneratorAction*);
    /// Destructor
    virtual ~PrimaryGeneratorMessenger();
    
    void SetNewValue(G4UIcommand*,G4String);
    
private:
    
    PrimaryGeneratorAction* generator;
    G4UIdirectory* genDir;

    G4UIcmdWithAnInteger* verbCmd;
    G4UIcmdWithoutParameter* moduleCRYcmd;
    G4UIcmdWithoutParameter* moduleIBDcmd;
    G4UIcmdWithoutParameter* moduleFisANucmd;
    G4UIcmdWithoutParameter* moduleCosMucmd;
    G4UIcmdWithoutParameter* moduleCosNcmd;
};

#endif
