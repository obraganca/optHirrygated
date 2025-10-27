#ifndef CONSTRUCTIVE_HEURISTIC_HPP
#define CONSTRUCTIVE_HEURISTIC_HPP

#include "Instance.hpp"
#include "Solution.hpp"

using namespace std;

namespace opthirrygated{
    // Adicionar no ConstructiveHeuristic.hpp

    class ConstructiveHeuristic {
    private:
        opthirrygated::Instance &inst;
    public:
        ConstructiveHeuristic(opthirrygated::Instance &instance);

        Solution executeA();
        Solution executeB();
        Solution executeC();
        Solution executeLookahead(int lookaheadDepth);

        // Nova heurística backward
        Solution executeBackward();
        void normalizerAdf(vector<int> &solution, vector<float> &solutionAdf);

    private:
        float simulateLookahead(size_t day, float adi, int depth);

        bool isFeasible(vector<int> &solution, vector<float> &solutionAdf, int d) const;

        void recalculatePerc(vector<int> &solution, vector<float> &solutionAdf, int d);
    };
}

#endif