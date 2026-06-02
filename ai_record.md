# AI record

> Includes the record where AI was used. Must justify necessity.
 
 | Date | What | Reason |
|---|---|---|
| 2026/6/1 | Searched what the pre-tokenize regex means: `'s\|'t\|'re\|'ve\|'m\|'ll\|'d\| ?\p{L}+\| ?\p{N}+\| ?[^\\s\p{L}\p{N}]+\|\\s+(?!\\S)\|\\s+`, replaced it with posix style `'s\|'t\|'re\|'ve\|'m\|'ll\|'d\| ?[[:L*:]]+\| ?[[:N*:]]+\| ?[^\s[:L*:][:N*:]]+\|\s+(?!\S)\|\s+`, and searched how to iterate over `boost::u32regex_iterator` match results. | I mean... look at that. |

