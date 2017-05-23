from conans import ConanFile, CMake 

class QEEntityConan(ConanFile):
    name = "QEEntity"
    version = "1.0.0"
    requires = "QEAnnotation/1.0.0@fmiguelgarcia/stable"
    settings = "os", "compiler", "build_type", "arch"
    license = "https://www.gnu.org/licenses/lgpl-3.0-standalone.html"
    generators = "cmake"
    description = "QE Annotated Entity library"
    exports_sources = ["src/*", "test/*", "tools/*", "CMakeLists.txt"]

    def build(self):
        cmake = CMake( self)
        cmake.configure()
        cmake.build()

    def package(self):
        self.copy( pattern="*.hpp", dst="include/qe/entity/",
                src="src/qe/entity/")
        self.copy( pattern="LICENSE.LGPLv3", dst="share/qe/entity")
        self.copy( pattern="libQEEntity.so*", dst="lib",
                src="src/qe/entity", links=True)
        if self.settings.os == "Windows":
            libNames = ["QEEntity", "libQEEntity"]
            libExts = [".dll", ".lib", ".dll.a", ".pdb"]
            for libName in libNames:
                for libExt in libExts:
                    filePattern = "**/" + libName + libExt
                    self.copy( pattern=filePattern, dst="lib", src="src/qe/entity", keep_path=False)

    def package_info(self):
        self.cpp_info.libs.extend(["QEEntity"])
