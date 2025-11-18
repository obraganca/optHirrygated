#ifndef ILOCAL_SEARCH_HPP
#define ILOCAL_SEARCH_HPP

#include "Solution.hpp"
#include "Instance.hpp"
#include "INeighborhood.hpp"

namespace opthirrygated {

    class ILocalSearch {
    public:
        virtual Solution run(Solution& s, Instance& inst, INeighborhood& nh) = 0;
        virtual ~ILocalSearch() = default;
    };

}

#endif
