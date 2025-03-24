#include "__preprocessor__.h"

#include "dynamic_vs_static_polymorphism.hpp"

#ifdef BUILD_EXECUTABLE
int main(int argc, char* argv[])
{
    srand(time(NULL));
    CORE::clear_terminal();
    time_stamp("It just works");

    main_dynamic_static_polymorphism();

    return 0;
}
#endif