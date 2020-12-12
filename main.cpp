#include <iostream>
#include <algorithm>
#include <vector>
#include <functional>
#include <numeric>
#include "matplotlibcpp.h"
#include "GeneticAlgorithm.h"
using namespace std;
namespace plt = matplotlibcpp;
int main()
{
	/*int iteration_count = 50;
	int company_count = 10000;
	int starting_money = 100;
	
	vector<Company> starting_companies(company_count);
	vector<double> coefficients = {0,0,0,0,0,0,0.05,0.1,0.15,0.2};
	for (size_t i = 0; i < company_count; i++)
		starting_companies[i] = Company(coefficients, starting_money);
	

	Simulator simulator = Simulator(starting_companies);
	
	for (size_t iteration = 0; iteration < iteration_count; iteration++)
	{
		simulator.step();        
		cout << "Iteration:" << iteration << "\n";
	}
	auto money = simulator.get_current_money();
	sort(money.begin(), money.end());
	plt::hist(money,30);
	auto total_money_history = simulator.get_money_history();
	//plt::plot(total_money_history);
	plt::show();*/

	HyperParameters hparams;
	hparams.population_size = 100;
	hparams.evaluation_steps = 50;
	hparams.generation_count = 500;
	hparams.genome_size = 30;
	hparams.mutation_stdev = 0.5;
	hparams.mutation_stddev_anneal_time = 80;
	GeneticAlgorithm ga = GeneticAlgorithm(hparams);
	auto trained = ga.train();

	auto companies = vector<Company>(trained.size());
	for (size_t i = 0; i < companies.size(); i++)
	{
		companies[i] = Company(trained[i].genome, 100);
	}
	auto sim = Simulator(companies);
	for (size_t i = 0; i < 50; i++)
	{
		sim.step();
	}
	auto money = sim.get_current_money();
	plt::hist(money, 30);
	plt::show();
	/*vector<double> x(hparams.genome_size);
	double step_size = 1.0001 / hparams.genome_size;
	double current_idea_value = step_size;
	for (size_t i = 0; i < hparams.genome_size; i++)
	{
		x[i] = current_idea_value;
		current_idea_value += step_size;
	}

	for (size_t _i = 0; _i < 20; _i++)
	{
		plt::plot(x,trained[_i].genome);
		cout << "\n";
	}
	plt::show();*/
	return 0;
}