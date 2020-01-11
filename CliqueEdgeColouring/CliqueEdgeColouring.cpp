#include <iostream>
#include <fstream>
#include <algorithm>
#include <time.h>
#include "SAT.h"

PropositionalFormula* getCliqueFormula(int N, int col)
{
	PropositionalAnd* c = new PropositionalAnd();
	for (int i = 0; i < N; i++)
	{
		for (int j = i + 1; j < N; j++)
		{
			PropositionalOr* o = new PropositionalOr();
			for (int k = 0; k < col; k++)
			{
				PropositionalAnd *a = new PropositionalAnd();
				for (int l = 0; l < col; l++)
				{
					a->addConjunct(new PropositionalFormula((k == l), "x" + std::to_string(i) + "_" + std::to_string(j) + "_" + std::to_string(l)));
				}
				o->addDisjunct(a);
			}
			c->addConjunct(o);
		}
	}
	for (int i = 0; i < N; i++)
	{
		for (int j = i + 1; j < N; j++)
		{
			for (int q = j + 1; q < N; q++)
			{
				for (int k = 0; k < col; k++)
				{
					PropositionalAnd* o = new PropositionalAnd(false);
					o->addConjunct(new PropositionalFormula(true, "x" + std::to_string(i) + "_" + std::to_string(j) + "_" + std::to_string(k)));
					o->addConjunct(new PropositionalFormula(true, "x" + std::to_string(i) + "_" + std::to_string(q) + "_" + std::to_string(k)));
					o->addConjunct(new PropositionalFormula(true, "x" + std::to_string(j) + "_" + std::to_string(q) + "_" + std::to_string(k)));
					c->addConjunct(o);
				}
			}
		}
	}
	return c;
}

int main(int argc, char *argv[])
{
	int col;
	std::cout << "Number of colours = ";
	std::cin >> col;
	int N = 2;
	char toPrint;
	std::cout << "Should edge colouring be printed? (Y/n)\n";
	std::cin >> toPrint;
	while (true)
	{
		PropositionalFormula* formula = getCliqueFormula(N, col)->getSimplified();
		SAT sat = SAT(formula);
		clock_t cl1 = clock(), cl2;
		bool isSolved = sat.solve();
		cl2 = clock();
		if (isSolved)
		{
			std::cout << "Solved for " << N << " in " << ((cl2 - cl1) * 1.0 / CLOCKS_PER_SEC) << " s\n";
			if (toPrint != 'n')
			{
				std::map<std::string, int> solution = sat.getSolution();
				for (std::pair<std::string, int> pair : solution)
				{
					if (pair.second)
					{
						std::cout << "Edge ";
						int i = 1;
						while (pair.first[i] != '_')
						{
							std::cout << pair.first[i];
							i++;
						}
						std::cout << " - ";
						i++;
						while (pair.first[i] != '_')
						{
							std::cout << pair.first[i];
							i++;
						}
						std::cout << " coloured in ";
						i++;
						while (i < pair.first.size())
						{
							std::cout << pair.first[i];
							i++;
						}
						std::cout << "\n";
					}
				}
				std::cout << "\n";
			}
		}
		else
		{
			std::cout << "Unsolved for " << N << "\n";
			return 0;
		}
		N++;
	}
}