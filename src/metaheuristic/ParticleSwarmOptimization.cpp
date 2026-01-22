
#include "../include/metaheuristic/ParticleSwarmOptimization.hpp"

using namespace opthirrygated;



double ParticleSwarmOptimization::rand01() {
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(rng);
}


Solution ParticleSwarmOptimization::perturbSolution(const Solution& sol, double perturbationRate) {
    Solution perturbed = sol;
    int numDays = perturbed.getSolution().size();
    int numPerturbations = static_cast<int>(numDays * perturbationRate);

    std::uniform_int_distribution<int> dayDist(0, numDays - 1);
    std::uniform_int_distribution<int> lampDist(0, inst.getLamp().size() - 1);

    for (int i = 0; i < numPerturbations; i++) {
        int day = dayDist(rng);
        int newLamp = lampDist(rng);

        Solution candidate = perturbed;
        candidate.updateSolution(day, newLamp);

        float deltaAdf = inst.getLamp()[perturbed.getSolution()[day]] - inst.getLamp()[newLamp];
        if (measurer.isFeasible(candidate, day, deltaAdf)) {
            candidate.propagate(inst, day);
            candidate.constructCriticalLimitDelt(inst);
            candidate.setScore(measurer.evaluate(candidate));
            perturbed = candidate;
        }
    }

    return perturbed;
}

void ParticleSwarmOptimization::initializeParticles(Solution& solution) {
    particles.clear();
    particles.reserve(nParticles);

    // ensure the starting solution has a score
    solution.setScore(measurer.evaluate(solution));
    gBestPosition = solution;

    // First particle = original (with evaluated score)
    particles.emplace_back(solution, solution);

    // Other particles = perturbed versions (increasing perturbation)
    for (int i = 1; i < nParticles; ++i) {
        double rate = 0.1 + 0.15 * i; // more gradual
        Solution perturbed = perturbSolution(solution, rate);
        perturbed.setScore(measurer.evaluate(perturbed));
        particles.emplace_back(perturbed, perturbed);

        if (perturbed.getScore() < gBestPosition.getScore()) {
            gBestPosition = perturbed;
        }
    }

    // defensive: ensure gBestPosition has a valid score
    gBestPosition.setScore(measurer.evaluate(gBestPosition));
}

vector<pair<int,int>> ParticleSwarmOptimization::getVelocity(Solution pPosition, Solution bPosition) {
    vector<pair<int, int>> velocity;
    for (int i = 0; i < (int)pPosition.getSolution().size(); ++i) {
        int pVal = pPosition.getSolution()[i];
        int bVal = bPosition.getSolution()[i];
        if (pVal != bVal) {
            // push (targetLamp, index) — target is what we want to move TO
            velocity.emplace_back(bVal, i);
        }
    }
    return velocity;
}

Particle ParticleSwarmOptimization::updateVelocity(Particle &particle) {

    vector<pair<int,int>> newVelocity;

    vector<pair<int,int>> diffCognitive = getVelocity(particle.position, particle.bPosition);
    for (const auto& dC : diffCognitive) {
        if (rand01() < cCognitive)
            newVelocity.push_back(dC);
    }

    vector<pair<int,int>> diffSocial = getVelocity(particle.position, gBestPosition);;
    for (const auto& dS : diffSocial) {
        if (rand01() < cSocial)
            newVelocity.push_back(dS);
    }

    for (const auto& p : particle.velocity) {
        if (rand01() < inertia)
            newVelocity.push_back(p);
    }
    particle.velocity = move(newVelocity);
    return particle;

}

Particle ParticleSwarmOptimization::updatePosition(Particle &particle) {
    const int maxMovesToApply = std::max(1, (int)particle.velocity.size());
    int applied = 0;

    for (const auto &v : particle.velocity) {
        if (applied >= maxMovesToApply) break;

        int targetLamp = v.first;
        int index = v.second;

        // já está igual → ignora
        if (particle.position.getSolution()[index] == targetLamp)
            continue;

        Solution cand = particle.position;
        cand.updateSolution(index, targetLamp);

        int oldLamp = particle.position.getSolution()[index];

        // deltaAdf deve ser OLD - NEW (igual perturbSolution)
        float deltaAdf = inst.getLamp()[oldLamp] - inst.getLamp()[targetLamp];

        if (measurer.isFeasible(cand, index, deltaAdf)) {
            cand.propagate(inst, index);
            cand.constructCriticalLimitDelt(inst);
            cand.setScore(measurer.evaluate(cand));

            particle.position = std::move(cand);
            applied++;
        }
    }

    // atualiza pBest
    if (particle.position.getScore() < particle.bPosition.getScore()) {
        particle.bPosition = particle.position;
    }

    return particle;
}

Solution ParticleSwarmOptimization::execute(Solution &solution) {
    initializeParticles(solution);

    for (int iter = 0; iter < maxIterations; iter++) {
        // Use referência para modificar as partículas originais
        for (Particle& particle : particles) {
            updateVelocity(particle);
            updatePosition(particle);


            if (particle.position.getScore() < gBestPosition.getScore()) {
                gBestPosition = particle.position;
                cout << "Iter " << iter << ": Nova melhor solução = "
                     << gBestPosition.getScore() << endl;

                particle.noImprovement = 0;
            }else {
                particle.noImprovement++;
                if (particle.noImprovement >= maxAttemptsNoImprovement) {
                    particle.position = perturbSolution(
                        gBestPosition,  // ou particle.bPosition
                        0.2
                    );
                    particle.velocity.clear();
                    particle.noImprovement = 0;
                }
            }
        }
    }

    return gBestPosition;
}