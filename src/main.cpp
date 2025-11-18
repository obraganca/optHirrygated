#include <iostream>
#include <vector>
#include <memory>

#include "../include/Instance.hpp"
#include "../include/Measurer.hpp"
#include "../include/Solution.hpp"
#include "../include/constructive/ConstructiveHeuristicBackward.hpp"
#include "../include/constructive/ConstructiveHeuristicSemiGreedy.hpp"
#include "../include/constructive/ConstructiveHeuristicFoward.hpp"
#include "../include/constructive/ConstructiveHeuristicLookahead.hpp"
#include "../include/refinement/RefinementHeuristicVND.hpp"
#include "../include/RefinementHeuristic.hpp"
#include "../include/Exact.hpp"
#include "../include/SolutionProcessor.hpp"
#include "../include/CSVExporter.hpp"
#include "neighborhood/NeighborhoodGradualReduction.hpp"
#include "neighborhood/NeighborhoodSmartReplacement.hpp"
#include "neighborhood/NeighborhoodReduceHighIrrigation.hpp"
#include "neighborhood/NeighborhoodPatternBased.hpp"
#include "local_search/BestImprovementLocalSearch.hpp"
#include "metaheuristic/MonteCarloTreeSearch.hpp"
using namespace std;
using namespace opthirrygated;

std::vector<std::unique_ptr<INeighborhood>> getNeighborhoods() {
    std::vector<std::unique_ptr<INeighborhood>> vNeighborhood;
    vNeighborhood.push_back(make_unique<NeighborhoodGradualReduction>());
    vNeighborhood.push_back(make_unique<NeighborhoodSmartReplacement>());
    vNeighborhood.push_back(make_unique<NeighborhoodReduceHighIrrigation>());
    vNeighborhood.push_back(make_unique<NeighborhoodPatternBased>());
    return vNeighborhood;
}


int main() {
    Instance instance("../datasource/planilha.xlsx");

    Solution exactSol;
    Exact exato(instance, 3600);
    exato.solve();
    if(exato.getStatus() == status::SOLUTIONFOUND)
    {
        cout << "Solucao encontrada!"<<endl;
        cout << "FO: " << exato.getFO() << endl;
        exato.showSolution();
        exactSol = exato.getSolution();
    }
    else
    {
        cout << "Verificar!!!" <<endl;
    }

    Solution solution;
    cout << endl;
    Measurer measurer(instance);
    cout << "---------------------------------------------------------------"<<endl;
    cout << "Solution Cost (Exato): R$" << measurer.evaluate(exactSol)<<endl << endl;
    cout << "---------------------------------------------------------------"<<endl;
    cout << "Heuristics Evaluation:"<<endl;

    cout << "Caixa Preta Solution Output: [ ";
    for (float val : exactSol.getSolution()) {
        cout << val << " ";
    }
    cout<<"]"<<endl;


    cout << "---------------------------------------------------------------"<<endl;

    ConstructiveHeuristicFoward constructiveHeuristic(instance);
    BestImprovementLocalSearch bestLocalSearch;
    RefinementHeuristicVND refinementHeuristic(instance);
    MonteCarloTreeSearch metaheuristic(instance, measurer, getNeighborhoods());

    solution = constructiveHeuristic.execute();
    //solution = refinementHeuristic.execute(solution, getNeighborhoods(), bestLocalSearch);
    //for(int i =0; i<36; i++)
        //solution = refinementHeuristic.execute(solution, getNeighborhoods(), bestLocalSearch);
        // solution = metaheuristic.execute(solution);


    cout << "Solution validation: " << (measurer.validation(solution) ? "is valid": "is invalid") << endl;

    cout << "Total day evaluated: " << instance.getCicle().size() <<endl;
    cout << "Solution Cost: R$" << measurer.evaluate(solution)<<endl;
    cout << "Solution Output: [ ";
    for (float val : solution.getSolution()) {
        cout << val << " ";
    }
    cout<<"]"<<endl;

    cout << "---------------------------------------------------------------"<<endl;


/*
    CSVExporter exporter;
    SolutionProcessor sp;
    exporter.exportToCSV(sp.processSolution(instance, solution), "result.csv");
    Measurer measurer(instance);
    std::vector<int> validationSolution = {
        10, 10, 10,  1, 10, 10, 10,  0,  1,  1,
         0,  1,  1,  1,  1,  1,  1,  1,  1,  1,
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
         1,  1,  0,  1,  1,  1,  0,  1,  1,  1,
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
         1,  1,  1,  1,  1, 10,  1,  0, 10,  1,
        10, 10, 10, 10,  1, 10, 10,  1, 10,  0,
         0,  0, 10, 10, 10, 10,  1, 10,  1,  1,
        10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
        10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
        10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
        10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
        10, 10, 10
    };

    solution.setSolution(validationSolution);

    cout << "Validating the solution exact "<< endl;
    if (measurer.validation(exactSol)) {
        cout << "Valid solution" <<endl;
    }else {
        cout << "Solution not valid" <<endl;
    }
    */

    return 0;
}
