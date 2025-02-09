#ifndef EBA6452D_3A51_4840_AF64_DC3BE781DC8D
#define EBA6452D_3A51_4840_AF64_DC3BE781DC8D

#include <limits>
#include <memory>
#include <string>
#include <string_view>
#include <wwa/coro/generator.h>

#include "../export.h"

namespace psb::json_detector_v1 {

struct json_detector_private;
class PSB_JSON_DETECTOR_EXPORT json_detector {
public:
    json_detector();
    json_detector(const json_detector&) = default;
    json_detector(json_detector&&)      = default;
    ~json_detector();

    json_detector& operator=(const json_detector&) = default;
    json_detector& operator=(json_detector&&)      = default;

    wwa::coro::generator<std::string> process(
        std::string_view input,
        std::string_view::size_type max_size = std::numeric_limits<std::string_view::size_type>::max()
    );

    [[nodiscard]] bool is_finished() const noexcept;
    [[nodiscard]] const std::string& get_json() const noexcept;
    void reset() noexcept;

    static constexpr int version() noexcept { return 1; }

private:
    std::shared_ptr<json_detector_private> d_ptr;
};

}  // namespace psb::json_detector_v1

#endif /* EBA6452D_3A51_4840_AF64_DC3BE781DC8D */
