/*
    nukesolve.cpp  -  Don Cross <cosinekitty@gmail.com>

    https://github.com/cosinekitty/sapphire
*/

#include <random>
#include "nucleus_engine.hpp"

static void Print(const Sapphire::NucleusEngine& engine);
static int WriteHeaderFile(const Sapphire::NucleusEngine& engine, const char *outHeaderFileName);
static int SolveMinimumEnergy(Sapphire::NucleusEngine& engine);

int main()
{
    using namespace Sapphire;

    const std::size_t NUM_PARTICLES = 5;
    NucleusEngine engine{NUM_PARTICLES};

    std::mt19937 rand{0x7c3aaf29u};     // seed was chosen using rolls of physical hexadecimal dice

    const double mean = 0.0;
    const double dev = 1.0;
    std::normal_distribution norm{mean, dev};

    // The first particle, P[0], will be held fixed at the origin.
    // for now, skip it and initialize 1..(N-1) using random positions.
    // Velocities will all start at zero and strong friction will be used
    // to overdamp toward convergence.

    for (std::size_t i = 1; i < NUM_PARTICLES; ++i)
    {
        Particle& p = engine.particle(i);
        p.pos[0] = norm(rand);
        p.pos[1] = norm(rand);
        p.pos[2] = norm(rand);
        p.pos[3] = 0.0f;

        p.vel = PhysicsVector::zero();
    }

    // Run the simulation until convergence.
    if (SolveMinimumEnergy(engine)) return 1;

    Print(engine);
    if (WriteHeaderFile(engine, "../src/nucleus_init.hpp")) return 1;
    return 0;
}


static void Print(const Sapphire::NucleusEngine& engine)
{
    using namespace Sapphire;
    const int n = static_cast<int>(engine.numParticles());
    for (int i = 0; i < n; ++i)
    {
        const Particle& p = engine.particle(i);
        printf("i=%u rx=%0.16lg ry=%0.16lg rz=%0.16lg vx=%0.16lg vy=%0.16lg vz=%0.16lg\n",
            static_cast<unsigned>(i),
            p.pos[0], p.pos[1], p.pos[2],
            p.vel[0], p.vel[1], p.vel[2]
        );
    }
}


static int WriteHeaderFile(const Sapphire::NucleusEngine& engine, const char *outHeaderFileName)
{
    using namespace Sapphire;

    const int n = static_cast<int>(engine.numParticles());

    FILE *outfile = fopen(outHeaderFileName, "wt");
    if (outfile == nullptr)
    {
        printf("ERROR(nukesolve): Cannot write to header file [%s]\n", outHeaderFileName);
        return 1;
    }

    fprintf(outfile, "// DO NOT EDIT - THIS CODE IS AUTO-GENERATED BY: %s\n", __FILE__);
    fprintf(outfile, "#pragma once\n");
    fprintf(outfile, "#include \"nucleus_engine.hpp\"\n");
    fprintf(outfile, "namespace Sapphire\n");
    fprintf(outfile, "{\n");
    fprintf(outfile, "    inline int SetMinimumEnergy(NucleusEngine& engine)\n");
    fprintf(outfile, "    {\n");
    fprintf(outfile, "        const int n = static_cast<int>(engine.numParticles());\n");
    fprintf(outfile, "        if (n != %d) return 777;\n", n);
    for (int i = 0; i < n; ++i)
    {
        fprintf(outfile, "\n");
        const Particle& p = engine.particle(i);
        fprintf(outfile, "        Particle& p%d = engine.particle(%d);\n", i, i);
        fprintf(outfile, "        p%d.vel = PhysicsVector::zero();\n", i);
        for (int k = 0; k < 4; ++k)
            fprintf(outfile, "        p%d.pos[%d] = %0.16lg;\n", i, k, p.pos[k]);
    }
    fprintf(outfile, "\n");
    fprintf(outfile, "        return 0;\n");
    fprintf(outfile, "    }\n");
    fprintf(outfile, "}\n");
    fclose(outfile);
    printf("nukesolve: Wrote header file [%s]\n", outHeaderFileName);
    return 0;
}


static int SolveMinimumEnergy(Sapphire::NucleusEngine& engine)
{
    using namespace Sapphire;

    const int n = static_cast<int>(engine.numParticles());
    int iter = 0;
    for(;;)
    {
        ++iter;
        if (iter > 1000000)
        {
            printf("nukesolve: EXCESSIVE ITERATION\n");
            return 1;
        }

        // Always force the "input" particle, the one at index zero,
        // to be fixed at the origin.
        Particle &input = engine.particle(0);
        input.pos = input.vel = PhysicsVector::zero();

        // Update the simulation.
        engine.update();

        // Fix the input particle AGAIN!
        input.pos = input.vel = PhysicsVector::zero();

        // Break out of the loop as soon as we believe we have converged.
        // We do this when all the particles are moving very slowly.
        // We are quite happy with a tiny amount of residual movement for two reasons:
        // (1) The code runs a little faster.
        // (2) It leaves a tiny amount of quiet impulse to the system, which is interesing.
        double score = 0.0;
        for (int i = 1; i < n; ++i)
        {
            double magSquared = Dot(input.vel, input.vel);
            score += magSquared;
        }
        score = std::sqrt(score);   // calculate RMS
        printf("nukesolve: iter=%d, score=%lg\n", iter, score);
        if (score < 1.0e-16)
            break;
    }

    printf("nukesolve: Solved local-minimum-energy state for %d particles.\n", n);
    return 0;
}