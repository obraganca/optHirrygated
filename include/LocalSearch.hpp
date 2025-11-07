#ifndef LOCAL_SEARCH_HPP
#define LOCAL_SEARCH_HPP

#include <iostream>

using namespace std;

namespace opthirrygated{
    class IBuscaLocal{
        virtual void execute()=0;
        virtual ~IBuscaLocal() = default;
    };
}

#endif