#pragma once

#include <random>

// Random number generator
class CRandom
{
public:

	 CRandom(void) {Seed();}
	~CRandom(void) {}

	static void		Seed(void);

	static uint32_t RandomUint(void);
	static uint32_t RandomUint(const uint32_t Min, const uint32_t Max);

private:

	static std::mt19937												m_sRandomEngine;
	static std::uniform_int_distribution<std::mt19937::result_type>	m_sDistribution;

};