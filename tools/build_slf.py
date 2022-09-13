import argparse
import os
import re
import sys
from os import path

# config
DEFAULT_ROOT_FILENAME = "anims.h"
DEFAULT_OUTPUT_FILENAME = "out.h"

# templates
HEADER_TMPL = """#ifndef {name_upper}_H
# define {name_upper}_H

# include "animation.h"

# define FRAME_WIDTH                    {width}
# define FRAME_HEIGHT                   {height}
# define FRAME_COUNT                    {frame_count}

static const char *__{name}_orig_frame[FRAME_HEIGHT] = {{
{orig_frame_rows}
}};

{frame_updates_list}

static struct animation __{name}_animation = {{
    .width = FRAME_WIDTH,
    .height = FRAME_HEIGHT,
    .frame_count = FRAME_COUNT,
    .orig_frame = __{name}_orig_frame,
    .frames = (struct animation_frame_update_list *)__{name}_fru_list__,
}};

# endif
"""
FRAME_ROW_TMPL = """    "{row}\""""
FRAME_UPDATES_ROW_TMPL = """{{
    .pos = {{ .x = {x}, .y = {y}, .len = {len} }},
    .content = "{content}",
}}"""

FRAME_UPDATES_TMPL = """/* Updates for frame {n} */
# define __{name_upper}_UPDATES_FR_{n}__ {updates_count}
static struct animation_frame_update __{name}_fu_list_{n}__[__{name_upper}_UPDATES_FR_{n}__] = {{
{frame_updates_rows}
}};

"""
FRAME_UPDATES_LIST_ROW_TMPL = """{{ .len = {fru_len}, .updates = (struct animation_frame_update *)&__{name}_fu_list_{n}__ }}"""
FRAME_UPDATES_LIST_TMPL = """{frame_updates}
static struct animation_frame_update_list __{name}_fru_list__[FRAME_COUNT - 1] = {{
{frame_updates_list_rows}
}};
"""
ROOT_HEADER_INCLUDES_TMPL = """# include \"{header_filename}\""""
ROOT_HEADER_TMPL = """#ifndef {name_upper}
# define {name_upper}

{header_includes}

# define ANIMATION_COUNT {animation_count}

static struct animation *__animations_list[ANIMATION_COUNT] = {{
{animation_names}
}};

#endif
"""

def escape_str(s):
    return s.replace('\\', '\\\\').replace('"', '\\"')


def read_frame(filename):
    with open(filename, 'r') as f:
        return f.read()


def write_header(header, filename):
    with open(filename, 'w+') as f:
        f.write(header)


def normalize_name(name):
    return re.sub('[^a-z]', '_', name.lower())


def retrieve_frames_from_slf(dirname):
    lsdir = sorted(os.listdir(dirname))
    frames = []
    i = 0
    for slf in lsdir:
        [frame_number, ext] = path.splitext(slf)
        if ext != '.slf':
            continue
        if frame_number != str(i):
            raise ValueError(f"{i}.slf is missing")
        frames.append(read_frame(f'{dirname}/{slf}'))
        i += 1
    return frames


def find_diffs(orig_row, frame_row, y):
    frame_len = len(frame_row)
    orig_frame_len = len(orig_row)
    i = 0
    while i < orig_frame_len:
        if i >= frame_len:
            break
        start = i
        if orig_row[i] != frame_row[i]:
            content = frame_row[i:]
            diff = {
                'x': start,
                'y': y,
                'len': len(content),
                'content': escape_str(content),
            }
            return diff
        i += 1
    return None


def find_frame_updates(frames):
    all_diffs = []
    orig = frames[0]
    orig_rows = orig.split('\n')
    frames = reversed(frames[1:])
    for frame in frames:
        frame_rows = frame.split('\n')
        frame_rows_len = len(frame_rows)
        frame_updates = []
        y = 0
        for i in range(len(orig_rows)):
            if i >= frame_rows_len:
                break
            row_diff = find_diffs(orig_rows[i], frame_rows[i], y)
            if row_diff is not None:
                frame_updates.append(row_diff)
            y += 1
        all_diffs.append(frame_updates)
    return all_diffs


def build_frame_list(name, frame_count, all_frame_updates):
    name_upper = name.upper()
    frame_updates = []
    frame_updates_list_rows = []
    for n, fr in enumerate(all_frame_updates):
        updates_count = len(fr)
        frame_updates_rows = ',\n'.join((
            FRAME_UPDATES_ROW_TMPL.format(**row)
            for row in fr
        ))
        fr_update = FRAME_UPDATES_TMPL.format(
            name=name,
            name_upper=name_upper,
            updates_count=updates_count,
            frame_updates_rows=frame_updates_rows,
            n=n + 1,
        )
        frame_updates.append(fr_update)
        fr_update_list_row = FRAME_UPDATES_LIST_ROW_TMPL.format(name=name, n=n + 1, fru_len=len(fr))
        frame_updates_list_rows.append(fr_update_list_row)
    frame_updates = '\n'.join(frame_updates)
    frame_updates_list_rows = ',\n'.join(frame_updates_list_rows)
    frame_updates_list = FRAME_UPDATES_LIST_TMPL.format(
        name=name,
        frame_updates=frame_updates,
        frame_updates_list_rows=frame_updates_list_rows,
    )
    return frame_updates_list


def build_header_from_frames(name, dirname):
    frames = retrieve_frames_from_slf(dirname)
    orig_frame = frames[0]
    orig_frame_rows = [row + ' ' for row in orig_frame.split('\n')]
    width = max((len(row) for row in orig_frame_rows))
    orig_frame_rows.append(' ') # last row is an empty line
    # pad lines with space
    orig_frame_rows = [row.ljust(width, ' ') for row in orig_frame_rows]

    name_upper = name.upper()
    height = len(orig_frame_rows)
    frame_count = len(frames)
    all_frame_updates = find_frame_updates(frames)
    orig_frame_rows = ',\n'.join(
        FRAME_ROW_TMPL.format(row=escape_str(row))
        for row in orig_frame_rows
    )
    frame_updates_list = build_frame_list(name, frame_count, all_frame_updates)

    header = HEADER_TMPL.format(
        name=name, name_upper=name_upper,
        width=width, height=height,
        frame_count=frame_count,
        orig_frame_rows=orig_frame_rows,
        frame_updates_list=frame_updates_list,
    )

    return header


def cmd_build(args):
    try:
        name = normalize_name(path.basename(args.anim_dir))
        header = build_header_from_frames(name, args.anim_dir)
        write_header(header, args.output)
    except IOError as err:
        print(err)
        return err.errno
    return 0


def cmd_export(args):
    ls_dir = os.listdir(args.anim_dir)
    anim_names = []
    for anim in ls_dir:
        if not path.isdir(path.join(args.anim_dir, anim)):
            continue
        anim_names.append(path.basename(anim))
    header_includes = (
        ROOT_HEADER_INCLUDES_TMPL.format(
            header_filename=f'{name}.h',
        )
        for name in anim_names
    )
    header_includes = '\n'.join(header_includes)
    name = normalize_name(path.basename(args.output))
    name_upper = name.upper()
    animation_names = '\n,'.join(f'&__{name}_animation' for name in anim_names)
    root_header = ROOT_HEADER_TMPL.format(
        name_upper=name_upper,
        header_includes=header_includes,
        animation_names=animation_names,
        animation_count=len(anim_names),
    )
    write_header(root_header, args.output)

    return 0


def main():
    parser = argparse.ArgumentParser()
    cmdparsers = parser.add_subparsers(dest='command')

    build_parser = cmdparsers.add_parser('build', help='build a header from an animation')

    build_parser.add_argument(
        '-o', '--output', type=str, default=DEFAULT_OUTPUT_FILENAME,
        help='output file name',
    )
    build_parser.add_argument(
        'anim_dir', type=str,
        help='path to an animation directory that contains *.slf files',
    )
    build_parser.set_defaults(cmd=cmd_build)

    export_parser = cmdparsers.add_parser('export_root', help='build a root header from animation headers')

    export_parser.add_argument(
        '-o', '--output', type=str, default=DEFAULT_ROOT_FILENAME,
        help='output file name',
    )
    export_parser.add_argument(
        'anim_dir', type=str,
        help='directory that contains animation headers',
    )
    export_parser.set_defaults(cmd=cmd_export)
    args = parser.parse_args()

    return args.cmd(args)


if __name__ == '__main__':
    sys.exit(main())
