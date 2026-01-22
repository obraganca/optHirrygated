#include <iostream>
#include <vector>
#include <float.h>

#include <memory>
#include "../include/Instance.hpp"
#include "../include/Measurer.hpp"
#include "../include/Solution.hpp"
#include "../include/constructive/ConstructiveHeuristicBackward.hpp"
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
#include "metaheuristic/ParticleSwarmOptimization.hpp"
using namespace std;
using namespace opthirrygated;

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

    std::vector<std::shared_ptr<INeighborhood>> neighborhoods;
    neighborhoods.push_back(std::make_shared<NeighborhoodGradualReduction>());
    neighborhoods.push_back(std::make_shared<NeighborhoodSmartReplacement>());
    neighborhoods.push_back(std::make_shared<NeighborhoodReduceHighIrrigation>());
    neighborhoods.push_back(std::make_shared<NeighborhoodPatternBased>());


    //ConstructiveHeuristicLookahead constructiveHeuristic(instance, 10);
    ConstructiveHeuristicBackward constructiveHeuristic(instance);
    BestImprovementLocalSearch bestLocalSearch;
    RefinementHeuristicVND refinementHeuristic(instance);
    //MonteCarloTreeSearch metaheuristic(instance, measurer, neighborhoods);
    ParticleSwarmOptimization metaheuristic(instance, measurer);
    solution = constructiveHeuristic.execute();

    solution = refinementHeuristic.execute(solution, neighborhoods, bestLocalSearch);
    solution = metaheuristic.execute(solution);


    cout << "Solution validation: " << (measurer.validation(solution) ? "is valid": "is invalid") << endl;

    cout << "Total day evaluated: " << instance.getCicle().size() <<endl;
    cout << "Solution Cost: R$" << measurer.evaluate(solution)<<endl;
    cout << "Solution Output: [ ";
    for (float val : solution.getSolution()) {
        cout << val << " ";
    }
    cout<<"]"<<endl;

    cout << "---------------------------------------------------------------"<<endl;

    return 0;
}