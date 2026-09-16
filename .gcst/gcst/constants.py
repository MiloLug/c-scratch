from functools import cached_property

import subprocess
from pathlib import Path

name = "CMakeAutoBuild"

class _Paths:
    defaultPresetFilename = ".default"

    @cached_property
    def here(self):
        return Path(__file__).resolve().parent

    @cached_property
    def repo(self):
        return Path(subprocess.check_output(['git', '-C', self.here, 'rev-parse', '--show-toplevel'], text = True).strip()).resolve()

    @cached_property
    def srepo(self):
        path = subprocess.check_output(['git', '-C', self.here, 'rev-parse', '--show-superproject-working-tree'], text = True).strip()
        if path == '':
            return self.repo
        return Path(path).resolve()

    @cached_property
    def submodule(self):
        try:
            submodules = subprocess.check_output(['git', '-C', self.repo, 'config', '--file', '.gitmodules', '--get-regexp', 'url'], text = True).strip().split("\n")
        except subprocess.CalledProcessError:
            return ''
        
        for sm in submodules:
            data = sm.split()
            if data[1].removesuffix('.git') == f'https://github.com/Gaymocoder/{name}':
                return Path(data[0][len('submodule.'):-len('.url')]).absolute()
        return ''

    @cached_property
    def gcst(self):
        return self.repo/".gcst"

    @cached_property
    def bin_dir(self):
        return self.repo/"bin"

    @cached_property
    def temp_dir(self):
        return self.gcst/".temp"

    @cached_property
    def build_dir(self):
        return self.repo/"build"

    @cached_property
    def configure_py(self):
        return self.gcst/"scripts"/"configure.py"
    
    @cached_property
    def default_preset(self):
        return self.gcst/self.defaultPresetFilename


paths = _Paths()