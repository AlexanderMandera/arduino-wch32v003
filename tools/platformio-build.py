# Copyright 2023-present Maximilian Gerhardt <maximilian.gerhardt@rub.de>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""
Arduino

Arduino Wiring-based Framework allows writing cross-platform software to
control devices attached to a wide range of Arduino boards to create all
kinds of creative coding, interactive objects, spaces or physical experiences.

http://arduino.cc/en/Reference/HomePage
"""

from os.path import isdir, join

from SCons.Script import DefaultEnvironment

env = DefaultEnvironment()
platform = env.PioPlatform()
board = env.BoardConfig()

FRAMEWORK_DIR = platform.get_package_dir("framework-arduinoch32v003")
assert isdir(FRAMEWORK_DIR)

machine_flags = [
    "-march=%s" % board.get("build.march"),
    "-mabi=%s" % board.get("build.mabi"),
]

# the compiler we're using in PlatformIO does not have "-misa-spec=2.2", so it's not included.

env.Append(
    ASFLAGS=machine_flags,
    ASPPFLAGS=[
        "-x", "assembler-with-cpp"
    ],

    CFLAGS=[
        "-std=gnu11"
    ],

    CCFLAGS=machine_flags + [
        "-Os",
        "-Wall",
        "-ffunction-sections",
        "-fdata-sections",
        #"-flto",
        #"-ffat-lto-objects",
        #"-fuse-linker-plugin",
        "-static-libgcc",
    ],

    CXXFLAGS=[
        "-fno-exceptions",
        "-fno-rtti",
        "-fpermissive",
        "-std=gnu++17"
    ],

    LINKFLAGS=machine_flags + [
        "-Os",
        "-Wl,--gc-sections",
        #"-flto",
        #"-fuse-linker-plugin",
        "-nostdlib",
        #"-fwhole-program",
        '-Wl,-Map="%s"' % join("${BUILD_DIR}", "${PROGNAME}.map")
    ],
    # no actual GCC preprocessing needed since no macros are used
    LDSCRIPT_PATH=join(FRAMEWORK_DIR, "ch32v003fun", "ch32v003fun", "ch32v003fun.ld"),

    CPPDEFINES= [
        "TINYVECTOR",
        "CPLUSPLUS",
        "ARDUINO_ARCH_WCH",
        ("ARDUINO", 10808)
    ],

    LIBS=[
        #File(join(FRAMEWORK_DIR, "misc", "libgcc.a"))
        "gcc"
    ],

    LIBSOURCE_DIRS=[
        join(FRAMEWORK_DIR, "libraries")
    ],

    CPPPATH=[
        join(FRAMEWORK_DIR, "cores", "arduino"),
        # we excplicitly cannot include the API folder here because "String.h" (Arduino)
        # conflicts with "string.h" (GCC toolchain standard header) on case-insensitive
        # filesystems (Windows)
        #join(FRAMEWORK_DIR, "cores", "arduino", "api"),
        join(FRAMEWORK_DIR, "cores", "arduino", "api", "deprecated"),
        join(FRAMEWORK_DIR, "cores", "arduino", "api", "deprecated-avr-comp"),
        join(FRAMEWORK_DIR, "ch32v003fun", "ch32v003fun"),
        join(FRAMEWORK_DIR, "cores", "arduino", "hal"),
    ]
)

#
# Target: Build Core Library
#

libs = []

if "build.variant" in board:
    variants_dir = join(
        "$PROJECT_DIR", board.get("build.variants_dir")) if board.get(
            "build.variants_dir", "") else join(FRAMEWORK_DIR, "variants")

    env.Append(
        CPPPATH=[
            join(variants_dir, board.get("build.variant"))
        ]
    )
    env.BuildSources(
        join("$BUILD_DIR", "FrameworkArduinoVariant"),
        join(variants_dir, board.get("build.variant"))
    )

# We have build this as sources and not as library
# to get linking to work properly (??? not sure why yet)
env.BuildSources(
    join("$BUILD_DIR", "FrameworkArduino"),
    join(FRAMEWORK_DIR, "cores", "arduino"),
    # Hack: including IPAddress.cpp in the build, although unused,
    # blows up firmware size. Neither gc-sections nor LTO can
    # get rid of this :(
    "+<*> -<api/IPAddress.cpp>"
)
# The "Released" package copies these files into core/arduino, but we use git version
# so we have to build those manually.
env.BuildSources(
    join("$BUILD_DIR", "ch32v003fun"),
    join(FRAMEWORK_DIR, "ch32v003fun", "ch32v003fun"),
)

env.Prepend(LIBS=libs)
