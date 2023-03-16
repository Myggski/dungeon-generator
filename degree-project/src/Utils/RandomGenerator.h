#pragma once

#include <random>

namespace Utils
{
    template<class T>
    using uniform_distribution =
    std::conditional_t<
        std::is_floating_point_v<T>,
        std::uniform_real_distribution<T>,
        std::conditional_t<
			std::is_integral_v<T>,
			std::uniform_int_distribution<T>,
			void
		>
    >;

    class RandomGenerator
    {
    public:
        static void CreateInstance();
        static RandomGenerator& GetInstance();
        std::mt19937& GetEngine();
        uint32_t GetSeed() const;
        void SetSeed(uint32_t NewSeed);
        void RandomizeSeed();

        template<class T>
        T GetRandom(T Min = std::numeric_limits<T>::min(), T Max = std::numeric_limits<T>::max()) {
            uniform_distribution<T> RandomDistribution(Min, Max);
            return RandomDistribution(RandomEngine);
        }
    private:
        RandomGenerator();

    private:
        static std::unique_ptr<RandomGenerator> Instance;
        uint32_t Seed;
        std::mt19937 RandomEngine;
    };
}