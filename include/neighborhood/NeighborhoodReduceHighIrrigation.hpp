#ifndef NEIGHBORHOOD_REDUCE_HIGH_IRRIGATION_HPP
#define NEIGHBORHOOD_REDUCE_HIGH_IRRIGATION_HPP

#include "interface/INeighborhood.hpp"

namespace opthirrygated {

    class NeighborhoodReduceHighIrrigation : public INeighborhood {
    public:
        Solution execute(const Solution& s, Instance& inst) override;
        virtual ~NeighborhoodReduceHighIrrigation() = default;
    };

}

#endif // NEIGHBORHOOD_REDUCE_HIGH_IRRIGATION_HPP
