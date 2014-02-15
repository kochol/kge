-- A solution contains projects, and defines the available configurations
solution "kge"
   configurations { "Debug", "Release" }
 
   configuration "Debug"
      defines { "DEBUG" }
      flags { "Symbols"}
 
    configuration "Release"
      defines { "NDEBUG" }
      flags { "Optimize" }    

   -- KGE
   project "kge"
      kind "SharedLib"
      language "C++"
      files { "Source/**.cpp" }
      excludes {"Source/sn/fmod/*.cpp", "Source/sn/bass/*.cpp", "Source/av/**.cpp"}
      includedirs { "Libs/sdl" }
      buildoptions { "-fpermissive" }
        links { "SDL2-2.0" }
        libdirs { "." }

   -- 01HelloWorld
   project "01HelloWorld"
      kind "ConsoleApp"
      language "C++"
      files { "tutorials/tut01/main.cpp" }
      buildoptions { "-fpermissive" }
        links { "kge" }
