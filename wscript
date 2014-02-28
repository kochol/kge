import urllib2
import os.path

APPNAME = 'kge'
VERSION = '0.2.0'

top = '.'
out = 'bin'
outsdl = out + 'sdlcmake'

def download(url, saveto):
	file_name = saveto + '/' + url.split('/')[-1]
	if os.path.isfile(file_name):
		print file_name + " already exist."
		return
	u = urllib2.urlopen(url)
	f = open(file_name, 'wb')
	meta = u.info()
	file_size = int(meta.getheaders("Content-Length")[0])
	print "Downloading: %s Bytes: %s" % (file_name, file_size)

	file_size_dl = 0
	block_sz = 8192
	while True:
		buffer = u.read(block_sz)
		if not buffer:
			break

		file_size_dl += len(buffer)
		f.write(buffer)
		status = r"%10d  [%3.2f%%]" % (file_size_dl, file_size_dl * 100. / file_size)
		status = status + chr(8)*(len(status)+1)
		print status,

	f.close()

def FindOS(ctx):
	arch = "32"
	if ctx.env.DEST_CPU == "x86_64" or ctx.env.DEST_CPU == "amd64":
		arch = "64"
	print arch

def options(opt):
        opt.load('compiler_c compiler_cxx')

def configure(conf):	
	download("https://github.com/kochol/kge/releases/download/0.1.0/kge010.zip", "Libs")
        conf.load('compiler_c compiler_cxx')
	conf.setenv('debug')
	print conf.env['DEST_CPU']

def build(bld):
	print 'C Flags:', bld.env['CFLAGS']
	print 'CXX Flags:', bld.env['CXXFLAGS']
	print 'Linker Flags:', bld.env['LINKFLAGS']
	print 'Defines:', bld.env['DEFINES']
	FindOS(bld)

	print 'Building SDL2 please wait ...'
	bld(rule='touch sdlcmake;cd sdlcmake;cmake ../../Libs/SDL;make;cd ..')

	bld.read_shlib('SDL2-2.0', paths=['bin/sdlcmake'])
	bld.read_shlib('IL', paths=['Libs'])
	bld.read_shlib('ILU', paths=['Libs'])
	bld.read_shlib('GLEW', paths=['Libs'])

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

	# Build Plugins
	bld.recurse('plugins')

	# Build tutorials
	bld.recurse('tutorials/tut01')
