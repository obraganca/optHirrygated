#ifndef OPTHIRRYGATED_PARTICLESWARMOPTIMIZATION_HPP
#define OPTHIRRYGATED_PARTICLESWARMOPTIMIZATION_HPP

#include "constructive/AbstractConstructiveHeuristic.hpp"
#include "metaheuristic/AbstractMetaheuristic.hpp"
#include "refinement/RefinementHeuristicVND.hpp"
#include "interface/ILocalSearch.hpp"
#include "interface/INeighborhood.hpp"
#include "../include/Instance.hpp"
#include "../include/Solution.hpp"
#include "Measurer.hpp"

#include <random>
#include <memory>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cfloat>

namespace opthirrygated {

    class Particle {
    public:
        Solution position;
        Solution bPosition;
        int noImprovement;

        vector<pair<int, int>> velocity;

        Particle(Solution _position, Solution _bPosition): position(_position), bPosition(_bPosition), noImprovement(0) {
            this->velocity = vector<pair<int, int>>();
        };
    };
    class ParticleSwarmOptimization: AbstractMetaheuristic{
        int maxIterations = 1000;
        int nParticles = 50;
        float inertia = 0.6;
        float cCognitive = 0.5;
        float cSocial = 0.4;
        int maxAttemptsNoImprovement = 6;
        vector<Particle> particles;
        Solution gBestPosition;
        mt19937_64 rng;

        Solution perturbSolution(const Solution& sol, double perturbationRate = 0.3);

    public:
        ParticleSwarmOptimization(Instance &inst, Measurer &measurer):
            AbstractMetaheuristic(inst, measurer) {};
        void initializeParticles(Solution& solution);
        Solution execute(Solution &solution) override;
        Particle updateVelocity(Particle &particle);
        Particle updatePosition(Particle &particle);
        vector<pair<int,int>> getVelocity(Solution pPosition, Solution bPosition);
        double rand01();
    };

} // namespace opthirrygated

#endif // OPTHIRRYGATED_PARTICLESWARMOPTIMIZATION_HPP