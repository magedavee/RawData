#ifndef DetectorConstruction_H
/// Assure header is only loaded once
#define DetectorConstruction_H

#include "Builder.hh"
#include "BuildingBuilder.hh"
#include "ScintCellBuilder.hh"
#include "ScatterSlabBuilder.hh"
#include "ScintSD.hh"

#include <G4VUserDetectorConstruction.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4UImessenger.hh>
#include <G4UIdirectory.hh>
#include <G4UIcmdWithAString.hh>

class DetectorConstruction: public G4VUserDetectorConstruction, public Builder,  public G4UImessenger {
public:
    /// Constructor
    DetectorConstruction();

    /// Assembles geometry of the simulation, returns world volume
    G4VPhysicalVolume* Construct();
    /// unused in this class
    virtual void construct() { }
    
    /// Respond to UI commands
    void SetNewValue(G4UIcommand* command, G4String newValue);
    
    /// Get reference to active scintillator volume
    G4LogicalVolume* getScintLog() const { return myBuilding.myDetUnit.myDet.myTank.scint_log; }
    
    enum buildMode {
        PROSPECT,       ///< PROSPECT detector
        TEST_CELL,      ///< scintillator testing cell
        SLAB            ///< material slab
    } mode;             ///< what kind of system to construct
    
    BuildingBuilder myBuilding;         ///< building containing detector
    ScintCellBuilder myTestCell;        ///< test scintillator-filled cell
    ScatterSlabBuilder mySlab;          ///< toy slab geometry
    
    G4VPhysicalVolume* theWorld;        ///< world volume
    G4VPhysicalVolume* ptclSrc;         ///< optional event generator source volume
    G4VPhysicalVolume* building_phys;   ///< physical building placement
    ShellLayerSpec worldShell;          ///< world outer vacuum layer dimensions
    
private:
    
    ScintSD* myScintSD;         ///< sensitive detector for scintillator volume
    G4UIdirectory geomDir;      ///< UI directory for construction geometry commands
    G4UIcmdWithAString modeCmd; ///< UI command for setting construction mode
    
    /// XML output contents
    virtual void fillNode(TXMLEngine& E);
};

#endif
