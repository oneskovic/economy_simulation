#include "GeneticAlgorithm.h"
#include <iostream>

GeneticAlgorithm::GeneticAlgorithm(HyperParameters hyperparameters)
{
	hparams = hyperparameters;
}

vector<Individual> GeneticAlgorithm::train()
{
	double stddev_step = hparams.mutation_stdev / hparams.mutation_stddev_anneal_time;
	double current_mutation_stddev = hparams.mutation_stdev;
	vector<Individual> current_generation = get_random_generation();

	for (size_t i = 0; i < hparams.generation_count; i++)
	{
		auto report = evaluate_generation(current_generation);
		current_generation = make_new_generation(current_generation);
		mutate_generation(current_generation,current_mutation_stddev);
		current_mutation_stddev -= stddev_step;
		cout << "Generation: " << i << "\n";
		cout << "Min score: " << report.min_score << "\n"
			 << "Average score: " << report.average_score << "\n"
			 << "Max score: " << report.max_score << "\n";
	}

	return current_generation;
}

vector<Individual> GeneticAlgorithm::get_random_generation()
{
	vector<Individual> random_generation(hparams.population_size);
	for (size_t i = 0; i < hparams.population_size; i++)
		random_generation[i].genome = rng.random_vector_uniform(hparams.genome_size, 0, 1);

	return random_generation;
}

GenerationReport GeneticAlgorithm::evaluate_generation(vector<Individual>& generation)
{

	vector<Company> companies(generation.size());
	for (size_t i = 0; i < generation.size(); i++)
		companies[i] = Company(generation[i].genome, 100);
	
	auto simulator = Simulator(companies);
	for (size_t i = 0; i < hparams.evaluation_steps; i++)
		simulator.step();

	double min_score = numeric_limits<double>::max(), max_score = numeric_limits<double>::lowest(), sum_of_scores = 0;
	auto money_at_end = simulator.get_current_money();
	for (size_t i = 0; i < generation.size(); i++)
	{
		generation[i].score = money_at_end[i];
		min_score = min(min_score, money_at_end[i]);
		max_score = max(max_score, money_at_end[i]);
		sum_of_scores += money_at_end[i];
	}

	return GenerationReport(min_score, sum_of_scores / generation.size(), max_score);
}

vector<Individual> GeneticAlgorithm::make_new_generation(const vector<Individual>& generation)
{
	auto scores = vector<double>(generation.size());
	for (size_t i = 0; i < generation.size(); i++)
		scores[i] = generation[i].score;
	
	vector<Individual> individuals_to_reporduce = rng.random_choices(generation, scores, generation.size() * 2);
	vector<Individual> new_generation; new_generation.reserve(generation.size());
	for (size_t i = 0; i < individuals_to_reporduce.size(); i+=2)
		new_generation.push_back(make_child(individuals_to_reporduce[i], individuals_to_reporduce[i + 1]));

	return new_generation;
}

void GeneticAlgorithm::mutate_generation(vector<Individual>& generation, double mutation_stddev)
{
	for (size_t individual = 0; individual < generation.size(); individual++)
	{
		auto mutations = rng.random_vector_normal(generation.size(), mutation_stddev, 0);
		for (size_t i = 0; i < generation[individual].genome.size(); i++)
		{
			generation[individual].genome[i] += generation[individual].genome[i] * mutations[i];
			if (generation[individual].genome[i] < 0)
			{
				generation[individual].genome[i] = 0;
			}
			if (generation[individual].genome[i] > 1)
			{
				generation[individual].genome[i] = 1;
			}
		}
	}
}

Individual GeneticAlgorithm::make_child(const Individual& parent1, const Individual& parent2)
{
	if (parent1.genome.size() != parent2.genome.size())
		throw exception("Incompatible genomes");
	
	vector<double> child_coefficients(parent1.genome.size());
	for (size_t i = 0; i < parent1.genome.size(); i++)
	{
		child_coefficients[i] = (parent1.genome[i] + parent2.genome[i]) / 2.0;
		if (child_coefficients[i] < 0)
		{
			child_coefficients[i] = 0;
		}
		if (child_coefficients[i] > 1)
		{
			child_coefficients[i] = 1;
		}
	}
	return Individual(child_coefficients);
}

Individual::Individual(vector<double> genome)
{
	this->genome = genome;
}

Individual::Individual()
{
}

HyperParameters::HyperParameters()
{
}

GenerationReport::GenerationReport(double min_score, double average_score, double max_score)
{
	this->min_score = min_score;
	this->average_score = average_score;
	this->max_score = max_score;
}
