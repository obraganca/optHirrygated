#include <stdio.h>
#include <float.h>
#include <iostream>
#include <algorithm>
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

    void Solution::constructCriticalLimitDelt(const Instance inst){
        vector<float> vc;
        float delt = FLT_MAX;
        for (int day =inst.getCicle().size()-1; day>=0; day--) {
            float auxDelt = getAdfSolutions()[day] - inst.getLc()[day];
            if(auxDelt < delt){
                vc.push_back(auxDelt);
                delt = auxDelt;
            }else{
                vc.push_back(delt);
            }
        }

        reverse(vc.begin(), vc.end());
        setCriticalSolution(vc);
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
