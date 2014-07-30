// Unrestricted Use - Property of AECL
// 
// PrimaryGeneratorAction.hh
// GEANT4 - geant4.9.3.p01
//
// Header File for Source and Initial Kinematics Specifications
//      Contains class functions/variables
//
// --------------------------------------------------------
//      Version 1.01 - 2011/04/29 - A. Ho
//      Edited 2014/07 M. P. Mendenhall
// --------------------------------------------------------

#ifndef PrimaryGeneratorAction_H
/// Assure header is only loaded once during compilation
#define PrimaryGeneratorAction_H

#include <G4VUserPrimaryGeneratorAction.hh>

#include <G4ThreeVector.hh>
#include <G4ParticleGun.hh>

class G4Event;
class G4ParticleGun;
class PrimaryGeneratorMessenger;
class DetectorConstruction;

class CRYModule;
class IBDModule;
class FissionAntiNuModule;
class CosmicMuonModule;
class CosmicNeutronModule;

/// Specification for a primary particle to throw
struct primaryPtcl {
    int PDGid;  ///< PDG particle ID enumeration
    double KE;  ///< particle kinetic energy
    G4ThreeVector pos;  ///< vertex position
    G4ThreeVector mom;  ///< momentum direction
    double t;           ///< particle time
};

class PrimaryGeneratorAction;

/// Base class interface for alternate generator modules
class PrimaryGeneratorModule {
public:
    /// Constructor
    PrimaryGeneratorModule(PrimaryGeneratorAction* P): myPGA(P) { }
    /// Destructor
    virtual ~PrimaryGeneratorModule() { }
    
    /// throw event particles
    virtual void GeneratePrimaries(G4Event* anEvent) = 0;
    /// get total "real time" for primaries thrown
    virtual G4double GetGeneratorTime() const { return 0; }
    
protected:
    
    /// convenience function for throwing listed primaries
    void throwPrimaries(const std::vector<primaryPtcl>& v, G4Event* anEvent);
    
    PrimaryGeneratorAction* myPGA;      ///< PrimaryGeneratorAction this module runs for
};


/// Class for generating primary particles for event
class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
    
friend class PrimaryGeneratorMessenger;

public:
    /// constructor
    PrimaryGeneratorAction();
    /// destructor
    virtual ~PrimaryGeneratorAction();

    /// Generates a particle and launches it into the geometry
    void GeneratePrimaries(G4Event* anEvent);

    G4ParticleGun* GetParticleGun() const { return particle_gun; };
    DetectorConstruction* GetDetector() const { return detect; }
    G4int GetVerbosity() const { return verbose; }
    
    /// load CRY as current generator
    void loadCRYModule();              
    /// load Inverse Beta Decay as current generator
    void loadIBDModule();
    /// load fission anti-neutrinos as current generator
    void loadFisAntNuModule();
    /// load cosmic muons as current generator
    void loadCosmicMuonModule();
    /// load cosmic neutrons as current generator
    void loadCosmicNeutronModule();
    
protected:
    
    void SetVerbosity(G4int);
    
    PrimaryGeneratorModule* genModule;          ///< generator module currently in use
    CRYModule* myCRYModule;                     ///< CRY generator module
    IBDModule* myIBDModule;                     ///< Inverse Beta Decay generator
    FissionAntiNuModule* myFisAntNuModule;      ///< Fission anti-neutrinos generator
    CosmicMuonModule* myCosmicMuonModule;       ///< Cosmic muons generator
    CosmicNeutronModule* myCosmicNeutronModule; ///< Cosmic neutrons generator

    G4int verbose;      ///< Verbosity (0 = silent, 1 = minimal, 2 = loud)

    G4ParticleGun* particle_gun;
    DetectorConstruction* detect;
    PrimaryGeneratorMessenger* myMessenger;
};

#endif
