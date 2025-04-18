#include "__preprocessor__.h"
#include <indicators/progress_bar.hpp>

void progress_bar()
{
    using namespace indicators;
    ProgressBar bar{option::BarWidth{50},
                    option::Start{"["},
                    option::Fill{"="},
                    option::Lead{">"},
                    option::Remainder{" "},
                    option::End{"]"},
                    option::PostfixText{"Extracting Archive"},
                    option::ForegroundColor{Color::green},
                    option::FontStyles{std::vector<FontStyle>{FontStyle::bold}}};

    // Update bar state
    while (true)
    {
        bar.tick();
        if (bar.is_completed()) break;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}