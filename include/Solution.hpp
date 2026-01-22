#ifndef SOLUTION_HPP
#define SOLUTION_HPP

#include <vector>
#include "Instance.hpp"

using namespace std;
namespace opthirrygated {
    class Solution {
    private:
        vector<int> solution;
        vector<float> adfSolutions;
        vector<float> criticalLimit;
        float score;

    public:
        void setSolution(const vector<int>& newSolution) { solution = newSolution; }
        vector<int> getSolution() const { return solution; }

        void setScore(float newScore) { score = newScore; }
        float getScore() const { return score; }



        void updateSolution(int index, int val) { solution[index]=val; }

        void setAdfSolution(const vector<float>& newAdfSolution) { adfSolutions = newAdfSolution; }
        vector<float> getAdfSolutions() const { return adfSolutions; }


        void updateAdfSolution(int index, float val) { adfSolutions[index]=val; }


        void setCriticalSolution(const vector<float>& newCriticalLimit) { criticalLimit = newCriticalLimit; }
        vector<float> getCriticalSolutions() const { return criticalLimit; }


        void updateCriticalSolution(int index, float val) { criticalLimit[index]=val; }

        void propagate(const opthirrygated::Instance &inst, int d);

        std::vector<int> getHighIrrigationDays();

        void constructCriticalLimitDelt(const Instance inst);
    };
}


#endif