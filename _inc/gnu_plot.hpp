#include "__preprocessor__.h"
#include "gplot++.h"

void gnu_plot()
{
    Gnuplot gnuplot{};
    std::vector<double> x{1, 2, 3, 4, 5}, y{5, 2, 4, 1, 3};

    gnuplot.redirect_to_png("output.png", "800,600");

    gnuplot.plot(x, y);
    gnuplot.show();
}
