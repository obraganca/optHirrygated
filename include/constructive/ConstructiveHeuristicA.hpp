#ifndef I_ABSTRACT_CONSTRUTIVE_HEURISTIC_HPP
#define I_ABSTRACT_CONSTRUTIVE_HEURISTIC_HPP

#include "Instance.hpp"
#include "Solution.hpp"
#include "interface/IConstructiveHeuristic.hpp"

using namespace std;

namespace opthirrygated{

    class AbstractConstructiveHeuristic : public IConstructiveHeuristic{
    private:
        Instance &inst;
    public:
        AbstractConstructiveHeuristic(Instance &instance):inst(instance){}
        virtual Solution execute() = 0;
    };
}

#endif