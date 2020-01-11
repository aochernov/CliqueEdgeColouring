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
				o->addDisjunct(new PropositionalFormula(true, "x" + std::to_string(i) + "_" + std::to_string(j) + "_" + std::to_string(k)));
				for (int l = k + 1; l < col; l++)
				{
					PropositionalOr* d = new PropositionalOr();
					d->addDisjunct(new PropositionalFormula(false, "x" + std::to_string(i) + "_" + std::to_string(j) + "_" + std::to_string(k)));
					d->addDisjunct(new PropositionalFormula(false, "x" + std::to_string(i) + "_" + std::to_string(j) + "_" + std::to_string(l)));
					c->addConjunct(d);
				}
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
					PropositionalOr* o = new PropositionalOr();
					o->addDisjunct(new PropositionalFormula(false, "x" + std::to_string(i) + "_" + std::to_string(j) + "_" + std::to_string(k)));
					o->addDisjunct(new PropositionalFormula(false, "x" + std::to_string(i) + "_" + std::to_string(q) + "_" + std::to_string(k)));
					o->addDisjunct(new PropositionalFormula(false, "x" + std::to_string(j) + "_" + std::to_string(q) + "_" + std::to_string(k)));
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
	std::map<std::string, int> solution;
	while (true)
	{
		PropositionalFormula* formula = getCliqueFormula(N, col)->getSimplified();
		SAT sat = SAT(formula, true);
		clock_t cl1 = clock(), cl2;
		bool isSolved = sat.solve(solution);
		cl2 = clock();
		if (isSolved)
		{
			solution = sat.getSolution();
			std::cout << "Solved for " << N << " in " << ((cl2 - cl1) * 1.0 / CLOCKS_PER_SEC) << " s\n";
			if (toPrint != 'n')
			{
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