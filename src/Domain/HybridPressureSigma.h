#ifndef __GEOMTK_HybridPressureSigma__
#define __GEOMTK_HybridPressureSigma__

#include "geomtk_commons.h"
#include "TerrainFollowing.h"

namespace geomtk {

/**
 *  This class describes the hybrid pressure-sigma vertical coordinate:
 *
 *
 *      p = A * p0 + B * ps
 *
 *  where A and B are coefficients, p0 is some reference pressure and
 *  ps is the surface pressure.
 */
class HybridPressureSigma : public TerrainFollowing {
public:
    vec A[2], B[2];
    double p0;

    HybridPressureSigma();
    virtual ~HybridPressureSigma();

    virtual void
    init(int size);
    
    virtual double
    calcPressure(int gridType, double ps, int k) const;
}; // HybridPressureSigma

} // geomtk

#endif // __GEOMTK_HybridPressureSigma__
