// File name: kge.h
// Des: in file tamam include haye lazem kar ba engine ra dar khod negah midarad ta
//      karbar baraye kar ba engine faghat lazem bashe in file ro include kone.
// Date: 6/5/1385
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifndef KGE_H
#define KGE_H

// ***
// kge
// ***
#include "kgedef.h"
#include "Device.h"
#include "PluginManager.h"

// *********
// kge::core
// *********
#include "core/String.h"
#include "core/Array.h"
#include "core/Array2D.h"
//#include "core/ArrayPriority.h"
#include "core/mem_fun.h"
#include "core/ptr_fun.h"
#include "core/KgeMemory.h"

// ********
// kge::gfx
// ********
#include "gfx/Renderer.h"
#include "gfx/Structs.h"
#include "gfx/Image.h"
#include "gfx/Tile.h"

// *******
// kge::av
// *******
#ifdef WIN32
#include "av/Structs.h"
#include "av/SoundBuffer.h"
#endif // WIN32

// ************
// kge::sn
// ************
#include "sn/AnimatedMesh.h"
#include "sn/StaticMesh.h"
#include "sn/Light.h"
#include "sn/TileTerrain.h"
#include "sn/TileTerrain2.h"
#include "sn/AnimatedTexture.h"
#include "sn/ParticleSystem.h"
#include "sn/RibbonTrail.h"

// *******
// kge::io
// *******
#include "io/Iwindow.h"
#include "io/Logger.h"
#include "io/File.h"
#include "io/JoyStick.h"

// *********
// kge::math
// *********
#include "math/Plane.h"
#include "math/Triangle.h"
#include "math/Shape.h"
#include "math/Sphere.h"
#include "math/AABB.h"
#include "math/Quat.h"

// ********
// kge::efx
// ********
#include "efx/Effect.h"
#include "efx/EffectManager.h"
#include "efx/ShadowMapProjection.h"
#include "efx/Refraction.h"

// *******
// PhysicsFS
// *******
#include "../Libs/physfs/physfs.h"

// *******
// TinyXML
// *******
//#include "../Libs/TinyXML/tinyxml.h"

// *******
// Visual Leak Detector
// *******
//#include <vld.h>

#endif // KGE_H


/*! \mainpage
\section intro Introduction

Kochol game engine is an easy to use game engine but it is in early releases.
It is written in C++ with good OO and the design is very flexible.
It uses Direct3D 9 and OpenGL and can be run on Windows and Linux.
KGE is open source and you can customize it with your need.

This is a documention which describes most of the classes and functions if you need more info
visit <A class="el" HREF="http://kge3d.org" >http://kge3d.org</A>

We want to learn game programming with developing this engine so we read the books and
online resources to make an advanced game engine.

\section link Links

<table>
<tr>
<td class="indexkey">
\ref tutorials
</td>
<td class="indexkey">
The KGE tutorials
</td>
</tr>
	<tr>
		<td class="indexkey">
			<A class="el" HREF="http://kge3d.org" >KGE website</A>
		</td>
		<td class="indexkey">
			The official website that you can find anything about KGE and game programming.
		</td>
	</tr>
	<tr>
		<td class="indexkey">
			<A class="el" HREF="http://kge3d.org/forums" >Forum</A>
		</td>
		<td class="indexkey">
			Ask your question about KGE and game programming here.
		</td>
	</tr>
	<tr>
		<td class="indexkey">
			<A class="el" HREF="http://wiki.kge3d.org" >Wiki</A>
		</td>
		<td class="indexkey">
			Find the tutorials and extra information here.
		</td>
	</tr>
</table>
*/
