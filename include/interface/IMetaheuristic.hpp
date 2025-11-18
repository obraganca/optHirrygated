#ifndef I_METAHEURISTIC_HPP
#define I_METAHEURISTIC_HPP

#include "Solution.hpp"

using namespace std;
namespace opthirrygated{
    class IMetaheuristic{
    public:
        virtual ~IMetaheuristic()=default;
        virtual Solution execute(Solution& solution) = 0;
    };
}

#endif