# AI record

> Includes the record where AI was used. Must justify.
 
| Date | What | Reason |
|---|---|---|
| 2026/6/1 | Asked chatgpt what the pre-tokenize regex means: `'s\|'t\|'re\|'ve\|'m\|'ll\|'d\| ?\p{L}+\| ?\p{N}+\| ?[^\\s\p{L}\p{N}]+\|\\s+(?!\\S)\|\\s+`, replaced it with posix style `'s\|'t\|'re\|'ve\|'m\|'ll\|'d\| ?[[:L*:]]+\| ?[[:N*:]]+\| ?[^\s[:L*:][:N*:]]+\|\s+(?!\S)\|\s+`, and searched how to iterate over `boost::u32regex_iterator` match results. | I mean... look at that. I'm not sorry. |
|2026/6/7|Asked chatgpt why linking to spdlog failed with undefined reference.|I just couldn't wrap around my head after searching for 2 hours straight just to link spdlog but ended up still seeing that error no matter what I try. In conclusion, it was the fact I was using spdlog and fmt together, so I'll have to fetch and build fmt first in CMakeLists.txt, then `set(SPDLOG_FMT_EXTERNAL ON CACHE BOOL "" FORCE)`, THEN FetchContent(spdlog). I'm sorry. |
|2026/6/7|asked chatgpt about a template instantiation issue when using fmtlib with c++20 ranges. ended up finding an issue at https://github.com/fmtlib/fmt/issues/4202|the error log was too complicated to read so i had chatgpt analyze that for me. i'm sorry.|
|2026/6/21|asked chatgpt about what data structure should i hold the storage in the `Blob`. Ended up being `unique_ptr<std::byte, CustomDeleter>`. I asked because I can't find/think of a reasonable Blob class impl that can hold both CPU and GPU memory. i'm sorry.|