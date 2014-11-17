#include "PhysicsList.hh"

#include <G4UnitsTable.hh>
#include <G4SystemOfUnits.hh>
#include <G4ProductionCutsTable.hh>

PhysicsList::PhysicsList(): XMLProvider("Physics"),
myHadronic(new QGSP_BERT_HP()),
physDir("/phys/"),
opticalCmd("/phys/enableOptical",this),
emCmd("/phys/enableEM",this),
stepMaxCmd("/phys/stepMax",this) {
    
    opticalCmd.SetGuidance("Enables optical photon physics.");
    opticalCmd.AvailableForStates(G4State_PreInit,G4State_Init);
    
    emCmd.SetGuidance("Enables precision low-energy electromagnetic physics.");
    emCmd.AvailableForStates(G4State_PreInit,G4State_Init);
    
    stepMaxCmd.SetGuidance("Set limit on maximum step length.");
    stepMaxCmd.AvailableForStates(G4State_PreInit);
}

PhysicsList::~PhysicsList() {
    // Do not delete myOptPhys --- automatically handled
}

void PhysicsList::ConstructProcess() {
    if(myEMPhys) {
        AddTransportation();
        myEMPhys->ConstructProcess();
    } else myHadronic->ConstructProcess();
    
    if(myStepMax) {
        printf("Setting step-size limits...\n");
        theParticleIterator->reset();
        while ((*theParticleIterator)()){
            G4ParticleDefinition* particle = theParticleIterator->value();
            if (myStepMax->IsApplicable(*particle)) particle->GetProcessManager()->AddDiscreteProcess(myStepMax);
        }
    }
}

void PhysicsList::ConstructParticle() {
    if(myEMPhys) myEMPhys->ConstructParticle();
    else myHadronic->ConstructParticle();
}

void PhysicsList::SetCuts() {
    if(myEMPhys) {
        G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(250*eV, 1*GeV);
        double rangecut = 1*um;
        SetCutValue(rangecut, "gamma");
        SetCutValue(rangecut, "e-");
        SetCutValue(rangecut, "e+");
    } else myHadronic->SetCuts();
}


void PhysicsList::fillNode(TXMLEngine& E) {
    addAttr(E, "optical", myOptPhys?"on":"off");
    addAttr(E, "EM", myEMPhys?"on":"off");
    if(myStepMax) addAttr(E, "stepmax", G4BestUnit(myStepMax->GetMaxStep(),"Length"));
}

void PhysicsList::SetNewValue(G4UIcommand* command, G4String newValue) {
    if(command == &opticalCmd) {
        if(!myOptPhys) {
            myOptPhys = new G4OpticalPhysics(0);
            RegisterPhysics(myOptPhys);
        }
    } else if(command == &emCmd) {
        if(!myEMPhys) {
            myEMPhys = new G4EmLivermorePhysics();
            //RegisterPhysics(myEMPhys);
        }
    } else if(command == &stepMaxCmd) {
        if(!myStepMax) myStepMax = new StepMax();
        myStepMax->SetMaxStep(stepMaxCmd.GetNewDoubleValue(newValue));
    }
}
