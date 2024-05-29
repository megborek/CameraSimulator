from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout
from conan.tools.system.package_manager import Apt

class ISPProjectConan(ConanFile):
    name = "ISPProject"
    version = "1.0"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"
    default_options = {
        "opencv/*:shared": True,
        "opencv/*:with_contrib": True  # Ensure the contrib modules are included
    }

    def requirements(self):
        self.requires("opencv/4.5.5")
        self.requires("cli11/2.2.0")

    def layout(self):
        cmake_layout(self)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin")
        self.copy("*.dylib*", dst="bin", src="lib")
        self.copy("*.so*", dst="bin", src="lib")
        self.copy("*.pdb", dst="bin", src="bin")
        self.copy("*.dSYM", dst="bin", src="lib")

    def deploy(self):
        self.copy("*", src="bin", dst="bin")
        self.copy("*.dll", src="bin", dst="bin")
        self.copy("*.dylib*", src="lib", dst="bin")
        self.copy("*.so*", src="lib", dst="bin")
