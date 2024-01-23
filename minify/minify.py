import argparse
import os
import re

includes = []
preprocessors = []

def remove_comments(code: str):
    def replacer(match):
        s = match.group(0)
        if s.startswith('/'):
            return " " # note: a space and not an empty string
        else:
            return s
    pattern = re.compile(
        r'//.*?$|/\*.*?\*/|\'(?:\\.|[^\\\'])*\'|"(?:\\.|[^\\"])*"',
        re.DOTALL | re.MULTILINE
    )

    return re.sub(pattern, replacer, code)


def remove_preprocessor(preproc_name, code: str, save=True):
    pattern_str = f"{preproc_name}.*"
    pattern = re.compile(
        pattern_str
    )
    if save:
        for p in re.findall(pattern, code):
            clean_inc = p.replace("\n","")
            preprocessors.append(clean_inc)
    return re.sub(pattern, "", code)

def remove_includes(code: str, save=True):
    pattern = re.compile(
        r'#include.*["|>].*\n'
    )
    if save:
        for p in re.findall(pattern, code):
            clean_inc = p.replace("\n","")
            clean_inc = clean_inc.replace(" ","")
            includes.append(clean_inc)
    
    return re.sub(pattern, "", code)

def remove_spaces_and_tabs(code: str, replace_str=""):
    code = code.replace("\t", replace_str)
    pattern = re.compile(
        r' {2,10000}'
    )
    return re.sub(pattern, "", code)

def remove_others(code: str):
    code = code.replace("{ ", "{")
    code = code.replace(" }", "}")
    code = code.replace(" {", "{")
    code = code.replace("} ", "}")
    code = code.replace("; ", ";")
    code = code.replace("= ", "=")
    code = code.replace(" =", "=")
    code = code.replace("( ", "(")
    code = code.replace(" )", ")")
    code = code.replace(" (", "(")
    code = code.replace(") ", ")")
    code = code.replace(", ", ",")
    return code

def remove_newlines(code: str, replace_str=" "):
    code = code.replace("\n", replace_str)
    code = code.replace("\r", replace_str)
    return code

def clean_code(code: str):
    code = remove_includes(code, save=True)
    code = remove_preprocessor("#ifndef", code, save=False)
    code = remove_preprocessor("#define", code, save=True)
    code = remove_preprocessor("#endif", code, save=False)
    code = remove_comments(code)
    code = remove_spaces_and_tabs(code, " ")
    code = remove_newlines(code, " ")
    return code


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--file', type=str, nargs='*')
    args = parser.parse_args()

    orig_chars = 0
    min_chars = 0

    minified_code = ""

    for path in args.file:
        if not os.path.exists(path):
            print("File", path, "does not exists!")
            exit()
        
        with open(path, "r") as fp:
            code = fp.read()
            num_chars = len(code)
        
        orig_chars += len(code)
        code = clean_code(code)
        
        minified_code += code + "\n"

    includes = set(includes)
    includes = [inc for inc in includes if '"'not in inc]
    preprocessors = [s for s in preprocessors if not s.endswith("_H")]

    minified_code = "\n".join(includes) +"\n" + minified_code
    minified_code = "\n".join(preprocessors) +"\n" + minified_code

    minified_code = remove_spaces_and_tabs(minified_code)
    minified_code = remove_others(minified_code)

    min_chars = len(minified_code)
    print("//Original chars: ",  orig_chars)
    print("//Minified chars: ",  min_chars)
    print(f"//minified/original:  {100*min_chars/orig_chars:.2f}%")

    print(minified_code)
