#ifndef PIVOT_HPP
#define PIVOT_HPP

#include <iostream>
#include <vector>

using namespace std;

namespace opthirrygated {
    class Pivot {
    private:
        vector<float> cicle;
        vector<int> perc;
        vector<float> cost;
        vector<float> lamp;
        vector<float> prec;
        vector<float> etc;
        vector<float> cad;
        vector<float> lc;

    public:
        // --- Getters ---
        const vector<float>& getCicle() const { return cicle; }
        const vector<int>& getPerc() const { return perc; }
        const vector<float>& getCost() const { return cost; }
        const vector<float>& getLamp() const { return lamp; }
        const vector<float>& getPrec() const { return prec; }
        const vector<float>& getEtc() const { return etc; }
        const vector<float>& getCad() const { return cad; }
        const vector<float>& getLc() const { return lc; }

        // --- Setters ---
        void setCicle(const vector<float>& value) { cicle = value; }
        void setPerc(const vector<int>& value) { perc = value; }
        void setCost(const vector<float>& value) { cost = value; }
        void setLamp(const vector<float>& value) { lamp = value; }
        void setPrec(const vector<float>& value) { prec = value; }
        void setEtc(const vector<float>& value) { etc = value; }
        void setCad(const vector<float>& value) { cad = value; }
        void setLc(const vector<float>& value) { lc = value; }
    };
}

#endif
