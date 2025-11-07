#include <iostream>
#include "../include/Instance.hpp"
#include "../include/Measurer.hpp"
#include "../include/Solution.hpp"
#include "../include/ConstructiveHeuristic.hpp"
#include "../include/RefinementHeuristic.hpp"
#include "../include/Exact.hpp"
#include "../include/SolutionProcessor.hpp"
#include "../include/CSVExporter.hpp"

using namespace std;
using namespace opthirrygated;
int main() {
    // Create an Instance object

    Instance instance("../datasource/planilha.xlsx");



    //Running exact method

    vector<float> limit;
    limit.push_back(20.0f); // mesma para todos os dias

    Exact exato(instance, 3600.0, limit);
    exato.solve();

    if (exato.isFeasible()) {
        cout << "Solucao encontrada! FO = " << exato.getFO() << endl;
        exato.showSolution();
        Solution sol = exato.getSolution();
        sol.show();
    } else {
        cout << "Modelo inviavel ou sem solução ótima." << endl;
    }



    //CSVExporter exporter;
    //SolutionProcessor sp;
    //exporter.exportToCSV(sp.processSolution(instance, exato.getSolution()), "result.csv");


    /*
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

    ConstructiveHeuristic constructiveHeuristic(instance);
    //solution = constructiveHeuristic.executeLookahead(4);
    solution = constructiveHeuristic.executeBackward();



    RefinementHeuristic refinementHeuristic(instance);
    //solution = refinementHeuristic.executeMCTS(solution);
    //solution = refinementHeuristic.executeA(solution);
    //for(int i =0; i<106; i++)
        //solution = refinementHeuristic.executeA(solution);
    cout << "Solution validation: " << (measurer.validation(solution) ? "is valid": "is invalid") << endl;

    cout << "Total day evaluated: " << instance.getCicle().size() <<endl;
    cout << "Solution Cost: R$" << measurer.evaluate(solution)<<endl;
    cout << "Solution Output: [ ";
    for (float val : solution.getSolution()) {
        cout << val << " ";
    }
    cout<<"]"<<endl;

    cout << "---------------------------------------------------------------"<<endl;


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
