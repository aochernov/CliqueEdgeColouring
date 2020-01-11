#include "SAT.h"
#include <iostream>

SAT::SAT(PropositionalFormula* formula, bool isCNF)
{
	IsSolved = formula->getValue();
	if (IsSolved == -1)
	{
		isCNF ? getCNF(formula) : tseytinTransform(formula);
	}
}

void SAT::getCNF(PropositionalFormula* formula)
{
	std::vector<PropositionalFormula*> children = formula->getChildren();
	for (PropositionalFormula* conjunct : children)
	{
		std::vector<std::string> disjunction;
		std::vector<PropositionalFormula*> disjuncts = conjunct->getChildren();
		for (PropositionalFormula* disjunct : disjuncts)
		{
			disjunction.push_back(disjunct->getAlias(disjunct->getSign()));
		}
		CNF.push_back(disjunction);
	}
}

void SAT::tseytinTransform(PropositionalFormula* formula)
{
	PropositionalFormula::resetNameCounter();
	std::vector<PropositionalFormula*> children = { formula };
	CNF = { {formula->getAlias(formula->getSign())} };
	while (children.size())
	{
		PropositionalFormula* child = children[0];
		children.erase(children.begin());
		switch (child->getType())
		{
		case PropositionalFormula::AND:
		{
			std::string commonDisjunct = child->getAlias(false);
			std::vector<std::string> longDisjunction = { child->getAlias(true) };
			std::vector<PropositionalFormula*> nextChildren = child->getChildren();
			for (PropositionalFormula* ch : nextChildren)
			{
				longDisjunction.push_back(ch->getAlias(!ch->getSign()));
				CNF.push_back({ commonDisjunct, ch->getAlias(ch->getSign()) });
			}
			CNF.push_back(longDisjunction);
			children.insert(children.end(), nextChildren.begin(), nextChildren.end());
			break;
		}
		case PropositionalFormula::OR:
		{
			std::string commonDisjunct = child->getAlias(true);
			std::vector<std::string> longDisjunction = { child->getAlias(false) };
			std::vector<PropositionalFormula*> nextChildren = child->getChildren();
			for (PropositionalFormula* ch : nextChildren)
			{
				longDisjunction.push_back(ch->getAlias(ch->getSign()));
				CNF.push_back({ commonDisjunct, ch->getAlias(!ch->getSign()) });
			}
			CNF.push_back(longDisjunction);
			children.insert(children.end(), nextChildren.begin(), nextChildren.end());
			break;
		}
		}
	}
}

std::map<std::string, int> SAT::getLiterals()
{
	std::map<std::string, int> literals;
	for (std::vector<std::string> conjunct : CNF)
	{
		for (std::string literal : conjunct)
		{
			if (literal[0] == '!')
			{
				std::string invLiteral = literal;
				invLiteral.erase(invLiteral.begin());
				literals.insert(std::pair<std::string, int>(invLiteral, -1));
			}
			else
			{
				literals.insert(std::pair<std::string, int>(literal, -1));
			}
		}
	}
	return literals;
}

bool SAT::DPLL(std::string chosen, std::map<std::string, int> literals, std::vector<std::vector<std::string>> cnf, std::map<std::string, int> literalsToAssign)
{
	std::string unit, unitInv;
	if (chosen.empty())
	{
		for (std::vector<std::string> conjunct : cnf)
		{
			if (conjunct.size() == 1)
			{
				unit = conjunct[0];
				break;
			}
		}
	}
	else
	{
		unit = chosen;
	}
	while (!unit.empty())
	{
		//Propagate
		if (unit[0] == '!')
		{
			unitInv = unit;
			unitInv.erase(unitInv.begin());
			literals[unitInv] = 0;
		}
		else
		{
			unitInv = "!" + unit;
			literals[unit] = 1;
		}
		//Elimination
		for (int i = cnf.size() - 1; i >= 0; i--)
		{
			if (std::find(cnf[i].begin(), cnf[i].end(), unit) != cnf[i].end())
			{
				cnf.erase(cnf.begin() + i);
			}
			else
			{
				std::vector<std::string>::iterator isFound = std::find(cnf[i].begin(), cnf[i].end(), unitInv);
				if (isFound != cnf[i].end())
				{
					cnf[i].erase(isFound);
				}
			}
		}
		unit.clear();
		for (std::vector<std::string> conjunct : cnf)
		{
			if (conjunct.size() == 1)
			{
				unit = conjunct[0];
				break;
			}
		}
	}
	if (cnf.empty())
	{
		for (std::pair<std::string, int> pair : literals)
		{
			if (pair.second != -1 && pair.first[0] != 't')
			{
				Solution.insert(pair);
			}
		}
		return true;
	}
	for (std::vector<std::string> conjunct : cnf)
	{
		if (conjunct.empty())
		{
			return false;
		}
	}
	std::string unassignedLiteral;
	int valueToAssign = -1;
	for (std::pair<std::string, int> literal : literalsToAssign)
	{
		if (literals[literal.first] == -1)
		{
			unassignedLiteral = literal.first;
			valueToAssign = literal.second;
			break;
		}
	}
	if (valueToAssign == -1)
	{
		for (std::pair<std::string, int> literal : literals)
		{
			if (literal.second == -1)
			{
				unassignedLiteral = literal.first;
				break;
			}
		}
		literals.insert(std::pair<std::string, int>(unassignedLiteral, 0));
		if (DPLL("!" + unassignedLiteral, literals, cnf))
		{
			return true;
		}
		literals.insert(std::pair<std::string, int>(unassignedLiteral, 1));
		return DPLL(unassignedLiteral, literals, cnf);
	}
	else if (valueToAssign == 1)
	{
		literals.insert(std::pair<std::string, int>(unassignedLiteral, 1));
		return DPLL(unassignedLiteral, literals, cnf, literalsToAssign);
	}
	else
	{
		literals.insert(std::pair<std::string, int>(unassignedLiteral, 0));
		if (DPLL("!" + unassignedLiteral, literals, cnf, literalsToAssign))
		{
			return true;
		}
		literals.insert(std::pair<std::string, int>(unassignedLiteral, 1));
		return DPLL(unassignedLiteral, literals, cnf);
	}
}

bool SAT::solve(std::map<std::string, int> literals)
{
	if (IsSolved == -1)
	{
		bool res = DPLL("", getLiterals(), CNF, literals);
		return res;
	}
	else
	{
		return IsSolved;
	}
}

std::map<std::string, int> SAT::getSolution()
{
	return Solution;
}