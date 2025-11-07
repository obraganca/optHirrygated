#include "../include/Measurer.hpp"
#include "../include/RefinementHeuristic.hpp"
#include <float.h>
#include <iomanip>
#include <iostream>
#include <limits>

using namespace opthirrygated;

Measurer::Measurer(Instance &instance, size_t _pivotIndex)
        : inst(instance), pivotIndex(_pivotIndex)
{
    if (inst.getPivots().empty()) {
        throw std::out_of_range("Measurer: no pivots available in Instance");
    }

    if (pivotIndex != std::numeric_limits<size_t>::max()) {
        // modo single-pivot
        if (pivotIndex >= inst.getPivots().size()) {
            throw std::out_of_range("Measurer: pivotIndex out of range");
        }
        pv = inst.getPivots().at(pivotIndex); // copia segura
        singlePivotMode = true;
    } else {
        // modo multi-pivot
        singlePivotMode = false;
    }
}

float Measurer::evaluate(Solution& solution) {
    if (singlePivotMode) {
        // espera que solution contenha a solução para o pivotIndex
        // preferimos acessar via getpivotSolution se disponível
        const auto& sols = solution.getSolutions();
        if (pivotIndex >= sols.size()) {
            std::cerr << "Measurer::evaluate: solution has no entry for pivotIndex " << pivotIndex << std::endl;
            return 0.0f;
        }
        const auto& solutionVec = sols.at(pivotIndex);
        float costSolution = 0.0f;
        const auto& costVec = pv.getCost();
        for (size_t i = 0; i < solutionVec.size(); ++i) {
            int sol = solutionVec[i];
            if (sol >= 0 && static_cast<size_t>(sol) < costVec.size()) {
                costSolution += costVec[sol];
            } else {
                std::cerr << "Warning: Index " << sol << " out of bounds in cost vector (day " << i << ")" << std::endl;
            }
        }
        return costSolution;
    } else {
        // multi-pivot: soma custo de todos os pivots
        const auto& allSols = solution.getSolutions();
        const auto& pivots = inst.getPivots();
        float totalCost = 0.0f;
        size_t T = std::min(allSols.size(), pivots.size());
        for (size_t t = 0; t < T; ++t) {
            const auto& solVec = allSols[t];
            const auto& costVec = pivots[t].getCost();
            for (size_t day = 0; day < solVec.size(); ++day) {
                int sol = solVec[day];
                if (sol >= 0 && static_cast<size_t>(sol) < costVec.size()) {
                    totalCost += costVec[sol];
                } else {
                    std::cerr << "Warning: Index " << sol << " out of bounds in cost vector for pivot " << t << " day " << day << std::endl;
                }
            }
        }
        return totalCost;
    }
}

bool Measurer::validation(Solution& solution) {
    // Multi-pivot: valida todos; Single-pivot: valida apenas o pivotIndex
    const auto& pivots = inst.getPivots();
    auto adfMatrix = solution.getAdfSolutions(); // cópia
    const auto& sols = solution.getSolutions();

    if (singlePivotMode) {
        if (pivotIndex >= sols.size()) {
            std::cerr << "Measurer::validation: solution has no entry for pivotIndex " << pivotIndex << std::endl;
            return false;
        }
        const auto& solVec = sols.at(pivotIndex);
        vector<float> adfSolutionsSingle;
        if (pivotIndex < adfMatrix.size()) adfSolutionsSingle = adfMatrix[pivotIndex];
        float adi = pv.getCad().empty() ? 0.0f : pv.getCad()[0];

        if (solVec.empty()) return false;
        for (size_t day = 0; day < solVec.size(); ++day) {
            int chosen = solVec[day];
            if (chosen < 0 || static_cast<size_t>(chosen) >= pv.getLamp().size()) {
                std::cerr << "Warning: chosen percentimeter index " << chosen << " out of range for day " << day << std::endl;
                return false;
            }
            if (day >= pv.getEtc().size() || day >= pv.getPrec().size() || day >= pv.getLc().size()) {
                std::cerr << "Warning: pivot vectors shorter than solution length at day " << day << std::endl;
                return false;
            }
            float auxAdf = adi - pv.getEtc()[day] + pv.getPrec()[day] + pv.getLamp()[chosen];
            float e = 0.0001f;
            if (auxAdf + e < pv.getLc()[day]) {
                std::cout << std::endl << "---------------------------------------------------------------" << std::endl;
                std::cout << "Solução invalida para pivot " << pivotIndex << " no dia: " << day << std::endl;
                std::cout << "Limite critico: " << std::setprecision(4) << pv.getLc()[day] << std::endl;
                std::cout << "Precipitação : " << pv.getPrec()[day] << std::endl;
                std::cout << "Agua disponivel no fim do dia esperado: " << auxAdf << std::endl;
                if (day < adfSolutionsSingle.size())
                    std::cout << "Agua disponivel no fim do dia (solution): " << adfSolutionsSingle[day] << std::endl;
                std::cout << "---------------------------------------------------------------" << std::endl;
                return false;
            }
            adi = auxAdf;
            if (day < adfSolutionsSingle.size()) adfSolutionsSingle[day] = auxAdf;
        }
        // grava de volta no matrix
        if (pivotIndex < adfMatrix.size()) adfMatrix[pivotIndex] = adfSolutionsSingle;
        else {
            // padroniza tamanho e insere
            while (adfMatrix.size() <= pivotIndex) adfMatrix.emplace_back(vector<float>());
            adfMatrix[pivotIndex] = adfSolutionsSingle;
        }
        solution.setAdfSolutions(adfMatrix);
        return true;
    } else {
        // validate all pivots
        if (sols.empty()) return false;
        size_t T = std::min(sols.size(), pivots.size());
        // ensure adfMatrix has T rows
        if (adfMatrix.size() < T) adfMatrix.resize(T);

        for (size_t t = 0; t < T; ++t) {
            const auto& solVec = sols[t];
            const auto& pvLocal = pivots[t];
            vector<float> adfVec = adfMatrix[t];
            float adi = pvLocal.getCad().empty() ? 0.0f : pvLocal.getCad()[0];

            if (solVec.empty()) return false;
            for (size_t day = 0; day < solVec.size(); ++day) {
                int chosen = solVec[day];
                if (chosen < 0 || static_cast<size_t>(chosen) >= pvLocal.getLamp().size()) {
                    std::cerr << "Measurer::validation: chosen percentimeter " << chosen << " out of range at pivot " << t << " day " << day << std::endl;
                    return false;
                }
                if (day >= pvLocal.getEtc().size() || day >= pvLocal.getPrec().size() || day >= pvLocal.getLc().size()) {
                    std::cerr << "Measurer::validation: pivot vectors too short at pivot " << t << " day " << day << std::endl;
                    return false;
                }
                float auxAdf = adi - pvLocal.getEtc()[day] + pvLocal.getPrec()[day] + pvLocal.getLamp()[chosen];
                float e = 0.0001f;
                if (auxAdf + e < pvLocal.getLc()[day]) {
                    std::cerr << "Invalid solution at pivot " << t << " day " << day << " (auxAdf " << auxAdf << " < lc " << pvLocal.getLc()[day] << ")\n";
                    return false;
                }
                adi = auxAdf;
                if (day < adfVec.size()) adfVec[day] = auxAdf;
            }
            adfMatrix[t] = adfVec;
        }

        // grava resultado
        solution.setAdfSolutions(adfMatrix);
        return true;
    }
}

bool Measurer::validationRange(Solution& solution, int startIdx, int endIdx) {
    const auto& pivots = inst.getPivots();
    auto adfMatrix = solution.getAdfSolutions();
    const auto& sols = solution.getSolutions();

    if (singlePivotMode) {
        if (pivotIndex >= sols.size()) {
            std::cerr << "Measurer::validationRange: solution has no entry for pivotIndex " << pivotIndex << std::endl;
            return false;
        }
        const auto& solVec = sols[pivotIndex];
        vector<float> adfVec = (pivotIndex < adfMatrix.size()) ? adfMatrix[pivotIndex] : vector<float>();

        if (startIdx < 0 || endIdx < 0 || startIdx >= static_cast<int>(solVec.size()) || endIdx >= static_cast<int>(solVec.size()) || startIdx > endIdx) {
            std::cerr << "Measurer::validationRange: invalid indices [" << startIdx << "," << endIdx << "]\n";
            return false;
        }

        float adi;
        if (startIdx == 0) adi = pv.getCad().empty() ? 0.0f : pv.getCad()[0];
        else {
            if (startIdx - 1 >= static_cast<int>(adfVec.size())) {
                std::cerr << "Measurer::validationRange: adfSolutions too short to infer adi for startIdx > 0\n";
                return false;
            }
            adi = adfVec[startIdx - 1];
        }

        for (int day = startIdx; day <= endIdx; ++day) {
            int chosen = solVec[day];
            if (chosen < 0 || static_cast<size_t>(chosen) >= pv.getLamp().size()) {
                std::cerr << "Measurer::validationRange: chosen percentimeter " << chosen << " out of range at day " << day << std::endl;
                return false;
            }
            if (day >= static_cast<int>(pv.getEtc().size()) || day >= static_cast<int>(pv.getPrec().size()) || day >= static_cast<int>(pv.getLc().size())) {
                std::cerr << "Measurer::validationRange: pivot vectors too short at day " << day << std::endl;
                return false;
            }
            float auxAdf = adi - pv.getEtc()[day] + pv.getPrec()[day] + pv.getLamp()[chosen];
            float e = 0.0001f;
            if (auxAdf + e < pv.getLc()[day]) return false;
            if (day >= 0 && static_cast<size_t>(day) < adfVec.size()) adfVec[day] = auxAdf;
            adi = auxAdf;
        }
        // write back and return
        if (pivotIndex < adfMatrix.size()) adfMatrix[pivotIndex] = adfVec;
        else {
            while (adfMatrix.size() <= pivotIndex) adfMatrix.emplace_back(vector<float>());
            adfMatrix[pivotIndex] = adfVec;
        }
        solution.setAdfSolutions(adfMatrix);
        return true;
    } else {
        // multi-pivot: apply validationRange per pivot
        if (sols.empty()) return false;
        size_t T = std::min(sols.size(), pivots.size());
        if (adfMatrix.size() < T) adfMatrix.resize(T);
        for (size_t t = 0; t < T; ++t) {
            const auto& solVec = sols[t];
            const auto& pvLocal = pivots[t];
            vector<float> adfVec = adfMatrix[t];

            if (startIdx < 0 || endIdx < 0 || startIdx >= static_cast<int>(solVec.size()) || endIdx >= static_cast<int>(solVec.size()) || startIdx > endIdx) {
                std::cerr << "Measurer::validationRange: invalid indices [" << startIdx << "," << endIdx << "] for pivot " << t << std::endl;
                return false;
            }
            float adi;
            if (startIdx == 0) adi = pvLocal.getCad().empty() ? 0.0f : pvLocal.getCad()[0];
            else {
                if (startIdx - 1 >= static_cast<int>(adfVec.size())) {
                    std::cerr << "Measurer::validationRange: adfSolutions too short to infer adi for pivot " << t << std::endl;
                    return false;
                }
                adi = adfVec[startIdx - 1];
            }

            for (int day = startIdx; day <= endIdx; ++day) {
                int chosen = solVec[day];
                if (chosen < 0 || static_cast<size_t>(chosen) >= pvLocal.getLamp().size()) {
                    std::cerr << "Measurer::validationRange: chosen percentimeter " << chosen << " out of range at pivot " << t << " day " << day << std::endl;
                    return false;
                }
                if (day >= static_cast<int>(pvLocal.getEtc().size()) || day >= static_cast<int>(pvLocal.getPrec().size()) || day >= static_cast<int>(pvLocal.getLc().size())) {
                    std::cerr << "Measurer::validationRange: pivot vectors too short at pivot " << t << " day " << day << std::endl;
                    return false;
                }
                float auxAdf = adi - pvLocal.getEtc()[day] + pvLocal.getPrec()[day] + pvLocal.getLamp()[chosen];
                float e = 0.0001f;
                if (auxAdf + e < pvLocal.getLc()[day]) {
                    return false;
                }
                if (day >= 0 && static_cast<size_t>(day) < adfVec.size()) adfVec[day] = auxAdf;
                adi = auxAdf;
            }
            adfMatrix[t] = adfVec;
        }
        solution.setAdfSolutions(adfMatrix);
        return true;
    }
}

float Measurer::evaluateRange(Solution& solution, int startIdx, int endIdx) {
    if (singlePivotMode) {
        const auto& sols = solution.getSolutions();
        if (pivotIndex >= sols.size()) {
            std::cerr << "Measurer::evaluateRange: solution has no entry for pivotIndex " << pivotIndex << std::endl;
            return 0.0f;
        }
        const auto& solVec = sols[pivotIndex];
        if (startIdx < 0 || endIdx < 0 || startIdx >= static_cast<int>(solVec.size()) || endIdx >= static_cast<int>(solVec.size()) || startIdx > endIdx) {
            std::cerr << "Warning: Invalid range [" << startIdx << ", " << endIdx << "] for solution of size " << solVec.size() << std::endl;
            return 0.0f;
        }
        float costSolution = 0.0f;
        const auto& costVec = pv.getCost();
        for (int i = startIdx; i <= endIdx; ++i) {
            int sol = solVec[i];
            if (sol >= 0 && static_cast<size_t>(sol) < costVec.size()) costSolution += costVec[sol];
            else std::cerr << "Warning: Index " << sol << " out of bounds in cost vector at position " << i << std::endl;
        }
        return costSolution;
    } else {
        // multi-pivot: sum over all pivots
        const auto& sols = solution.getSolutions();
        const auto& pivots = inst.getPivots();
        if (sols.empty()) return 0.0f;
        size_t T = std::min(sols.size(), pivots.size());
        float totalCost = 0.0f;
        for (size_t t = 0; t < T; ++t) {
            const auto& solVec = sols[t];
            if (startIdx < 0 || endIdx < 0 || startIdx >= static_cast<int>(solVec.size()) || endIdx >= static_cast<int>(solVec.size()) || startIdx > endIdx) {
                std::cerr << "Warning: Invalid range [" << startIdx << ", " << endIdx << "] for pivot " << t << " of size " << solVec.size() << std::endl;
                continue;
            }
            const auto& costVec = pivots[t].getCost();
            for (int i = startIdx; i <= endIdx; ++i) {
                int sol = solVec[i];
                if (sol >= 0 && static_cast<size_t>(sol) < costVec.size()) totalCost += costVec[sol];
                else std::cerr << "Warning: Index " << sol << " out of bounds in cost vector for pivot " << t << " at position " << i << std::endl;
            }
        }
        return totalCost;
    }
}
