"""
Auto-gen used to convert the JSON-based character names, as downloaded from
https://github.com/w3c/html/blob/master/entities.json
to a C array.
"""

import json

def getUTF8(code):
    utf8 = b'';
    for c in code:
        utf8 += chr(c).encode('utf-8');
    result = "";
    for c in utf8:
        result += "\\x%02X" % int(c);
    return result;

with open("CharacterNames.json", "r") as data:
    try:
        json_map = json.load(data)
    except json.JSONDecodeError as err:
        print("\nInvalid JSON in file -", err, "\n")
        sys.exit(1)

with open("CharacterNames.h", "w", newline="\n") as file:
    file.write('#ifndef CharacterNames_h\n');
    file.write('#define CharacterNames_h\n');
    file.write('//--------------------------------------');
    file.write('----------------------------------------\n');
    file.write('\n');
    file.write('/**\n');
    file.write('This list is used in the scanner to evaluate escape sequences of the form\n');
    file.write('"\&nnnn;", where nnnn is any of the 2 125 HTML-5 character names.             */\n');
    file.write('//--------------------------------------');
    file.write('----------------------------------------\n');
    file.write('\n');
    file.write('static const char* CharacterNames[] = {\n');
    for name in json_map:
        if(name[0] == '&' and name[-1] == ';'):
            file.write(f'  "{name[1:-1]}", "{getUTF8(json_map[name]["codepoints"])}",\n');
    file.write('  0, 0\n');
    file.write('};\n');
    file.write('//--------------------------------------');
    file.write('----------------------------------------\n');
    file.write('\n');
    file.write('#endif\n');
    file.write('//--------------------------------------');
    file.write('----------------------------------------\n');

