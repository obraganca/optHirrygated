#ifndef EXACT_CPP_
#define EXACT_CPP_

#include "Exact.hpp"

using namespace opthirrygated;

void Exact::addConstraint_Percentimetro(IloEnv& env, IloModel& modelo)
{
	for(int t=0; t<p.getPivots().size(); t++){
		for (unsigned int c = 0; c < p.getPivots()[t].getCicle().size(); c++) // para cada dia do Ciclo
		{

			//criando restrição
			IloExpr temp(env);

			for (unsigned int perc = 0; perc < p.getPivots()[t].getPerc().size(); perc++) // a soma das escolhas dos percentimetros usados para o dia c
			{
				temp += x[t][perc][c];

			}

			IloConstraint constr = (temp) == 1; // deve ser igual a 1
			stringstream name;
			name << "PERC dia("<<t << c << ") : ";

			constr.setName(name.str().c_str());
			modelo.add(constr);
			constraints.add(constr);

		}
	}

}

void Exact::addConstraint_AguaInicial(IloEnv& env, IloModel& modelo)
{
	//a quantidade de água disponível no primeiro dia é igual a CAD do primeiro dia

	IloExpr temp(env);

	for(int t=0; t< p.getPivots().size(); t++){
		temp = adi[t][0] - p.getPivots()[t].getCad().at(0);

		IloConstraint constr = (temp) == 0;
		stringstream name;
		name << "Dia"<<t<<"0:";
		constr.setName(name.str().c_str());
		modelo.add(constr);
		constraints.add(constr);
	}

}


void Exact::addConstraint_AguaInicialRestante(IloEnv& env, IloModel& modelo)
{

	for(int t=0; t<p.getPivots().size(); t++) {
		for (unsigned int c = 1; c < p.getPivots()[t].getCicle().size(); c++) // para todos os dias do ciclo, exceto o primeiro (0)
		{
			IloExpr temp(env);

			temp = adi[t][c] - adf[t][c - 1];

			IloConstraint constr = temp == 0;

			stringstream name;
			name << "Dia[t](c): ";
			constr.setName(name.str().c_str());
			modelo.add(constr);
			constraints.add(constr);

		}
	}
}


void Exact::addConstraint_AguaFinal(IloEnv& env, IloModel& modelo)
{
	//para cada dia do ciclo
	for(int t=0; t<p.getPivots().size(); t++){
		for(unsigned int c = 0 ; c < p.getPivots()[t].getCicle().size(); c++)
		{
			IloExpr temp(env);

			IloExpr lamina(env);
			for(unsigned int perc = 0; perc < p.getPivots()[t].getPerc().size(); perc++)
			{
				lamina += p.getPivots()[t].getLamp().at(perc) * x[t][perc][c];
			}

			temp = adf[t][c] - adi[t][c] + p.getPivots()[t].getEtc().at(c) - p.getPivots()[t].getPrec().at(c) - lamina;

			IloConstraint constr = (temp) == 0;
			stringstream name;
			name << "AguaF(" << t<<c << ") : ";
			constr.setName(name.str().c_str());
			modelo.add(constr);
			constraints.add(constr);

		}
	}

}


void Exact::addConstraint_LimiteCritico(IloEnv& env, IloModel& modelo)
{
	for(int t=0; t<p.getPivots().size(); t++){
		for(unsigned int c = 0 ; c < p.getPivots()[t].getCicle().size(); c++)
		{
			IloExpr temp(env);

			temp = adf[t][c] - p.getPivots()[t].getLc().at(c);

			IloConstraint constr = (temp) >= 0;
			stringstream name;
			name << "Lc(" << t <<c << ") : ";
			constr.setName(name.str().c_str());
			modelo.add(constr);
			constraints.add(constr);

		}
	}

}

void Exact::addConstraint_IrrigationLimit(IloEnv& env, IloModel& modelo)
{
	if (limitDay.empty()) return;

    for(unsigned int c = 0; c < 120 /*p.getPivots()[t].getCicle().size()*/; c++){
        IloExpr irrigation(env);
        for (int t = 0; t < p.getPivots().size(); t++) {
            for(unsigned int perc = 0; perc < p.getPivots()[t].getPerc().size(); perc++) {
                irrigation += p.getPivots()[t].getLamp().at(perc) * x[t][perc][c];
            }
        }
        float limit_val = (c < limitDay.size()) ? limitDay[c] : limitDay.back();
        IloConstraint limConstr = (irrigation <= limit_val);
        stringstream n;
        n << "IrrLimit(" << c << ") <= " << limit_val;
        limConstr.setName(n.str().c_str());
        modelo.add(limConstr);
        constraints.add(limConstr);

        irrigation.end();
    }
}

Exact::Exact(Instance& _p, double _timelimit, vector<float> limitDay) :
		p(_p), timelimit(_timelimit), limitDay(limitDay)
{
	modelo = IloModel(env);
	constraints = IloConstraintArray(env);
	LBdefined = false;

	//definindo a variavel x_tpc
	//  - binária, representa se o percentímetro p foi utilizado no dia c do Ciclo do pivot t


    x = IloNumVar3Matrix (env, p.getPivots().size());

    for(unsigned int t = 0; t<p.getPivots().size(); t++){
        x[t] = IloNumVarMatrix(env, p.getPivots()[t].getPerc().size());

        for (unsigned int perc = 0; perc < p.getPivots()[t].getPerc().size(); perc++)
        {
            x[t][perc] = IloNumVarArray(env, p.getPivots()[t].getCicle().size(), 0, 1, ILOINT);
            for(unsigned int c = 0; c < p.getPivots()[t].getCicle().size(); c++)
            {
                stringstream var;
                var << "x[" << t<<","<< perc <<","<<c<<"]";

                x[t][perc][c].setName(var.str().c_str());

                modelo.add(x[t][perc][c]);

            }
        }

    }

	//definindo variavel adf_c
	//  - quantidade de água disponível no solo no final do dia c do Ciclo do pivô t
    // inicializa membros adf e adi
    adf = IloNumVarMatrix(env, (IloInt)p.getPivots().size()); // typedef IloNumVarMatrix = IloArray<IloNumVarArray>
    for (int t = 0; t < (int)p.getPivots().size(); ++t) {
        adf[t] = IloNumVarArray(env, (IloInt)p.getPivots()[t].getCicle().size(), 0.0, IloInfinity, ILOFLOAT);
        for (unsigned int c = 0; c < p.getPivots()[t].getCicle().size(); ++c) {
            stringstream var;
            var << "adf[" << t << "," << c << "]";
            adf[t][c].setName(var.str().c_str());
            modelo.add(adf[t][c]);
        }
    }

    adi = IloNumVarMatrix(env, (IloInt)p.getPivots().size());
    for (int t = 0; t < (int)p.getPivots().size(); ++t) {
        adi[t] = IloNumVarArray(env, (IloInt)p.getPivots()[t].getCicle().size(), 0.0, IloInfinity, ILOFLOAT);
        for (unsigned int c = 0; c < p.getPivots()[t].getCicle().size(); ++c) {
            stringstream var;
            var << "adi[" << t << "," << c << "]";
            adi[t][c].setName(var.str().c_str());
            modelo.add(adi[t][c]);
        }
    }


    // função objetivo
	IloExpr FO(env);

	for(int t=0; t<p.getPivots().size(); t++){
		for (int perc = 0; perc < p.getPivots()[t].getPerc().size(); perc++)
		{
			for (int c = 0; c < p.getPivots()[t].getCicle().size(); c++)
			{
				FO += p.getPivots()[t].getCost().at(perc)*x[t][perc][c];
			}
		}
	}

	//adicionando a fo ao modelo
	modelo.add(IloMinimize(env, FO));

	addConstraint_Percentimetro(env, modelo);
	addConstraint_AguaInicial(env, modelo);
	addConstraint_AguaInicialRestante(env, modelo);
	addConstraint_AguaFinal(env, modelo);
	addConstraint_LimiteCritico(env, modelo);
    addConstraint_IrrigationLimit(env, modelo);
}

Exact::~Exact()
{
	pirr.end();
}
/*
Solution Exact::getSolution()
{
	Solution s = vector<int>(p.N);
	for (unsigned int i = 0; i < p.N; i++)
	{
		for (unsigned int k = 0; k < p.N; k++)
		{
			if (mla.getValue(x[i][k]) > 0.9999)
			{
				s.at(i) = k+1;
			}
		}
	}

	return s;
}

*/
void Exact::solve()
{

	// Gera LP
	pirr = IloCplex(modelo);
	pirr.exportModel("lp.lp");

	pirr.setParam(IloCplex::Param::MIP::Display, 0);
	pirr.setParam(IloCplex::Param::Simplex::Display, 0);
	pirr.setParam(IloCplex::Param::Barrier::Display, 0);
	pirr.setParam(IloCplex::Param::Network::Display, 0);

	pirr.setParam(IloCplex::Param::TimeLimit, timelimit);
	//resolvendo o modelo
	pirr.solve();

}


void Exact::showSolution()
{
	for(int t=0; t<p.getPivots().size(); t++){
		cout << "Solution Output: [ ";
		for(unsigned int c = 0; c < p.getPivots()[t].getCicle().size(); c++)
		{
			for(unsigned int perc = 0; perc < p.getPivots()[t].getPerc().size(); perc++)
			{
				if( pirr.getValue(x[t][perc][c]) >= 0.5 )
				{
					cout << perc << " ";
				}
			}
		}
		cout << "]" << endl;
	}

}

Solution Exact::getSolution()
{
    Solution sol;
    vector<int> v;
	vector<vector<int>> vv;

	for(int t=0; t<p.getPivots().size(); t++){
        v.clear();
		for(unsigned int c = 0; c < p.getPivots()[t].getCicle().size(); c++)
		{
			for(unsigned int perc = 0; perc < p.getPivots()[t].getPerc().size(); perc++)
			{
				if( pirr.getValue(x[t][perc][c]) >= 0.5 )
				{
					v.push_back(perc);
				}
			}
		}

		vv.push_back(v);


	}

	sol.setSolutions(vv);

    return sol;
}

double Exact::getFO()
{
    if(getStatus() == SOLUTIONFOUND)
    {
        return pirr.getObjValue();
    }
    else
    {
        return -1;
    }

}

status Exact::getStatus()
{
	if(pirr.getStatus() == IloAlgorithm::Infeasible)
	{
		return INFEASIBLE;
	}
	else
	{
		return SOLUTIONFOUND;
	}
}

void Exact::showVars()
{

	for(int t=0; t<p.getPivots().size(); t++){
		for(unsigned int c=0; c< p.getPivots()[t].getCicle().size(); c++)
		{
			printf("D[%d] - Ai: %.2f Af: %.2f Lc: %.2f\n",c,  pirr.getValue(adi[t][c]), pirr.getValue(adf[t][c]), p.getPivots()[t].getLc().at(c));
		}
	}

}


bool Exact::isFeasible()
{
    try {
        return pirr.isPrimalFeasible();
    } catch (...) {
        return false;
    }
}

#endif //EXACT_CPP_
