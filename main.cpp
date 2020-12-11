#include <iostream>
#include <algorithm>
#include <vector>
#include <functional>
#include <numeric>
#include "Simulator.h"
#include "matplotlibcpp.h"
using namespace std;
namespace plt = matplotlibcpp;
int main()
{
	int iteration_count = 100;
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
	plt::show();

	return 0;
}