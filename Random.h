#pragma once
#include <map>
#include <random>
#include <vector>
#include <numeric>
using namespace std;
class Random
{
public:
	Random(size_t seed = std::random_device()());
	double get_uniform_double(double lower_bound, double upper_bound);
	int get_uniform_int(int lower_bound, int upper_bound);
	double get_normal_double(double stddev, double mean);
	vector<double> random_vector_normal(int size, double stddev, double mean);
	vector<double> random_vector_uniform(int size, double min, double max);

	/// <summary>
	/// Reorders the given vector randomly
	/// </summary>
	template <class t>
	void random_shuffle(vector<t>& elements);
	/// <summary>
	/// Produces a vector of required length (count) by picking random elements
	/// with given probabilities
	/// </summary>
	template<typename t>
	vector<t> random_choices(const vector<t>& elements, vector<double> probabilities, int count);

	std::mt19937 mersenne_twister;
private:
	std::map<std::pair<int, int>, std::uniform_int_distribution<int>> integer_uniform_distributions;
	std::map<std::pair<double, double>, std::uniform_real_distribution<double>> real_uniform_distributions;
	std::map<std::pair<int, int>, std::normal_distribution<double>> real_normal_distributions;
};



template<class t>
inline void Random::random_shuffle(vector<t>& elements)
{
	for (int i = elements.size() - 1; i > 0; i--)
	{
		int rand_pos = rand_int(i);
		swap(elements[i], elements[rand_pos]);
	}
}

template<typename t>
vector<t> Random::random_choices(const vector<t>& elements, vector<double> probabilities, int count)
{
	vector<t> choices; choices.reserve(count);
	// Make sure probabilities are in range [0,1]
	double probs_sum = accumulate(probabilities.begin(), probabilities.end(), 0.0);
	for (size_t i = 0; i < probabilities.size(); i++)
		probabilities[i] /= probs_sum;

	// Pair up probabilities with elements and sort
	vector<pair<double, t>> probs_elements(elements.size());
	for (size_t i = 0; i < elements.size(); i++)
		probs_elements[i] = { probabilities[i],elements[i] };

	sort(probs_elements.begin(), probs_elements.end(),
		[](const pair<double, t>& lhs, const pair<double, t>& rhs) {return lhs.first < rhs.first;});

	for (size_t i = 0; i < count; i++)
	{
		double rand_value = get_uniform_double(0, 1);
		double current_prob_sum = 0;
		for (size_t j = 0; j < probs_elements.size(); j++)
		{
			current_prob_sum += probs_elements[j].first;
			if (rand_value <= current_prob_sum || j == probs_elements.size() - 1)
			{
				choices.push_back(probs_elements[j].second);
				break;
			}
		}
	}
	return choices;
}