#include "__preprocessor__.h"

#include "adding_iterators_and_hash_to_custom_struct.hpp"
#include "boost_serialization.hpp"
#include "dynamic_vs_static_polymorphism.hpp"
#include "gnu_plot.hpp"
#include "likely_unlikely.hpp"
#include "progress_bar.hpp"
#include "ram_access.hpp"
#include "singleton.hpp"
#include "span.hpp"
#include "tipping_buffer.hpp"
#include "vector_custom_allocator_and_overloaded_global_new_and_delete.hpp"
#include "xor_linked_list.hpp"

#ifdef BUILD_EXECUTABLE
int main(int argc, char* argv[])
{
    srand(time(NULL));
    // CORE::clear_terminal();
    time_stamp("It just works");

    // var(NUM(1, 000));
    // var(NUM(1, 000, 000));
    // var(NUM(1, 000, 000, 000));
    // var(NUM(1, 000, 000, 000, 000));

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
        // ram_access();
    }

    {
        // gnu_plot();
        // progress_bar();
    }

    {
        // smart_pointers(); // TODO
    }

    {
        // boost_serialization();
    }

    {
        // vector_custom_allocator_and_overloaded_global_new_and_delete();
    }

    {
        // trippin();
    }

    {
        // singletining();
    }

    {
        // likely_unlikelying();
    }

    {
        xoiring();
    }

    time_stamp("End of main");

    return 0;
}
#endif