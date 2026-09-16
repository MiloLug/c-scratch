from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps

class gcstConan(ConanFile):
    settings = "os", "arch", "compiler", "build_type"
    
    def requirements(self):
        self.requires("sdl/2.32.10")
        self.requires("sdl_image/2.8.12")

    def generate(self):
        CMakeToolchain(self).generate()
        CMakeDeps(self).generate()