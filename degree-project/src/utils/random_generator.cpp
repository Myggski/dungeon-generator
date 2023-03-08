#include "random_generator.h"

namespace utils
{
	random_generator::random_generator()
        : seed(0),
		random_engine(seed)
    {
        randomize_seed();
    }

    unsigned int random_generator::get_seed() const
    {
        return seed;
    }

    void random_generator::set_seed(unsigned int new_seed)
    {
        seed = new_seed;
        random_engine = std::mt19937(seed);
    }

    void random_generator::randomize_seed()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<unsigned int> dist;
        seed = dist(gen);
        random_engine = std::mt19937(seed);
    }
}