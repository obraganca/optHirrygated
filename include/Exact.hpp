#ifndef EXACT_HPP_
#define EXACT_HPP_

enum status {SOLUTIONFOUND, INFEASIBLE};

#include <ilcplex/ilocplex.h>

#include "Instance.hpp"
#include "Solution.hpp"

typedef IloArray<IloNumVarArray> IloNumVarMatrix;
typedef IloArray<IloNumVarMatrix> IloNumVar3Matrix;
typedef IloArray<IloNumVar3Matrix> IloNumVar4Matrix;

typedef IloArray<IloNumArray> IloNumMatrix;
typedef IloArray<IloNumMatrix> IloNum3Matrix;

namespace opthirrygated {
    class Exact
    {
    private:

        Instance& p;
        vector<float> limitDay;
        IloConstraintArray constraints;
        IloEnv env;
        IloModel modelo;
        IloCplex pirr;
        bool LBdefined;

        IloNumVar3Matrix x;
        IloArray<IloNumVarArray> adf;
        IloArray<IloNumVarArray> adi;

        double timelimit;

        void addConstraint_Percentimetro(IloEnv& env, IloModel& modelo);
        void addConstraint_AguaInicial(IloEnv& env, IloModel& modelo);
        void addConstraint_AguaInicialRestante(IloEnv& env, IloModel& modelo);
        void addConstraint_AguaFinal(IloEnv& env, IloModel& modelo);
        void addConstraint_LimiteCritico(IloEnv& env, IloModel& modelo);
        void addConstraint_IrrigationLimit(); // irrigation_tc <= dailyLimit[c] (se dailyLimit não vazio)

    public:
        Exact(Instance& _p, double _timelimit, vector<float> limitDay);

        ~Exact();

        void solve();
        status getStatus();
        void showSolution();
        double getFO();
        Solution getSolution();
        void showVars();

        void addConstraint_IrrigationLimit(IloEnv &env, IloModel &modelo);
        bool isFeasible();
    };
}

#endif // EXACT_HPP_
