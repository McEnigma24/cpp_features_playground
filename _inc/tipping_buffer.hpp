#include <algorithm>
#include <cstdint>
#include <cstring>
#include <optional>
#include <tuple>
#include <vector>

// --- TippingBuffer Templates ---
template <bool active>
class TippingBuffer;

// true / false -> sets TippingBuffer active or no //

// if it's false - values get right-throught it, and obj does not wast memory space
// if it's true - values are summed together until num of entry limit is reached, that summed value is released (mechanism to get slot stats with
// granularity measurement)

template <>
class TippingBuffer<true>
{
    size_t tippingThreshold;
    uint64_t sum;
    uint64_t count;

public:
    TippingBuffer() : tippingThreshold(1) { resetBuffer(); }

    void setThreshold(size_t _tippingThreshold) { tippingThreshold = _tippingThreshold; }

    void resetBuffer()
    {
        sum = 0;
        count = 0;
    }

    std::optional<uint64_t> add(uint64_t value)
    {
        sum += value; // accumulating
        // count++;
        linee("add");
        varr(value);
        varr(sum);
        varr(count);
        if (++count == tippingThreshold)
        {
            line("RELEASE");
            uint64_t result = sum;
            resetBuffer();
            return result; // RELEASE
        }
        line("accumulating");
        return std::nullopt;
    }
};

template <>
class TippingBuffer<false>
{
public:
    TippingBuffer() = default;

    void setThreshold(size_t) {}

    void resetBuffer() {}

    std::optional<uint64_t> add(uint64_t value) { return value; }
};

// --- StatsPack Template ---
template <bool active>
class StatsPack
{
    TippingBuffer<active> tippingBuffer;
    uint64_t min;
    uint64_t max;
    uint64_t sum;
    uint64_t count;

public:
    StatsPack() { reset(); }
    void setThreshold(size_t threshold) { tippingBuffer.setThreshold(threshold); }

    void reset()
    {
        tippingBuffer.resetBuffer();
        min = UINT64_MAX;
        max = 0;
        sum = 0;
        count = 0;
    }

    void add(uint64_t p)
    {
        auto result = tippingBuffer.add(p); // accumulating
        if (!result.has_value()) return;

        p = result.value();
        if (p < min) min = p;
        if (p > max) max = p;
        sum += p; // RELEASE
        count++;
    }

    std::tuple<uint64_t, uint64_t, uint64_t> get() { return {min, max, count > 0 ? sum / count : 0}; }
};

// --- Histogram Class ---
template <bool active>
class Histogram
{
    const uint64_t singleBucketDuration;
    std::vector<uint64_t> histogram;
    TippingBuffer<active> tippingBuffer; // tutaj się wlewa i dopiero potem przekazywane jest do histogramu jako jedna wartość
    StatsPack<false> histogramStats;

    size_t getIndex(uint64_t time) const
    {
        uint64_t index = time / singleBucketDuration;
        return std::clamp(index, static_cast<uint64_t>(0), histogram.size() - 1);
    }

public:
    Histogram(uint64_t totalDuration, uint64_t numBuckets) : singleBucketDuration(totalDuration / numBuckets), histogram(numBuckets)
    {
        clearHistData();
    }

    void setThreshold(size_t threshold) { tippingBuffer.setThreshold(threshold); }

    void clearHistData()
    {
        std::memset(histogram.data(), 0, histogram.size() * sizeof(uint64_t));
        tippingBuffer.resetBuffer();
    }

    void add(uint64_t p)
    {
        auto result = tippingBuffer.add(p); // accumulating
        if (!result.has_value()) return;

        p = result.value(); // RELEASE

        histogramStats.add(p);
        histogram[getIndex(p)]++;
    }

    string howManyLines(int count) const
    {
        string result;
        for (size_t i = 0; i < count; ++i)
        {
            result += "|";
        }
        return result;
    }

    void log()
    {
        uint64_t bucketStart = 0;
        uint64_t bucketEnd = singleBucketDuration;

        nline;

        auto [min, max, avg] = histogramStats.get();

        varr(min);
        varr(max);
        var(avg);

        for (auto& bucket : histogram)
        {
            line("Bucket (" << bucketStart << " - " << bucketEnd << ") : " << howManyLines(bucket) << bucket << " ");

            bucketStart += singleBucketDuration;
            bucketEnd += singleBucketDuration;
        }
    }
};

// --- Enums ---
enum class L2ProfilerSectionsPerSlotActions
{
    SMDLUL_PROCESS_SMCELLS,

    ALL_SECTIONS
};
enum class L2ProfilerSectionsMsgProcessing
{
    MIB,
    SIB1,

    ALL_SECTIONS
};

// --- L2Profiler ---
class L2Profiler
{
    constexpr static uint64_t oneGranularityDuration = 62500;
    constexpr static uint64_t NUM_OF_PER_SLOT_CODE_SECTIONS = static_cast<uint64_t>(L2ProfilerSectionsPerSlotActions::ALL_SECTIONS);
    constexpr static uint64_t NUM_OF_PER_MSG_PROC_CODE_SECTIONS = static_cast<uint64_t>(L2ProfilerSectionsMsgProcessing::ALL_SECTIONS);

    // true / false -> sets TippingBuffer active or no //

    Histogram<false> mGranularityHistogram;
    Histogram<true> mSlotHistogram;

    StatsPack<false> perGranuleCodeSections[NUM_OF_PER_SLOT_CODE_SECTIONS];
    StatsPack<true> perSlotCodeSections[NUM_OF_PER_SLOT_CODE_SECTIONS];
    StatsPack<false> perMsgProcessingCodeSections[NUM_OF_PER_MSG_PROC_CODE_SECTIONS];

    void setSlotThreshold(uint32_t threshold)
    {
        // oddamy jeszcze dla Histogramu to samo -> tam będzie osobny wektor //

        mSlotHistogram.setThreshold(threshold);

        for (auto& stat : perSlotCodeSections)
            stat.setThreshold(threshold);
    }

public:
    L2Profiler(uint32_t cellScsNumerology)
        : mGranularityHistogram(oneGranularityDuration, 25), mSlotHistogram(getSlotGranularity(cellScsNumerology), 25)
    {
        setSlotThreshold(cellScsNumerology);
    }

    uint64_t getSlotGranularity(uint32_t cellScsNumerology) const { return cellScsNumerology * oneGranularityDuration; }

    void addGranularityMeasurement(uint64_t p)
    {
        mGranularityHistogram.add(p);
        mSlotHistogram.add(p);
    }

    void addSectionMeasurement(L2ProfilerSectionsPerSlotActions section, uint64_t p)
    {
        perGranuleCodeSections[static_cast<uint64_t>(section)].add(p);
        perSlotCodeSections[static_cast<uint64_t>(section)].add(p);
    }
    void addSectionMeasurement(L2ProfilerSectionsMsgProcessing section, uint64_t p)
    {
        perMsgProcessingCodeSections[static_cast<uint64_t>(section)].add(p);
    }

    void log()
    {
        line("Granularity Histogram:");
        mGranularityHistogram.log();

        line("Slot Histogram:");
        mSlotHistogram.log();

        line("Per Granule Actions:");
        for (auto& perGranule : perGranuleCodeSections)
        {
            auto [min, max, avg] = perGranule.get();
            varr(min);
            varr(max);
            var(avg);
        }

        line("Per Slot Actions:");
        for (auto& perSlot : perSlotCodeSections)
        {
            auto [min, max, avg] = perSlot.get();
            varr(min);
            varr(max);
            var(avg);
        }

        line("Msg Processing:");
        for (auto& msgProcessing : perMsgProcessingCodeSections)
        {
            auto [min, max, avg] = msgProcessing.get();
            varr(min);
            varr(max);
            var(avg);
        }
    }
};

// zmienić getSlotGranularity
// dorobić scoped time expiry
// dorobić section explicite start / stop
// raprojektować tak komunikację z Executorem, że dodanie nowej sekcji nie będzie trudne i że z poziomu Executora będzie można
// włączać / wyłączać sekcje, które mają być mierzone

// + włączanie konkretnych sekcji (bitset, array<bool>)
// MACRO automatyczne sprawdzenie podczas markowania sekcji kodu !!!
// -> periodyczne wysyłanie (np. jakiś counter wewnątrz L2Profiler, jak się ileś zbierze to wysyłamy, albo liczymi ticki i wysyłamy po jakimś czasie)
// -> on request (msg handle)

// Dodatek -> funkcja co zwraca ilość bucketów, kiedy podajemy jej jaki chcemy interwał pomiędzy binami
//

int main()
{
    L2Profiler profiler(16);

    for (int i = 0; i < 1000; ++i)
    {
        profiler.addGranularityMeasurement(rand() % 62500);

        profiler.addSectionMeasurement(L2ProfilerSectionsPerSlotActions::SMDLUL_PROCESS_SMCELLS, rand() % 100);
        profiler.addSectionMeasurement(L2ProfilerSectionsMsgProcessing::MIB, rand() % 100);
        profiler.addSectionMeasurement(L2ProfilerSectionsMsgProcessing::SIB1, rand() % 100);
    }

    profiler.log();

    StatsPack<true> t;
    StatsPack<false> f;

    var(sizeof(t));
    var(sizeof(f));

    TippingBuffer<true> tt;
    TippingBuffer<false> ff;

    var(sizeof(tt));
    var(sizeof(ff));

    line("End of trippin");
}
