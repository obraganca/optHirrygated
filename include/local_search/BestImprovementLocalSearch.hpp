#ifndef BEST_IMPROVEMENT_LOCAL_SEARCH_HPP
#define BEST_IMPROVEMENT_LOCAL_SEARCH_HPP

#include "interface/ILocalSearch.hpp"
#include "interface/INeighborhood.hpp"

namespace opthirrygated {

    class BestImprovementLocalSearch : public ILocalSearch {
    public:
        Solution run(Solution& s, Instance& inst, INeighborhood& nh) override;
    };

}

#endif
