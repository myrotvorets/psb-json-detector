#include <gtest/gtest.h>

#include <algorithm>
#include <iterator>
#include <limits>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>

#include "json_detector.h"

template<typename Detector>
class JDTest : public testing::Test {
public:
    Detector& detector() { return this->m_detector; }

protected:
    void SetUp() override { this->m_detector.reset(); }

    // There is no official way to combine type-parameterized tests and value-parameterized tests.
    // See https://groups.google.com/g/googletestframework/c/3d8H5J5Isnw
    void runner(std::string_view input, const std::vector<std::string>& expected)
    {
        auto& detector = this->detector();
        std::vector<std::string> actual;

        auto&& gen = detector.process(input);
        std::ranges::copy(gen, std::back_inserter(actual));

        EXPECT_EQ(actual, expected);
        EXPECT_TRUE(detector.is_finished());
    }

    void runner_chunks(
        std::string_view input, std::string_view::size_type chunk_size, const std::vector<std::string>& expected,
        bool multi = false
    )
    {
        auto& detector = this->detector();
        std::vector<std::string> actual;

        const auto size = input.size();
        for (std::string_view::size_type pos = 0; pos < size; pos += chunk_size) {
            auto&& gen = detector.process(input.substr(pos, chunk_size));
            std::ranges::copy(gen, std::back_inserter(actual));

            if (!multi) {
                EXPECT_EQ(detector.is_finished(), pos + chunk_size >= size);
            }
        }

        EXPECT_EQ(actual, expected);
        EXPECT_TRUE(detector.is_finished());
    }

    void runner_exception(
        std::string_view input, const std::string& expected_json, const std::vector<std::string>& expected,
        std::string_view::size_type max_size = std::numeric_limits<std::string_view::size_type>::max()
    )
    {
        std::vector<std::string> actual;

        auto& detector = this->detector();
        auto&& gen     = detector.process(input, max_size);

        EXPECT_THROW(std::ranges::copy(gen, std::back_inserter(actual)), std::system_error);
        EXPECT_EQ(detector.get_json(), expected_json);
        EXPECT_EQ(actual, expected);
    }

private:
    Detector m_detector;
};

using Detectors = ::testing::Types<psb::json_detector_v1::json_detector, psb::json_detector_v2::json_detector>;
TYPED_TEST_SUITE(JDTest, Detectors);

TYPED_TEST(JDTest, Empty)
{
    const std::string_view input;
    const std::vector<std::string> expected;

    this->runner(input, expected);
}

TYPED_TEST(JDTest, Spaces)
{
    const std::string_view input{"   \t \r\n "};
    const std::vector<std::string> expected;

    this->runner(input, expected);
}

TYPED_TEST(JDTest, SingleObject)
{
    const std::string_view input{"{}"};
    const std::vector<std::string> expected{std::string{input}};

    this->runner(input, expected);
}

TYPED_TEST(JDTest, SingleArray)
{
    const std::string_view input{"[]"};
    const std::vector<std::string> expected{std::string{input}};

    this->runner(input, expected);
}

TYPED_TEST(JDTest, Strings)
{
    const std::string_view input{R"(["a", "b", "c"])"};
    const std::vector<std::string> expected{std::string{input}};

    this->runner(input, expected);
}

TYPED_TEST(JDTest, StringsWithSlashes)
{
    const std::string_view input{R"(["a", "b\"", "c\"d\"\"e"])"};
    const std::vector<std::string> expected{std::string{input}};

    this->runner(input, expected);
}

TYPED_TEST(JDTest, Multi)
{
    const std::string_view input{R"([{}])"
                                 R"({"a": []})"};
    const std::vector<std::string> expected{R"([{}])", R"({"a": []})"};

    this->runner_chunks(input, 1U, expected, true);
}

TYPED_TEST(JDTest, Chunk)
{
    const std::string_view input{R"(["a",)"
                                 R"( "b\")"
                                 R"(", "c)"
                                 R"(\"d\")"
                                 R"(\"e",)"
                                 R"(0])"};
    const std::vector<std::string> expected{std::string{input}};

    constexpr std::string_view::size_type chunk_size = 5U;
    this->runner_chunks(input, chunk_size, expected);
}

TYPED_TEST(JDTest, ByteChunk)
{
    const std::string_view input{R"(["a",)"
                                 R"( "b\")"
                                 R"(", "c)"
                                 R"(\"d\")"
                                 R"(\"e",)"
                                 R"(0])"};
    const std::vector<std::string> expected{std::string{input}};

    constexpr std::string_view::size_type chunk_size = 1U;
    this->runner_chunks(input, chunk_size, expected);
}

TYPED_TEST(JDTest, TooLarge)
{
    const std::string_view input{"{{{{{{}}}}}}"};
    constexpr std::string_view::size_type max_size = 10U;

    this->runner_exception(input, std::string(input.substr(0, max_size)), {}, max_size);
}

TYPED_TEST(JDTest, NotArrayObj)
{
    const std::string_view input{"null"};

    this->runner_exception(input, std::string(input.substr(0, 1)), {});
}

TYPED_TEST(JDTest, TopLevelString)
{
    const std::string_view input{R"("top-level string")"};

    this->runner_exception(input, std::string(input.substr(0, 1)), {});
}

TYPED_TEST(JDTest, TopLevelSlash)
{
    const std::string_view input{R"(\)"};

    this->runner_exception(input, std::string(input.substr(0, 1)), {});
}

TYPED_TEST(JDTest, ClosingBracket)
{
    const std::string_view input{R"(])"};

    this->runner_exception(input, std::string(input.substr(0, 1)), {});
}
