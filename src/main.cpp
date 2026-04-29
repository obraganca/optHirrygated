#include <iostream>
#include <vector>
#include <float.h>
#include <string>
#include <stdexcept>

#include <memory>
#include "../include/Instance.hpp"
#include "../include/Measurer.hpp"
#include "../include/Solution.hpp"
#include "../include/constructive/ConstructiveHeuristicBackward.hpp"
#include "../include/constructive/ConstructiveHeuristicForwardA.hpp"
#include "../include/constructive/ConstructiveHeuristicForwardB.hpp"
#include "../include/constructive/ConstructiveHeuristicForwardC.hpp"
#include "../include/constructive/ConstructiveHeuristicLookaheadMemoization.hpp"
#include "../include/constructive/ConstructiveHeuristicLookahead.hpp"
#include "../include/refinement/RefinementHeuristicVND.hpp"
#include "../include/refinement/RefinementHeuristicRVND.hpp"
#include "../include/RefinementHeuristic.hpp"
#include "../include/SolutionProcessor.hpp"
#include "../include/CSVExporter.hpp"
#include "neighborhood/NeighborhoodGradualReduction.hpp"
#include "neighborhood/NeighborhoodSmartReplacement.hpp"
#include "neighborhood/NeighborhoodReduceHighIrrigation.hpp"
#include "neighborhood/NeighborhoodPatternBased.hpp"
#include "local_search/BestImprovementLocalSearch.hpp"
#include "metaheuristic/MonteCarloTreeSearch.hpp"
#include "metaheuristic/ParticleSwarmOptimization.hpp"
#include "metaheuristic/ILS.hpp"
#include "../include/Exact.hpp"

using namespace std;
using namespace opthirrygated;

// ─────────────────────────────────────────────
//  Helpers
// ─────────────────────────────────────────────

void printUsage(const char* prog) {
    cout << "Usage: " << prog << " [OPTIONS]\n\n"
         << "Options:\n"
         << "  --constructive <method>    forward (default) | backward | lookahead\n"
         << "  --lookahead-depth <int>    Depth for lookahead constructive (default: 2)\n"
         << "  --refinement <method>      none (default) | vnd | rvnd\n"
         << "  --method <method>          none (default) | heuristic | exact\n"
         << "  --metaheuristic <method>   none | mcts (default) | pso | ils | exact\n"
         << "  --ils-iterations <int>     Max iterations for ILS (default: 100)\n"
         << "  --ils-perturb <int>        Perturbation strength for ILS (default: 5)\n"
         << "  --timelimit <seconds>      Time limit for exact solver (default: 3600)\n"
         << "  --datasource <path>        Path to xlsx file (default: ../datasource/planilha.xlsx)\n"
         << "  --quiet                    Suppress solution vector output\n"
         << "  --csv                      Print result as CSV line (for scripting)\n"
         << "  --help                     Show this help\n\n"
         << "CSV output format: constructive,refinement,metaheuristic,valid,cost\n";
}

string getArg(int argc, char* argv[], const string& flag, const string& defaultVal = "") {
    for (int i = 1; i < argc - 1; ++i)
        if (string(argv[i]) == flag)
            return string(argv[i + 1]);
    return defaultVal;
}

bool hasFlag(int argc, char* argv[], const string& flag) {
    for (int i = 1; i < argc; ++i)
        if (string(argv[i]) == flag) return true;
    return false;
}

// ─────────────────────────────────────────────
//  Main
// ─────────────────────────────────────────────

int main(int argc, char* argv[]) {

    if (hasFlag(argc, argv, "--help")) {
        printUsage(argv[0]);
        return 0;
    }

    // ── Parse arguments ──────────────────────
    string constructiveMethod = getArg(argc, argv, "--constructive", "forwardA");
    string method = getArg(argc, argv, "--method", "heuristic");
    int    lookaheadDepth     = stoi(getArg(argc, argv, "--lookahead-depth", "2"));
    string refinementMethod   = getArg(argc, argv, "--refinement",   "none");
    string metaheuristicMethod= getArg(argc, argv, "--metaheuristic","mcts");
    int    ilsIterations      = stoi(getArg(argc, argv, "--ils-iterations", "100"));
    int    ilsPerturb         = stoi(getArg(argc, argv, "--ils-perturb", "5"));
    double timelimit          = stod(getArg(argc, argv, "--timelimit", "3600"));
    string datasource         = getArg(argc, argv, "--datasource", "../datasource/planilha_milho.xlsx");
    bool   quiet              = hasFlag(argc, argv, "--quiet");
    bool   csvMode            = hasFlag(argc, argv, "--csv");

    // ── Validate choices ─────────────────────
    auto validate = [](const string& val, const vector<string>& valid, const string& name) {
        for (auto& v : valid) if (val == v) return;
        throw invalid_argument("Invalid value '" + val + "' for " + name +
                               ". Run with --help for options.");
    };
    validate(constructiveMethod,  {"forwardA","forwardB","forwardC","backward","lookahead","lookaheadMemoization"}, "--constructive");
    validate(refinementMethod,    {"none","vnd","rvnd"},              "--refinement");
    validate(metaheuristicMethod, {"none","mcts","pso","ils","exact"}, "--metaheuristic");
    validate(method, {"heuristic","exact"}, "--method");

    // ── Instance & Measurer ──────────────────
    Instance instance(datasource);
    Measurer measurer(instance);
    Solution solution;

    // ── Neighborhoods (shared by all that need them) ──
    std::vector<std::shared_ptr<INeighborhood>> neighborhoods;
    neighborhoods.push_back(std::make_shared<NeighborhoodGradualReduction>());
    neighborhoods.push_back(std::make_shared<NeighborhoodSmartReplacement>());
    neighborhoods.push_back(std::make_shared<NeighborhoodReduceHighIrrigation>());
    neighborhoods.push_back(std::make_shared<NeighborhoodPatternBased>());

    BestImprovementLocalSearch bestLocalSearch;

    // ── Constructive ─────────────────────────
    if (!csvMode)
        cout << "[Constructive] " << constructiveMethod << "\n";


    if (method == "exact") {
        Exact exato(instance, timelimit);
        exato.solve();
        if (exato.getStatus() == SOLUTIONFOUND) {
            if (!csvMode)
                cout << "[Exact] Solution found. FO: " << exato.getFO() << "\n";
            solution = exato.getSolution();
        } else {
            if (!csvMode)
                cout << "[Exact] No solution found.\n";
        }
    }else{
        if (constructiveMethod == "forwardA") {
            ConstructiveHeuristicForwardA h(instance);

            solution = h.execute();
        } else if (constructiveMethod == "backward") {
            ConstructiveHeuristicBackward h(instance);
            solution = h.execute();
        } else if (constructiveMethod == "lookahead") {
            ConstructiveHeuristicLookahead h(instance, lookaheadDepth);
            solution = h.execute();
        }else if(constructiveMethod == "forwardB") {
            ConstructiveHeuristicForwardB h(instance);
            solution = h.execute();
        } else if (constructiveMethod == "forwardC") {
            ConstructiveHeuristicForwardC h(instance);
            solution = h.execute();
        } else if (constructiveMethod == "lookaheadMemoization") {
            ConstructiveHeuristicLookaheadMemoization h(instance, lookaheadDepth);
            solution = h.execute();
        }

        if (!csvMode) {
            cout << "[Constructive] Valid: "
                << (measurer.validation(solution) ? "yes" : "no") << "\n";
            cout << "[Constructive] Cost: R$" << measurer.evaluate(solution) << "\n";
        }

        // ── Refinement ───────────────────────────
        if (refinementMethod != "none") {
            if (!csvMode)
                cout << "[Refinement]   " << refinementMethod << "\n";

            if (refinementMethod == "vnd") {
                RefinementHeuristicVND ref(instance);
                solution = ref.execute(solution, neighborhoods, bestLocalSearch);
            } else if (refinementMethod == "rvnd") {
                RefinementHeuristicRVND ref(instance);
                solution = ref.execute(solution, neighborhoods, bestLocalSearch);
            }

            if (!csvMode) {
                cout << "[Refinement]   Valid: "
                    << (measurer.validation(solution) ? "yes" : "no") << "\n";
                cout << "[Refinement]   Cost: R$" << measurer.evaluate(solution) << "\n";
            }
        }

        // ── Metaheuristic ────────────────────────
        if (metaheuristicMethod != "none") {
            if (!csvMode)
                cout << "[Metaheuristic] " << metaheuristicMethod << "\n";

            if (metaheuristicMethod == "mcts") {
                MonteCarloTreeSearch meta(instance, measurer, neighborhoods);
                solution = meta.execute(solution);
            } else if (metaheuristicMethod == "pso") {
                ParticleSwarmOptimization meta(instance, measurer);
                solution = meta.execute(solution);
            } else if (metaheuristicMethod == "ils") {
                // Cria o refinement fora dos blocos internos
                std::unique_ptr<IRefinementHeuristic> ref;

                if (refinementMethod == "vnd") {
                    RefinementHeuristicVND ref(instance);
                } else if (refinementMethod == "rvnd") {
                    RefinementHeuristicRVND ref(instance);
                } else {
                    RefinementHeuristicVND ref(instance);
                }

                // Aplica o refinement inicial antes do ILS
                solution = ref->execute(solution, neighborhoods, bestLocalSearch);

                ILS meta(instance, measurer, neighborhoods, bestLocalSearch, *ref,
                        ilsIterations, ilsPerturb);
                solution = meta.execute(solution);
            }
        }
    }
    // ── Final output ─────────────────────────
    bool   valid = measurer.validation(solution);
    double cost  = measurer.evaluate(solution);

    if (csvMode) {
        // Machine-readable single line for the Python runner
        if(method == "exact"){
            cout << method << ","
                << (valid ? "1" : "0") << ","
                << cost << "\n";
        }else{
            cout << constructiveMethod << ","
                << refinementMethod   << ","
                << metaheuristicMethod << ","
                << (valid ? "1" : "0") << ","
                << cost << "\n";
        }
    } else {
        if(method == "exact"){
            cout << "\n=== Final Solution ===\n";
            cout << "Method  : " << method  << "\n";
            cout << "Valid         : " << (valid ? "yes" : "no") << "\n";
            cout << "Total days    : " << instance.getCicle().size() << "\n";
            cout << "Cost          : R$" << cost << "\n";

            if (!quiet) {
                cout << "Solution      : [ ";
                for (float val : solution.getSolution())
                    cout << val << " ";
                cout << "]\n";
            }
        }else{
            cout << "\n=== Final Solution ===\n";
            cout << "Constructive  : " << constructiveMethod  << "\n";
            cout << "Refinement    : " << refinementMethod    << "\n";
            cout << "Metaheuristic : " << metaheuristicMethod << "\n";
            cout << "Valid         : " << (valid ? "yes" : "no") << "\n";
            cout << "Total days    : " << instance.getCicle().size() << "\n";
            cout << "Cost          : R$" << cost << "\n";

            if (!quiet) {
                cout << "Solution      : [ ";
                for (float val : solution.getSolution())
                    cout << val << " ";
                cout << "]\n";
            }
        }
        
    }

    return 0;
}