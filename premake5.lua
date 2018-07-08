-- A solution contains projects, and defines the available configurations
workspace "kge"
  configurations { "Debug", "Release" }
  platforms { "Win32", "Win64" }

  filter { "platforms:Win32" }
      system "Windows"
      architecture "x32"
  
  filter { "platforms:Win64" }
      system "Windows"
      architecture "x64"

  filter "configurations:Debug"
    defines { "DEBUG" }
    if os.host() == "windows" then
      defines { "WIN32" }
    end
    symbols "On"
    targetdir "bin/debug"

  filter "configurations:Release"
    defines { "NDEBUG" }
    if os.host() == "windows" then
      defines { "WIN32" }
    end
    optimize "On"
    targetdir "bin/release"

  -- KGE
  project "kge"
    kind "SharedLib"
    language "C++"
    files { "src/**.cpp", "src/gfx/stb_image.c" }
    excludes {"src/sn/fmod/*.cpp", "src/sn/bass/*.cpp", "src/av/**.cpp"}
    includedirs { "deps/SDL2", "deps/devil/include" }
    defines { "KGE_EXPORT" }    
    buildoptions { "-fpermissive" }
    links { "SDL2",  "ILU"}
    if os.host() == "windows" then
      links { "DevIL" }
    else
      links { "IL" }
    end
    characterset("MBCS")   
    if os.is64bit() and os.host() == "windows" then
      libdirs ({ "deps/devil/lib/x64/Release", "deps/SDL2/lib/x64" })
    end
    if os.is64bit == false and os.host() == "windows" then
      libdirs ({ "deps/devil/lib/x86/Release", "deps/SDL2/lib/x86" })
    end
  
  -- OpenGL
  project "Renderer_ogl"
    kind "SharedLib"
    language "C++"
    files { "plugins/Renderer_ogl/*.cpp" }
    defines { "KGELIB_EXPORTS" }    
    buildoptions { "-fpermissive" }
    links { "kge", "Cg", "CgGL" }
    includedirs { "deps/SDL2" }
    libdirs {}
    if os.host() == "windows" and os.getenv("CG_INC_PATH") ~= nil then
      includedirs { os.getenv("CG_INC_PATH") }
      if os.is64bit() and os.host() == "windows" then
        libdirs ({ os.getenv("CG_LIB64_PATH"), "deps/SDL2/lib/x64", "Libs/glew/x64" })
        links { "glew32" }
      end
      if os.is64bit == false and os.host() == "windows" then
        libdirs ({ os.getenv("CG_LIB_PATH"), "deps/SDL2/lib/x86", "Libs/glew/Win32" })
        links { "glew32" }
      end
    elseif os.getenv("CG_INC_PATH") == nil then
      print "CG_INC_PATH not found install CG"
    end

  -- Drict3D9
  project "Renderer_d3d9"
    kind "SharedLib"
    language "C++"
    files { "plugins/Renderer_d3d9/*.cpp" }
    defines { "KGELIB_EXPORTS" }    
    buildoptions { "-fpermissive" }
    links { "kge", "legacy_stdio_definitions" }
    characterset("MBCS")   
    includedirs { "deps/SDL2" }
    if os.getenv("DXSDK_DIR") ~= nil then
      includedirs { os.getenv("DXSDK_DIR") .. "include" }
      if os.is64bit() then
        libdirs ({ os.getenv("DXSDK_DIR") .. "Lib/x64"})
      end
      if os.is64bit == false then
        libdirs ({ os.getenv("DXSDK_DIR") .. "Lib/x86"})
      end
    else
        print "DXSDK_DIR not found"
    end

  -- 01HelloWorld
  project "01HelloWorld"
    kind "ConsoleApp"
    language "C++"
    files { "tutorials/tut01/main.cpp" }
    buildoptions { "-fpermissive" }
    links { "kge" }
