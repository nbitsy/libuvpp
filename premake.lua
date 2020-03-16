solution "LIBUVPP"
    location ".build"
    language "C++"
    objdir ".obj"
    configurations { "DebugLib", "ReleaseLib" }

project "uvpp"
    kind "StaticLib" -- ConsoleApp,SharedLib,StaticLib
    files { "**.cpp", "**.c", "**.h", "**.hpp" }
    excludes { "tests/*" }
    includedirs { "include", "3Party/libuv", "3Party/google" } 
    libdirs { "lib", "3Party/lib" }
    configuration { "not vs* or codeblocks" }
    buildoptions { "-std=c++11", "-fPIC" }

    configuration { "DebugLib" }
        symbols "On"
        defines "_DEBUG"
        defines { "USE_TC_MALLOC", "MEMPOOL_CHECK_OVERFLOW" }
        targetdir "lib"
        buildoptions { "-g3" }
        buildoptions { "-std=c++11" }
        links { "uv", "tcmalloc_minimal", "protobuf-lite" }

    configuration { "ReleaseLib" }
        symbols "On"
        defines { "USE_TC_MALLOC" }
        targetdir "lib"
        buildoptions { "-O3" }
        linkoptions { "-g3" }
        buildoptions { "-std=c++11" }
        links { "uv", "tcmalloc_minimal", "protobuf-lite" }

