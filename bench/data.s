    .section .rodata

    .global g_jsonl_data_start
    .type   g_jsonl_data_start, @object
    .balign 16
g_jsonl_data_start:
    .incbin "stream.jsonl"
g_jsonl_data_end:

    .global g_jsonl_data_size
    .type   g_jsonl_data_size, @object
    .balign 8
g_jsonl_data_size:
    .int    g_jsonl_data_end - g_jsonl_data_start

    .global g_five_mb_formatted_json_data_start
    .type   g_five_mb_formatted_json_data_start, @object
    .balign 16
g_five_mb_formatted_json_data_start:
    .incbin "5MB.json"
g_five_mb_formatted_json_data_end:

    .global g_five_mb_formatted_json_data_size
    .type   g_five_mb_formatted_json_data_size, @object
    .balign 8
g_five_mb_formatted_json_data_size:
    .int    g_five_mb_formatted_json_data_end - g_five_mb_formatted_json_data_start

    .section .note.GNU-stack,"",@progbits
