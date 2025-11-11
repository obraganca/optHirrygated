#ifndef I_METAHEURISTICA_HPP
#define I_METAHEURISTICA_HPP

#include "Solution.hpp"

using namespace std;
namespace opthirrygated{
    class IMetaheuristica{
    public:
        ~IMetaheuristica()=default;
        virtual Solution execute() = 0;
    };
}

#endif