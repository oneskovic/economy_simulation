#include "Simulator.h"

Simulator::Simulator(vector<Company> companies)
{
	this->companies = companies;
}

void Simulator::step()
{
	auto ideas = generate_ideas(companies.size());
	auto idea_perceptions = ideas_noise(ideas);
	for (size_t company = 0; company < companies.size(); company++)
	{
		double investment = companies[company].invest(idea_perceptions[company]);
		double company_revenue = revenue(ideas[company], investment);
		companies[company].money += company_revenue - investment;
		total_money += company_revenue - investment;
		total_money_history.push_back(total_money);
	}

}

vector<double> Simulator::get_money_history()
{
	return total_money_history;
}

vector<double> Simulator::get_current_money()
{
	vector<double> money(companies.size());
	for (size_t i = 0; i < companies.size(); i++)
		money[i] = companies[i].money;
	return money;
}

vector<double> operator+(const vector<double>& lhs, const vector<double>& rhs)
{
	if (lhs.size() != rhs.size())
		throw "Jedi govna";

	vector<double> result(lhs.size());
	for (size_t i = 0; i < lhs.size(); i++)
		result[i] = lhs[i] + rhs[i];
	return result;
}

vector<double> Simulator::random_vector(int size, double stddev, double mean)
{
	vector<double> elements(size);
	for (size_t i = 0; i < size; i++)
		elements[i] = rng.get_normal_double(stddev, mean);
	return elements;
}

vector<double> Simulator::generate_ideas(int count)
{
	double mean = 0.5;
	double standard_deviation = 0.1;
	auto ideas = random_vector(count, standard_deviation, mean);
	return ideas;
}

vector<double> Simulator::ideas_noise(const vector<double>& ideas)
{
	double mean = 0, standard_deviation = 0.4;
	auto noise = random_vector(ideas.size(), standard_deviation, mean);
	vector<double> idea_perceptions = ideas + noise;
	for (size_t i = 0; i < idea_perceptions.size(); i++)
	{
		if (idea_perceptions[i] < 0)
			idea_perceptions[i] = 0;
		if (idea_perceptions[i] > 1)
			idea_perceptions[i] = 1;
	}
	return idea_perceptions;
}

double Simulator::revenue(double idea_value, double investment)
{
	if (idea_value < 0.2)
		investment *= 0.5;
	else if (idea_value < 0.5)
		investment *= 0.9;
	else if (idea_value < 0.6)
		investment *= 1.2;
	else if (idea_value < 0.7)
		investment *= 1.5;
	else if (idea_value < 0.8)
		investment *= 2.5;
	else if (idea_value < 0.9)
		investment *= 4;
	else if (idea_value < 0.95)
		investment *= 6;
	else if (idea_value < 0.99)
		investment *= 8;
	else
		investment *= 10;
	return investment;
}

Company::Company()
{
	coefficients = vector<double>();
	money = 0;
	starting_money = 0;
}

Company::Company(const vector<double>& investment_coefficients, double money)
{
	coefficients = investment_coefficients;
	this->money = money;
	starting_money = money;
}

double Company::invest(double idea_value)
{
	if (idea_value > 1)
		throw "Jedi govna";

	// Apply the investment function given by company
	// investment coefficients
	double investment = max(0.0, money);
	double step_size = 1.0001 / coefficients.size();
	double current_idea_value = step_size;
	for (size_t i = 0; i < coefficients.size(); i++)
	{
		if (idea_value <= current_idea_value)
		{
			investment *= coefficients[i];
			break;
		}
		current_idea_value += step_size;
	}
	return investment;
}

double Company::get_profit()
{
	return money - starting_money;
}
