-- A solution contains projects, and defines the available configurations
workspace "kge"
  configurations { "Debug", "Release" }
  platforms { "Win32", "Win64" }
  includedirs { "deps/bx/include", "deps/bx/3rdparty", "deps/bgfx/include", "deps/bimg/include" }

  if _ACTION:find("vs") then
    includedirs { "deps/bx/include/compat/msvc" }
  end

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
    links { "SDL2",  "ILU" }
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

  -- bx
  project "bx"
    kind "StaticLib"
    language "C++"    
    files { "deps/bx/src/*.cpp"}
    defines {
      "__STDC_LIMIT_MACROS",
      "__STDC_FORMAT_MACROS",
      "__STDC_CONSTANT_MACROS",
    }
  
  -- bimg
  project "bimg"
    kind "StaticLib"
    language "C++"
    files { "deps/bimg/src/*.cpp"}
    includedirs { "deps/bimg/3rdparty", "deps/bimg/3rdparty/iqa/include" }

  -- bgfx
  project "bgfx"
    kind "StaticLib"
    language "C++"
    files { "deps/bgfx/src/*.cpp" }
    includedirs { "deps/bgfx/3rdparty", "deps/bgfx/3rdparty/khronos", "deps/bgfx/3rdparty/dxsdk/include" }

  -- RendererBgfx
  project "Renderer_Bgfx"
    kind "SharedLib"
    language "C++"
    files { "plugins/Renderer_bgfx/*.cpp" }
    links { "bgfx", "bx" }

  -- 01HelloWorld
  project "01HelloWorld"
    kind "ConsoleApp"
    language "C++"
    files { "tutorials/tut01/main.cpp" }
    buildoptions { "-fpermissive" }
    links { "kge" }
