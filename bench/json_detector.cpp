#include <benchmark/benchmark.h>

#include <cstdint>
#include <string_view>

#include "data.h"
#include "json_detector.h"

namespace {

const std::string_view input{jsonl_data_start, jsonl_data_end};
const std::string_view::size_type size = input.size();

template<typename Detector>
void BM_JsonDetector(benchmark::State& state)
{
    Detector detector;

    for (auto _ : state) {
        const auto count = static_cast<std::string_view::size_type>(state.range(0));
        for (std::string_view::size_type pos = 0; pos < size; pos += count) {
            auto&& generator = detector.process(input.substr(pos, count));
            for (auto& chunk : generator) {
                benchmark::DoNotOptimize(chunk);
            }
        }
    }

    state.SetBytesProcessed(static_cast<std::int64_t>(size));
}

constexpr int multiplier = 4;
BENCHMARK(BM_JsonDetector<psb::json_detector_v1::json_detector>)
    ->Name("json_detector_v1")
    ->RangeMultiplier(multiplier)
    ->Range(1, jsonl_data_end - jsonl_data_start);

BENCHMARK(BM_JsonDetector<psb::json_detector_v2::json_detector>)
    ->Name("json_detector_v2")
    ->RangeMultiplier(multiplier)
    ->Range(1, jsonl_data_end - jsonl_data_start);

}  // namespace
