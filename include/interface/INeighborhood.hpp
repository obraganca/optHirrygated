#ifndef I_NEIGHBORHOOD_HPP
#define I_NEIGHBORHOOD_HPP

#include <iostream>
#include "Solution.hpp"
#include "Instance.hpp"
#include "Measurer.hpp"

namespace opthirrygated {

    class INeighborhood {
    public:
        virtual ~INeighborhood() = default;
        virtual Solution execute(const Solution& s,  Instance& inst) = 0;
    };

} // namespace opthirrygated

#endif
