#ifndef NEIGHBORHOOD_SMART_REPLACEMENT_HPP
#define NEIGHBORHOOD_SMART_REPLACEMENT_HPP

#include "interface/INeighborhood.hpp"
#include "Measurer.hpp"

namespace opthirrygated {

    class NeighborhoodSmartReplacement : public INeighborhood {
    public:
        Solution execute(const Solution& s, Instance& inst) override;
        virtual ~NeighborhoodSmartReplacement() = default;
    };

}

#endif // NEIGHBORHOOD_SMART_REPLACEMENT_HPP
