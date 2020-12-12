#include <algorithm>
#include <vector>
#include "Random.h"
using namespace std;

class Company
{
public:
	Company();
	Company(const vector<double>& investment_coefficients, double money);
	double invest(double idea_value);
	double money;
	double starting_money;
	double get_profit();
private:
	vector<double> coefficients;
};

vector<double> operator+(const vector<double>& lhs, const vector<double>& rhs);
class Simulator
{
public:
	Simulator(vector<Company> companies);
	void step();
	vector<double> get_money_history();
	vector<double> get_current_money();
	vector<double> generate_ideas(int count);
	vector<double> ideas_noise(const vector<double>& ideas);
	double revenue(double idea_value, double investment);
private:
	vector<double> total_money_history;
	vector<Company> companies;
	double total_money;
	Random rng;

};