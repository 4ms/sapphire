#pragma once
#include "mesh_physics.hpp"

namespace Sapphire
{
    // This class custom-optimizes the force calculator for the particular
    // mesh layout that Elastika uses.

    class ElastikaMesh : public PhysicsMesh
    {
    public:
        ElastikaMesh();
        static MeshAudioParameters getAudioParameters();

    protected:
        void Dampen(BallList& blist, float dt, float halflife) override;
        void CalcForces(const BallList& blist, PhysicsVectorList& forceList) override;
    };
}
