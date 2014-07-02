#ifndef __Geomtk_Domain__
#define __Geomtk_Domain__

#include "geomtk_commons.h"

namespace geomtk {

enum BndType {
    PERIODIC, OPEN, POLE, RIGID, INVALID
};

enum VertCoordType {
    HEIGHT, CLASSIC_PRESSURE_SIGMA, HYBRID_PRESSURE_SIGMA
};

class VertCoord {
public:
    VertCoord() {}
    virtual ~VertCoord() {}
};

/**
 *  This class describes the space coordinate of a point.
 */
class SpaceCoord {
protected:
    vec coord;
public:
    SpaceCoord(int numDim);
    SpaceCoord(const SpaceCoord &other);
    virtual ~SpaceCoord();

    virtual void setCoord(double x, double y);
    virtual void setCoord(double x, double y, double z);
    virtual void setCoordComp(int dim, double comp);

    virtual SpaceCoord& operator=(const SpaceCoord &other) {
        if (this != &other) {
            coord = other.coord;
        }
        return *this;
    }

    virtual double operator()(int i) const { return coord(i); }
    virtual double& operator()(int i) { return coord(i); }
    virtual const vec& operator()() const { return coord; }
    virtual vec& operator()() { return coord; }

    /**
     *  This method return the Cartesian coordinate, in this case it is `coord`,
     *  so this method is just for compatible with SphereCoord.
     *
     *  @return The Cartesian coordinate vector.
     */
    const vec& getCartCoord() const { return coord; }

    virtual void print() const;
};

/**
 *  This class describes the body coordinate referenced to a point.
 */
class BodyCoord : public SpaceCoord {
public:
    BodyCoord(int numDim);
    virtual ~BodyCoord();
};

// -----------------------------------------------------------------------------

class Velocity {
protected:
    vec v;
public:
    Velocity();
    Velocity(int numDim);
    virtual ~Velocity();

    virtual void setNumDim(int numDim);

    double operator()(int i) const { return v(i); }
    double& operator()(int i) { return v(i); }
    vec& operator()() { return v; }

    const Velocity operator+(const Velocity &other) const {
        Velocity res;
        res.v = v+other.v;
        return res;
    }
    const Velocity operator-(const Velocity &other) const {
        Velocity res;
        res.v = v-other.v;
        return res;
    }
    const Velocity operator*(double scale) const {
        Velocity res;
        res.v = v*scale;
        return res;
    }
    const Velocity operator/(double scale) const {
        Velocity res;
        res.v = v/scale;
        return res;
    }

    virtual void print() const;
};

// -----------------------------------------------------------------------------

class Domain {
protected:
    int numDim;
    vector<string> axisName;
    vector<string> axisLongName;
    vector<string> axisUnits;
    vec axisStarts;
    vec axisEnds;
    vec axisSpans;
    BndType *bndTypeStarts;
    BndType *bndTypeEnds;
    /**
     *  The vertical coordinate is so special that we need an object to handle
     *  it.
     */
    VertCoordType vertCoordType;
    VertCoord *vertCoord;
public:
    Domain();
    Domain(int numDim);
    Domain(VertCoordType type);
    virtual ~Domain();

    /**
     *  Return the dimension number of the domain.
     *
     *  @return The dimension number.
     */
    virtual int getNumDim() const { return numDim; }

    /**
     *  Set the axis along the specified dimension, including the meta info.
     *
     *  @param dim          the dimension index.
     *  @param axisName     the short name of the axis, e.g. 'x'.
     *  @param axisLongName the long name of the axis.
     *  @param axisUnits    the coordinate units of the axis.
     *  @param start        the start coordinate value.
     *  @param bndTypeStart the boundary type of the axis start.
     *  @param end          the end coordinate value.
     *  @param bndTypeEnd   the boundary type of the axis end.
     */
    virtual void setAxis(int dim, const string &axisName,
                         const string &axisLongName, const string &axisUnits,
                         double start, BndType bndTypeStart,
                         double end, BndType bndTypeEnd);

    /**
     *  Get the axis short name.
     *
     *  @param i the dimension index.
     *  @return The axis short name.
     */
    virtual const string& getAxisName(int i) const { return axisName[i]; }

    /**
     *  Get the axis long name.
     *
     *  @param i the dimension index.
     *  @return The axis long name.
     */
    virtual const string& getAxisLongName(int i) const { return axisLongName[i]; }

    /**
     *  Get the axis coordinate units.
     *
     *  @param i the dimension index.
     *  @return The axis coordinate units.
     */
    virtual const string& getAxisUnits(int i) const { return axisUnits[i]; }

    /**
     *  Get the axis start coordinate value.
     *
     *  @param i the dimension index.
     *  @return The axis start coordinate value.
     */
    virtual double getAxisStart(int i) const { return axisStarts(i); }

    /**
     *  Get the axis end coordinate value.
     *
     *  @param i the dimension index.
     *  @return The axis end coordinate value.
     */
    virtual double getAxisEnd(int i) const { return axisEnds(i); }

    /**
     *  Get the axis coordinate span.
     *
     *  @param i the dimension index.
     *  @return The axis coordinate span.
     */
    virtual double getAxisSpan(int i) const { return axisSpans(i); }

    /**
     *  Get the axis start boundary type.
     *
     *  @param i the dimension index.
     *  @return The axis start boundary type.
     */
    virtual BndType getAxisStartBndType(int i) const { return bndTypeStarts[i]; }

    /**
     *  Get the axis end boundary type.
     *
     *  @param i the dimension index.
     *  @return The axis end boundary type.
     */
    virtual BndType getAxisEndBndType(int i) const { return bndTypeEnds[i]; }

    VertCoordType getVertCoordType() const { return vertCoordType; }

    VertCoord& getVertCoord() { return *vertCoord; }

    /**
     *  Check the given space coordinate, especially when the boundary condition
     *  is periodic. The out-of-range coordinate should be put back.
     *
     *  @param x the space coordinate.
     */
    virtual void check(SpaceCoord &x) const;

    /**
     *  Calculate distance between two space coordinates.
     *
     *  @param x the first space coordinate.
     *  @param y the second space coordinate.
     *
     *  @return The distance.
     */
    double calcDistance(const SpaceCoord &x, const SpaceCoord &y) const;

    /**
     *  Difference two space coordinates.
     *
     *  @param x the first space coordinate.
     *  @param y the second space coordinate.
     *
     *  @return The distance.
     */
    vec diffCoord(const SpaceCoord &x, const SpaceCoord &y) const;

    /**
     *  Get a brief about the domain.
     *
     *  @return The brief.
     */
    virtual string getBrief() const;
};

}

#endif
