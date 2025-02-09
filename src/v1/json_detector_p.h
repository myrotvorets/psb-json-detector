#ifndef A66B8F31_DEC4_4E10_B8F4_A67D663FCFA0
#define A66B8F31_DEC4_4E10_B8F4_A67D663FCFA0

#include <string>

namespace psb::json_detector_v1 {

struct json_detector_private {
    std::string buf;
    int brace_count   = 0;
    int bracket_count = 0;
    bool in_string    = false;
    bool escape       = false;
};

}  // namespace psb::json_detector_v1

#endif /* A66B8F31_DEC4_4E10_B8F4_A67D663FCFA0 */
