#include <stdio.h>
#include <iostream>
#include "../include/Solution.hpp"
#include "../include/Instance.hpp"

namespace opthirrygated{

    void Solution::propagate(const Instance& inst, int d) {
        std::vector<float> adfSolutions = this->getAdfSolutions();

        if (d < 0 || d >= static_cast<int>(adfSolutions.size())) {
            return;
        }

        for (int day = d; day < static_cast<int>(adfSolutions.size()); ++day) {
            float adi = day == 0 ? inst.getCad()[0] : this->getAdfSolutions()[day-1];
            float auxAdf = adi - inst.getEtc()[day] + inst.getPrec()[day] + inst.getLamp()[this->getSolution()[day]];
            this->updateAdfSolution(day, auxAdf);
        }
    }

    std::vector<int> Solution::getHighIrrigationDays() {
        vector<int> highIrrigationDays;
        for (int day = 0; day < this->solution.size(); ++day) {
            if (this->getSolution()[day] >= 3 && this->getSolution()[day] < 10) {
                highIrrigationDays.push_back(day);
            }
        }
        return highIrrigationDays;
    }
}
