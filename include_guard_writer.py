# created by Chuilian Kong on 04/23/2018
# this script is intended to write "include guard" for all header files
# in a directory. The files that already had a properly formatted include guard
# will not be modified.
# this script refers Google c++ style for the guard style
# https://google.github.io/styleguide/cppguide.html#The__define_Guard

# The writer will only check if the first two and the last statements are
# properly formatted header guard. (the checker will omit comments)
# It won't change any other #ifndef statements

# this script is only intended to be used on Unix-like systems

from pathlib import Path
import sys, os

# first param (optional): the target directory,
# the writer will search all headers under this directory, default is cwd
target_path = sys.argv[1]
if not target_path:
    target_path = os.getcwd()

p = Path(target_path)

# second param (optional): the root directory of the project,
# Google header guard style require guard to be relative path of
# the project root, default is cwd
root_path = sys.argv[2]
if not root_path:
    root_path = os.getcwd()



headers = list(p.glob('**/*.h'))
lines = []
for header in headers:
    print('checking ' + str(header))

    # load header file
    with header.open() as f:
        lines = f.readlines()

    # define header guard style
    try:
        guard = str(header.relative_to(root_path)).replace('/','_').replace('.','_').replace('\\','_').upper() + '_'
        print(guard)
    except ValueError:
        sys.exit('target directory should start with project root')

    # define guard statement
    standard = []
    standard.append('#ifndef ' + guard + '\n')
    standard.append('#define ' + guard + '\n')

    standard.append('#endif  // ' + guard +'\n')

    # find first non-comment line
    line_counter = -1
    begin_comment = False
    for line in lines:
        line_counter += 1
        no_space_line = line.strip(' \n\t')

        if no_space_line.startswith('/*'):
            begin_comment = True
            continue
        elif no_space_line.endswith('*/'):
            begin_comment = False
            continue
        elif line in ['\n', '\r\n'] or begin_comment == True or no_space_line.startswith('//'):
            continue
        else:
            break

    begin_line = line_counter

    # find last non-comment line
    line_counter = len(lines)
    begin_comment = False
    for line in reversed(lines):
        line_counter -= 1
        no_space_line = line.strip()

        if no_space_line.endswith('*/'):
            begin_comment = True
            continue
        elif no_space_line.startswith('/*'):
            begin_comment = False
            continue
        elif line in ['\n', '\r\n'] or begin_comment == True or no_space_line.startswith('//'):
            continue
        else:
            break

    end_line = line_counter

    print(begin_line)
    print(end_line)
    print(len(lines))

    # perform check
    has_guard = True
    if end_line - begin_line < 2:
        has_guard = False
    elif lines[begin_line] != standard[0] or lines[begin_line + 1] != standard[1] or lines[end_line] != standard[2]:
        has_guard = False

    if has_guard:
        print('pass')
    else:
        print('fail')

        new_header = header.with_name('temp.tmp')
        # write modification to a new file
        with new_header.open(mode = 'w') as f:
            # write forehead comment, if any
            if begin_line >= 1:
                for i in range(0, begin_line):
                    f.write(lines[i])
            # write the first two guard statements
            f.write(standard[0])
            f.write(standard[1])
            f.write('\n')
            # write following code blocks, if any
            if begin_line >= 0:
                for i in range(begin_line, end_line + 1):
                    f.write(lines[i])
            # write tailing guard statements
            f.write('\n')
            f.write(standard[2])
            # write tailing comments, if any
            if end_line < len(lines):
                for i in range(end_line + 1, len(lines)):
                    f.write(lines[i])

        # replace the original file with the modified file
        new_header.replace(header)

        print('modified')
