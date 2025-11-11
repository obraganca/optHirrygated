#ifndef I_NEIGHBORHOOD_HPP
#define I_NEIGHBORHOOD_HPP

#include <iostream>
#include "Solution.hpp"
#include "Instance.hpp"

namespace opthirrygated {

    class INeighborhood {
    public:
        virtual ~INeighborhood() = default;
        virtual Solution execute() = 0;
    };

} // namespace opthirrygated

#endif
