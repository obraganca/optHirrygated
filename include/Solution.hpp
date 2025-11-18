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
    public:
        void setSolution(const vector<int>& newSolution) { solution = newSolution; }
        vector<int> getSolution() const { return solution; }


        void updateSolution(int index, int val) { solution[index]=val; }

        void setAdfSolution(const vector<float>& newAdfSolution) { adfSolutions = newAdfSolution; }
        vector<float> getAdfSolutions() const { return adfSolutions; }


        void updateAdfSolution(int index, float val) { adfSolutions[index]=val; }

        void propagate(const opthirrygated::Instance &inst, int d);

        std::vector<int> getHighIrrigationDays();
    };
}


#endif