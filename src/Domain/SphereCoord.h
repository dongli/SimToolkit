#ifndef __GEOMTK_SphereCoord__
#define __GEOMTK_SphereCoord__

#include "SpaceCoord.h"

namespace geomtk {

class SphereDomain;

enum Pole {
    SOUTH_POLE = 0, NORTH_POLE = 1, NOT_POLE = 2
};

/**
 *  This class describes the coordinate of sphere.
 */

class SphereCoord : public SpaceCoord {
protected:
    vec xt; //>! horizontal coordinate on polar stereographic plane
    // TODO: How to calculate Cartesian coordinate, since the vertical
    //       coordinate may not be height?
    vec::fixed<3> _cartCoord; //>! Cartesian coordinate representation
    double _cosLon, _sinLon, _cosLat, _sinLat;
public:
    SphereCoord();
    SphereCoord(int numDim);
    SphereCoord(const SphereCoord &other);
    virtual ~SphereCoord();

    virtual void
    init(int numDim);

    virtual vec
    data() const {
        vec res(coord.size()+4);
        int m;
        for (m = 0; m < coord.size(); ++m) {
            res[m] = coord[m];
        }
        res[m++] = _cosLon;
        res[m++] = _sinLon;
        res[m++] = _cosLat;
        res[m]   = _sinLat;
        return res;
    }

    virtual void
    set(double lon, double lat);

    virtual void
    set(double lon, double lat, double lev);

    virtual void
    setComp(int i, double comp);

    void
    setCartCoord(double x, double y, double z);

    void
    updateTrigonometricFunctions();

    double
    cosLon() const {
        return _cosLon;
    }

    double
    sinLon() const {
        return _sinLon;
    }

    double
    cosLat() const {
        return _cosLat;
    }

    double
    sinLat() const {
        return _sinLat;
    }

    const vec&
    psCoord() const {
        return xt;
    }

    vec&
    psCoord() {
        return xt;
    }

    virtual const vec&
    cartCoord() const {
        return _cartCoord;
    }

    virtual vec&
    cartCoord() {
        return _cartCoord;
    }

    SphereCoord&
    operator=(const SphereCoord& other);

    void
    transformToPS(const SphereDomain &domain);

    void
    transformFromPS(const SphereDomain &domain, Pole pole);

    void
    transformToCart(const SphereDomain &domain);

    virtual void
    print() const;
}; // SphereCoord

} // geomtk

#endif // __GEOMTK_SphereCoord__
