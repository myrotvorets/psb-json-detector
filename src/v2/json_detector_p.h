#ifndef FBA4D9C1_670D_412B_91EE_72E5949E03C8
#define FBA4D9C1_670D_412B_91EE_72E5949E03C8

#include <string>

namespace psb::inline json_detector_v2 {

struct json_detector_private {
    std::string buf;
    int count      = 0;
    bool in_string = false;
    bool escape    = false;
    char open      = 0;
    char close     = 0;
};

}  // namespace psb::inline json_detector_v2

#endif /* FBA4D9C1_670D_412B_91EE_72E5949E03C8 */
