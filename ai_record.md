# AI record

> Includes the record where AI was used. Must justify.
 
 | Date | What | Reason |
|---|---|---|
| 2026/6/1 | Asked chatgpt what the pre-tokenize regex means: `'s\|'t\|'re\|'ve\|'m\|'ll\|'d\| ?\p{L}+\| ?\p{N}+\| ?[^\\s\p{L}\p{N}]+\|\\s+(?!\\S)\|\\s+`, replaced it with posix style `'s\|'t\|'re\|'ve\|'m\|'ll\|'d\| ?[[:L*:]]+\| ?[[:N*:]]+\| ?[^\s[:L*:][:N*:]]+\|\s+(?!\S)\|\s+`, and searched how to iterate over `boost::u32regex_iterator` match results. | I mean... look at that. |
|2026/6/7|Asked chatgpt why linking to spdlog failed with undefined reference.|I just couldn't wrap around my head after searching for 2 hours straight just to link spdlog but ended up still seeing that error no matter what I try. In conclusion, it was the fact I was using spdlog and fmt together, so I'll have to fetch and build fmt first in CMakeLists.txt, then `set(SPDLOG_FMT_EXTERNAL ON CACHE BOOL "" FORCE)`, THEN FetchContent(spdlog). I'm sorry. |