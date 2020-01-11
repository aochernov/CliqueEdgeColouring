#include "PropositionalFormula.h"

int PropositionalFormula::NameCounter;

PropositionalFormula::PropositionalFormula(bool sign, std::string name)
{
	Type = VARIABLE;
	Sign = sign;
	if (name == "0")
	{
		Sign = !Sign;
		Name = "";
	}
	else if (name == "1")
	{
		Name = "";
	}
	else
	{
		Name = name;
	}
}

void PropositionalFormula::negate()
{
	Sign = !Sign;
}

PropositionalFormula::FormulaType PropositionalFormula::getType()
{
	return Type;
}

bool PropositionalFormula::getSign()
{
	return Sign;
}

int PropositionalFormula::getValue()
{
	if (Name.empty())
	{
		return Sign;
	}
	else
	{
		return -1;
	}
}

std::string PropositionalFormula::getFormula()
{
	switch (getValue())
	{
	case -1:
		return (Sign ? Name : ("!" + Name));
	case 0:
		return "0";
	case 1:
		return "1";
	}
}

PropositionalFormula* PropositionalFormula::getSimplified()
{
	return this;
}

std::vector<PropositionalFormula*> PropositionalFormula::getChildren()
{
	return { this };
}

std::string PropositionalFormula::getAlias(bool sign)
{
	if (Name.empty())
	{
		if (getType() == VARIABLE)
		{
			return std::to_string(sign ? Sign : !Sign);
		}
		else
		{
			Name = "t" + std::to_string(NameCounter);
			NameCounter++;
		}
	}
	return (sign ? Name : ("!" + Name));
}

void PropositionalFormula::resetNameCounter()
{
	NameCounter = 1;
}

PropositionalAnd::PropositionalAnd(bool sign) : PropositionalFormula(sign, "1")
{
	Defined = false;
	Type = AND;
}

void PropositionalAnd::addConjunct(PropositionalFormula* conjunct)
{
	if (Defined)
	{
		return;
	}
	switch (conjunct->getValue())
	{
	case -1:
		Conjuncts.push_back(conjunct->getSimplified());
		break;
	case 0:
		Defined = true;
		Conjuncts.clear();
		break;
	}
}

std::string PropositionalAnd::getFormula()
{
	switch (getValue())
	{
	case -1:
		if (Conjuncts.size() == 1)
		{
			return (Sign ? Conjuncts[0]->getFormula() : ("!" + Conjuncts[0]->getFormula()));
		}
		else
		{
			std::string formula = (Sign ? "(" : "!(") + Conjuncts[0]->getFormula();
			for (int i = 1; i < Conjuncts.size(); i++)
			{
				formula.append(" & " + Conjuncts[i]->getFormula());
			}
			formula.append(")");
			return formula;
		}
	case 0:
		return "0";
	case 1:
		return "1";
	}
}

int PropositionalAnd::getValue()
{
	if (Defined)
	{
		return 0;
	}
	else if (Conjuncts.size())
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

PropositionalFormula* PropositionalAnd::getSimplified()
{
	switch (getValue())
	{
	case -1:
		return ((Conjuncts.size() == 1) ? Conjuncts[0] : this);
	case 0:
		return new PropositionalFormula(true, "0");
	case 1:
		return new PropositionalFormula(true, "1");
	}
}

std::vector<PropositionalFormula*> PropositionalAnd::getChildren()
{
	return Conjuncts;
}

PropositionalOr::PropositionalOr(bool sign) : PropositionalFormula(sign, "1")
{
	Defined = false;
	Type = OR;
}

void PropositionalOr::addDisjunct(PropositionalFormula* disjunct)
{
	if (Defined)
	{
		return;
	}
	switch (disjunct->getValue())
	{
	case -1:
		Disjuncts.push_back(disjunct->getSimplified());
		break;
	case 1:
		Defined = true;
		Disjuncts.clear();
		break;
	}
}

std::string PropositionalOr::getFormula()
{
	switch (getValue())
	{
	case -1:
		if (Disjuncts.size() == 1)
		{
			return (Sign ? Disjuncts[0]->getFormula() : ("!" + Disjuncts[0]->getFormula()));
		}
		else
		{
			std::string formula = (Sign ? "(" : "!(") + Disjuncts[0]->getFormula();
			for (int i = 1; i < Disjuncts.size(); i++)
			{
				formula.append(" | " + Disjuncts[i]->getFormula());
			}
			formula.append(")");
			return formula;
		}
	case 0:
		return "0";
	case 1:
		return "1";
	}
}

int PropositionalOr::getValue()
{
	if (Defined)
	{
		return 1;
	}
	else if (Disjuncts.size())
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

PropositionalFormula* PropositionalOr::getSimplified()
{
	switch (getValue())
	{
	case -1:
		return ((Disjuncts.size() == 1) ? Disjuncts[0] : this);
	case 0:
		return new PropositionalFormula(true, "0");
	case 1:
		return new PropositionalFormula(true, "1");
	}
}

std::vector<PropositionalFormula*> PropositionalOr::getChildren()
{
	return Disjuncts;
}