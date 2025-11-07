#ifndef MEASURER_HPP
#define MEASURER_HPP

#include "Instance.hpp"
#include "Solution.hpp"
#include <cstddef>
#include <limits>

namespace opthirrygated {
    class Measurer {
    private:
        Instance& inst;
        size_t pivotIndex;
        Pivot pv;            // cópia do pivo selecionado (se modo single-pivo)
        bool singlePivotMode;

    public:
        // pivoIndex default = npos -> opera sobre todos os pivos do Solution
        Measurer(Instance &instance, size_t pivoIndex = std::numeric_limits<size_t>::max());

        // Avalia a solução (se singlePivoMode: avalia um pivo; senão: soma para todos)
        float evaluate(Solution& solution);

        // Valida a solução (se singlePivoMode: valida um pivo; senão: valida todos)
        bool validation(Solution& solution);

        // Valida apenas um intervalo [startIdx, endIdx] (aplica-se a cada pivo separadamente)
        bool validationRange(Solution& solution, int startIdx, int endIdx);

        // Avalia custo somente no intervalo (soma sobre todos os pivos ou apenas o pivo selecionado)
        float evaluateRange(Solution& solution, int startIdx, int endIdx);
    };
}

#endif
