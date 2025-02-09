#ifndef AD81D711_EAC3_46D0_AE68_81108134E1E4
#define AD81D711_EAC3_46D0_AE68_81108134E1E4

extern "C" {
extern const char g_jsonl_data_start[];
extern const int g_jsonl_data_size;

extern const char g_five_mb_formatted_json_data_start[];
extern const int g_five_mb_formatted_json_data_size;
}

// NOLINTBEGIN(cppcoreguidelines-pro-bounds-pointer-arithmetic,cppcoreguidelines-pro-bounds-array-to-pointer-decay)
inline const char* const jsonl_data_start                  = g_jsonl_data_start;
inline const char* const jsonl_data_end                    = g_jsonl_data_start + g_jsonl_data_size;
inline const char* const five_mb_formatted_json_data_start = g_five_mb_formatted_json_data_start;
inline const char* const five_mb_formatted_json_data_end =
    g_five_mb_formatted_json_data_start + g_five_mb_formatted_json_data_size;
// NOLINTEND(cppcoreguidelines-pro-bounds-pointer-arithmetic,cppcoreguidelines-pro-bounds-array-to-pointer-decay)

#endif /* AD81D711_EAC3_46D0_AE68_81108134E1E4 */
