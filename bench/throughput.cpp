#include <benchmark/benchmark.h>

#include <cstdint>
#include <string_view>

#include "data.h"
#include "json_detector.h"

namespace {

// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
std::string data;
std::string_view input;
std::string_view::size_type size;
// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

void setup(const benchmark::State&)
{
    data  = {five_mb_formatted_json_data_start, five_mb_formatted_json_data_end};
    data += data;  // x2, 10 MB
    data += data;  // x4, 20 MB
    data += data;  // x8, 40 MB
    data += data;  // x16, 80 MB
    data += data;  // x32, 160 MB
    data += data;  // x64, 320 MB
    data += data;  // x128, 640 MB

    input = data;
    size  = input.size();
}

template<typename Detector>
void BM_Throughput(benchmark::State& state)
{
    Detector detector;

    for (auto _ : state) {
        const auto count = static_cast<std::string_view::size_type>(state.range(0));
        for (std::string_view::size_type pos = 0; pos < size; pos += count) {
            auto generator = detector.process(input.substr(pos, count));
            for (auto& chunk : generator) {
                benchmark::DoNotOptimize(chunk);
            }
        }
    }

    state.SetBytesProcessed(static_cast<std::int64_t>(size));
}

}  // namespace

constexpr int multiplier      = 16;
constexpr std::int64_t factor = 128;
BENCHMARK(BM_Throughput<psb::json_detector_v1::json_detector>)
    ->Name("Throughput (json_detector_v1)")
    ->Setup(setup)
    ->RangeMultiplier(multiplier)
    ->Range(1, factor*(five_mb_formatted_json_data_end - five_mb_formatted_json_data_start))
    ->Unit(benchmark::kMillisecond);
BENCHMARK(BM_Throughput<psb::json_detector_v2::json_detector>)
    ->Name("Throughput (json_detector_v2)")
    ->Setup(setup)
    ->RangeMultiplier(multiplier)
    ->Range(1, factor*(five_mb_formatted_json_data_end - five_mb_formatted_json_data_start))
    ->Unit(benchmark::kMillisecond);
