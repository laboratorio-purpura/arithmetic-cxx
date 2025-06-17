from conan import ConanFile

class PurpleRecipe(ConanFile):
    settings = "os","compiler","build_type","arch"
    generators = "CMakeDeps"

    def requirements(self):
        self.requires("fmt/11.2.0")
        self.requires("gtest/1.16.0")
