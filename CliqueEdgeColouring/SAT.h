#pragma once
#include "PropositionalFormula.h"
#include <map>

class SAT
{
private:
	std::vector<std::vector<std::string>> CNF;
	int IsSolved;
	std::map<std::string, int> Solution;

	void getCNF(PropositionalFormula* formula);
	void tseytinTransform(PropositionalFormula* formula);
	std::map<std::string, int> getLiterals();
	bool DPLL(std::string chosen, std::map<std::string, int> literals, std::vector<std::vector<std::string>> cnf, std::map<std::string, int> literalsToAssign = std::map<std::string, int>());

public:
	SAT(PropositionalFormula* formula, bool isCNF = false);
	bool solve(std::map<std::string, int> literals = std::map<std::string, int>());
	std::map<std::string, int> getSolution();
};

