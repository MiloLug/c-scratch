from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps

class gcstConan(ConanFile):
    settings = "os", "arch", "compiler", "build_type"
    requires = (
        "sdl/2.32.10",
        "sdl_image/2.8.12"
    )

    def generate(self):
        CMakeToolchain(self).generate()
        CMakeDeps(self).generate()