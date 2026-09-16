import os, sys
import shutil, filecmp
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / ".gcst"))
import gcst

install_and_update = [
    ".github/workflows/ci.yml",
    ".github/workflows/scripts/g++.sh",
    ".github/workflows/scripts/clang.sh",
    ".github/workflows/scripts/mingw32.bat",
 
    ".gcst/presets.json",
    ".gcst/gcst/__init__.py",
    ".gcst/gcst/constants.py",
    ".gcst/scripts/build.py",
    ".gcst/scripts/configure.py",
 
    "cmake/gcst/utils.cmake",
    "cmake/gcst/warnings.cmake",
    "cmake/gcst/toolchains/w64-mingw32.cmake",
 
    "scripts/.gcstu-install-only",
    "scripts/gcst_update.py",

    ".gitignore",
    "build.bat",
    "build.sh",

    "conanfile.py",
    "CMakeLists.txt",
]

install_only = []

SRC = gcst.paths.repo
DEST = gcst.paths.srepo

def get_install_only():
    global install_and_update, install_only
    with open(Path(__file__).parent/".gcstu-install-only", 'a+', encoding = 'utf-8') as gio_file:
        gio_file.seek(0)
        install_only = [line.strip() for line in gio_file.readlines()]

    install_and_update = [file for file in install_and_update if file not in install_only]

def get_updating_files():
    all_files = []
    files_to_update = install_and_update + install_only
    for entry in files_to_update:
        path = Path(SRC / entry).absolute()
        if not path.is_dir():
            all_files.append(path.relative_to(SRC, walk_up = True))
            continue

        for file in path.rglob("*"):
            if file.is_dir():
                continue
            all_files.append(file.relative_to(SRC, walk_up = True))

    _, mismatches, errors = filecmp.cmpfiles(DEST, SRC, all_files, shallow=False)
    files_to_update = []
    for name in all_files:
        if (name in set(mismatches) | set(errors)) and (SRC/name).exists():
            if (str(name) in install_only) and (DEST/name).exists():
                continue
            files_to_update.append(name)

    return files_to_update

def main():
    global SRC
    if (DEST == SRC):
        SRC = gcst.paths.submodule
        if (SRC == ''):
            print("The gcstemplate is not a submodule of any repo. Merging impossible")
            return 1

    get_install_only()
    mismatches = get_updating_files()
    if (mismatches == []):
        print("Everything is up-to-date")
        return 0

    files_list = ''.join(f'  .{os.sep}{p}\n' for p in mismatches)
    print(f"WARNING! The gcstemplate files will replace these files in your root repo directory:\n{files_list}")
    confirm = input("Make sure you've backuped all important edits from the files before updating them\nDo you want to continue? [Y/n] ")
    if confirm.lower() != "y":
        print("Aborted.")
        return 0

    for file in mismatches:
        ifile = SRC / file
        ofile = DEST / file

        print(f'Copying "./{(ifile).relative_to(DEST, walk_up = True)}" to "./{(ofile).relative_to(DEST, walk_up = True)}"')
        ofile.parent.mkdir(parents = True, exist_ok = True)
        shutil.copy(ifile, ofile)

if __name__ == '__main__':
    main()