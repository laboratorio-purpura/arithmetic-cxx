from conan import ConanFile

class PurpleRecipe(ConanFile):
    settings = "os","compiler","build_type","arch"
    generators = "CMakeDeps"

    def requirements(self):
        self.requires("benchmark/1.9.5")
        self.requires("fmt/11.2.0")
        self.requires("gmp/6.3.0")
        self.requires("gtest/1.17.0")
