#include "RandomGenerator.h"

namespace Utils
{
    std::unique_ptr<RandomGenerator> RandomGenerator::Instance = nullptr;

	RandomGenerator::RandomGenerator()
        : Seed(0),
		RandomEngine(Seed)
    {
        RandomizeSeed();
    }

    void RandomGenerator::CreateInstance()
    {
        if (Instance != nullptr) {
            throw std::logic_error("Instance of CommandStack is already created!");
        }

        auto rnd = RandomGenerator();
        rnd.RandomizeSeed();

        Instance = std::make_unique<RandomGenerator>(rnd);
    }

    RandomGenerator& RandomGenerator::GetInstance()
    {
        if (Instance == nullptr)
        {
            throw std::logic_error("Instance of CommandStack has not been created yet, or has been destroyed!");
        }

        return *Instance;
    }

    std::mt19937& RandomGenerator::GetEngine()
    {
        return RandomEngine;
    }

    uint32_t RandomGenerator::GetSeed() const
    {
        return Seed;
    }

    void RandomGenerator::SetSeed(uint32_t NewSeed)
    {
        Seed = NewSeed;
        RandomEngine = std::mt19937(Seed);
        RandomEngine.seed(Seed);
    }

    void RandomGenerator::RandomizeSeed()
    {
        std::random_device RandomDevice;
        std::mt19937 RandomSeedEngine(RandomDevice());
        std::uniform_int_distribution<uint32_t> RandomDistribution;
        Seed = RandomDistribution(RandomSeedEngine);
        RandomEngine = std::mt19937(Seed);
    }
}