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
      files { "Source/**.cpp", "Libs/stb_image.c" }
      excludes {"Source/sn/fmod/*.cpp", "Source/sn/bass/*.cpp", "Source/av/**.cpp"}
      includedirs { "Libs/SDL/include" }
      buildoptions { "-fpermissive" }
        links { "SDL2" }
        libdirs { "." }
  
   -- OpenGL
   project "Renderer_ogl"
      kind "SharedLib"
      language "C++"
      files { "plugins/Renderer_ogl/*.cpp" }
      includedirs { "Libs/SDL/include" }
      buildoptions { "-fpermissive" }
        links { "kge", "Cg", "CgGL", "GLEW" }
        libdirs { "." }

   -- 01HelloWorld
   project "01HelloWorld"
      kind "ConsoleApp"
      language "C++"
      files { "tutorials/tut01/main.cpp" }
      buildoptions { "-fpermissive" }
        links { "kge" }
