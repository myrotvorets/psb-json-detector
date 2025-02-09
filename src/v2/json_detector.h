#ifndef E83B6F35_1B86_4D2A_B96F_3118D0905D4F
#define E83B6F35_1B86_4D2A_B96F_3118D0905D4F

#include <limits>
#include <memory>
#include <string>
#include <string_view>
#include <wwa/coro/generator.h>

#include "../export.h"

namespace psb::inline json_detector_v2 {

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

    static constexpr int version() noexcept { return 2; }

private:
    std::shared_ptr<json_detector_private> d_ptr;
};

}  // namespace psb::inline json_detector_v2

#endif /* E83B6F35_1B86_4D2A_B96F_3118D0905D4F */
