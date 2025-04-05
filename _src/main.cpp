#include "__preprocessor__.h"

#include "dynamic_vs_static_polymorphism.hpp"
#include "iterators.hpp"
#include "span.hpp"

#ifdef BUILD_EXECUTABLE
int main(int argc, char* argv[])
{
    srand(time(NULL));
    CORE::clear_terminal();
    time_stamp("It just works");

    // polymorphism_benchmark();

    // span_play();

    iterator_example();

    return 0;
}
#endif