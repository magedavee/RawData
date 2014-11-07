#ifndef VERTEXPOSITIONER_HH
#define VERTEXPOSITIONER_HH

#include "XMLProvider.hh"

#include <vector>
using std::vector;

#include <G4ThreeVector.hh>

/// Specification for a primary particle to throw
struct primaryPtcl {
    int PDGid;          ///< PDG particle ID enumeration
    double KE;          ///< particle kinetic energy
    G4ThreeVector pos;  ///< vertex position
    G4ThreeVector mom;  ///< momentum direction
    double t;           ///< particle time
};

/// Base class for setting event vertex position and momentum direction
class VertexPositioner: public XMLProvider {
public:
    /// Constructor
    VertexPositioner(const string& nm = "VertexPositioner"): XMLProvider(nm) { }
    /// Destructor
    virtual ~VertexPositioner() { }
    
    /// Set position, momentum for list of particles; return effective number of vertices created
    virtual double setVertex(vector<primaryPtcl>& v) = 0;
    
    /// utility function for generating isotropic random direction
    static G4ThreeVector randomDirection();
};

/// Vertex positioner for isotropic momenta from a fixed point
class IsotPtPositioner: public VertexPositioner {
public:
    /// Constructor
    IsotPtPositioner(const G4ThreeVector& x = G4ThreeVector()): VertexPositioner("IsotPtPositioner"), x0(x) { }
    
    /// Set position, momentum for list of particles
    virtual double setVertex(vector<primaryPtcl>& v);
    
    G4ThreeVector x0;   ///< vertex position
 
protected:
    /// XML output contents
    virtual void fillNode(TXMLEngine& E);
};

#endif
