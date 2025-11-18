#ifndef NEIGHBORHOOD_GRADUAL_REDUCTION_HPP
#define NEIGHBORHOOD_GRADUAL_REDUCTION_HPP

#include "interface/INeighborhood.hpp"

namespace opthirrygated {

    class NeighborhoodGradualReduction : public INeighborhood {
    public:
        Solution execute(const Solution& s, Instance& inst) override;
        virtual ~NeighborhoodGradualReduction() = default;
    };

}

#endif // NEIGHBORHOOD_GRADUAL_REDUCTION_HPP
