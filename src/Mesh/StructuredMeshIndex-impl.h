namespace geomtk {

template <class MeshType, class CoordType>
StructuredMeshIndex<MeshType, CoordType>::StructuredMeshIndex(int numDim)
        : MeshIndex<MeshType, CoordType>(numDim) {
    // NOTE: Index is 3D no matter the dimension size of domain.
    indices = new int*[3];
    for (int i = 0; i < 3; ++i) {
        indices[i] = new int[2];
    }
    reset();
}

template <class MeshType, class CoordType>
StructuredMeshIndex<MeshType, CoordType>::~StructuredMeshIndex() {
    for (int i = 0; i < 3; ++i) {
        delete [] indices[i];
    }
    delete [] indices;
}

template <class MeshType, class CoordType>
void StructuredMeshIndex<MeshType, CoordType>::reset() {
    for (int m = 0; m < this->numDim; ++m) {
        indices[m][GridType::FULL] = UNDEFINED_MESH_INDEX;
        indices[m][GridType::HALF] = UNDEFINED_MESH_INDEX;
    }
    // set the extra dimension indices to 0
    for (int m = this->numDim; m < 3; ++m) {
        indices[m][GridType::FULL] = 0;
        indices[m][GridType::HALF] = 0;
    }
}

template <class MeshType, class CoordType>
int StructuredMeshIndex<MeshType, CoordType>::operator()(int axisIdx, int gridType) const {
    return indices[axisIdx][gridType];
}

template <class MeshType, class CoordType>
int& StructuredMeshIndex<MeshType, CoordType>::operator()(int axisIdx, int gridType) {
    return indices[axisIdx][gridType];
}

template <class MeshType, class CoordType>
StructuredMeshIndex<MeshType, CoordType>& StructuredMeshIndex<MeshType, CoordType>::
        operator=(const StructuredMeshIndex<MeshType, CoordType> &other) {
    MeshIndex<MeshType, CoordType>::operator=(other);
    if (this != &other) {
        for (int m = 0; m < 3; ++m) {
            for (int i = 0; i < 2; ++i) {
                indices[m][i] = other.indices[m][i];
            }
        }
    }
    return *this;
}

template <class MeshType, class CoordType>
void StructuredMeshIndex<MeshType, CoordType>::locate(const MeshType &mesh, const CoordType &x) {
    const Domain &domain = mesh.getDomain();
    for (int m = 0; m < domain.getNumDim(); ++m) {
        // sanity check
        if (x(m) < domain.getAxisStart(m) ||
            x(m) > domain.getAxisEnd(m)) {
            REPORT_ERROR("Coordinate x is out of range on dimension " << m << "!");
        }
        if (indices[m][GridType::FULL] == UNDEFINED_MESH_INDEX) {
            if (domain.getAxisStartBndType(m) == PERIODIC) {
                for (int i = 0; i < mesh.getNumGrid(m, GridType::FULL); ++i) {
                    // cout << setw(5) << i;
                    // cout << setw(10) << mesh.getGridCoord(m, GridType::FULL, i);
                    // cout << setw(10) << x(m);
                    // cout << setw(10) << mesh.getGridCoord(m, GridType::FULL, i+1) << endl;
                    if (x(m) >= mesh.getGridCoordComp(m, GridType::FULL, i) &&
                        x(m) <= mesh.getGridCoordComp(m, GridType::FULL, i+1)) {
                        indices[m][GridType::FULL] = i;
                        break;
                    }
                }
                for (int i = indices[m][GridType::FULL]-1;
                     i < indices[m][GridType::FULL]+1; ++i) {
                    if (x(m) >= mesh.getGridCoordComp(m, GridType::HALF, i) &&
                        x(m) <= mesh.getGridCoordComp(m, GridType::HALF, i+1)) {
                        indices[m][GridType::HALF] = i;
                        break;
                    }
                }
            } else {
                for (int i = 0; i < mesh.getNumGrid(m, GridType::FULL)-1; ++i) {
                    if (x(m) >= mesh.getGridCoordComp(m, GridType::FULL, i) &&
                        x(m) <= mesh.getGridCoordComp(m, GridType::FULL, i+1)) {
                        indices[m][GridType::FULL] = i;
                        break;
                    }
                }
                if (domain.getAxisStartBndType(m) == POLE) {
                    if (x(m) < mesh.getGridCoordComp(m, GridType::HALF, 0)) {
                        indices[m][GridType::HALF] = -1;
                        continue;
                    } else if (x(m) > mesh.getGridCoordComp(m, GridType::HALF,
                                                            mesh.getNumGrid(m, GridType::HALF)-1)) {
                        indices[m][GridType::HALF] =
                            mesh.getNumGrid(m, GridType::HALF)-1;
                        continue;
                    }
                }
                int i1 = max(indices[m][GridType::FULL]-2, 0);
                int i2 = min(indices[m][GridType::FULL]+2,
                             mesh.getNumGrid(m, GridType::FULL)-1);
                for (int i = i1; i < i2; ++i) {
                    if (x(m) >= mesh.getGridCoordComp(m, GridType::HALF, i) &&
                        x(m) <= mesh.getGridCoordComp(m, GridType::HALF, i+1)) {
                        indices[m][GridType::HALF] = i;
                        break;
                    }
                }
            }
        } else {
            if (domain.getAxisStartBndType(m) == PERIODIC) {
                if (x(m) < mesh.getGridCoordComp(m, GridType::FULL,
                                                 indices[m][GridType::FULL])) {
                    for (int i = indices[m][GridType::FULL]-1; i >= 0; --i) {
                        // cout << setw(5) << i;
                        // cout << setw(10) << mesh.getGridCoord(m, GridType::FULL, i);
                        // cout << setw(10) << x(m);
                        // cout << setw(10) << mesh.getGridCoord(m, GridType::FULL, i+1) << endl;
                        if (x(m) >= mesh.getGridCoordComp(m, GridType::FULL, i) &&
                            x(m) <= mesh.getGridCoordComp(m, GridType::FULL, i+1)) {
                            indices[m][GridType::FULL] = i;
                            break;
                        }
                    }
                } else if (x(m) > mesh.getGridCoordComp(m, GridType::FULL,
                                                        indices[m][GridType::FULL]+1)) {
                    for (int i = indices[m][GridType::FULL]+1;
                         i < mesh.getNumGrid(m, GridType::FULL); ++i) {
                        if (x(m) >= mesh.getGridCoordComp(m, GridType::FULL, i) &&
                            x(m) <= mesh.getGridCoordComp(m, GridType::FULL, i+1)) {
                            indices[m][GridType::FULL] = i;
                            break;
                        }
                    }
                }
                for (int i = indices[m][GridType::FULL]-1;
                     i < indices[m][GridType::FULL]+1; ++i) {
                    if (x(m) >= mesh.getGridCoordComp(m, GridType::HALF, i) &&
                        x(m) <= mesh.getGridCoordComp(m, GridType::HALF, i+1)) {
                        indices[m][GridType::HALF] = i;
                        break;
                    }
                }
            } else {
                if (x(m) < mesh.getGridCoordComp(m, GridType::FULL,
                                                 indices[m][GridType::FULL])) {
                    for (int i = indices[m][GridType::FULL]-1; i >= 0; --i) {
                        // cout << setw(5) << i;
                        // cout << setw(10) << mesh.getGridCoord(m, GridType::FULL, i);
                        // cout << setw(10) << x(m);
                        // cout << setw(10) << mesh.getGridCoord(m, GridType::FULL, i+1) << endl;
                        if (x(m) >= mesh.getGridCoordComp(m, GridType::FULL, i) &&
                            x(m) <= mesh.getGridCoordComp(m, GridType::FULL, i+1)) {
                            indices[m][GridType::FULL] = i;
                            break;
                        }
                    }
                } else if (x(m) > mesh.getGridCoordComp(m, GridType::FULL,
                                                        indices[m][GridType::FULL]+1)) {
                    for (int i = indices[m][GridType::FULL]+1;
                         i < mesh.getNumGrid(m, GridType::FULL); ++i) {
                        if (x(m) >= mesh.getGridCoordComp(m, GridType::FULL, i) &&
                            x(m) <= mesh.getGridCoordComp(m, GridType::FULL, i+1)) {
                            indices[m][GridType::FULL] = i;
                            break;
                        }
                    }
                }
                if (domain.getAxisStartBndType(m) == POLE) {
                    if (x(m) < mesh.getGridCoordComp(m, GridType::HALF, 0)) {
                        indices[m][GridType::HALF] = -1;
                        continue;
                    } else if (x(m) > mesh.getGridCoordComp(m, GridType::HALF,
                                                            mesh.getNumGrid(m, GridType::HALF)-1)) {
                        indices[m][GridType::HALF] =
                            mesh.getNumGrid(m, GridType::HALF)-1;
                        continue;
                    }
                }
                int i1 = max(indices[m][GridType::FULL]-2, 0);
                int i2 = min(indices[m][GridType::FULL]+2,
                             mesh.getNumGrid(m, GridType::FULL)-1);
                for (int i = i1; i < i2; ++i) {
                    if (x(m) >= mesh.getGridCoordComp(m, GridType::HALF, i) &&
                        x(m) <= mesh.getGridCoordComp(m, GridType::HALF, i+1)) {
                        indices[m][GridType::HALF] = i;
                        break;
                    }
                }
            }
        }
#ifndef NDEBUG
        assert(indices[m][GridType::FULL] != UNDEFINED_MESH_INDEX);
        assert(indices[m][GridType::HALF] != UNDEFINED_MESH_INDEX);
#endif
    }
}

template <class MeshType, class CoordType>
int StructuredMeshIndex<MeshType, CoordType>::wrapIndex(const MeshType &mesh, int loc) const {
    if (this->numDim == 2) {
        switch (loc) {
            case Location::CENTER:
                return mesh.wrapIndex(indices[0][GridType::FULL], indices[1][GridType::FULL], loc);
            case Location::X_FACE:
                return mesh.wrapIndex(indices[0][GridType::HALF], indices[1][GridType::FULL], loc);
            case Location::Y_FACE:
                return mesh.wrapIndex(indices[0][GridType::FULL], indices[1][GridType::HALF], loc);
            case Location::XY_VERTEX:
                return mesh.wrapIndex(indices[0][GridType::HALF], indices[1][GridType::HALF], loc);
            default:
                REPORT_ERROR("Unsupported stagger location!");
        }
    } else if (this->numDim == 3) {
        switch (loc) {
            case Location::CENTER:
                return mesh.wrapIndex(indices[0][GridType::FULL], indices[1][GridType::FULL], indices[2][GridType::FULL], loc);
            case Location::X_FACE:
                return mesh.wrapIndex(indices[0][GridType::HALF], indices[1][GridType::FULL], indices[2][GridType::FULL], loc);
            case Location::Y_FACE:
                return mesh.wrapIndex(indices[0][GridType::FULL], indices[1][GridType::HALF], indices[2][GridType::FULL], loc);
            case Location::XY_VERTEX:
                return mesh.wrapIndex(indices[0][GridType::HALF], indices[1][GridType::HALF], indices[2][GridType::FULL], loc);
            case Location::Z_FACE:
                return mesh.wrapIndex(indices[0][GridType::FULL], indices[1][GridType::FULL], indices[2][GridType::HALF], loc);
            default:
                REPORT_ERROR("Unsupported stagger location!");
        }
    }
}

template <class MeshType, class CoordType>
void StructuredMeshIndex<MeshType, CoordType>::print() const {
    cout << "Center indices:";
    for (int m = 0; m < 3; ++m) {
        cout << setw(10) << indices[m][GridType::FULL];
    }
    cout << endl;
    cout << "Edge indices:  ";
    for (int m = 0; m < 3; ++m) {
        cout << setw(10) << indices[m][GridType::HALF];
    }
    cout << endl;
}

} // geomtk