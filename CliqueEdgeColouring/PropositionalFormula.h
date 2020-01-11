#pragma once
#include <string>
#include <vector>

class PropositionalFormula //Variables
{
public:
	enum FormulaType { VARIABLE, OR, AND };

protected:
	bool Sign;
	std::string Name;
	FormulaType Type;
	static int NameCounter;

public:
	PropositionalFormula(bool sign, std::string name);
	void negate();
	bool getSign();
	FormulaType getType();
	std::string getAlias(bool sign);
	virtual int getValue();
	virtual std::string getFormula();
	virtual PropositionalFormula* getSimplified();
	virtual std::vector<PropositionalFormula*> getChildren();
	static void resetNameCounter();
};

class PropositionalAnd : public PropositionalFormula
{
private:
	bool Defined;
	std::vector<PropositionalFormula*> Conjuncts;

public:
	PropositionalAnd(bool sign = true);
	void addConjunct(PropositionalFormula* conjunct);
	virtual int getValue() override;
	virtual std::string getFormula() override;
	virtual PropositionalFormula* getSimplified() override;
	virtual std::vector<PropositionalFormula*> getChildren() override;
};

class PropositionalOr : public PropositionalFormula
{
private:
	bool Defined;
	std::vector<PropositionalFormula*> Disjuncts;

public:
	PropositionalOr(bool sign = true);
	void addDisjunct(PropositionalFormula* disjunct);
	virtual int getValue() override;
	virtual std::string getFormula() override;
	virtual PropositionalFormula* getSimplified() override;
	virtual std::vector<PropositionalFormula*> getChildren() override;
};