#include "gtest/gtest.h"
#include "Domain_test.h"
#include "SphereDomain_test.h"
#include "Mesh_test.h"
#include "StructuredMesh_test.h"
#include "StructuredMeshIndex_test.h"
#include "RLLMesh_test.h"
#include "RLLMeshIndex_test.h"
#include "RLLScalarField_test.h"
#include "RLLVectorField_test.h"
#include "RLLVelocityField_test.h"
#include "StructuredRegrid_test.h"
#include "RLLRegrid_test.h"

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
