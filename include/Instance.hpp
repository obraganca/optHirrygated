#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include <vector>
#include <string>
#include <iostream>

#include "./Pivot.hpp"

using namespace std;

namespace opthirrygated {

    class Instance {
    private:
        vector<Pivot> pivots;  // agora temos vários itens
        string path;

    public:
        // Construtor
        Instance(string path);

        virtual void exec();

        // Métodos de acesso
        vector<Pivot>& getPivots() { return pivots; }
        const vector<Pivot>& getPivots() const { return pivots; }
        void setPivots(const vector<Pivot>& newPivots) { pivots = newPivots; }

        // Manipulação de itens
        void addPivo(const Pivot& pivot) { pivots.push_back(pivot); }
        void clearPivots() { pivots.clear(); }

        // Getter e Setter para path
        string getPath() const { return path; }
        void setPath(const string& newPath) { path = newPath; }
    };

}

#endif