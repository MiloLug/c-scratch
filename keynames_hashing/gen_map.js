const fs = require("fs");
const path = require("path");
const exec = require('child_process').exec;
const keymap = require("./keymap.json");

const configPath = path.resolve(__dirname, "config.gperf");

const config = (
`struct KeyData { char * name; uint16_t scancode; };
%%
${keymap.map(([k, v]) => `"${k.replace('\\', '\\\\')}", ${v}`).join("\n")}`);


fs.writeFileSync(
    configPath,
    config,
);

const headers = (
`#include "include_sdl.h"
#include <cstdint>
#include <cwchar>
#include <cstring>
`);

exec(`gperf -L "C++" -C "${configPath}" -t`, (error, stdout, stderr) => {
    if (error !== null) {
        console.error(error);
        console.log(stderr);
        return;
    }

    const result = headers + (
        stdout
        .replace(/[\s\S]*<bug-gperf@gnu\.org>\."\n#endif/gmi, '')
        .replace(/#line.*\n/gmi, '')
        .replace(/"(.*?)"/gmi, 'L"$1"')
        .replace(/char \*(\w)/gmi, 'wchar_t * $1')
        .replace(/char \*/gmi, 'wchar_t *')
        .replace(/unsigned short/gmi, 'uint32_t')
        .replace(/unsigned int/gmi, 'uint32_t')
        .replace(/size_t/gmi, 'uint64_t')
        .replace(/unsigned char/gmi, 'uint32_t')
        .replace(/Perfect_Hash/gmi, 'ScanCodesMap')
        .replace(/(hash|in_word_set) \(/gmi, '$1(')
        .replace(/(=|\))\n.*\{/gmi, '$1 {')
        .replace(/strcmp \(/gmi, 'wcscmp(')
    );

    console.log(result);
});
