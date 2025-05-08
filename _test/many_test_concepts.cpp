// clang-format off
#include <gtest/gtest.h> // must be first
// clang-format on
#include "__preprocessor__.h"

// 1) Prosty test niezależny
TEST(SampleTest, Sanity) { EXPECT_EQ(1, 1); }

// 2) TEST_F – testy oparte na fixtures
class MyFixture : public ::testing::Test
{
protected:
    std::vector<int> data;

    void SetUp() override
    {
        // kod wykonywany przed każdym testem
        data = {0, 1, 2, 3, 4};
    }

    void TearDown() override
    {
        // kod wykonywany po każdym teście (opcjonalnie)
        data.clear();
    }
};

TEST_F(MyFixture, SumIsCorrect)
{
    int sum = 0;
    for (int v : data)
        sum += v;
    EXPECT_EQ(sum, 0 + 1 + 2 + 3 + 4);
}

TEST_F(MyFixture, FirstElementIsZero)
{
    ASSERT_FALSE(data.empty());
    EXPECT_EQ(data.front(), 0);
}

// 3) TEST_P + INSTANTIATE_TEST_CASE_P – testy parametryzowane
// 3a) Definicja klasy-fixtury parametrów
typedef std::pair<int, bool> IntBoolPair;
class ParityTest : public ::testing::TestWithParam<IntBoolPair>
{
};

// 3b) Logika testu korzystająca z GetParam()
TEST_P(ParityTest, CheckEven)
{
    int value = GetParam().first;
    bool isEven = GetParam().second;
    EXPECT_EQ((value % 2 == 0), isEven);
}

// 3c) Instancjonowanie testów z zestawem wartości
INSTANTIATE_TEST_CASE_P(ParityChecks, // prefiks wygenerowanych testów
                        ParityTest,   // nazwa klasy TEST_P
                        ::testing::Values(IntBoolPair(0, true), IntBoolPair(1, false), IntBoolPair(42, true), IntBoolPair(17, false)));

// Główny punkt wejścia do testów
typedef ::testing::InitGoogleTest InitGTest;

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}