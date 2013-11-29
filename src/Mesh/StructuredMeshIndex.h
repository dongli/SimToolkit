#ifndef __StructuredMeshIndex__
#define __StructuredMeshIndex__

#include "MeshIndex.h"
#include "StructuredMesh.h"

class StructuredMeshIndex : public MeshIndex {
protected:
    vector<int> indices[3];
public:
    StructuredMeshIndex(Mesh &mesh);
    virtual ~StructuredMeshIndex();

    /**
     * @brief Reset the indices to undefined status.
     */
    virtual void reset();

    virtual int operator()(int dim, StaggerType staggerType) const;
    virtual int& operator()(int dim, StaggerType staggerType);

    /**
     * @see MeshIndex::locate(SpaceCoord x)
     */
    virtual void locate(SpaceCoord &x);
};

#endif
