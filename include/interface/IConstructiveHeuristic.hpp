#ifndef I_CONSTRUCTIVE_HEURISTIC_HPP
#define I_CONSTRUCTIVE_HEURISTIC_HPP

#include "Solution.hpp"

#include <limits>
namespace opthirrygated{
    class IConstructiveHeuristic{
    public:
        virtual ~IConstructiveHeuristic()=default;
        virtual Solution execute() = 0;
    };
}

#endif