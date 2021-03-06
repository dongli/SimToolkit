#ifndef __GEOMTK_RLLVelocityField_test__
#define __GEOMTK_RLLVelocityField_test__

#include "RLLVelocityField.h"

using namespace geomtk;

class RLLVelocityFieldTest : public ::testing::Test {
protected:
    const int FULL = RLLStagger::GridType::FULL;
    const int HALF = RLLStagger::GridType::HALF;
    const int CENTER = RLLStagger::Location::CENTER;

    SphereDomain *domain;
    RLLMesh *mesh;
    RLLVelocityField v;
    TimeLevelIndex<2> timeIdx;

    virtual void SetUp() {
        domain = new SphereDomain(2);
        mesh = new RLLMesh(*domain);

        mesh->init(5, 5);
        v.create(*mesh, true);
    }

    virtual void TearDown() {
        delete mesh;
        delete domain;
    }
};

TEST_F(RLLVelocityFieldTest, Basics) {
    ASSERT_EQ(mesh, &v.mesh());
    ASSERT_EQ(2, v.v.size());
    ASSERT_EQ("div", v.div.name());
    ASSERT_EQ("vor_xy", v.vor[0].name());
}

TEST_F(RLLVelocityFieldTest, RingVelocity) {
    for (auto j = mesh->js(FULL); j <= mesh->je(FULL); ++j) {
        for (auto i = mesh->is(HALF); i <= mesh->ie(HALF); ++i) {
            v(0)(timeIdx, i, j) = 5.0;
        }
    }
    for (auto j = mesh->js(HALF); j <= mesh->je(HALF); ++j) {
        for (auto i = mesh->is(FULL); i <= mesh->ie(FULL); ++i) {
            v(1)(timeIdx, i, j) = 5.0;
        }
    }
    v.applyBndCond(timeIdx);
    for (uword m = 0; m < domain->numDim(); ++m) {
        for (auto l = 0; l < 2; ++l) {
            ASSERT_LE(fabs(v.rings[l].originalData(m, timeIdx, mesh->is(FULL)-1)-
                           v.rings[l].originalData(m, timeIdx, mesh->ie(FULL))), 1.0e-14);
            ASSERT_LE(fabs(v.rings[l].originalData(m, timeIdx, mesh->ie(FULL)+1)-
                           v.rings[l].originalData(m, timeIdx, mesh->is(FULL))), 1.0e-14);
            ASSERT_LE(fabs(v.rings[l].transformedData(m, timeIdx, mesh->is(FULL)-1)-
                           v.rings[l].transformedData(m, timeIdx, mesh->ie(FULL))), 1.0e-14);
            ASSERT_LE(fabs(v.rings[l].transformedData(m, timeIdx, mesh->ie(FULL)+1)-
                           v.rings[l].transformedData(m, timeIdx, mesh->is(FULL))), 1.0e-14);
        }
    }
    // Check original velocity on the rings.
    for (auto l = 0; l < 2; ++l) {
        auto j = l == 0 ? mesh->js(FULL)+1 : mesh->je(FULL)-1;
        for (auto i = mesh->is(FULL); i <= mesh->ie(FULL); ++i) {
            std::cout << "l = " << l << ", j = " << j << " i = " << i << std::endl;
            ASSERT_EQ(v.rings[l].originalData(0, timeIdx, i),
                      (v(0)(timeIdx, i, j)+v(0)(timeIdx, i-1, j))*0.5);
            ASSERT_EQ(v.rings[l].originalData(1, timeIdx, i),
                      (v(1)(timeIdx, i, j)+v(1)(timeIdx, i, j-1))*0.5);
        }
    }
    // Check transformed velocity on the rings.
    for (auto l = 0; l < 2; ++l) {
        auto j = l == 0 ? mesh->js(FULL)+1 : mesh->je(FULL)-1;
        for (auto i = mesh->is(FULL); i <= mesh->ie(FULL); ++i) {
            const SphereCoord &x = mesh->gridCoord(CENTER, mesh->wrapIndex(CENTER, i, j));
            SphereVelocity u(2);
            u = v.rings[l].vr(i, 0)->level(timeIdx);
            u.transformFromPS(x);
            ASSERT_GT(1.0e-15, fabs(v.rings[l].originalData(0, timeIdx, i)-u(0)));
            ASSERT_GT(1.0e-15, fabs(v.rings[l].originalData(1, timeIdx, i)-u(1)));
        }
    }
}

#endif // __GEOMTK_RLLVelocityField_test__
