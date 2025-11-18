#include "constructive/ConstructiveHeuristicLookahead.hpp"
#include "../include/Instance.hpp"
#include "../include/Solution.hpp"
#include <float.h>

using namespace std;
using namespace opthirrygated;

Solution ConstructiveHeuristicLookahead::execute() {
    Solution objSolution;
    vector<int> solution;
    vector<float> adfSol;

    float adi = inst.getCad()[0], adf = 0;
    size_t numDays = inst.getCicle().size();

    for (size_t day = 0; day < numDays; day++) {
        float bestScore = FLT_MAX;
        int bestPerc = -1;
        float bestAdf = adi;

        for (int perc : inst.getPerc()) {
            float currAdf = adi - inst.getEtc()[day] + inst.getPrec()[day] + inst.getLamp()[perc];

            if (currAdf >= inst.getLc()[day]) {
                float currCost = inst.getCost()[perc];
                float futureCost = simulateLookahead(day + 1, currAdf, lookaheadDepth - 1);
                float totalScore = currCost + futureCost;

                if (totalScore < bestScore) {
                    bestScore = totalScore;
                    bestPerc = perc;
                    bestAdf = currAdf;
                }
            }
        }

        if (bestPerc != -1) {
            solution.push_back(bestPerc);
            adf = bestAdf;
        } else {
            adf = adi; // fallback
        }

        adi = adf;
        adfSol.push_back(adf);
    }

    objSolution.setSolution(std::move(solution));
    objSolution.setAdfSolution(std::move(adfSol));

    return objSolution;
}

float ConstructiveHeuristicLookahead::simulateLookahead( size_t day, float adi, int depth) {
    if (day >= inst.getCicle().size() || depth == 0)
        return 0;

    float preAdf = adi - inst.getEtc()[day] + inst.getPrec()[day];

    float bestCost = FLT_MAX;

    if (preAdf >= inst.getCad()[day]) {
        bestCost = simulateLookahead(day + 1, preAdf, depth - 1);
    }

    for (int perc : inst.getPerc()) {
        float auxAdf = preAdf + inst.getLamp()[perc];
        float cost = inst.getCost()[perc];

        if (auxAdf >= inst.getLc()[day]) {
            float futureCost = simulateLookahead(day + 1, auxAdf, depth - 1);
            bestCost = std::min(bestCost, cost + futureCost);
        }
    }

    return bestCost;
}
