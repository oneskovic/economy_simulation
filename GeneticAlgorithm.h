#include "Random.h"
#include "Simulator.h"
class HyperParameters
{
public:
	HyperParameters();
	int generation_count;
	int population_size;
	int genome_size;
	double mutation_stdev;
	int mutation_stddev_anneal_time;
	int evaluation_steps;
};

class Individual
{
public:
	Individual(vector<double> genome);
	Individual();
	vector<double> genome;
	double score;
	bool operator<(const Individual& other)
	{
		return score < other.score;
	}
};

class GenerationReport
{
public:
	GenerationReport(double min_score, double average_score, double max_score);
	double min_score;
	double average_score;
	double max_score;
};


class GeneticAlgorithm
{
public:
	GeneticAlgorithm(HyperParameters hyperparameters);
	vector<Individual> train();
private:
	vector<Individual> get_random_generation();
	/// <summary>
	/// Evaluates the given generation and returns a generation report
	/// </summary>
	GenerationReport evaluate_generation(vector<Individual>& generation);
	vector<Individual> make_new_generation(const vector<Individual>& generation);
	void mutate_generation(vector<Individual>& generation, double mutation_stddev);
	Individual make_child(const Individual& parent1, const Individual& parent2);

	Random rng;
	HyperParameters hparams;
};