#if 0
#include "../include/ConstructiveHeuristic.hpp"
#include "../include/Solution.hpp"
#include "../include/Measurer.hpp"
#include <float.h>
#include <unordered_map>
#include <algorithm>

using namespace std;
using namespace opthirrygated;

ConstructiveHeuristic::ConstructiveHeuristic(opthirrygated::Instance &instance) : inst(instance) {}

Solution ConstructiveHeuristic::executeA() {
  Solution objSolution;
  vector<int> solution;
  vector<float> adfSol;

  float adi = inst.getCad()[0], adf = 0;

  size_t numDays = inst.getCicle().size();
  for (size_t day = 0; day < numDays; day++) {
    float bestPrice = FLT_MAX;
    int bestPerc = -1;

    for (int perc : inst.getPerc()) {
      float auxAdf = adi - inst.getEtc()[day] + inst.getPrec()[day] + inst.getLamp()[perc];
      float percCost = inst.getCost()[perc];

      if (percCost < bestPrice && auxAdf >= inst.getLc()[day]) {
        bestPrice = percCost;
        bestPerc = perc;
        adf = auxAdf;
      }
    }

    if (bestPerc != -1) {
      solution.push_back(bestPerc);
    } else {
      adf = adi;
    }

    adi = adf;
    adfSol.push_back(adf);
  }

  objSolution.setSolution(std::move(solution));
  objSolution.setAdfSolution(std::move(adfSol));
  return objSolution;
}

Solution ConstructiveHeuristic::executeB() {
    Solution objSolution;
    vector<int> solution;
    vector<float> adfSolution;

  float adi = inst.getCad()[0], adf = 0;

  size_t numDays = inst.getCicle().size();
  for (size_t day = 0; day < numDays; day++) {
    float bestPrice = FLT_MAX;
    int bestPerc = 10;

  	float preAdf = adi - inst.getEtc()[day] + inst.getPrec()[day];

  	if (preAdf >= inst.getCad()[day]) {
      solution.push_back(bestPerc);
      adi = preAdf;
      continue;
    }
	bestPerc=-1;
	float auxAdf =0;
    for (int perc : inst.getPerc()) {
      auxAdf = preAdf + inst.getLamp()[perc];
      float percCost = inst.getCost()[perc];

      if (percCost < bestPrice && auxAdf >= inst.getCad()[day]) {
        bestPrice = percCost;
        bestPerc = perc;
        adf = auxAdf;
      }
    }

    if(adf<inst.getLc()[day]){
      cout<<auxAdf << "ERROR"<<endl;
    }

    if (bestPerc != -1) {
      solution.push_back(bestPerc);
    } else {
      adf = adi;
    }

    adi = adf;
    adfSolution.push_back(adf);
  }

  objSolution.setSolution(std::move(solution));
  objSolution.setAdfSolution(std::move(adfSolution));
  return objSolution;
}

Solution ConstructiveHeuristic::executeC() {
 	Solution objSolution;
	vector<int> solution;
    vector<float> adfSol;

  	float adi = inst.getCad()[0], adf = 0;

  size_t numDays = inst.getCicle().size();
  for (size_t day = 0; day < numDays; day++) {
    float bestPrice = FLT_MAX;
    int bestPerc = 10;

	float preAdf = adi - inst.getEtc()[day] + inst.getPrec()[day];

	bestPerc=-1;
    for (int perc : inst.getPerc()) {
      float auxAdf = preAdf + inst.getLamp()[perc];
      float percCost = inst.getCost()[perc];

      if (auxAdf <= inst.getCad()[day] && auxAdf >= inst.getLc()[day]) {
        bestPrice = percCost;
        bestPerc = perc;
        adf = auxAdf;
      }
    }

	if(adf<inst.getLc()[day]){
		cout<<"ERROR"<<endl;
	}

    if (bestPerc != -1) {
      solution.push_back(bestPerc);
    } else {
      adf = adi;
    }

    adi = adf;
    adfSol.push_back(adf);
  }

  objSolution.setSolution(std::move(solution));
  objSolution.setAdfSolution(std::move(adfSol));
  return objSolution;
}

Solution ConstructiveHeuristic::executeBackward() {
    Solution objSolution;
    vector<int> solution;
    vector<float> adfSol;

    float adi=0 , adf = 0;

    int numDays = inst.getCicle().size();
    for (int day = numDays-1; day >= 0; day--) {
        float bestScore = FLT_MAX;
        int bestPerc = -1;

        if(day == 0){
            adi = inst.getCad()[0];
        }else{
            adi = inst.getPrec()[day-1] - inst.getEtc()[day-1];
        }

        for (int perc : inst.getPerc()) {
            float auxAdf = adi - inst.getEtc()[day] + inst.getPrec()[day] + inst.getLamp()[perc];
            float percCost = inst.getCost()[perc];

            if (percCost < bestScore && auxAdf >= inst.getLc()[day]) {
                bestScore = percCost;
                bestPerc = perc;
                adf = auxAdf;
            }
        }

        if(bestPerc == -1){
            bestPerc = inst.getPerc()[inst.getPerc().size()-2];
            adf = adi - inst.getEtc()[day] + inst.getPrec()[day] + inst.getLamp()[bestPerc];
        }

        solution.push_back(bestPerc);
        adi = adf;
        adfSol.push_back(adf);
    }
    reverse(solution.begin(), solution.end());
    reverse(adfSol.begin(), adfSol.end());

    normalizerAdf(solution, adfSol);

    objSolution.setSolution(move(solution));
    objSolution.setAdfSolution(move(adfSol));
    return objSolution;
}

void ConstructiveHeuristic::normalizerAdf(
        vector<int> &solution,
        vector<float> &solutionAdf
) {

    vector<int> auxSolution = solution;
    vector<float> auxSolutionAdf = solutionAdf;

    for(int day =0; day< solution.size(); day++){

        float adi = day == 0 ? inst.getCad()[0] : auxSolutionAdf[day-1];
        float auxAdf = adi - inst.getEtc()[day] + inst.getPrec()[day] + inst.getLamp()[auxSolution[day]];
        auxSolutionAdf[day] = auxAdf;
    }
    solution    = move(auxSolution);
    solutionAdf = move(auxSolutionAdf);
}

void ConstructiveHeuristic::recalculatePerc(
        vector<int> &solution,
        vector<float> &solutionAdf,
        int totalDays
) {
    int k = (int)solution.size();
    for (int idx = k-2; idx >= 0; --idx) {
        float bestScore = FLT_MAX;
        int   bestPerc  = solution[idx];
        float bestAdf   = solutionAdf[idx];

        float adi = solutionAdf[idx + 1];

        int day = (totalDays - 1) - idx;

        for (int perc : inst.getPerc()) {
            float auxAdf = adi
                           - inst.getEtc()[day]
                           + inst.getPrec()[day]
                           + inst.getLamp()[perc];
            float cost   = inst.getCost()[perc];
            if (auxAdf >= inst.getLc()[day] && cost < bestScore) {
                bestScore = cost;
                bestPerc  = perc;
                bestAdf   = auxAdf;
            }
        }

        solution[idx]      = bestPerc;
        solutionAdf[idx]   = bestAdf;
    }
}


bool ConstructiveHeuristic::isFeasible(
        vector<int> &solution,
        vector<float> &solutionAdf,
        int d
) const {
    return true;
    for (int day = d; day >= 0; day++) {
        float adi = day == 0 && inst.getCicle().size()-1 == d ? inst.getCad()[0] : solutionAdf[day+1];
        float auxAdf = adi - inst.getEtc()[day] + inst.getPrec()[day] + inst.getLamp()[solution[day]];
        float e = 0.0001;

        if (auxAdf + e < inst.getLc()[day]) {
            return false;
        }

        solutionAdf[day] = auxAdf;
    }
    return true;
}



Solution ConstructiveHeuristic::executeLookahead(int lookaheadDepth) {
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

float ConstructiveHeuristic::simulateLookahead( size_t day, float adi, int depth) {
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
#endif