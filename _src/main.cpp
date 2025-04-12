#include "__preprocessor__.h"

#include "adding_iterators_and_hash_to_custom_struct.hpp"
#include "boost_serialization.hpp"
#include "dynamic_vs_static_polymorphism.hpp"
#include "ram_access.hpp"
#include "span.hpp"

#ifdef BUILD_EXECUTABLE
int main(int argc, char* argv[])
{
    srand(time(NULL));
    // CORE::clear_terminal();
    time_stamp("It just works");

    {
        // polymorphism_benchmark();
    }

    {
        // span_play();
    }

    {
        // hash_example();
        // iterator_example();
    }

    {
        ram_access();
    }

    return 0;
}
#endif