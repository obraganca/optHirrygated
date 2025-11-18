#ifndef NEIGHBORHOOD_PATTERN_BASED_HPP
#define NEIGHBORHOOD_PATTERN_BASED_HPP

#include "interface/INeighborhood.hpp"

namespace opthirrygated {

    class NeighborhoodPatternBased : public INeighborhood {
    public:
        Solution execute(const Solution& s, Instance& inst) override;
        virtual ~NeighborhoodPatternBased() = default;
    };

}

#endif // NEIGHBORHOOD_PATTERN_BASED_HPP
