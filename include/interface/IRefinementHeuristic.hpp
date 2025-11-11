#ifndef I_REFINEMENT_HEURISTIC_HPP
#define I_REFINEMENT_HEURISTIC_HPP

#include "Solution.hpp"
namespace opthirrygated{
    class IRefinementHeuristic{
        virtual ~IRefinementHeuristic()=default;
        virtual Solution execute() = 0;
    };
}

#endif