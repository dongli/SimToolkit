#ifndef __RLLRegrid__
#define __RLLRegrid__

#include "Regrid.h"
#include "RLLMeshIndex.h"

namespace geomtk {

class RLLVelocityField;
class SphereVelocity;

class RLLRegrid : public Regrid {
protected:
public:
    RLLRegrid(const Mesh &mesh);
    virtual ~RLLRegrid();

    virtual void run(RegridMethod method, int timeLevel, const Field &f,
                     const SpaceCoord &x, double &y, MeshIndex *idx = NULL);
    virtual void run(RegridMethod method, int timelevel, const Field &f,
                     const SpaceCoord &x, vec &y, MeshIndex *idx = NULL);
    /**
     * Interpolator of velocity field onto given coordinate.
     * Velocity field is special, because its direction is singular on Poles.
     */
    virtual void run(RegridMethod method, int timeLevel, const RLLVelocityField &f,
                     const SpaceCoord &x, SphereVelocity &y, RLLMeshIndex *idx = NULL);
};

}

#endif
