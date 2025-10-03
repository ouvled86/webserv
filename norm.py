import re
import sys
import os

MAX_FUNC_LINES = 30
MAX_FILE_LINES = 500
INDENT = 4

def check_header_guard(filename, lines):
    guard = os.path.basename(filename).replace('.', '_').upper()
    if not (lines[0].startswith("#ifndef") and guard in lines[0] and
            lines[1].startswith("#define") and guard in lines[1] and
            lines[-1].startswith("#endif")):
        print(f"[Header Guard] {filename}: Missing or incorrect header guard.")

def check_class_naming(line, filename):
    match = re.match(r'\s*class\s+([A-Za-z_][A-Za-z0-9_]*)', line)
    if match:
        class_name = match.group(1)
        if not class_name[0].isupper():
            print(f"[Class Naming] {filename}: Class '{class_name}' should start with uppercase.")

def check_function_naming(line, filename):
    match = re.match(r'\s*(?:[\w:<>]+[\s*&]+)+([\w_]+)\s*\(', line)
    if match:
        func_name = match.group(1)
        if not (func_name[0].islower() and re.match(r'^[a-zA-Z0-9_]+$', func_name)):
            print(f"[Function Naming] {filename}: Function '{func_name}' should be snake_case or camelCase, start with lowercase.")

def check_function_length(lines, filename):
    func_start = None
    func_name = None
    for i, line in enumerate(lines):
        if re.match(r'\s*(?:[\w:<>]+[\s*&]+)+([\w_]+)\s*\(', line):
            func_start = i
            func_name = re.match(r'\s*(?:[\w:<>]+[\s*&]+)+([\w_]+)\s*\(', line).group(1)
        if func_start is not None and '{' in line:
            brace_count = 1
            for j in range(i+1, len(lines)):
                brace_count += lines[j].count('{')
                brace_count -= lines[j].count('}')
                if brace_count == 0:
                    func_len = j - func_start + 1
                    if func_len > MAX_FUNC_LINES:
                        print(f"[Function Length] {filename}: Function '{func_name}' is {func_len} lines (max {MAX_FUNC_LINES}).")
                    func_start = None
                    break

def check_file_length(lines, filename):
    if len(lines) > MAX_FILE_LINES:
        print(f"[File Length] {filename}: {len(lines)} lines (max {MAX_FILE_LINES}).")

def check_indentation(lines, filename):
    for i, line in enumerate(lines):
        if line.strip() and not line.startswith(' ' * INDENT) and not line.startswith('\t'):
            print(f"[Indentation] {filename}: Line {i+1} not indented with {INDENT} spaces.")

def check_function_comments(lines, filename):
    for i, line in enumerate(lines):
        if re.match(r'\s*(?:[\w:<>]+[\s*&]+)+([\w_]+)\s*\(', line):
            if i == 0 or not lines[i-1].strip().startswith('//'):
                print(f"[Comment] {filename}: Function at line {i+1} missing comment.")

def check_file(filename):
    with open(filename, 'r') as f:
        lines = f.readlines()
    if filename.endswith('.hpp'):
        check_header_guard(filename, lines)
    check_file_length(lines, filename)
    check_indentation(lines, filename)
    check_function_length(lines, filename)
    for line in lines:
        check_class_naming(line, filename)
        check_function_naming(line, filename)
    check_function_comments(lines, filename)

if __name__ == "__main__":
    for filename in sys.argv[1:]:
        check_file(filename)

