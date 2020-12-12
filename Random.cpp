#include "Random.h"

Random::Random(size_t seed)
{
	mersenne_twister = std::mt19937(seed);
}

/// <summary>
/// Produces a uniform random double in range [lower_bound,upper_bound]
/// </summary>
double Random::get_uniform_double(double lower_bound, double upper_bound)
{
	std::pair<double, double> interval = std::pair<double, double>(lower_bound, upper_bound);
	if (real_uniform_distributions.find(interval) == real_uniform_distributions.end()) //distribution not found
	{
		//Create new distribution
		real_uniform_distributions[interval] = std::uniform_real_distribution<double>(lower_bound, upper_bound);
	}
	return real_uniform_distributions[interval](mersenne_twister);
}

/// <summary>
/// Produces a uniform random integer in range [lower_bound,upper_bound]
/// </summary>
int Random::get_uniform_int(int lower_bound, int upper_bound)
{
	std::pair<int, int> interval = std::pair<int, int>(lower_bound, upper_bound);
	if (integer_uniform_distributions.find(interval) == integer_uniform_distributions.end()) //distribution not found
	{
		//Create new distribution
		integer_uniform_distributions[interval] = std::uniform_int_distribution<int>(lower_bound, upper_bound);
	}
	return integer_uniform_distributions[interval](mersenne_twister);
}

double Random::get_normal_double(double stddev, double mean)
{
	std::pair<int, int> parameters = std::pair<int, int>(stddev, mean);
	if (real_normal_distributions.find(parameters) == real_normal_distributions.end()) //distribution not found
	{
		//Create new distribution
		real_normal_distributions[parameters] = std::normal_distribution<double>(mean, stddev);
	}
	return real_normal_distributions[parameters](mersenne_twister);
}

vector<double> Random::random_vector_normal(int size, double stddev, double mean)
{
	vector<double> elements(size);
	for (size_t i = 0; i < size; i++)
		elements[i] = get_normal_double(stddev, mean);
	return elements;
}

vector<double> Random::random_vector_uniform(int size, double min, double max)
{
	vector<double> elements(size);
	for (size_t i = 0; i < size; i++)
		elements[i] = get_uniform_double(min, max);
	return elements;
}

