solution "LIBUVPP"
    location ".build"
    language "C++"
    objdir ".obj"
    configurations { "DebugLib", "ReleaseLib" }

project "uvpp"
    kind "StaticLib" -- ConsoleApp,SharedLib,StaticLib
    files { "**.cpp", "**.c", "**.h", "**.hpp" }
    excludes { "tests/*" }
    includedirs { "include" } 
    libdirs { "lib" }
    configuration { "not vs* or codeblocks" }
    buildoptions { "-std=c++11", "-fPIC" }

    configuration { "DebugLib" }
        symbols "On"
        defines "_DEBUG"
        targetdir "lib"
        buildoptions { "-g3" }

    configuration { "ReleaseLib" }
        symbols "On"
        targetdir "lib"
        buildoptions { "-O3" }

    linkoptions { "-g3" }
    buildoptions { "-std=c++11" }
    links { "uv" }

