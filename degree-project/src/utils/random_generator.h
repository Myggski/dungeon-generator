#pragma once

#include <random>

namespace utils
{
    template<class T>
    using uniform_distribution =
        typename std::conditional<
        std::is_floating_point<T>::value,
        std::uniform_real_distribution<T>,
        typename std::conditional<
        std::is_integral<T>::value,
        std::uniform_int_distribution<T>,
        void
        >::type
        >::type;

    class random_generator
    {
    public:
        static random_generator& instance()
        {
            static random_generator instance;
            return instance;
        }
        
        unsigned int get_seed() const;
        void set_seed(unsigned int new_seed);
        void randomize_seed();

        template<class T>
        T get_random(T min = std::numeric_limits<T>::min(), T max = std::numeric_limits<T>::max()) {
            uniform_distribution<T> dis(min, max);
            return dis(random_engine);
        }
    private:
        random_generator();

    private:
        unsigned int seed;
        std::mt19937 random_engine;
    };
}