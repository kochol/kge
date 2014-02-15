APPNAME = 'kge'
VERSION = '0.1.0'

top = '.'
out = 'bin'
outsdl = out + 'sdlcmake'

def options(opt):
        opt.load('compiler_c compiler_cxx')

def configure(conf):
        conf.load('compiler_c compiler_cxx')

def build(bld):
	print 'C Flags:', bld.env['CFLAGS']
	print 'CXX Flags:', bld.env['CXXFLAGS']
	print 'Linker Flags:', bld.env['LINKFLAGS']
	print 'Defines:', bld.env['DEFINES']

	print 'Building SDL2 please wait ...'
	bld(rule='touch sdlcmake;cd sdlcmake;cmake ../../Libs/SDL;make;cd ..')

	bld.read_shlib('SDL2-2.0', paths=['bin/sdlcmake'])
	bld.read_shlib('IL', paths=['Libs'])
	bld.read_shlib('ILU', paths=['Libs'])

	# Add source files
	files = []
	files.extend(bld.path.ant_glob("Source/*.cpp"))
	files.extend(bld.path.ant_glob("Source/*/*.cpp"))
	files.extend(bld.path.ant_glob("Libs/stb_image.c"))

	# Bulid KGE
	bld.shlib(
		source = files,
		target = "kge",
		name = "kge",
		includes     = ['Libs/SDL/include'],
		cxxflags     = ['-fpermissive'],
		use = ['SDL2-2.0', 'IL', 'ILU']
	)

	# Build tutorials
	bld.recurse('tutorials/tut01')
