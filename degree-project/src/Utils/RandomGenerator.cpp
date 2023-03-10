#include "RandomGenerator.h"

namespace Utils
{
	RandomGenerator::RandomGenerator()
        : Seed(0),
		RandomEngine(Seed)
    {
        RandomizeSeed();
    }

    uint32_t RandomGenerator::GetSeed() const
    {
        return Seed;
    }

    void RandomGenerator::SetSeed(uint32_t NewSeed)
    {
        Seed = NewSeed;
        RandomEngine = std::mt19937(Seed);
    }

    void RandomGenerator::RandomizeSeed()
    {
        std::random_device RandomDevice;
        std::mt19937 RandomEngine(RandomDevice());
        std::uniform_int_distribution<uint32_t> RandomDistribution;
        Seed = RandomDistribution(RandomEngine);
        RandomEngine = std::mt19937(Seed);
    }
}