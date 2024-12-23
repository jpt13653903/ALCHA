"""
Auto-gen used to convert the JSON-based character names, as downloaded from
https://html.spec.whatwg.org/entities.json
to a C array.
"""

import json

def getChar(code):
    utf8 = b''
    for c in code:
        utf8 += chr(c).encode('utf-8')
    return utf8

def getUTF8(code):
    utf8 = getChar(code)
    result = ''
    for c in utf8:
        result += f'\\x{int(c):02X}'
    return result

with open('CharacterNames.json', 'r') as data:
    try:
        json_map = json.load(data)
    except json.JSONDecodeError as err:
        print(f'\nInvalid JSON in file - {err} \n')
        sys.exit(1)

with open('CharacterNames.h', 'wb') as file:
    file.write(b'#ifndef CharacterNames_h\n')
    file.write(b'#define CharacterNames_h\n')
    file.write(b'//--------------------------------------')
    file.write(b'----------------------------------------\n')
    file.write(b'\n')
    file.write(b'/**\n')
    file.write(b'This list is used in the scanner to evaluate escape sequences of the form\n')
    file.write(b'"\\&nnnn;", where nnnn is any of the HTML-5 character names defined in\n')
    file.write(b'https://html.spec.whatwg.org/entities.json                                    */\n')
    file.write(b'//------------------------------------------------------------------------------\n')
    file.write(b'\n')
    file.write(b'static const char* characterNames[] = {\n')
    for name in json_map:
        if(name[0] == '&' and name[-1] == ';'):
            file.write(bytes(f'  "{name[1:-1]}"'.ljust(35), 'utf-8'))
            file.write(bytes(f', "{getUTF8(json_map[name]["codepoints"])}"'.ljust(30), 'utf-8'))
            file.write(b', /* ')
            c = getChar(json_map[name]["codepoints"])
            if(c >= b' '): file.write(c)
            else:          file.write(b' ')
            file.write(b' */\n')
    file.write(b'  0, 0\n')
    file.write(b'};\n')
    file.write(b'//------------------------------------------------------------------------------\n')
    file.write(b'\n')
    file.write(b'#endif\n')
    file.write(b'//------------------------------------------------------------------------------\n')

