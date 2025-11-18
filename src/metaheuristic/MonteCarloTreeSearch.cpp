#include "metaheuristic/MonteCarloTreeSearch.hpp"

using namespace std;
using namespace opthirrygated;

Solution MonteCarloTreeSearch::execute(opthirrygated::Solution &rootSolution) {

    float initialCost = measurer.evaluate(const_cast<Solution &>(rootSolution));

    // Initialize the root node
    MCTSNode root(rootSolution, nullptr);
    Solution globalBest = rootSolution;
    float globalBestCost = initialCost;

    std::unordered_map<int, int> irrigationFrequency;
    std::unordered_map<std::string, double> patternRewards;

    for (int it = 0; it < mctsIters; ++it) {
        if ((it + 1) % 500 == 0) {
            std::cout << "Iteration " << (it + 1) << "/" << mctsIters
                      << " - Best cost: " << globalBestCost << std::endl;
        }

        MCTSNode* node = treePolicy(&root);
        if (!node) break;

        double reward = defaultPolicy(node->sol, irrigationFrequency, patternRewards);
        backup(node, reward);

        // Update global best
        float currentCost = measurer.evaluate(node->sol);
        if (currentCost < globalBestCost) {
            globalBestCost = currentCost;
            globalBest = node->sol;
            //std::cout << "*** NEW GLOBAL BEST: " << globalBestCost << " at iteration " << (it + 1) << std::endl;
        }

        // Adaptive exploration: increase exploration if no improvement for a while
        if (it > 0 && it % 1000 == 0) {
            mctsC = std::min(mctsC * 1.1, 3.0); // Increase exploration
        }
    }

    for (const auto& [level, freq] : irrigationFrequency) {
    }

    return globalBest;
}



MonteCarloTreeSearch::MCTSNode* MonteCarloTreeSearch::treePolicy(MCTSNode* node) {
    int maxDepth = 50;
    int depth = 0;

    while (node && depth < maxDepth) {
        if (!node->fullyExpanded) {
            return expand(node);
        } else {
            node = bestChild(node, mctsC);
            if (!node) break;
        }
        depth++;
    }
    return node;
}

bool MonteCarloTreeSearch::isValidCandidate(const Solution& candidate, const Solution& original) {
    // Check if candidate is different and feasible
    const auto& candSol = candidate.getSolution();
    const auto& origSol = original.getSolution();

    bool isDifferent = false;
    for (size_t i = 0; i < std::min(candSol.size(), origSol.size()); ++i) {
        if (candSol[i] != origSol[i]) {
            isDifferent = true;
            break;
        }
    }

    if (!isDifferent) return false;

    // Quick feasibility check
    return measurer.isFeasible(candidate, 0);
}

/*
 * UCT
 * */
MonteCarloTreeSearch::MCTSNode* MonteCarloTreeSearch::bestChild(MCTSNode* node, double c) {
    if (!node || node->children.empty()) return nullptr;

    MCTSNode* best = nullptr;
    double bestScore = -std::numeric_limits<double>::infinity();

    for (auto& child : node->children) {
        if (child->N > 0) {
            double exploit = child->W / child->N;
            double explore = c * std::sqrt(std::log(node->N) / child->N);
            double score = exploit + explore;

            if (score > bestScore) {
                bestScore = score;
                best = child.get();
            }
        }
    }

    return best;
}


double MonteCarloTreeSearch::defaultPolicy(const Solution& sol,
                                          std::unordered_map<int, int>& irrigationFreq,
                                          std::unordered_map<std::string, double>& patternRewards) {
    Solution sim = sol;
    Measurer measurer(inst);
    float initialCost = measurer.evaluate(sim);

    // Focused rollout strategies
    int D = sim.getAdfSolutions().size();
    if (D == 0) return -initialCost;

    // Strategy: Aggressive reduction of high irrigation (>= 3 and < 10)
    std::vector<int> highIrrigationDays;
    for (int day = 0; day < D; ++day) {
        if (sol.getSolution()[day] >= 3 && sol.getSolution()[day] < 10) {
            highIrrigationDays.push_back(day);
        }
    }

    int improvements = 0;

    //Max Moves: Minimal default = 20, Maximal = quantity of high irrigation perc
    int maxMoves = std::min(mctsRolloutDepth, static_cast<int>(highIrrigationDays.size()));

    for (int move = 0; move < maxMoves; ++move) {
        if (highIrrigationDays.empty()) break;

        // Select a day with high irrigation
        std::uniform_int_distribution<int> dayDist(0, highIrrigationDays.size() - 1);
        int dayIdx = dayDist(rng);
        int day = highIrrigationDays[dayIdx];

        // Try reducing to minimal irrigation levels (including ID 10 = no irrigation)
        std::vector<int> reductionLevels = {0, 1, 2, 10};
        std::shuffle(reductionLevels.begin(), reductionLevels.end(), rng);

        bool foundValidMove = false;
        for (int newLevel : reductionLevels) {
            Solution candidate = sim;
            int oldLevel = candidate.getSolution()[day];

            if (oldLevel == newLevel) continue;

            candidate.updateSolution(day, newLevel);
            candidate.propagate( inst, day);
            if (measurer.isFeasible(candidate, day)) {
                float newCost = measurer.evaluate(candidate);
                if (newCost <= initialCost * 1.05) { // Allow slight cost increase for exploration
                    sim = candidate;
                    improvements++;
                    irrigationFreq[newLevel]++;

                    // Remove this day from high irrigation list
                    highIrrigationDays.erase(highIrrigationDays.begin() + dayIdx);
                    foundValidMove = true;
                    break;
                }
            }
        }

        if (!foundValidMove) {
            // Remove the day from consideration if no valid move found
            highIrrigationDays.erase(highIrrigationDays.begin() + dayIdx);
        }
    }

    float finalCost = measurer.evaluate(sim);

    // Enhanced reward function
    double costImprovement = (initialCost - finalCost) / initialCost;
    double improvementBonus = improvements * 0.1;
    double patternBonus = calculatePatternBonus(sim);

    double reward = costImprovement + improvementBonus + patternBonus;

    return reward;
}

double MonteCarloTreeSearch::calculatePatternBonus(const Solution& sol) {
    const auto& solution = sol.getSolution();
    double bonus = 0.0;

    // Bonus for consecutive low irrigation levels (including ID 10)
    int consecutiveLow = 0;
    int maxConsecutiveLow = 0;

    // Bonus for reducing high irrigation (>= 3 and < 10)
    int reducedHighCount = 0;

    for (int day = 0; day < static_cast<int>(solution.size()); ++day) {
        int level = solution[day];

        // Count consecutive low irrigation (0, 1, 2, 10)
        if (level <= 2 || level == 10) {
            consecutiveLow++;
            maxConsecutiveLow = std::max(maxConsecutiveLow, consecutiveLow);
        } else {
            consecutiveLow = 0;
        }

        // Count reduced high irrigation
        if (level <= 2 || level == 10) {
            reducedHighCount++;
        }
    }

    // Bonuses
    bonus += maxConsecutiveLow * 0.02; // Reward long sequences of low irrigation
    bonus += (static_cast<double>(reducedHighCount) / solution.size()) * 0.5; // Reward overall reduction

    return bonus;
}

/*
 * Expansion
 * */
MonteCarloTreeSearch::MCTSNode* MonteCarloTreeSearch::expand(MCTSNode* node) {
    if (!node) return nullptr;

    Solution s = node->sol;
    int D = s.getAdfSolutions().size();
    if (D == 0) return node;

    // Strategy 1: Focus on days with high irrigation (>= 3 and < 10)
    std::vector<int> highIrrigationDays;
    for (int day = 0; day < D; ++day) {
        if (s.getSolution()[day] >= 3 && s.getSolution()[day] < 10) {
            highIrrigationDays.push_back(day);
        }
    }


    for (int attempt = 0; attempt < 10 && node->children.size() < 8; ++attempt) {
        std::uniform_int_distribution<int> strategyDist(0, getNeighborhoods().size() - 1);
        int strategyIdx = strategyDist(rng);

        Solution candidate = (*getNeighborhoods()[strategyIdx]).execute(s, inst);

        // Validate the candidate
        if (isValidCandidate(candidate, s)) {
            auto child = std::make_unique<MCTSNode>(candidate, node);
            node->children.push_back(std::move(child));

            if (node->children.size() >= 8) {
                node->fullyExpanded = true;
            }
            return node->children.back().get();
        }
    }

    node->fullyExpanded = true;
    return node;
}

void MonteCarloTreeSearch::backup(MCTSNode* node, double reward) {
    //std::cout << "    Backup: propagating reward=" << reward << std::endl;

    int level = 0;
    while (node) {
        node->N += 1;
        node->W += reward;
        //std::cout << "    Backup level " << level << ": N=" << node->N
        //<< ", W=" << node->W
        //<< ", avg=" << (node->W / node->N) << std::endl;
        node = node->parent;
        level++;
    }
}

