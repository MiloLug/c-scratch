from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps

class gcstConan(ConanFile):
    settings = "os", "arch", "compiler", "build_type"
    requires = (
        "sdl/2.32.10",
        "sdl_image/2.8.12"
    )

    default_options = {
        "sdl/*:pulse": False,
        "sdl/*:alsa": False,
        "sdl/*:wayland": False,
        "sdl/*:libunwind": False,
        "sdl/*:iconv": False,
        "sdl_image/*:with_libtiff": False,
        "sdl_image/*:with_libwebp": False,
        "sdl_image/*:with_libjpeg": False,
    }

    def requirements(self):
        if self.settings.os == "Linux":
            self.requires("xorg/system")

    def generate(self):
        CMakeToolchain(self).generate()
        CMakeDeps(self).generate()