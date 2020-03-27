solution "LIBUVPP"
    location ".build"
    language "C++"
    objdir ".obj"
    configurations { "DebugLib", "ReleaseLib" }

project "uvpp"
    kind "StaticLib" -- ConsoleApp,SharedLib,StaticLib
    files { "**.cpp", "**.c", "**.h", "**.hpp" }
    excludes { "tests/*" }
    includedirs { "include", "3party/libuv", "3party/libcore/include", "3party/tcmalloc/include" }
    libdirs { "lib", "3party/libuv/lib", "3party/libcore/lib", "3party/tcmalloc/lib" }
    configuration { "not vs* or codeblocks" }
    buildoptions { "-std=c++11" }

    configuration { "DebugLib" }
        symbols "On"
        defines "_DEBUG"
        defines { "USE_TC_MALLOC", "MEMPOOL_CHECK_OVERFLOW" }
        targetdir "lib"
        -- buildoptions { "-g3", "-gdwarf-2", "-std=c++11" }
        buildoptions { "-g3", "-gdwarf-2" }
        -- linkoptions { "-pagezero_size 0x10000", "-image_base 100000000" }
        links { "uv", "tcmalloc_minimal", "core" }

    configuration { "ReleaseLib" }
        symbols "On"
        defines "NDEBUG"
        defines { "USE_TC_MALLOC" }
        targetdir "lib"
        buildoptions { "-O3" }
        links { "uv", "tcmalloc_minimal", "core" }
