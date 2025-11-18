#ifndef ABSTRACT_METAHEURISTIC_HPP
#define ABSTRACT_METAHEURISTIC_HPP

#include "interface/IMetaheuristic.hpp"
#include "Measurer.hpp"

#include <unordered_map>
#include <memory>
#include <random>

namespace opthirrygated {

    class AbstractMetaheuristic : public IMetaheuristic {


    public:
        Instance& inst;
        Measurer& measurer;

        AbstractMetaheuristic(Instance &instance, Measurer& ms): inst(instance), measurer(ms) {};
        Solution execute(Solution& solution) override = 0;
    };

}

#endif // MONTE_CARLO_TREE_SEARCH_HPP
