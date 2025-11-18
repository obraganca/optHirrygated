#include "../include/RefinementHeuristic.hpp"
#include "../include/ConstructiveHeuristic.hpp"
#include "../include/Measurer.hpp"
#include "../include/interface/INeighborhood.hpp"
#include <iostream>
#include <algorithm>    // for std::swap
#include <cmath>
#include <random>
#include <limits>
#include <iomanip>

#define LEN 7
namespace opthirrygated {

    std::mt19937_64 rng(std::random_device{}());
    std::uniform_real_distribution<double> unif(0.0, 1.0);

    Solution RefinementHeuristic::randomizedPertubation(Solution solution, int n) {
        int totalSize = solution.getSolution().size();
        if (totalSize == 0) return solution;

        std::uniform_int_distribution<int> dayDist(0, totalSize - 1);
        std::uniform_int_distribution<int> percDist(0, inst.getPerc().size() - 1);

        int minDay = totalSize + 1;
        int startDay = dayDist(rng);

        Measurer measurer(inst);

        for (int d = startDay; d < std::min(n, totalSize); d++) {
            Solution candidate = solution;

            float perc = inst.getPerc()[percDist(rng)];
            minDay = std::min(minDay, d);

            candidate.updateSolution(d, perc);

            if (perc == candidate.getSolution()[d]) continue;

            propagate(candidate, d, 0);
            if (!isFeasible(candidate, d)) continue;

            solution = candidate;
        }

        return solution;
    }

    Solution RefinementHeuristic::executeB(Solution &solution) {
        int totalSize = solution.getSolution().size();
        if (totalSize < LEN) return solution; // Safety check

        int maxBlocks = totalSize - LEN + 1; // Maximum number of overlapping blocks
        Measurer measurer(inst);
        int k = 1;

        while (k <= maxBlocks) {
            int startIdx = k - 1; // Starts at 0, then 1, then 2, etc.
            int endIdx = startIdx + LEN - 1;

            Solution aux = findBestNeighbor(solution, startIdx, endIdx);

            if (measurer.evaluate(aux) < measurer.evaluate(solution)) {
                solution = aux;
                k = 1; // Reset to first block when improvement found
            } else {
                ++k;
            }
        }
        return solution;
    }

    Solution RefinementHeuristic::findBestNeighbor(Solution &solution, int startIdx, int endIdx) {
        Solution bestSolution = solution;
        Measurer measurer(inst);

        int solutionSize = solution.getSolution().size();

        // Enhanced boundary check
        if (startIdx < 0 || endIdx >= solutionSize || startIdx > endIdx || solutionSize == 0) {
            return bestSolution;
        }

        std::vector<int> rangeValues;
        for (int i = startIdx; i <= endIdx; i++) {
            rangeValues.push_back(solution.getSolution()[i]);
        }

        if (rangeValues.empty()) return bestSolution; // Safety check

        std::sort(rangeValues.begin(), rangeValues.end());

        float bestCost = measurer.evaluate(solution);

        do {
            Solution candidate = solution;

            // Ensure we don't go out of bounds
            for (int i = 0; i < static_cast<int>(rangeValues.size()) && (startIdx + i) < solutionSize; i++) {
                candidate.getSolution()[startIdx + i] = rangeValues[i];
            }

            if (measurer.validationRange(candidate, startIdx, endIdx)) {
                float candidateCost = measurer.evaluate(candidate);

                if (candidateCost < bestCost) {
                    bestSolution = candidate;
                    bestCost = candidateCost;
                }
            }

        } while (std::next_permutation(rangeValues.begin(), rangeValues.end()));

        return bestSolution;
    }










    Solution RefinementHeuristic::executeSA(Solution &solution, double T, double T_min, double alpha, int iterPerT){
        Measurer measurer(inst);
        float C = measurer.evaluate(solution);
        double T_initial = T;

        int solutionSize = solution.getAdfSolutions().size();
        if (solutionSize == 0) return solution; // Safety check

        std::uniform_int_distribution<int> dayDist(0, solutionSize - 1);

        while (T > T_min) {
            for (int iter = 0; iter < iterPerT; ++iter) {
                int d = dayDist(rng);

                Solution candidate = solution;

                double tempRatio = T / T_initial;
                int maxPerturbation = std::max(1, static_cast<int>(inst.getPerc().size() * 0.1));
                int perturbationLevel = std::max(1, static_cast<int>(maxPerturbation * tempRatio));

                std::uniform_int_distribution<int> stepDist(-perturbationLevel, perturbationLevel);
                int step = stepDist(rng);

                int currentIdx = candidate.getSolution()[d];
                int newIdx = std::max(0, std::min(currentIdx + step, static_cast<int>(inst.getPerc().size()) - 1));

                if (newIdx == currentIdx) continue;

                float oldAdf = candidate.getAdfSolutions()[d];
                candidate.updateSolution(d, newIdx);
                float newAdf = candidate.getAdfSolutions()[d];
                float deltaAdf = newAdf - oldAdf;
                propagate(candidate, d, deltaAdf);
                if (!isFeasible(candidate, d)) continue;

                float C2 = measurer.evaluate(candidate);
                double delta = C2 - C;

                if (delta < 0 || unif(rng) < std::exp(-delta / T)) {
                    solution = std::move(candidate);
                    C = C2;
                }
            }
            T *= alpha;
            //std::cout << "T=" << T << ", best cost=" << C << std::endl;
        }

        return solution;
    }

    bool RefinementHeuristic::isFeasible(const Solution &solution, int d) const {
        auto adfSolutions = solution.getAdfSolutions();
        const auto& solutionVec = solution.getSolution();

        // Safety checks
        if (d < 0 || adfSolutions.empty() || solutionVec.empty() || adfSolutions.size() < d) {
            return false;
        }

        float adi = adfSolutions[d-1], adf;
        if (d == 0) {
            if (inst.getCad().empty()) return false; // Safety check
            adi = inst.getCad()[0];
        }

        // Fixed loop condition - should be < not <=
        for (int day = d; day < static_cast<int>(adfSolutions.size()); day++) {
            // Safety checks for array access
            if (day >= static_cast<int>(inst.getEtc().size()) ||
                day >= static_cast<int>(inst.getPrec().size()) ||
                day >= static_cast<int>(inst.getLc().size()) ||
                day >= static_cast<int>(solutionVec.size())) {
                return false;
            }

            if (solutionVec[day] >= static_cast<int>(inst.getLamp().size())) {
                return false;
            }

            float auxAdf = adi - inst.getEtc()[day] + inst.getPrec()[day] + inst.getLamp()[solutionVec[day]];
            float e = 0.0001;

            if (auxAdf + e < inst.getLc()[day]) {
                return false;
            }

            // Note: This modifies a copy, not the original
            adfSolutions[day] = auxAdf;
            adi = auxAdf;
        }
        return true;
    }

    void RefinementHeuristic::propagate(Solution &solution, int d, float val) {
        std::vector<float> adfSolutions = solution.getAdfSolutions(); // Get copy, not reference

        if (d < 0 || d >= static_cast<int>(adfSolutions.size())) {
            return; // Safety check
        }

        for (int day = d; day < static_cast<int>(adfSolutions.size()); ++day) {
            float adi = day == 0 ? inst.getCad()[0] : solution.getAdfSolutions()[day-1];
            float auxAdf = adi - inst.getEtc()[day] + inst.getPrec()[day] + inst.getLamp()[solution.getSolution()[day]];
            solution.updateAdfSolution(day, auxAdf);
        }
    }

}