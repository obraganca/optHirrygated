#ifndef SOLUTION_HPP
#define SOLUTION_HPP

#include <vector>
#include <iostream>

using namespace std;

namespace opthirrygated {

    class Solution {
    private:
        vector<vector<int>> solutions;
        vector<vector<float>> adfSolutions;
    public:
        const vector<vector<int>>& getSolutions() const { return solutions; }
        const vector<vector<float>>& getAdfSolutions() const { return adfSolutions; }

        void setSolutions(const vector<vector<int>>& newSolutions) { solutions = newSolutions; }
        void setAdfSolutions(const vector<vector<float>>& newAdfSolutions) { adfSolutions = newAdfSolutions; }

        void addPivoSolution(const vector<int>& sol) { solutions.push_back(sol); }
        void addPivoAdfSolution(const vector<float>& adf) { adfSolutions.push_back(adf); }

        vector<int> getPivoSolution(int pivoIndex) const { return solutions.at(pivoIndex); }
        vector<float> getPivoAdfSolution(int pivoIndex) const { return adfSolutions.at(pivoIndex); }

        void updateSolution(int pivoIndex, int elemIndex, int val) {
            if (pivoIndex < (int)solutions.size() && elemIndex < (int)solutions[pivoIndex].size())
                solutions[pivoIndex][elemIndex] = val;
        }

        void updateAdfSolution(int pivoIndex, int elemIndex, float val) {
            if (pivoIndex < (int)adfSolutions.size() && elemIndex < (int)adfSolutions[pivoIndex].size())
                adfSolutions[pivoIndex][elemIndex] = val;
        }

        void show() const {
            for (size_t i = 0; i < solutions.size(); i++) {
                cout << "=== Pivo " << i+1 << " ===" << endl;
                cout << "Solution: ";
                for (auto val : solutions[i]) cout << val << " ";
                cout<<endl;
            }
        }
    };

}

#endif
