from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps


class HSharpVERecipe(ConanFile):
    name = "HSharpVE"
    version = "0.1"
    package_type = "application"

    license = "GPLv3"
    author = "HyperWinX, AshFungor"
    url = "https://github.com/randommfs/HSharpVE"
    description = "Simple programming language with a stack based VM"

    settings = "os", "compiler", "build_type", "arch"

    requires = [
        "gtest/1.14.0",
        #"boost/1.87.0",
        "spdlog/1.15.0",
        "argparse/3.2",
        "eternal/1.0.1",
        "fmt/11.0.2"
    ]

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    

    
