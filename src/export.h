#ifndef C7A84CD6_8108_4AE3_A25E_12D40E3960E3
#define C7A84CD6_8108_4AE3_A25E_12D40E3960E3

#ifdef PSB_JSON_DETECTOR_STATIC_DEFINE
#    define PSB_JSON_DETECTOR_EXPORT
#    define PSB_JSON_DETECTOR_NO_EXPORT
#else
#    ifdef psb_json_detector_EXPORTS
#        if defined _WIN32 || defined __CYGWIN__
#            define PSB_JSON_DETECTOR_EXPORT __declspec(dllexport)
#            define PSB_JSON_DETECTOR_NO_EXPORT
#        else
#            define PSB_JSON_DETECTOR_EXPORT    [[gnu::visibility("default")]]
#            define PSB_JSON_DETECTOR_NO_EXPORT [[gnu::visibility("hidden")]]
#        endif
#    else
#        if defined _WIN32 || defined __CYGWIN__
#            define PSB_JSON_DETECTOR_EXPORT __declspec(dllimport)
#            define PSB_JSON_DETECTOR_NO_EXPORT
#        else
#            define PSB_JSON_DETECTOR_EXPORT    [[gnu::visibility("default")]]
#            define PSB_JSON_DETECTOR_NO_EXPORT [[gnu::visibility("hidden")]]
#        endif
#    endif
#endif

#endif /* C7A84CD6_8108_4AE3_A25E_12D40E3960E3 */
