//#############################################################################
//#	file:	initGL2.cpp
//#	Copyright (c) 2010	Pooya Shahinfar (Pswin)
//#############################################################################

#include "initGL.h"
#include "../include/Logger.h"



#if (KGE_PLATFORM == KGE_PLATFORM_WINDOWS) \
						&& !defined(APIENTRY)
	#define APIENTRY __stdcall
#endif

#if KGE_PLATFORM != KGE_PLATFORM_ANDROID

//--------------------------------
//	prototypes
//--------------------------------
void* loadEntryPoint(const char*);


//--------------------------------
//	global variables and functions
//--------------------------------
extern "C"
{

	PFNGLBLENDCOLORPROC glBlendColor;
	PFNGLBLENDEQUATIONPROC glBlendEquation;
	PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements;
	PFNGLTEXIMAGE3DPROC glTexImage3D;
	PFNGLTEXSUBIMAGE3DPROC glTexSubImage3D;
	PFNGLCOPYTEXSUBIMAGE3DPROC glCopyTexSubImage3D;
	PFNGLACTIVETEXTUREPROC glActiveTexture;
	PFNGLSAMPLECOVERAGEPROC glSampleCoverage;
	PFNGLCOMPRESSEDTEXIMAGE3DPROC glCompressedTexImage3D;
	PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D;
	PFNGLCOMPRESSEDTEXIMAGE1DPROC glCompressedTexImage1D;
	PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC glCompressedTexSubImage3D;
	PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glCompressedTexSubImage2D;
	PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC glCompressedTexSubImage1D;
	PFNGLGETCOMPRESSEDTEXIMAGEPROC glGetCompressedTexImage;
	PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate;
	PFNGLMULTIDRAWARRAYSPROC glMultiDrawArrays;
	PFNGLMULTIDRAWELEMENTSPROC glMultiDrawElements;
	PFNGLPOINTPARAMETERFPROC glPointParameterf;
	PFNGLPOINTPARAMETERFVPROC glPointParameterfv;
	PFNGLPOINTPARAMETERIPROC glPointParameteri;
	PFNGLPOINTPARAMETERIVPROC glPointParameteriv;
	PFNGLGENQUERIESPROC glGenQueries;
	PFNGLDELETEQUERIESPROC glDeleteQueries;
	PFNGLISQUERYPROC glIsQuery;
	PFNGLBEGINQUERYPROC glBeginQuery;
	PFNGLENDQUERYPROC glEndQuery;
	PFNGLGETQUERYIVPROC glGetQueryiv;
	PFNGLGETQUERYOBJECTIVPROC glGetQueryObjectiv;
	PFNGLGETQUERYOBJECTUIVPROC glGetQueryObjectuiv;
	PFNGLBINDBUFFERPROC glBindBuffer;
	PFNGLDELETEBUFFERSPROC glDeleteBuffers;
	PFNGLGENBUFFERSPROC glGenBuffers;
	PFNGLISBUFFERPROC glIsBuffer;
	PFNGLBUFFERDATAPROC glBufferData;
	PFNGLBUFFERSUBDATAPROC glBufferSubData;
	PFNGLGETBUFFERSUBDATAPROC glGetBufferSubData;
	PFNGLMAPBUFFERPROC glMapBuffer;
	PFNGLUNMAPBUFFERPROC glUnmapBuffer;
	PFNGLGETBUFFERPARAMETERIVPROC glGetBufferParameteriv;
	PFNGLGETBUFFERPOINTERVPROC glGetBufferPointerv;
	PFNGLBLENDEQUATIONSEPARATEPROC glBlendEquationSeparate;
	PFNGLDRAWBUFFERSPROC glDrawBuffers;
	PFNGLSTENCILOPSEPARATEPROC glStencilOpSeparate;
	PFNGLSTENCILFUNCSEPARATEPROC glStencilFuncSeparate;
	PFNGLSTENCILMASKSEPARATEPROC glStencilMaskSeparate;
	PFNGLATTACHSHADERPROC glAttachShader;
	PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;
	PFNGLCOMPILESHADERPROC glCompileShader;
	PFNGLCREATEPROGRAMPROC glCreateProgram;
	PFNGLCREATESHADERPROC glCreateShader;
	PFNGLDELETEPROGRAMPROC glDeleteProgram;
	PFNGLDELETESHADERPROC glDeleteShader;
	PFNGLDETACHSHADERPROC glDetachShader;
	PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
	PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
	PFNGLGETACTIVEATTRIBPROC glGetActiveAttrib;
	PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform;
	PFNGLGETATTACHEDSHADERSPROC glGetAttachedShaders;
	PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
	PFNGLGETPROGRAMIVPROC glGetProgramiv;
	PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
	PFNGLGETSHADERIVPROC glGetShaderiv;
	PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
	PFNGLGETSHADERSOURCEPROC glGetShaderSource;
	PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
	PFNGLGETUNIFORMFVPROC glGetUniformfv;
	PFNGLGETUNIFORMIVPROC glGetUniformiv;
	PFNGLGETVERTEXATTRIBDVPROC glGetVertexAttribdv;
	PFNGLGETVERTEXATTRIBFVPROC glGetVertexAttribfv;
	PFNGLGETVERTEXATTRIBIVPROC glGetVertexAttribiv;
	PFNGLGETVERTEXATTRIBPOINTERVPROC glGetVertexAttribPointerv;
	PFNGLISPROGRAMPROC glIsProgram;
	PFNGLISSHADERPROC glIsShader;
	PFNGLLINKPROGRAMPROC glLinkProgram;
	PFNGLSHADERSOURCEPROC glShaderSource;
	PFNGLUSEPROGRAMPROC glUseProgram;
	PFNGLUNIFORM1FPROC glUniform1f;
	PFNGLUNIFORM2FPROC glUniform2f;
	PFNGLUNIFORM3FPROC glUniform3f;
	PFNGLUNIFORM4FPROC glUniform4f;
	PFNGLUNIFORM1IPROC glUniform1i;
	PFNGLUNIFORM2IPROC glUniform2i;
	PFNGLUNIFORM3IPROC glUniform3i;
	PFNGLUNIFORM4IPROC glUniform4i;
	PFNGLUNIFORM1FVPROC glUniform1fv;
	PFNGLUNIFORM2FVPROC glUniform2fv;
	PFNGLUNIFORM3FVPROC glUniform3fv;
	PFNGLUNIFORM4FVPROC glUniform4fv;
	PFNGLUNIFORM1IVPROC glUniform1iv;
	PFNGLUNIFORM2IVPROC glUniform2iv;
	PFNGLUNIFORM3IVPROC glUniform3iv;
	PFNGLUNIFORM4IVPROC glUniform4iv;
	PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv;
	PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv;
	PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
	PFNGLVALIDATEPROGRAMPROC glValidateProgram;
	PFNGLVERTEXATTRIB1DPROC glVertexAttrib1d;
	PFNGLVERTEXATTRIB1DVPROC glVertexAttrib1dv;
	PFNGLVERTEXATTRIB1FPROC glVertexAttrib1f;
	PFNGLVERTEXATTRIB1FVPROC glVertexAttrib1fv;
	PFNGLVERTEXATTRIB1SPROC glVertexAttrib1s;
	PFNGLVERTEXATTRIB1SVPROC glVertexAttrib1sv;
	PFNGLVERTEXATTRIB2DPROC glVertexAttrib2d;
	PFNGLVERTEXATTRIB2DVPROC glVertexAttrib2dv;
	PFNGLVERTEXATTRIB2FPROC glVertexAttrib2f;
	PFNGLVERTEXATTRIB2FVPROC glVertexAttrib2fv;
	PFNGLVERTEXATTRIB2SPROC glVertexAttrib2s;
	PFNGLVERTEXATTRIB2SVPROC glVertexAttrib2sv;
	PFNGLVERTEXATTRIB3DPROC glVertexAttrib3d;
	PFNGLVERTEXATTRIB3DVPROC glVertexAttrib3dv;
	PFNGLVERTEXATTRIB3FPROC glVertexAttrib3f;
	PFNGLVERTEXATTRIB3FVPROC glVertexAttrib3fv;
	PFNGLVERTEXATTRIB3SPROC glVertexAttrib3s;
	PFNGLVERTEXATTRIB3SVPROC glVertexAttrib3sv;
	PFNGLVERTEXATTRIB4NBVPROC glVertexAttrib4Nbv;
	PFNGLVERTEXATTRIB4NIVPROC glVertexAttrib4Niv;
	PFNGLVERTEXATTRIB4NSVPROC glVertexAttrib4Nsv;
	PFNGLVERTEXATTRIB4NUBPROC glVertexAttrib4Nub;
	PFNGLVERTEXATTRIB4NUBVPROC glVertexAttrib4Nubv;
	PFNGLVERTEXATTRIB4NUIVPROC glVertexAttrib4Nuiv;
	PFNGLVERTEXATTRIB4NUSVPROC glVertexAttrib4Nusv;
	PFNGLVERTEXATTRIB4BVPROC glVertexAttrib4bv;
	PFNGLVERTEXATTRIB4DPROC glVertexAttrib4d;
	PFNGLVERTEXATTRIB4DVPROC glVertexAttrib4dv;
	PFNGLVERTEXATTRIB4FPROC glVertexAttrib4f;
	PFNGLVERTEXATTRIB4FVPROC glVertexAttrib4fv;
	PFNGLVERTEXATTRIB4IVPROC glVertexAttrib4iv;
	PFNGLVERTEXATTRIB4SPROC glVertexAttrib4s;
	PFNGLVERTEXATTRIB4SVPROC glVertexAttrib4sv;
	PFNGLVERTEXATTRIB4UBVPROC glVertexAttrib4ubv;
	PFNGLVERTEXATTRIB4UIVPROC glVertexAttrib4uiv;
	PFNGLVERTEXATTRIB4USVPROC glVertexAttrib4usv;
	PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
	PFNGLUNIFORMMATRIX2X3FVPROC glUniformMatrix2x3fv;
	PFNGLUNIFORMMATRIX3X2FVPROC glUniformMatrix3x2fv;
	PFNGLUNIFORMMATRIX2X4FVPROC glUniformMatrix2x4fv;
	PFNGLUNIFORMMATRIX4X2FVPROC glUniformMatrix4x2fv;
	PFNGLUNIFORMMATRIX3X4FVPROC glUniformMatrix3x4fv;
	PFNGLUNIFORMMATRIX4X3FVPROC glUniformMatrix4x3fv;
	PFNGLCOLORMASKIPROC glColorMaski;
	PFNGLGETboolEANI_VPROC glGetbooleani_v;
	PFNGLGETINTEGERI_VPROC glGetIntegeri_v;
	PFNGLENABLEIPROC glEnablei;
	PFNGLDISABLEIPROC glDisablei;
	PFNGLISENABLEDIPROC glIsEnabledi;
	PFNGLBEGINTRANSFORMFEEDBACKPROC glBeginTransformFeedback;
	PFNGLENDTRANSFORMFEEDBACKPROC glEndTransformFeedback;
	PFNGLBINDBUFFERRANGEPROC glBindBufferRange;
	PFNGLBINDBUFFERBASEPROC glBindBufferBase;
	PFNGLTRANSFORMFEEDBACKVARYINGSPROC glTransformFeedbackVaryings;
	PFNGLGETTRANSFORMFEEDBACKVARYINGPROC glGetTransformFeedbackVarying;
	PFNGLCLAMPCOLORPROC glClampColor;
	PFNGLBEGINCONDITIONALRENDERPROC glBeginConditionalRender;
	PFNGLENDCONDITIONALRENDERPROC glEndConditionalRender;
	PFNGLVERTEXATTRIBIPOINTERPROC glVertexAttribIPointer;
	PFNGLGETVERTEXATTRIBIIVPROC glGetVertexAttribIiv;
	PFNGLGETVERTEXATTRIBIUIVPROC glGetVertexAttribIuiv;
	PFNGLVERTEXATTRIBI1IPROC glVertexAttribI1i;
	PFNGLVERTEXATTRIBI2IPROC glVertexAttribI2i;
	PFNGLVERTEXATTRIBI3IPROC glVertexAttribI3i;
	PFNGLVERTEXATTRIBI4IPROC glVertexAttribI4i;
	PFNGLVERTEXATTRIBI1UIPROC glVertexAttribI1ui;
	PFNGLVERTEXATTRIBI2UIPROC glVertexAttribI2ui;
	PFNGLVERTEXATTRIBI3UIPROC glVertexAttribI3ui;
	PFNGLVERTEXATTRIBI4UIPROC glVertexAttribI4ui;
	PFNGLVERTEXATTRIBI1IVPROC glVertexAttribI1iv;
	PFNGLVERTEXATTRIBI2IVPROC glVertexAttribI2iv;
	PFNGLVERTEXATTRIBI3IVPROC glVertexAttribI3iv;
	PFNGLVERTEXATTRIBI4IVPROC glVertexAttribI4iv;
	PFNGLVERTEXATTRIBI1UIVPROC glVertexAttribI1uiv;
	PFNGLVERTEXATTRIBI2UIVPROC glVertexAttribI2uiv;
	PFNGLVERTEXATTRIBI3UIVPROC glVertexAttribI3uiv;
	PFNGLVERTEXATTRIBI4UIVPROC glVertexAttribI4uiv;
	PFNGLVERTEXATTRIBI4BVPROC glVertexAttribI4bv;
	PFNGLVERTEXATTRIBI4SVPROC glVertexAttribI4sv;
	PFNGLVERTEXATTRIBI4UBVPROC glVertexAttribI4ubv;
	PFNGLVERTEXATTRIBI4USVPROC glVertexAttribI4usv;
	PFNGLGETUNIFORMUIVPROC glGetUniformuiv;
	PFNGLBINDFRAGDATALOCATIONPROC glBindFragDataLocation;
	PFNGLGETFRAGDATALOCATIONPROC glGetFragDataLocation;
	PFNGLUNIFORM1UIPROC glUniform1ui;
	PFNGLUNIFORM2UIPROC glUniform2ui;
	PFNGLUNIFORM3UIPROC glUniform3ui;
	PFNGLUNIFORM4UIPROC glUniform4ui;
	PFNGLUNIFORM1UIVPROC glUniform1uiv;
	PFNGLUNIFORM2UIVPROC glUniform2uiv;
	PFNGLUNIFORM3UIVPROC glUniform3uiv;
	PFNGLUNIFORM4UIVPROC glUniform4uiv;
	PFNGLTEXPARAMETERIIVPROC glTexParameterIiv;
	PFNGLTEXPARAMETERIUIVPROC glTexParameterIuiv;
	PFNGLGETTEXPARAMETERIIVPROC glGetTexParameterIiv;
	PFNGLGETTEXPARAMETERIUIVPROC glGetTexParameterIuiv;
	PFNGLCLEARBUFFERIVPROC glClearBufferiv;
	PFNGLCLEARBUFFERUIVPROC glClearBufferuiv;
	PFNGLCLEARBUFFERFVPROC glClearBufferfv;
	PFNGLCLEARBUFFERFIPROC glClearBufferfi;
	PFNGLGETSTRINGIPROC glGetStringi;
	PFNGLDRAWARRAYSINSTANCEDPROC glDrawArraysInstanced;
	PFNGLDRAWELEMENTSINSTANCEDPROC glDrawElementsInstanced;
	PFNGLTEXBUFFERPROC glTexBuffer;
	PFNGLPRIMITIVERESTARTINDEXPROC glPrimitiveRestartIndex;
	PFNGLGETINTEGER64I_VPROC glGetInteger64i_v;
	PFNGLGETBUFFERPARAMETERI64VPROC glGetBufferParameteri64v;
	PFNGLFRAMEBUFFERTEXTUREPROC glFramebufferTexture;
	PFNGLVERTEXATTRIBDIVISORPROC glVertexAttribDivisor;
	PFNGLMINSAMPLESHADINGPROC glMinSampleShading;
	PFNGLBLENDEQUATIONIPROC glBlendEquationi;
	PFNGLBLENDEQUATIONSEPARATEIPROC glBlendEquationSeparatei;
	PFNGLBLENDFUNCIPROC glBlendFunci;
	PFNGLBLENDFUNCSEPARATEIPROC glBlendFuncSeparatei;
	PFNGLISRENDERBUFFERPROC glIsRenderbuffer;
	PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer;
	PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers;
	PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers;
	PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage;
	PFNGLGETRENDERBUFFERPARAMETERIVPROC glGetRenderbufferParameteriv;
	PFNGLISFRAMEBUFFERPROC glIsFramebuffer;
	PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
	PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
	PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
	PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;
	PFNGLFRAMEBUFFERTEXTURE1DPROC glFramebufferTexture1D;
	PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
	PFNGLFRAMEBUFFERTEXTURE3DPROC glFramebufferTexture3D;
	PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer;
	PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC glGetFramebufferAttachmentParameteriv;
	PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
	PFNGLBLITFRAMEBUFFERPROC glBlitFramebuffer;
	PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC glRenderbufferStorageMultisample;
	PFNGLFRAMEBUFFERTEXTURELAYERPROC glFramebufferTextureLayer;
	PFNGLMAPBUFFERRANGEPROC glMapBufferRange;
	PFNGLFLUSHMAPPEDBUFFERRANGEPROC glFlushMappedBufferRange;
	PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
	PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
	PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
	PFNGLISVERTEXARRAYPROC glIsVertexArray;
	PFNGLGETUNIFORMINDICESPROC glGetUniformIndices;
	PFNGLGETACTIVEUNIFORMSIVPROC glGetActiveUniformsiv;
	PFNGLGETACTIVEUNIFORMNAMEPROC glGetActiveUniformName;
	PFNGLGETUNIFORMBLOCKINDEXPROC glGetUniformBlockIndex;
	PFNGLGETACTIVEUNIFORMBLOCKIVPROC glGetActiveUniformBlockiv;
	PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC glGetActiveUniformBlockName;
	PFNGLUNIFORMBLOCKBINDINGPROC glUniformBlockBinding;
	PFNGLCOPYBUFFERSUBDATAPROC glCopyBufferSubData;
	PFNGLDRAWELEMENTSBASEVERTEXPROC glDrawElementsBaseVertex;
	PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC glDrawRangeElementsBaseVertex;
	PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC glDrawElementsInstancedBaseVertex;
	PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC glMultiDrawElementsBaseVertex;
	PFNGLPROVOKINGVERTEXPROC glProvokingVertex;
	PFNGLFENCESYNCPROC glFenceSync;
	PFNGLISSYNCPROC glIsSync;
	PFNGLDELETESYNCPROC glDeleteSync;
	PFNGLCLIENTWAITSYNCPROC glClientWaitSync;
	PFNGLWAITSYNCPROC glWaitSync;
	PFNGLGETINTEGER64VPROC glGetInteger64v;
	PFNGLGETSYNCIVPROC glGetSynciv;
	PFNGLTEXIMAGE2DMULTISAMPLEPROC glTexImage2DMultisample;
	PFNGLTEXIMAGE3DMULTISAMPLEPROC glTexImage3DMultisample;
	PFNGLGETMULTISAMPLEFVPROC glGetMultisamplefv;
	PFNGLSAMPLEMASKIPROC glSampleMaski;
	PFNGLBLENDEQUATIONIARBPROC glBlendEquationiARB;
	PFNGLBLENDEQUATIONSEPARATEIARBPROC glBlendEquationSeparateiARB;
	PFNGLBLENDFUNCIARBPROC glBlendFunciARB;
	PFNGLBLENDFUNCSEPARATEIARBPROC glBlendFuncSeparateiARB;
	PFNGLMINSAMPLESHADINGARBPROC glMinSampleShadingARB;
	PFNGLNAMEDSTRINGARBPROC glNamedStringARB;
	PFNGLDELETENAMEDSTRINGARBPROC glDeleteNamedStringARB;
	PFNGLCOMPILESHADERINCLUDEARBPROC glCompileShaderIncludeARB;
	PFNGLISNAMEDSTRINGARBPROC glIsNamedStringARB;
	PFNGLGETNAMEDSTRINGARBPROC glGetNamedStringARB;
	PFNGLGETNAMEDSTRINGIVARBPROC glGetNamedStringivARB;
	PFNGLBINDFRAGDATALOCATIONINDEXEDPROC glBindFragDataLocationIndexed;
	PFNGLGETFRAGDATAINDEXPROC glGetFragDataIndex;
	PFNGLGENSAMPLERSPROC glGenSamplers;
	PFNGLDELETESAMPLERSPROC glDeleteSamplers;
	PFNGLISSAMPLERPROC glIsSampler;
	PFNGLBINDSAMPLERPROC glBindSampler;
	PFNGLSAMPLERPARAMETERIPROC glSamplerParameteri;
	PFNGLSAMPLERPARAMETERIVPROC glSamplerParameteriv;
	PFNGLSAMPLERPARAMETERFPROC glSamplerParameterf;
	PFNGLSAMPLERPARAMETERFVPROC glSamplerParameterfv;
	PFNGLSAMPLERPARAMETERIIVPROC glSamplerParameterIiv;
	PFNGLSAMPLERPARAMETERIUIVPROC glSamplerParameterIuiv;
	PFNGLGETSAMPLERPARAMETERIVPROC glGetSamplerParameteriv;
	PFNGLGETSAMPLERPARAMETERIIVPROC glGetSamplerParameterIiv;
	PFNGLGETSAMPLERPARAMETERFVPROC glGetSamplerParameterfv;
	PFNGLGETSAMPLERPARAMETERIUIVPROC glGetSamplerParameterIuiv;
	PFNGLQUERYCOUNTERPROC glQueryCounter;
	PFNGLGETQUERYOBJECTI64VPROC glGetQueryObjecti64v;
	PFNGLGETQUERYOBJECTUI64VPROC glGetQueryObjectui64v;
	PFNGLVERTEXP2UIPROC glVertexP2ui;
	PFNGLVERTEXP2UIVPROC glVertexP2uiv;
	PFNGLVERTEXP3UIPROC glVertexP3ui;
	PFNGLVERTEXP3UIVPROC glVertexP3uiv;
	PFNGLVERTEXP4UIPROC glVertexP4ui;
	PFNGLVERTEXP4UIVPROC glVertexP4uiv;
	PFNGLTEXCOORDP1UIPROC glTexCoordP1ui;
	PFNGLTEXCOORDP1UIVPROC glTexCoordP1uiv;
	PFNGLTEXCOORDP2UIPROC glTexCoordP2ui;
	PFNGLTEXCOORDP2UIVPROC glTexCoordP2uiv;
	PFNGLTEXCOORDP3UIPROC glTexCoordP3ui;
	PFNGLTEXCOORDP3UIVPROC glTexCoordP3uiv;
	PFNGLTEXCOORDP4UIPROC glTexCoordP4ui;
	PFNGLTEXCOORDP4UIVPROC glTexCoordP4uiv;
	PFNGLMULTITEXCOORDP1UIPROC glMultiTexCoordP1ui;
	PFNGLMULTITEXCOORDP1UIVPROC glMultiTexCoordP1uiv;
	PFNGLMULTITEXCOORDP2UIPROC glMultiTexCoordP2ui;
	PFNGLMULTITEXCOORDP2UIVPROC glMultiTexCoordP2uiv;
	PFNGLMULTITEXCOORDP3UIPROC glMultiTexCoordP3ui;
	PFNGLMULTITEXCOORDP3UIVPROC glMultiTexCoordP3uiv;
	PFNGLMULTITEXCOORDP4UIPROC glMultiTexCoordP4ui;
	PFNGLMULTITEXCOORDP4UIVPROC glMultiTexCoordP4uiv;
	PFNGLNORMALP3UIPROC glNormalP3ui;
	PFNGLNORMALP3UIVPROC glNormalP3uiv;
	PFNGLCOLORP3UIPROC glColorP3ui;
	PFNGLCOLORP3UIVPROC glColorP3uiv;
	PFNGLCOLORP4UIPROC glColorP4ui;
	PFNGLCOLORP4UIVPROC glColorP4uiv;
	PFNGLSECONDARYCOLORP3UIPROC glSecondaryColorP3ui;
	PFNGLSECONDARYCOLORP3UIVPROC glSecondaryColorP3uiv;
	PFNGLVERTEXATTRIBP1UIPROC glVertexAttribP1ui;
	PFNGLVERTEXATTRIBP1UIVPROC glVertexAttribP1uiv;
	PFNGLVERTEXATTRIBP2UIPROC glVertexAttribP2ui;
	PFNGLVERTEXATTRIBP2UIVPROC glVertexAttribP2uiv;
	PFNGLVERTEXATTRIBP3UIPROC glVertexAttribP3ui;
	PFNGLVERTEXATTRIBP3UIVPROC glVertexAttribP3uiv;
	PFNGLVERTEXATTRIBP4UIPROC glVertexAttribP4ui;
	PFNGLVERTEXATTRIBP4UIVPROC glVertexAttribP4uiv;
	PFNGLDRAWARRAYSINDIRECTPROC glDrawArraysIndirect;
	PFNGLDRAWELEMENTSINDIRECTPROC glDrawElementsIndirect;
	PFNGLUNIFORM1DPROC glUniform1d;
	PFNGLUNIFORM2DPROC glUniform2d;
	PFNGLUNIFORM3DPROC glUniform3d;
	PFNGLUNIFORM4DPROC glUniform4d;
	PFNGLUNIFORM1DVPROC glUniform1dv;
	PFNGLUNIFORM2DVPROC glUniform2dv;
	PFNGLUNIFORM3DVPROC glUniform3dv;
	PFNGLUNIFORM4DVPROC glUniform4dv;
	PFNGLUNIFORMMATRIX2DVPROC glUniformMatrix2dv;
	PFNGLUNIFORMMATRIX3DVPROC glUniformMatrix3dv;
	PFNGLUNIFORMMATRIX4DVPROC glUniformMatrix4dv;
	PFNGLUNIFORMMATRIX2X3DVPROC glUniformMatrix2x3dv;
	PFNGLUNIFORMMATRIX2X4DVPROC glUniformMatrix2x4dv;
	PFNGLUNIFORMMATRIX3X2DVPROC glUniformMatrix3x2dv;
	PFNGLUNIFORMMATRIX3X4DVPROC glUniformMatrix3x4dv;
	PFNGLUNIFORMMATRIX4X2DVPROC glUniformMatrix4x2dv;
	PFNGLUNIFORMMATRIX4X3DVPROC glUniformMatrix4x3dv;
	PFNGLGETUNIFORMDVPROC glGetUniformdv;
	PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC glGetSubroutineUniformLocation;
	PFNGLGETSUBROUTINEINDEXPROC glGetSubroutineIndex;
	PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC glGetActiveSubroutineUniformiv;
	PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC glGetActiveSubroutineUniformName;
	PFNGLGETACTIVESUBROUTINENAMEPROC glGetActiveSubroutineName;
	PFNGLUNIFORMSUBROUTINESUIVPROC glUniformSubroutinesuiv;
	PFNGLGETUNIFORMSUBROUTINEUIVPROC glGetUniformSubroutineuiv;
	PFNGLGETPROGRAMSTAGEIVPROC glGetProgramStageiv;
	PFNGLPATCHPARAMETERIPROC glPatchParameteri;
	PFNGLPATCHPARAMETERFVPROC glPatchParameterfv;
	PFNGLBINDTRANSFORMFEEDBACKPROC glBindTransformFeedback;
	PFNGLDELETETRANSFORMFEEDBACKSPROC glDeleteTransformFeedbacks;
	PFNGLGENTRANSFORMFEEDBACKSPROC glGenTransformFeedbacks;
	PFNGLISTRANSFORMFEEDBACKPROC glIsTransformFeedback;
	PFNGLPAUSETRANSFORMFEEDBACKPROC glPauseTransformFeedback;
	PFNGLRESUMETRANSFORMFEEDBACKPROC glResumeTransformFeedback;
	PFNGLDRAWTRANSFORMFEEDBACKPROC glDrawTransformFeedback;
	PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC glDrawTransformFeedbackStream;
	PFNGLBEGINQUERYINDEXEDPROC glBeginQueryIndexed;
	PFNGLENDQUERYINDEXEDPROC glEndQueryIndexed;
	PFNGLGETQUERYINDEXEDIVPROC glGetQueryIndexediv;
	PFNGLRELEASESHADERCOMPILERPROC glReleaseShaderCompiler;
	PFNGLSHADERBINARYPROC glShaderBinary;
	PFNGLGETSHADERPRECISIONFORMATPROC glGetShaderPrecisionFormat;
	PFNGLDEPTHRANGEFPROC glDepthRangef;
	PFNGLCLEARDEPTHFPROC glClearDepthf;
	PFNGLGETPROGRAMBINARYPROC glGetProgramBinary;
	PFNGLPROGRAMBINARYPROC glProgramBinary;
	PFNGLPROGRAMPARAMETERIPROC glProgramParameteri;
	PFNGLUSEPROGRAMSTAGESPROC glUseProgramStages;
	PFNGLACTIVESHADERPROGRAMPROC glActiveShaderProgram;
	PFNGLCREATESHADERPROGRAMVPROC glCreateShaderProgramv;
	PFNGLBINDPROGRAMPIPELINEPROC glBindProgramPipeline;
	PFNGLDELETEPROGRAMPIPELINESPROC glDeleteProgramPipelines;
	PFNGLGENPROGRAMPIPELINESPROC glGenProgramPipelines;
	PFNGLISPROGRAMPIPELINEPROC glIsProgramPipeline;
	PFNGLGETPROGRAMPIPELINEIVPROC glGetProgramPipelineiv;
	PFNGLPROGRAMUNIFORM1IPROC glProgramUniform1i;
	PFNGLPROGRAMUNIFORM1IVPROC glProgramUniform1iv;
	PFNGLPROGRAMUNIFORM1FPROC glProgramUniform1f;
	PFNGLPROGRAMUNIFORM1FVPROC glProgramUniform1fv;
	PFNGLPROGRAMUNIFORM1DPROC glProgramUniform1d;
	PFNGLPROGRAMUNIFORM1DVPROC glProgramUniform1dv;
	PFNGLPROGRAMUNIFORM1UIPROC glProgramUniform1ui;
	PFNGLPROGRAMUNIFORM1UIVPROC glProgramUniform1uiv;
	PFNGLPROGRAMUNIFORM2IPROC glProgramUniform2i;
	PFNGLPROGRAMUNIFORM2IVPROC glProgramUniform2iv;
	PFNGLPROGRAMUNIFORM2FPROC glProgramUniform2f;
	PFNGLPROGRAMUNIFORM2FVPROC glProgramUniform2fv;
	PFNGLPROGRAMUNIFORM2DPROC glProgramUniform2d;
	PFNGLPROGRAMUNIFORM2DVPROC glProgramUniform2dv;
	PFNGLPROGRAMUNIFORM2UIPROC glProgramUniform2ui;
	PFNGLPROGRAMUNIFORM2UIVPROC glProgramUniform2uiv;
	PFNGLPROGRAMUNIFORM3IPROC glProgramUniform3i;
	PFNGLPROGRAMUNIFORM3IVPROC glProgramUniform3iv;
	PFNGLPROGRAMUNIFORM3FPROC glProgramUniform3f;
	PFNGLPROGRAMUNIFORM3FVPROC glProgramUniform3fv;
	PFNGLPROGRAMUNIFORM3DPROC glProgramUniform3d;
	PFNGLPROGRAMUNIFORM3DVPROC glProgramUniform3dv;
	PFNGLPROGRAMUNIFORM3UIPROC glProgramUniform3ui;
	PFNGLPROGRAMUNIFORM3UIVPROC glProgramUniform3uiv;
	PFNGLPROGRAMUNIFORM4IPROC glProgramUniform4i;
	PFNGLPROGRAMUNIFORM4IVPROC glProgramUniform4iv;
	PFNGLPROGRAMUNIFORM4FPROC glProgramUniform4f;
	PFNGLPROGRAMUNIFORM4FVPROC glProgramUniform4fv;
	PFNGLPROGRAMUNIFORM4DPROC glProgramUniform4d;
	PFNGLPROGRAMUNIFORM4DVPROC glProgramUniform4dv;
	PFNGLPROGRAMUNIFORM4UIPROC glProgramUniform4ui;
	PFNGLPROGRAMUNIFORM4UIVPROC glProgramUniform4uiv;
	PFNGLPROGRAMUNIFORMMATRIX2FVPROC glProgramUniformMatrix2fv;
	PFNGLPROGRAMUNIFORMMATRIX3FVPROC glProgramUniformMatrix3fv;
	PFNGLPROGRAMUNIFORMMATRIX4FVPROC glProgramUniformMatrix4fv;
	PFNGLPROGRAMUNIFORMMATRIX2DVPROC glProgramUniformMatrix2dv;
	PFNGLPROGRAMUNIFORMMATRIX3DVPROC glProgramUniformMatrix3dv;
	PFNGLPROGRAMUNIFORMMATRIX4DVPROC glProgramUniformMatrix4dv;
	PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC glProgramUniformMatrix2x3fv;
	PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC glProgramUniformMatrix3x2fv;
	PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC glProgramUniformMatrix2x4fv;
	PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC glProgramUniformMatrix4x2fv;
	PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC glProgramUniformMatrix3x4fv;
	PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC glProgramUniformMatrix4x3fv;
	PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC glProgramUniformMatrix2x3dv;
	PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC glProgramUniformMatrix3x2dv;
	PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC glProgramUniformMatrix2x4dv;
	PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC glProgramUniformMatrix4x2dv;
	PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC glProgramUniformMatrix3x4dv;
	PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC glProgramUniformMatrix4x3dv;
	PFNGLVALIDATEPROGRAMPIPELINEPROC glValidateProgramPipeline;
	PFNGLGETPROGRAMPIPELINEINFOLOGPROC glGetProgramPipelineInfoLog;
	PFNGLVERTEXATTRIBL1DPROC glVertexAttribL1d;
	PFNGLVERTEXATTRIBL2DPROC glVertexAttribL2d;
	PFNGLVERTEXATTRIBL3DPROC glVertexAttribL3d;
	PFNGLVERTEXATTRIBL4DPROC glVertexAttribL4d;
	PFNGLVERTEXATTRIBL1DVPROC glVertexAttribL1dv;
	PFNGLVERTEXATTRIBL2DVPROC glVertexAttribL2dv;
	PFNGLVERTEXATTRIBL3DVPROC glVertexAttribL3dv;
	PFNGLVERTEXATTRIBL4DVPROC glVertexAttribL4dv;
	PFNGLVERTEXATTRIBLPOINTERPROC glVertexAttribLPointer;
	PFNGLGETVERTEXATTRIBLDVPROC glGetVertexAttribLdv;
	PFNGLVIEWPORTARRAYVPROC glViewportArrayv;
	PFNGLVIEWPORTINDEXEDFPROC glViewportIndexedf;
	PFNGLVIEWPORTINDEXEDFVPROC glViewportIndexedfv;
	PFNGLSCISSORARRAYVPROC glScissorArrayv;
	PFNGLSCISSORINDEXEDPROC glScissorIndexed;
	PFNGLSCISSORINDEXEDVPROC glScissorIndexedv;
	PFNGLDEPTHRANGEARRAYVPROC glDepthRangeArrayv;
	PFNGLDEPTHRANGEINDEXEDPROC glDepthRangeIndexed;
	PFNGLGETFLOATI_VPROC glGetFloati_v;
	PFNGLGETDOUBLEI_VPROC glGetDoublei_v;
	PFNGLCREATESYNCFROMCLEVENTARBPROC glCreateSyncFromCLeventARB;
	PFNGLDEBUGMESSAGECONTROLARBPROC glDebugMessageControlARB;
	PFNGLDEBUGMESSAGEINSERTARBPROC glDebugMessageInsertARB;
	PFNGLDEBUGMESSAGECALLBACKARBPROC glDebugMessageCallbackARB;
	PFNGLGETDEBUGMESSAGELOGARBPROC glGetDebugMessageLogARB;
	PFNGLGETGRAPHICSRESETSTATUSARBPROC glGetGraphicsResetStatusARB;
	PFNGLGETNMAPDVARBPROC glGetnMapdvARB;
	PFNGLGETNMAPFVARBPROC glGetnMapfvARB;
	PFNGLGETNMAPIVARBPROC glGetnMapivARB;
	PFNGLGETNPIXELMAPFVARBPROC glGetnPixelMapfvARB;
	PFNGLGETNPIXELMAPUIVARBPROC glGetnPixelMapuivARB;
	PFNGLGETNPIXELMAPUSVARBPROC glGetnPixelMapusvARB;
	PFNGLGETNPOLYGONSTIPPLEARBPROC glGetnPolygonStippleARB;
	PFNGLGETNCOLORTABLEARBPROC glGetnColorTableARB;
	PFNGLGETNCONVOLUTIONFILTERARBPROC glGetnConvolutionFilterARB;
	PFNGLGETNSEPARABLEFILTERARBPROC glGetnSeparableFilterARB;
	PFNGLGETNHISTOGRAMARBPROC glGetnHistogramARB;
	PFNGLGETNMINMAXARBPROC glGetnMinmaxARB;
	PFNGLGETNTEXIMAGEARBPROC glGetnTexImageARB;
	PFNGLREADNPIXELSARBPROC glReadnPixelsARB;
	PFNGLGETNCOMPRESSEDTEXIMAGEARBPROC glGetnCompressedTexImageARB;
	PFNGLGETNUNIFORMFVARBPROC glGetnUniformfvARB;
	PFNGLGETNUNIFORMIVARBPROC glGetnUniformivARB;
	PFNGLGETNUNIFORMUIVARBPROC glGetnUniformuivARB;
	PFNGLGETNUNIFORMDVARBPROC glGetnUniformdvARB;
}

#endif // KGE_PLATFORM != KGE_PLATFORM_ANDROID

int g_MajorVer = 0;
int g_MinorVer = 0;

//--------------------------------
//	function implementations
//--------------------------------

void* loadEntryPoint(const char* name)
{
    void* proc = NULL;
	#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
		proc = wglGetProcAddress( name );
	#elif KGE_PLATFORM == KGE_PLATFORM_LINUX
		proc = (void*)glXGetProcAddress( (const GLubyte*)name );
	#endif

	if ( proc != NULL )
	{
		kge::io::Logger::Info("\t\t%s\t\tsupported",name);
	}
	else
	{
		kge::io::Logger::Info("\t\t%s\t\tnot supported", name);
	}

	return proc;
}


// initGL
bool initGL(int version)
{
    #if KGE_PLATFORM != KGE_PLATFORM_ANDROID

	g_MajorVer = version / 10;
	g_MinorVer = version % 10;

	kge::io::Logger::Info("initializing OpenGL %d.%d :",g_MajorVer,g_MinorVer);

	// OpenGL 1.2
	if (version >= 12)
	{
		kge::io::Logger::Info("\tOpenGL 1.2 commands:");
		glTexSubImage3D = (PFNGLTEXSUBIMAGE3DPROC) loadEntryPoint("glTexSubImage3D");
		glCopyTexSubImage3D = (PFNGLCOPYTEXSUBIMAGE3DPROC) loadEntryPoint("glCopyTexSubImage3D");
	}

	// OpenGL 1.3
	if (version >= 13)
	{
		kge::io::Logger::Info("\tOpenGL 1.3 commands:");

		glActiveTexture = (PFNGLACTIVETEXTUREPROC) loadEntryPoint("glActiveTexture");
		glSampleCoverage = (PFNGLSAMPLECOVERAGEPROC) loadEntryPoint("glSampleCoverage");
		glCompressedTexImage3D = (PFNGLCOMPRESSEDTEXIMAGE3DPROC) loadEntryPoint("glCompressedTexImage3D");
		glCompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2DPROC) loadEntryPoint("glCompressedTexImage2D");
		glCompressedTexImage1D = (PFNGLCOMPRESSEDTEXIMAGE1DPROC) loadEntryPoint("glCompressedTexImage1D");
		glCompressedTexSubImage3D = (PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC) loadEntryPoint("glCompressedTexSubImage3D");
		glCompressedTexSubImage2D = (PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC) loadEntryPoint("glCompressedTexSubImage2D");
		glCompressedTexSubImage1D = (PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC) loadEntryPoint("glCompressedTexSubImage1D");
		glGetCompressedTexImage = (PFNGLGETCOMPRESSEDTEXIMAGEPROC) loadEntryPoint("glGetCompressedTexImage");
	}

	// OpenGL 1.4
	if (version >= 14)
	{
		kge::io::Logger::Info("\tOpenGL 1.4 commands:");

		glBlendFuncSeparate = (PFNGLBLENDFUNCSEPARATEPROC) loadEntryPoint("glBlendFuncSeparate");
		glMultiDrawArrays = (PFNGLMULTIDRAWARRAYSPROC) loadEntryPoint("glMultiDrawArrays");
		glMultiDrawElements = (PFNGLMULTIDRAWELEMENTSPROC) loadEntryPoint("glMultiDrawElements");
		glPointParameterf = (PFNGLPOINTPARAMETERFPROC) loadEntryPoint("glPointParameterf");
		glPointParameterfv = (PFNGLPOINTPARAMETERFVPROC) loadEntryPoint("glPointParameterfv");
		glPointParameteri = (PFNGLPOINTPARAMETERIPROC) loadEntryPoint("glPointParameteri");
		glPointParameteriv = (PFNGLPOINTPARAMETERIVPROC) loadEntryPoint("glPointParameteriv");
	}

	// OpenGL 1.5
	if (version >= 15)
	{
		kge::io::Logger::Info("\tOpenGL 1.5 commands:");

		glGenQueries = (PFNGLGENQUERIESPROC) loadEntryPoint("glGenQueries");
		glDeleteQueries = (PFNGLDELETEQUERIESPROC) loadEntryPoint("glDeleteQueries");
		glIsQuery = (PFNGLISQUERYPROC) loadEntryPoint("glIsQuery");
		glBeginQuery = (PFNGLBEGINQUERYPROC) loadEntryPoint("glBeginQuery");
		glEndQuery = (PFNGLENDQUERYPROC) loadEntryPoint("glEndQuery");
		glGetQueryiv = (PFNGLGETQUERYIVPROC) loadEntryPoint("glGetQueryiv");
		glGetQueryObjectiv = (PFNGLGETQUERYOBJECTIVPROC) loadEntryPoint("glGetQueryObjectiv");
		glGetQueryObjectuiv = (PFNGLGETQUERYOBJECTUIVPROC) loadEntryPoint("glGetQueryObjectuiv");
		glBindBuffer = (PFNGLBINDBUFFERPROC) loadEntryPoint("glBindBuffer");
		glDeleteBuffers = (PFNGLDELETEBUFFERSPROC) loadEntryPoint("glDeleteBuffers");
		glGenBuffers = (PFNGLGENBUFFERSPROC) loadEntryPoint("glGenBuffers");
		glIsBuffer = (PFNGLISBUFFERPROC) loadEntryPoint("glIsBuffer");
		glBufferData = (PFNGLBUFFERDATAPROC) loadEntryPoint("glBufferData");
		glBufferSubData = (PFNGLBUFFERSUBDATAPROC) loadEntryPoint("glBufferSubData");
		glGetBufferSubData = (PFNGLGETBUFFERSUBDATAPROC) loadEntryPoint("glGetBufferSubData");
		glMapBuffer = (PFNGLMAPBUFFERPROC) loadEntryPoint("glMapBuffer");
		glUnmapBuffer = (PFNGLUNMAPBUFFERPROC) loadEntryPoint("glUnmapBuffer");
		glGetBufferParameteriv = (PFNGLGETBUFFERPARAMETERIVPROC) loadEntryPoint("glGetBufferParameteriv");
		glGetBufferPointerv = (PFNGLGETBUFFERPOINTERVPROC) loadEntryPoint("glGetBufferPointerv");
	}

	// OpenGL 2.0
	if (version >= 20)
	{
		kge::io::Logger::Info("\tOpenGL 2.0 commands:");

		glBlendEquationSeparate = (PFNGLBLENDEQUATIONSEPARATEPROC) loadEntryPoint("glBlendEquationSeparate");
		glDrawBuffers = (PFNGLDRAWBUFFERSPROC) loadEntryPoint("glDrawBuffers");
		glStencilOpSeparate = (PFNGLSTENCILOPSEPARATEPROC) loadEntryPoint("glStencilOpSeparate");
		glStencilFuncSeparate = (PFNGLSTENCILFUNCSEPARATEPROC) loadEntryPoint("glStencilFuncSeparate");
		glStencilMaskSeparate = (PFNGLSTENCILMASKSEPARATEPROC) loadEntryPoint("glStencilMaskSeparate");
		glAttachShader = (PFNGLATTACHSHADERPROC) loadEntryPoint("glAttachShader");
		glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC) loadEntryPoint("glBindAttribLocation");
		glCompileShader = (PFNGLCOMPILESHADERPROC) loadEntryPoint("glCompileShader");
		glCreateProgram = (PFNGLCREATEPROGRAMPROC) loadEntryPoint("glCreateProgram");
		glCreateShader = (PFNGLCREATESHADERPROC) loadEntryPoint("glCreateShader");
		glDeleteProgram = (PFNGLDELETEPROGRAMPROC) loadEntryPoint("glDeleteProgram");
		glDeleteShader = (PFNGLDELETESHADERPROC) loadEntryPoint("glDeleteShader");
		glDetachShader = (PFNGLDETACHSHADERPROC) loadEntryPoint("glDetachShader");
		glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC) loadEntryPoint("glDisableVertexAttribArray");
		glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC) loadEntryPoint("glEnableVertexAttribArray");
		glGetActiveAttrib = (PFNGLGETACTIVEATTRIBPROC) loadEntryPoint("glGetActiveAttrib");
		glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC) loadEntryPoint("glGetActiveUniform");
		glGetAttachedShaders = (PFNGLGETATTACHEDSHADERSPROC) loadEntryPoint("glGetAttachedShaders");
		glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC) loadEntryPoint("glGetAttribLocation");
		glGetProgramiv = (PFNGLGETPROGRAMIVPROC) loadEntryPoint("glGetProgramiv");
		glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC) loadEntryPoint("glGetProgramInfoLog");
		glGetShaderiv = (PFNGLGETSHADERIVPROC) loadEntryPoint("glGetShaderiv");
		glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC) loadEntryPoint("glGetShaderInfoLog");
		glGetShaderSource = (PFNGLGETSHADERSOURCEPROC) loadEntryPoint("glGetShaderSource");
		glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC) loadEntryPoint("glGetUniformLocation");
		glGetUniformfv = (PFNGLGETUNIFORMFVPROC) loadEntryPoint("glGetUniformfv");
		glGetUniformiv = (PFNGLGETUNIFORMIVPROC) loadEntryPoint("glGetUniformiv");
		glGetVertexAttribdv = (PFNGLGETVERTEXATTRIBDVPROC) loadEntryPoint("glGetVertexAttribdv");
		glGetVertexAttribfv = (PFNGLGETVERTEXATTRIBFVPROC) loadEntryPoint("glGetVertexAttribfv");
		glGetVertexAttribiv = (PFNGLGETVERTEXATTRIBIVPROC) loadEntryPoint("glGetVertexAttribiv");
		glGetVertexAttribPointerv = (PFNGLGETVERTEXATTRIBPOINTERVPROC) loadEntryPoint("glGetVertexAttribPointerv");
		glIsProgram = (PFNGLISPROGRAMPROC) loadEntryPoint("glIsProgram");
		glIsShader = (PFNGLISSHADERPROC) loadEntryPoint("glIsShader");
		glLinkProgram = (PFNGLLINKPROGRAMPROC) loadEntryPoint("glLinkProgram");
		glShaderSource = (PFNGLSHADERSOURCEPROC) loadEntryPoint("glShaderSource");
		glUseProgram = (PFNGLUSEPROGRAMPROC) loadEntryPoint("glUseProgram");
		glUniform1f = (PFNGLUNIFORM1FPROC) loadEntryPoint("glUniform1f");
		glUniform2f = (PFNGLUNIFORM2FPROC) loadEntryPoint("glUniform2f");
		glUniform3f = (PFNGLUNIFORM3FPROC) loadEntryPoint("glUniform3f");
		glUniform4f = (PFNGLUNIFORM4FPROC) loadEntryPoint("glUniform4f");
		glUniform1i = (PFNGLUNIFORM1IPROC) loadEntryPoint("glUniform1i");
		glUniform2i = (PFNGLUNIFORM2IPROC) loadEntryPoint("glUniform2i");
		glUniform3i = (PFNGLUNIFORM3IPROC) loadEntryPoint("glUniform3i");
		glUniform4i = (PFNGLUNIFORM4IPROC) loadEntryPoint("glUniform4i");
		glUniform1fv = (PFNGLUNIFORM1FVPROC) loadEntryPoint("glUniform1fv");
		glUniform2fv = (PFNGLUNIFORM2FVPROC) loadEntryPoint("glUniform2fv");
		glUniform3fv = (PFNGLUNIFORM3FVPROC) loadEntryPoint("glUniform3fv");
		glUniform4fv = (PFNGLUNIFORM4FVPROC) loadEntryPoint("glUniform4fv");
		glUniform1iv = (PFNGLUNIFORM1IVPROC) loadEntryPoint("glUniform1iv");
		glUniform2iv = (PFNGLUNIFORM2IVPROC) loadEntryPoint("glUniform2iv");
		glUniform3iv = (PFNGLUNIFORM3IVPROC) loadEntryPoint("glUniform3iv");
		glUniform4iv = (PFNGLUNIFORM4IVPROC) loadEntryPoint("glUniform4iv");
		glUniformMatrix2fv = (PFNGLUNIFORMMATRIX2FVPROC) loadEntryPoint("glUniformMatrix2fv");
		glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC) loadEntryPoint("glUniformMatrix3fv");
		glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC) loadEntryPoint("glUniformMatrix4fv");
		glValidateProgram = (PFNGLVALIDATEPROGRAMPROC) loadEntryPoint("glValidateProgram");
		glVertexAttrib1d = (PFNGLVERTEXATTRIB1DPROC) loadEntryPoint("glVertexAttrib1d");
		glVertexAttrib1dv = (PFNGLVERTEXATTRIB1DVPROC) loadEntryPoint("glVertexAttrib1dv");
		glVertexAttrib1f = (PFNGLVERTEXATTRIB1FPROC) loadEntryPoint("glVertexAttrib1f");
		glVertexAttrib1fv = (PFNGLVERTEXATTRIB1FVPROC) loadEntryPoint("glVertexAttrib1fv");
		glVertexAttrib1s = (PFNGLVERTEXATTRIB1SPROC) loadEntryPoint("glVertexAttrib1s");
		glVertexAttrib1sv = (PFNGLVERTEXATTRIB1SVPROC) loadEntryPoint("glVertexAttrib1sv");
		glVertexAttrib2d = (PFNGLVERTEXATTRIB2DPROC) loadEntryPoint("glVertexAttrib2d");
		glVertexAttrib2dv = (PFNGLVERTEXATTRIB2DVPROC) loadEntryPoint("glVertexAttrib2dv");
		glVertexAttrib2f = (PFNGLVERTEXATTRIB2FPROC) loadEntryPoint("glVertexAttrib2f");
		glVertexAttrib2fv = (PFNGLVERTEXATTRIB2FVPROC) loadEntryPoint("glVertexAttrib2fv");
		glVertexAttrib2s = (PFNGLVERTEXATTRIB2SPROC) loadEntryPoint("glVertexAttrib2s");
		glVertexAttrib2sv = (PFNGLVERTEXATTRIB2SVPROC) loadEntryPoint("glVertexAttrib2sv");
		glVertexAttrib3d = (PFNGLVERTEXATTRIB3DPROC) loadEntryPoint("glVertexAttrib3d");
		glVertexAttrib3dv = (PFNGLVERTEXATTRIB3DVPROC) loadEntryPoint("glVertexAttrib3dv");
		glVertexAttrib3f = (PFNGLVERTEXATTRIB3FPROC) loadEntryPoint("glVertexAttrib3f");
		glVertexAttrib3fv = (PFNGLVERTEXATTRIB3FVPROC) loadEntryPoint("glVertexAttrib3fv");
		glVertexAttrib3s = (PFNGLVERTEXATTRIB3SPROC) loadEntryPoint("glVertexAttrib3s");
		glVertexAttrib3sv = (PFNGLVERTEXATTRIB3SVPROC) loadEntryPoint("glVertexAttrib3sv");
		glVertexAttrib4Nbv = (PFNGLVERTEXATTRIB4NBVPROC) loadEntryPoint("glVertexAttrib4Nbv");
		glVertexAttrib4Niv = (PFNGLVERTEXATTRIB4NIVPROC) loadEntryPoint("glVertexAttrib4Niv");
		glVertexAttrib4Nsv = (PFNGLVERTEXATTRIB4NSVPROC) loadEntryPoint("glVertexAttrib4Nsv");
		glVertexAttrib4Nub = (PFNGLVERTEXATTRIB4NUBPROC) loadEntryPoint("glVertexAttrib4Nub");
		glVertexAttrib4Nubv = (PFNGLVERTEXATTRIB4NUBVPROC) loadEntryPoint("glVertexAttrib4Nubv");
		glVertexAttrib4Nuiv = (PFNGLVERTEXATTRIB4NUIVPROC) loadEntryPoint("glVertexAttrib4Nuiv");
		glVertexAttrib4Nusv = (PFNGLVERTEXATTRIB4NUSVPROC) loadEntryPoint("glVertexAttrib4Nusv");
		glVertexAttrib4bv = (PFNGLVERTEXATTRIB4BVPROC) loadEntryPoint("glVertexAttrib4bv");
		glVertexAttrib4d = (PFNGLVERTEXATTRIB4DPROC) loadEntryPoint("glVertexAttrib4d");
		glVertexAttrib4dv = (PFNGLVERTEXATTRIB4DVPROC) loadEntryPoint("glVertexAttrib4dv");
		glVertexAttrib4f = (PFNGLVERTEXATTRIB4FPROC) loadEntryPoint("glVertexAttrib4f");
		glVertexAttrib4fv = (PFNGLVERTEXATTRIB4FVPROC) loadEntryPoint("glVertexAttrib4fv");
		glVertexAttrib4iv = (PFNGLVERTEXATTRIB4IVPROC) loadEntryPoint("glVertexAttrib4iv");
		glVertexAttrib4s = (PFNGLVERTEXATTRIB4SPROC) loadEntryPoint("glVertexAttrib4s");
		glVertexAttrib4sv = (PFNGLVERTEXATTRIB4SVPROC) loadEntryPoint("glVertexAttrib4sv");
		glVertexAttrib4ubv = (PFNGLVERTEXATTRIB4UBVPROC) loadEntryPoint("glVertexAttrib4ubv");
		glVertexAttrib4uiv = (PFNGLVERTEXATTRIB4UIVPROC) loadEntryPoint("glVertexAttrib4uiv");
		glVertexAttrib4usv = (PFNGLVERTEXATTRIB4USVPROC) loadEntryPoint("glVertexAttrib4usv");
		glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC) loadEntryPoint("glVertexAttribPointer");
	}

	// OpenGL 2.1
	if (version >= 21)
	{
		kge::io::Logger::Info("\tOpenGL 2.1 commands:");
		glUniformMatrix2x3fv = (PFNGLUNIFORMMATRIX2X3FVPROC) loadEntryPoint("glUniformMatrix2x3fv");
		glUniformMatrix3x2fv = (PFNGLUNIFORMMATRIX3X2FVPROC) loadEntryPoint("glUniformMatrix3x2fv");
		glUniformMatrix2x4fv = (PFNGLUNIFORMMATRIX2X4FVPROC) loadEntryPoint("glUniformMatrix2x4fv");
		glUniformMatrix4x2fv = (PFNGLUNIFORMMATRIX4X2FVPROC) loadEntryPoint("glUniformMatrix4x2fv");
		glUniformMatrix3x4fv = (PFNGLUNIFORMMATRIX3X4FVPROC) loadEntryPoint("glUniformMatrix3x4fv");
		glUniformMatrix4x3fv = (PFNGLUNIFORMMATRIX4X3FVPROC) loadEntryPoint("glUniformMatrix4x3fv");
	}

	// OpenGL 3.0
	if (version >= 30)
	{
		kge::io::Logger::Info("\tOpenGL 3.0 commands:");

		glColorMaski = (PFNGLCOLORMASKIPROC) loadEntryPoint("glColorMaski");
		glGetbooleani_v = (PFNGLGETboolEANI_VPROC) loadEntryPoint("glGetbooleani_v");
		glGetIntegeri_v = (PFNGLGETINTEGERI_VPROC) loadEntryPoint("glGetIntegeri_v");
		glEnablei = (PFNGLENABLEIPROC) loadEntryPoint("glEnablei");
		glDisablei = (PFNGLDISABLEIPROC) loadEntryPoint("glDisablei");
		glIsEnabledi = (PFNGLISENABLEDIPROC) loadEntryPoint("glIsEnabledi");
		glBeginTransformFeedback = (PFNGLBEGINTRANSFORMFEEDBACKPROC) loadEntryPoint("glBeginTransformFeedback");
		glEndTransformFeedback = (PFNGLENDTRANSFORMFEEDBACKPROC) loadEntryPoint("glEndTransformFeedback");
		glBindBufferRange = (PFNGLBINDBUFFERRANGEPROC) loadEntryPoint("glBindBufferRange");
		glBindBufferBase = (PFNGLBINDBUFFERBASEPROC) loadEntryPoint("glBindBufferBase");
		glTransformFeedbackVaryings = (PFNGLTRANSFORMFEEDBACKVARYINGSPROC) loadEntryPoint("glTransformFeedbackVaryings");
		glGetTransformFeedbackVarying = (PFNGLGETTRANSFORMFEEDBACKVARYINGPROC) loadEntryPoint("glGetTransformFeedbackVarying");
		glClampColor = (PFNGLCLAMPCOLORPROC) loadEntryPoint("glClampColor");
		glBeginConditionalRender = (PFNGLBEGINCONDITIONALRENDERPROC) loadEntryPoint("glBeginConditionalRender");
		glEndConditionalRender = (PFNGLENDCONDITIONALRENDERPROC) loadEntryPoint("glEndConditionalRender");
		glVertexAttribIPointer = (PFNGLVERTEXATTRIBIPOINTERPROC) loadEntryPoint("glVertexAttribIPointer");
		glGetVertexAttribIiv = (PFNGLGETVERTEXATTRIBIIVPROC) loadEntryPoint("glGetVertexAttribIiv");
		glGetVertexAttribIuiv = (PFNGLGETVERTEXATTRIBIUIVPROC) loadEntryPoint("glGetVertexAttribIuiv");
		glVertexAttribI1i = (PFNGLVERTEXATTRIBI1IPROC) loadEntryPoint("glVertexAttribI1i");
		glVertexAttribI2i = (PFNGLVERTEXATTRIBI2IPROC) loadEntryPoint("glVertexAttribI2i");
		glVertexAttribI3i = (PFNGLVERTEXATTRIBI3IPROC) loadEntryPoint("glVertexAttribI3i");
		glVertexAttribI4i = (PFNGLVERTEXATTRIBI4IPROC) loadEntryPoint("glVertexAttribI4i");
		glVertexAttribI1ui = (PFNGLVERTEXATTRIBI1UIPROC) loadEntryPoint("glVertexAttribI1ui");
		glVertexAttribI2ui = (PFNGLVERTEXATTRIBI2UIPROC) loadEntryPoint("glVertexAttribI2ui");
		glVertexAttribI3ui = (PFNGLVERTEXATTRIBI3UIPROC) loadEntryPoint("glVertexAttribI3ui");
		glVertexAttribI4ui = (PFNGLVERTEXATTRIBI4UIPROC) loadEntryPoint("glVertexAttribI4ui");
		glVertexAttribI1iv = (PFNGLVERTEXATTRIBI1IVPROC) loadEntryPoint("glVertexAttribI1iv");
		glVertexAttribI2iv = (PFNGLVERTEXATTRIBI2IVPROC) loadEntryPoint("glVertexAttribI2iv");
		glVertexAttribI3iv = (PFNGLVERTEXATTRIBI3IVPROC) loadEntryPoint("glVertexAttribI3iv");
		glVertexAttribI4iv = (PFNGLVERTEXATTRIBI4IVPROC) loadEntryPoint("glVertexAttribI4iv");
		glVertexAttribI1uiv = (PFNGLVERTEXATTRIBI1UIVPROC) loadEntryPoint("glVertexAttribI1uiv");
		glVertexAttribI2uiv = (PFNGLVERTEXATTRIBI2UIVPROC) loadEntryPoint("glVertexAttribI2uiv");
		glVertexAttribI3uiv = (PFNGLVERTEXATTRIBI3UIVPROC) loadEntryPoint("glVertexAttribI3uiv");
		glVertexAttribI4uiv = (PFNGLVERTEXATTRIBI4UIVPROC) loadEntryPoint("glVertexAttribI4uiv");
		glVertexAttribI4bv = (PFNGLVERTEXATTRIBI4BVPROC) loadEntryPoint("glVertexAttribI4bv");
		glVertexAttribI4sv = (PFNGLVERTEXATTRIBI4SVPROC) loadEntryPoint("glVertexAttribI4sv");
		glVertexAttribI4ubv = (PFNGLVERTEXATTRIBI4UBVPROC) loadEntryPoint("glVertexAttribI4ubv");
		glVertexAttribI4usv = (PFNGLVERTEXATTRIBI4USVPROC) loadEntryPoint("glVertexAttribI4usv");
		glGetUniformuiv = (PFNGLGETUNIFORMUIVPROC) loadEntryPoint("glGetUniformuiv");
		glBindFragDataLocation = (PFNGLBINDFRAGDATALOCATIONPROC) loadEntryPoint("glBindFragDataLocation");
		glGetFragDataLocation = (PFNGLGETFRAGDATALOCATIONPROC) loadEntryPoint("glGetFragDataLocation");
		glUniform1ui = (PFNGLUNIFORM1UIPROC) loadEntryPoint("glUniform1ui");
		glUniform2ui = (PFNGLUNIFORM2UIPROC) loadEntryPoint("glUniform2ui");
		glUniform3ui = (PFNGLUNIFORM3UIPROC) loadEntryPoint("glUniform3ui");
		glUniform4ui = (PFNGLUNIFORM4UIPROC) loadEntryPoint("glUniform4ui");
		glUniform1uiv = (PFNGLUNIFORM1UIVPROC) loadEntryPoint("glUniform1uiv");
		glUniform2uiv = (PFNGLUNIFORM2UIVPROC) loadEntryPoint("glUniform2uiv");
		glUniform3uiv = (PFNGLUNIFORM3UIVPROC) loadEntryPoint("glUniform3uiv");
		glUniform4uiv = (PFNGLUNIFORM4UIVPROC) loadEntryPoint("glUniform4uiv");
		glTexParameterIiv = (PFNGLTEXPARAMETERIIVPROC) loadEntryPoint("glTexParameterIiv");
		glTexParameterIuiv = (PFNGLTEXPARAMETERIUIVPROC) loadEntryPoint("glTexParameterIuiv");
		glGetTexParameterIiv = (PFNGLGETTEXPARAMETERIIVPROC) loadEntryPoint("glGetTexParameterIiv");
		glGetTexParameterIuiv = (PFNGLGETTEXPARAMETERIUIVPROC) loadEntryPoint("glGetTexParameterIuiv");
		glClearBufferiv = (PFNGLCLEARBUFFERIVPROC) loadEntryPoint("glClearBufferiv");
		glClearBufferuiv = (PFNGLCLEARBUFFERUIVPROC) loadEntryPoint("glClearBufferuiv");
		glClearBufferfv = (PFNGLCLEARBUFFERFVPROC) loadEntryPoint("glClearBufferfv");
		glClearBufferfi = (PFNGLCLEARBUFFERFIPROC) loadEntryPoint("glClearBufferfi");
		glGetStringi = (PFNGLGETSTRINGIPROC) loadEntryPoint("glGetStringi");
		glIsRenderbuffer = (PFNGLISRENDERBUFFERPROC) loadEntryPoint("glIsRenderbuffer");
		glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC) loadEntryPoint("glBindRenderbuffer");
		glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC) loadEntryPoint("glDeleteRenderbuffers");
		glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC) loadEntryPoint("glGenRenderbuffers");
		glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC) loadEntryPoint("glRenderbufferStorage");
		glGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVPROC) loadEntryPoint("glGetRenderbufferParameteriv");
		glIsFramebuffer = (PFNGLISFRAMEBUFFERPROC) loadEntryPoint("glIsFramebuffer");
		glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC) loadEntryPoint("glBindFramebuffer");
		glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC) loadEntryPoint("glDeleteFramebuffers");
		glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC) loadEntryPoint("glGenFramebuffers");
		glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC) loadEntryPoint("glCheckFramebufferStatus");
		glFramebufferTexture1D = (PFNGLFRAMEBUFFERTEXTURE1DPROC) loadEntryPoint("glFramebufferTexture1D");
		glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC) loadEntryPoint("glFramebufferTexture2D");
		glFramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3DPROC) loadEntryPoint("glFramebufferTexture3D");
		glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC) loadEntryPoint("glFramebufferRenderbuffer");
		glGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC) loadEntryPoint("glGetFramebufferAttachmentParameteriv");
		glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC) loadEntryPoint("glGenerateMipmap");
		glBlitFramebuffer = (PFNGLBLITFRAMEBUFFERPROC) loadEntryPoint("glBlitFramebuffer");
		glRenderbufferStorageMultisample = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC) loadEntryPoint("glRenderbufferStorageMultisample");
		glFramebufferTextureLayer = (PFNGLFRAMEBUFFERTEXTURELAYERPROC) loadEntryPoint("glFramebufferTextureLayer");
		glMapBufferRange = (PFNGLMAPBUFFERRANGEPROC) loadEntryPoint("glMapBufferRange");
		glFlushMappedBufferRange = (PFNGLFLUSHMAPPEDBUFFERRANGEPROC) loadEntryPoint("glFlushMappedBufferRange");
		glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC) loadEntryPoint("glBindVertexArray");
		glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC) loadEntryPoint("glDeleteVertexArrays");
		glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC) loadEntryPoint("glGenVertexArrays");
		glIsVertexArray = (PFNGLISVERTEXARRAYPROC) loadEntryPoint("glIsVertexArray");
	}

	// OpenGL 3.1
	if (version >= 31)
	{
		kge::io::Logger::Info("\tOpenGL 3.1 commands:");

		glDrawArraysInstanced = (PFNGLDRAWARRAYSINSTANCEDPROC) loadEntryPoint("glDrawArraysInstanced");
		glDrawElementsInstanced = (PFNGLDRAWELEMENTSINSTANCEDPROC) loadEntryPoint("glDrawElementsInstanced");
		glTexBuffer = (PFNGLTEXBUFFERPROC) loadEntryPoint("glTexBuffer");
		glPrimitiveRestartIndex = (PFNGLPRIMITIVERESTARTINDEXPROC) loadEntryPoint("glPrimitiveRestartIndex");
		glGetUniformIndices = (PFNGLGETUNIFORMINDICESPROC) loadEntryPoint("glGetUniformIndices");
		glGetActiveUniformsiv = (PFNGLGETACTIVEUNIFORMSIVPROC) loadEntryPoint("glGetActiveUniformsiv");
		glGetActiveUniformName = (PFNGLGETACTIVEUNIFORMNAMEPROC) loadEntryPoint("glGetActiveUniformName");
		glGetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEXPROC) loadEntryPoint("glGetUniformBlockIndex");
		glGetActiveUniformBlockiv = (PFNGLGETACTIVEUNIFORMBLOCKIVPROC) loadEntryPoint("glGetActiveUniformBlockiv");
		glGetActiveUniformBlockName = (PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC) loadEntryPoint("glGetActiveUniformBlockName");
		glUniformBlockBinding = (PFNGLUNIFORMBLOCKBINDINGPROC) loadEntryPoint("glUniformBlockBinding");
		glCopyBufferSubData = (PFNGLCOPYBUFFERSUBDATAPROC) loadEntryPoint("glCopyBufferSubData");
	}

	// OpenGL 3.2
	if (version >= 32)
	{
		kge::io::Logger::Info("\tOpenGL 3.2 commands:");

		glGetInteger64i_v = (PFNGLGETINTEGER64I_VPROC) loadEntryPoint("glGetInteger64i_v");
		glGetBufferParameteri64v = (PFNGLGETBUFFERPARAMETERI64VPROC) loadEntryPoint("glGetBufferParameteri64v");
		glFramebufferTexture = (PFNGLFRAMEBUFFERTEXTUREPROC) loadEntryPoint("glFramebufferTexture");
		glDrawElementsBaseVertex = (PFNGLDRAWELEMENTSBASEVERTEXPROC) loadEntryPoint("glDrawElementsBaseVertex");
		glDrawRangeElementsBaseVertex = (PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC) loadEntryPoint("glDrawRangeElementsBaseVertex");
		glDrawElementsInstancedBaseVertex = (PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC) loadEntryPoint("glDrawElementsInstancedBaseVertex");
		glMultiDrawElementsBaseVertex = (PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC) loadEntryPoint("glMultiDrawElementsBaseVertex");
		glProvokingVertex = (PFNGLPROVOKINGVERTEXPROC) loadEntryPoint("glProvokingVertex");
		glFenceSync = (PFNGLFENCESYNCPROC) loadEntryPoint("glFenceSync");
		glIsSync = (PFNGLISSYNCPROC) loadEntryPoint("glIsSync");
		glDeleteSync = (PFNGLDELETESYNCPROC) loadEntryPoint("glDeleteSync");
		glClientWaitSync = (PFNGLCLIENTWAITSYNCPROC) loadEntryPoint("glClientWaitSync");
		glWaitSync = (PFNGLWAITSYNCPROC) loadEntryPoint("glWaitSync");
		glGetInteger64v = (PFNGLGETINTEGER64VPROC) loadEntryPoint("glGetInteger64v");
		glGetSynciv = (PFNGLGETSYNCIVPROC) loadEntryPoint("glGetSynciv");
		glTexImage2DMultisample = (PFNGLTEXIMAGE2DMULTISAMPLEPROC) loadEntryPoint("glTexImage2DMultisample");
		glTexImage3DMultisample = (PFNGLTEXIMAGE3DMULTISAMPLEPROC) loadEntryPoint("glTexImage3DMultisample");
		glGetMultisamplefv = (PFNGLGETMULTISAMPLEFVPROC) loadEntryPoint("glGetMultisamplefv");
		glSampleMaski = (PFNGLSAMPLEMASKIPROC) loadEntryPoint("glSampleMaski");
	}

	// OpenGL 3.3
	if (version >= 33)
	{
		kge::io::Logger::Info("\tOpenGL 3.3 commands:");

		glVertexAttribDivisor = (PFNGLVERTEXATTRIBDIVISORPROC) loadEntryPoint("glVertexAttribDivisor");
		glBindFragDataLocationIndexed = (PFNGLBINDFRAGDATALOCATIONINDEXEDPROC) loadEntryPoint("glBindFragDataLocationIndexed");
		glGetFragDataIndex = (PFNGLGETFRAGDATAINDEXPROC) loadEntryPoint("glGetFragDataIndex");
		glGenSamplers = (PFNGLGENSAMPLERSPROC) loadEntryPoint("glGenSamplers");
		glDeleteSamplers = (PFNGLDELETESAMPLERSPROC) loadEntryPoint("glDeleteSamplers");
		glIsSampler = (PFNGLISSAMPLERPROC) loadEntryPoint("glIsSampler");
		glBindSampler = (PFNGLBINDSAMPLERPROC) loadEntryPoint("glBindSampler");
		glSamplerParameteri = (PFNGLSAMPLERPARAMETERIPROC) loadEntryPoint("glSamplerParameteri");
		glSamplerParameteriv = (PFNGLSAMPLERPARAMETERIVPROC) loadEntryPoint("glSamplerParameteriv");
		glSamplerParameterf = (PFNGLSAMPLERPARAMETERFPROC) loadEntryPoint("glSamplerParameterf");
		glSamplerParameterfv = (PFNGLSAMPLERPARAMETERFVPROC) loadEntryPoint("glSamplerParameterfv");
		glSamplerParameterIiv = (PFNGLSAMPLERPARAMETERIIVPROC) loadEntryPoint("glSamplerParameterIiv");
		glSamplerParameterIuiv = (PFNGLSAMPLERPARAMETERIUIVPROC) loadEntryPoint("glSamplerParameterIuiv");
		glGetSamplerParameteriv = (PFNGLGETSAMPLERPARAMETERIVPROC) loadEntryPoint("glGetSamplerParameteriv");
		glGetSamplerParameterIiv = (PFNGLGETSAMPLERPARAMETERIIVPROC) loadEntryPoint("glGetSamplerParameterIiv");
		glGetSamplerParameterfv = (PFNGLGETSAMPLERPARAMETERFVPROC) loadEntryPoint("glGetSamplerParameterfv");
		glGetSamplerParameterIuiv = (PFNGLGETSAMPLERPARAMETERIUIVPROC) loadEntryPoint("glGetSamplerParameterIuiv");
		glQueryCounter = (PFNGLQUERYCOUNTERPROC) loadEntryPoint("glQueryCounter");
		glGetQueryObjecti64v = (PFNGLGETQUERYOBJECTI64VPROC) loadEntryPoint("glGetQueryObjecti64v");
		glGetQueryObjectui64v = (PFNGLGETQUERYOBJECTUI64VPROC) loadEntryPoint("glGetQueryObjectui64v");
		glVertexP2ui = (PFNGLVERTEXP2UIPROC) loadEntryPoint("glVertexP2ui");
		glVertexP2uiv = (PFNGLVERTEXP2UIVPROC) loadEntryPoint("glVertexP2uiv");
		glVertexP3ui = (PFNGLVERTEXP3UIPROC) loadEntryPoint("glVertexP3ui");
		glVertexP3uiv = (PFNGLVERTEXP3UIVPROC) loadEntryPoint("glVertexP3uiv");
		glVertexP4ui = (PFNGLVERTEXP4UIPROC) loadEntryPoint("glVertexP4ui");
		glVertexP4uiv = (PFNGLVERTEXP4UIVPROC) loadEntryPoint("glVertexP4uiv");
		glTexCoordP1ui = (PFNGLTEXCOORDP1UIPROC) loadEntryPoint("glTexCoordP1ui");
		glTexCoordP1uiv = (PFNGLTEXCOORDP1UIVPROC) loadEntryPoint("glTexCoordP1uiv");
		glTexCoordP2ui = (PFNGLTEXCOORDP2UIPROC) loadEntryPoint("glTexCoordP2ui");
		glTexCoordP2uiv = (PFNGLTEXCOORDP2UIVPROC) loadEntryPoint("glTexCoordP2uiv");
		glTexCoordP3ui = (PFNGLTEXCOORDP3UIPROC) loadEntryPoint("glTexCoordP3ui");
		glTexCoordP3uiv = (PFNGLTEXCOORDP3UIVPROC) loadEntryPoint("glTexCoordP3uiv");
		glTexCoordP4ui = (PFNGLTEXCOORDP4UIPROC) loadEntryPoint("glTexCoordP4ui");
		glTexCoordP4uiv = (PFNGLTEXCOORDP4UIVPROC) loadEntryPoint("glTexCoordP4uiv");
		glMultiTexCoordP1ui = (PFNGLMULTITEXCOORDP1UIPROC) loadEntryPoint("glMultiTexCoordP1ui");
		glMultiTexCoordP1uiv = (PFNGLMULTITEXCOORDP1UIVPROC) loadEntryPoint("glMultiTexCoordP1uiv");
		glMultiTexCoordP2ui = (PFNGLMULTITEXCOORDP2UIPROC) loadEntryPoint("glMultiTexCoordP2ui");
		glMultiTexCoordP2uiv = (PFNGLMULTITEXCOORDP2UIVPROC) loadEntryPoint("glMultiTexCoordP2uiv");
		glMultiTexCoordP3ui = (PFNGLMULTITEXCOORDP3UIPROC) loadEntryPoint("glMultiTexCoordP3ui");
		glMultiTexCoordP3uiv = (PFNGLMULTITEXCOORDP3UIVPROC) loadEntryPoint("glMultiTexCoordP3uiv");
		glMultiTexCoordP4ui = (PFNGLMULTITEXCOORDP4UIPROC) loadEntryPoint("glMultiTexCoordP4ui");
		glMultiTexCoordP4uiv = (PFNGLMULTITEXCOORDP4UIVPROC) loadEntryPoint("glMultiTexCoordP4uiv");
		glNormalP3ui = (PFNGLNORMALP3UIPROC) loadEntryPoint("glNormalP3ui");
		glNormalP3uiv = (PFNGLNORMALP3UIVPROC) loadEntryPoint("glNormalP3uiv");
		glColorP3ui = (PFNGLCOLORP3UIPROC) loadEntryPoint("glColorP3ui");
		glColorP3uiv = (PFNGLCOLORP3UIVPROC) loadEntryPoint("glColorP3uiv");
		glColorP4ui = (PFNGLCOLORP4UIPROC) loadEntryPoint("glColorP4ui");
		glColorP4uiv = (PFNGLCOLORP4UIVPROC) loadEntryPoint("glColorP4uiv");
		glSecondaryColorP3ui = (PFNGLSECONDARYCOLORP3UIPROC) loadEntryPoint("glSecondaryColorP3ui");
		glSecondaryColorP3uiv = (PFNGLSECONDARYCOLORP3UIVPROC) loadEntryPoint("glSecondaryColorP3uiv");
		glVertexAttribP1ui = (PFNGLVERTEXATTRIBP1UIPROC) loadEntryPoint("glVertexAttribP1ui");
		glVertexAttribP1uiv = (PFNGLVERTEXATTRIBP1UIVPROC) loadEntryPoint("glVertexAttribP1uiv");
		glVertexAttribP2ui = (PFNGLVERTEXATTRIBP2UIPROC) loadEntryPoint("glVertexAttribP2ui");
		glVertexAttribP2uiv = (PFNGLVERTEXATTRIBP2UIVPROC) loadEntryPoint("glVertexAttribP2uiv");
		glVertexAttribP3ui = (PFNGLVERTEXATTRIBP3UIPROC) loadEntryPoint("glVertexAttribP3ui");
		glVertexAttribP3uiv = (PFNGLVERTEXATTRIBP3UIVPROC) loadEntryPoint("glVertexAttribP3uiv");
		glVertexAttribP4ui = (PFNGLVERTEXATTRIBP4UIPROC) loadEntryPoint("glVertexAttribP4ui");
		glVertexAttribP4uiv = (PFNGLVERTEXATTRIBP4UIVPROC) loadEntryPoint("glVertexAttribP4uiv");
	}

	// OpenGL 4.0
	if (version >= 40)
	{
		kge::io::Logger::Info("\tOpenGL 4.0 commands:");

		glMinSampleShading = (PFNGLMINSAMPLESHADINGPROC) loadEntryPoint("glMinSampleShading");
		glBlendEquationi = (PFNGLBLENDEQUATIONIPROC) loadEntryPoint("glBlendEquationi");
		glBlendEquationSeparatei = (PFNGLBLENDEQUATIONSEPARATEIPROC) loadEntryPoint("glBlendEquationSeparatei");
		glBlendFunci = (PFNGLBLENDFUNCIPROC) loadEntryPoint("glBlendFunci");
		glBlendFuncSeparatei = (PFNGLBLENDFUNCSEPARATEIPROC) loadEntryPoint("glBlendFuncSeparatei");
		glDrawArraysIndirect = (PFNGLDRAWARRAYSINDIRECTPROC) loadEntryPoint("glDrawArraysIndirect");
		glDrawElementsIndirect = (PFNGLDRAWELEMENTSINDIRECTPROC) loadEntryPoint("glDrawElementsIndirect");
		glUniform1d = (PFNGLUNIFORM1DPROC) loadEntryPoint("glUniform1d");
		glUniform2d = (PFNGLUNIFORM2DPROC) loadEntryPoint("glUniform2d");
		glUniform3d = (PFNGLUNIFORM3DPROC) loadEntryPoint("glUniform3d");
		glUniform4d = (PFNGLUNIFORM4DPROC) loadEntryPoint("glUniform4d");
		glUniform1dv = (PFNGLUNIFORM1DVPROC) loadEntryPoint("glUniform1dv");
		glUniform2dv = (PFNGLUNIFORM2DVPROC) loadEntryPoint("glUniform2dv");
		glUniform3dv = (PFNGLUNIFORM3DVPROC) loadEntryPoint("glUniform3dv");
		glUniform4dv = (PFNGLUNIFORM4DVPROC) loadEntryPoint("glUniform4dv");
		glUniformMatrix2dv = (PFNGLUNIFORMMATRIX2DVPROC) loadEntryPoint("glUniformMatrix2dv");
		glUniformMatrix3dv = (PFNGLUNIFORMMATRIX3DVPROC) loadEntryPoint("glUniformMatrix3dv");
		glUniformMatrix4dv = (PFNGLUNIFORMMATRIX4DVPROC) loadEntryPoint("glUniformMatrix4dv");
		glUniformMatrix2x3dv = (PFNGLUNIFORMMATRIX2X3DVPROC) loadEntryPoint("glUniformMatrix2x3dv");
		glUniformMatrix2x4dv = (PFNGLUNIFORMMATRIX2X4DVPROC) loadEntryPoint("glUniformMatrix2x4dv");
		glUniformMatrix3x2dv = (PFNGLUNIFORMMATRIX3X2DVPROC) loadEntryPoint("glUniformMatrix3x2dv");
		glUniformMatrix3x4dv = (PFNGLUNIFORMMATRIX3X4DVPROC) loadEntryPoint("glUniformMatrix3x4dv");
		glUniformMatrix4x2dv = (PFNGLUNIFORMMATRIX4X2DVPROC) loadEntryPoint("glUniformMatrix4x2dv");
		glUniformMatrix4x3dv = (PFNGLUNIFORMMATRIX4X3DVPROC) loadEntryPoint("glUniformMatrix4x3dv");
		glGetUniformdv = (PFNGLGETUNIFORMDVPROC) loadEntryPoint("glGetUniformdv");
		glGetSubroutineUniformLocation = (PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC) loadEntryPoint("glGetSubroutineUniformLocation");
		glGetSubroutineIndex = (PFNGLGETSUBROUTINEINDEXPROC) loadEntryPoint("glGetSubroutineIndex");
		glGetActiveSubroutineUniformiv = (PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC) loadEntryPoint("glGetActiveSubroutineUniformiv");
		glGetActiveSubroutineUniformName = (PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC) loadEntryPoint("glGetActiveSubroutineUniformName");
		glGetActiveSubroutineName = (PFNGLGETACTIVESUBROUTINENAMEPROC) loadEntryPoint("glGetActiveSubroutineName");
		glUniformSubroutinesuiv = (PFNGLUNIFORMSUBROUTINESUIVPROC) loadEntryPoint("glUniformSubroutinesuiv");
		glGetUniformSubroutineuiv = (PFNGLGETUNIFORMSUBROUTINEUIVPROC) loadEntryPoint("glGetUniformSubroutineuiv");
		glGetProgramStageiv = (PFNGLGETPROGRAMSTAGEIVPROC) loadEntryPoint("glGetProgramStageiv");
		glPatchParameteri = (PFNGLPATCHPARAMETERIPROC) loadEntryPoint("glPatchParameteri");
		glPatchParameterfv = (PFNGLPATCHPARAMETERFVPROC) loadEntryPoint("glPatchParameterfv");
		glBindTransformFeedback = (PFNGLBINDTRANSFORMFEEDBACKPROC) loadEntryPoint("glBindTransformFeedback");
		glDeleteTransformFeedbacks = (PFNGLDELETETRANSFORMFEEDBACKSPROC) loadEntryPoint("glDeleteTransformFeedbacks");
		glGenTransformFeedbacks = (PFNGLGENTRANSFORMFEEDBACKSPROC) loadEntryPoint("glGenTransformFeedbacks");
		glIsTransformFeedback = (PFNGLISTRANSFORMFEEDBACKPROC) loadEntryPoint("glIsTransformFeedback");
		glPauseTransformFeedback = (PFNGLPAUSETRANSFORMFEEDBACKPROC) loadEntryPoint("glPauseTransformFeedback");
		glResumeTransformFeedback = (PFNGLRESUMETRANSFORMFEEDBACKPROC) loadEntryPoint("glResumeTransformFeedback");
		glDrawTransformFeedback = (PFNGLDRAWTRANSFORMFEEDBACKPROC) loadEntryPoint("glDrawTransformFeedback");
		glDrawTransformFeedbackStream = (PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC) loadEntryPoint("glDrawTransformFeedbackStream");
		glBeginQueryIndexed = (PFNGLBEGINQUERYINDEXEDPROC) loadEntryPoint("glBeginQueryIndexed");
		glEndQueryIndexed = (PFNGLENDQUERYINDEXEDPROC) loadEntryPoint("glEndQueryIndexed");
		glGetQueryIndexediv = (PFNGLGETQUERYINDEXEDIVPROC) loadEntryPoint("glGetQueryIndexediv");
	}

	// OpenGL 4.1
	if (version >= 41)
	{
		kge::io::Logger::Info("\tOpenGL 4.1 commands:");

		glReleaseShaderCompiler = (PFNGLRELEASESHADERCOMPILERPROC) loadEntryPoint("glReleaseShaderCompiler");
		glShaderBinary = (PFNGLSHADERBINARYPROC) loadEntryPoint("glShaderBinary");
		glGetShaderPrecisionFormat = (PFNGLGETSHADERPRECISIONFORMATPROC) loadEntryPoint("glGetShaderPrecisionFormat");
		glDepthRangef = (PFNGLDEPTHRANGEFPROC) loadEntryPoint("glDepthRangef");
		glClearDepthf = (PFNGLCLEARDEPTHFPROC) loadEntryPoint("glClearDepthf");
		glGetProgramBinary = (PFNGLGETPROGRAMBINARYPROC) loadEntryPoint("glGetProgramBinary");
		glProgramBinary = (PFNGLPROGRAMBINARYPROC) loadEntryPoint("glProgramBinary");
		glProgramParameteri = (PFNGLPROGRAMPARAMETERIPROC) loadEntryPoint("glProgramParameteri");
		glUseProgramStages = (PFNGLUSEPROGRAMSTAGESPROC) loadEntryPoint("glUseProgramStages");
		glActiveShaderProgram = (PFNGLACTIVESHADERPROGRAMPROC) loadEntryPoint("glActiveShaderProgram");
		glCreateShaderProgramv = (PFNGLCREATESHADERPROGRAMVPROC) loadEntryPoint("glCreateShaderProgramv");
		glBindProgramPipeline = (PFNGLBINDPROGRAMPIPELINEPROC) loadEntryPoint("glBindProgramPipeline");
		glDeleteProgramPipelines = (PFNGLDELETEPROGRAMPIPELINESPROC) loadEntryPoint("glDeleteProgramPipelines");
		glGenProgramPipelines = (PFNGLGENPROGRAMPIPELINESPROC) loadEntryPoint("glGenProgramPipelines");
		glIsProgramPipeline = (PFNGLISPROGRAMPIPELINEPROC) loadEntryPoint("glIsProgramPipeline");
		glGetProgramPipelineiv = (PFNGLGETPROGRAMPIPELINEIVPROC) loadEntryPoint("glGetProgramPipelineiv");
		glProgramUniform1i = (PFNGLPROGRAMUNIFORM1IPROC) loadEntryPoint("glProgramUniform1i");
		glProgramUniform1iv = (PFNGLPROGRAMUNIFORM1IVPROC) loadEntryPoint("glProgramUniform1iv");
		glProgramUniform1f = (PFNGLPROGRAMUNIFORM1FPROC) loadEntryPoint("glProgramUniform1f");
		glProgramUniform1fv = (PFNGLPROGRAMUNIFORM1FVPROC) loadEntryPoint("glProgramUniform1fv");
		glProgramUniform1d = (PFNGLPROGRAMUNIFORM1DPROC) loadEntryPoint("glProgramUniform1d");
		glProgramUniform1dv = (PFNGLPROGRAMUNIFORM1DVPROC) loadEntryPoint("glProgramUniform1dv");
		glProgramUniform1ui = (PFNGLPROGRAMUNIFORM1UIPROC) loadEntryPoint("glProgramUniform1ui");
		glProgramUniform1uiv = (PFNGLPROGRAMUNIFORM1UIVPROC) loadEntryPoint("glProgramUniform1uiv");
		glProgramUniform2i = (PFNGLPROGRAMUNIFORM2IPROC) loadEntryPoint("glProgramUniform2i");
		glProgramUniform2iv = (PFNGLPROGRAMUNIFORM2IVPROC) loadEntryPoint("glProgramUniform2iv");
		glProgramUniform2f = (PFNGLPROGRAMUNIFORM2FPROC) loadEntryPoint("glProgramUniform2f");
		glProgramUniform2fv = (PFNGLPROGRAMUNIFORM2FVPROC) loadEntryPoint("glProgramUniform2fv");
		glProgramUniform2d = (PFNGLPROGRAMUNIFORM2DPROC) loadEntryPoint("glProgramUniform2d");
		glProgramUniform2dv = (PFNGLPROGRAMUNIFORM2DVPROC) loadEntryPoint("glProgramUniform2dv");
		glProgramUniform2ui = (PFNGLPROGRAMUNIFORM2UIPROC) loadEntryPoint("glProgramUniform2ui");
		glProgramUniform2uiv = (PFNGLPROGRAMUNIFORM2UIVPROC) loadEntryPoint("glProgramUniform2uiv");
		glProgramUniform3i = (PFNGLPROGRAMUNIFORM3IPROC) loadEntryPoint("glProgramUniform3i");
		glProgramUniform3iv = (PFNGLPROGRAMUNIFORM3IVPROC) loadEntryPoint("glProgramUniform3iv");
		glProgramUniform3f = (PFNGLPROGRAMUNIFORM3FPROC) loadEntryPoint("glProgramUniform3f");
		glProgramUniform3fv = (PFNGLPROGRAMUNIFORM3FVPROC) loadEntryPoint("glProgramUniform3fv");
		glProgramUniform3d = (PFNGLPROGRAMUNIFORM3DPROC) loadEntryPoint("glProgramUniform3d");
		glProgramUniform3dv = (PFNGLPROGRAMUNIFORM3DVPROC) loadEntryPoint("glProgramUniform3dv");
		glProgramUniform3ui = (PFNGLPROGRAMUNIFORM3UIPROC) loadEntryPoint("glProgramUniform3ui");
		glProgramUniform3uiv = (PFNGLPROGRAMUNIFORM3UIVPROC) loadEntryPoint("glProgramUniform3uiv");
		glProgramUniform4i = (PFNGLPROGRAMUNIFORM4IPROC) loadEntryPoint("glProgramUniform4i");
		glProgramUniform4iv = (PFNGLPROGRAMUNIFORM4IVPROC) loadEntryPoint("glProgramUniform4iv");
		glProgramUniform4f = (PFNGLPROGRAMUNIFORM4FPROC) loadEntryPoint("glProgramUniform4f");
		glProgramUniform4fv = (PFNGLPROGRAMUNIFORM4FVPROC) loadEntryPoint("glProgramUniform4fv");
		glProgramUniform4d = (PFNGLPROGRAMUNIFORM4DPROC) loadEntryPoint("glProgramUniform4d");
		glProgramUniform4dv = (PFNGLPROGRAMUNIFORM4DVPROC) loadEntryPoint("glProgramUniform4dv");
		glProgramUniform4ui = (PFNGLPROGRAMUNIFORM4UIPROC) loadEntryPoint("glProgramUniform4ui");
		glProgramUniform4uiv = (PFNGLPROGRAMUNIFORM4UIVPROC) loadEntryPoint("glProgramUniform4uiv");
		glProgramUniformMatrix2fv = (PFNGLPROGRAMUNIFORMMATRIX2FVPROC) loadEntryPoint("glProgramUniformMatrix2fv");
		glProgramUniformMatrix3fv = (PFNGLPROGRAMUNIFORMMATRIX3FVPROC) loadEntryPoint("glProgramUniformMatrix3fv");
		glProgramUniformMatrix4fv = (PFNGLPROGRAMUNIFORMMATRIX4FVPROC) loadEntryPoint("glProgramUniformMatrix4fv");
		glProgramUniformMatrix2dv = (PFNGLPROGRAMUNIFORMMATRIX2DVPROC) loadEntryPoint("glProgramUniformMatrix2dv");
		glProgramUniformMatrix3dv = (PFNGLPROGRAMUNIFORMMATRIX3DVPROC) loadEntryPoint("glProgramUniformMatrix3dv");
		glProgramUniformMatrix4dv = (PFNGLPROGRAMUNIFORMMATRIX4DVPROC) loadEntryPoint("glProgramUniformMatrix4dv");
		glProgramUniformMatrix2x3fv = (PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC) loadEntryPoint("glProgramUniformMatrix2x3fv");
		glProgramUniformMatrix3x2fv = (PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC) loadEntryPoint("glProgramUniformMatrix3x2fv");
		glProgramUniformMatrix2x4fv = (PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC) loadEntryPoint("glProgramUniformMatrix2x4fv");
		glProgramUniformMatrix4x2fv = (PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC) loadEntryPoint("glProgramUniformMatrix4x2fv");
		glProgramUniformMatrix3x4fv = (PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC) loadEntryPoint("glProgramUniformMatrix3x4fv");
		glProgramUniformMatrix4x3fv = (PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC) loadEntryPoint("glProgramUniformMatrix4x3fv");
		glProgramUniformMatrix2x3dv = (PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC) loadEntryPoint("glProgramUniformMatrix2x3dv");
		glProgramUniformMatrix3x2dv = (PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC) loadEntryPoint("glProgramUniformMatrix3x2dv");
		glProgramUniformMatrix2x4dv = (PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC) loadEntryPoint("glProgramUniformMatrix2x4dv");
		glProgramUniformMatrix4x2dv = (PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC) loadEntryPoint("glProgramUniformMatrix4x2dv");
		glProgramUniformMatrix3x4dv = (PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC) loadEntryPoint("glProgramUniformMatrix3x4dv");
		glProgramUniformMatrix4x3dv = (PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC) loadEntryPoint("glProgramUniformMatrix4x3dv");
		glValidateProgramPipeline = (PFNGLVALIDATEPROGRAMPIPELINEPROC) loadEntryPoint("glValidateProgramPipeline");
		glGetProgramPipelineInfoLog = (PFNGLGETPROGRAMPIPELINEINFOLOGPROC) loadEntryPoint("glGetProgramPipelineInfoLog");
		glVertexAttribL1d = (PFNGLVERTEXATTRIBL1DPROC) loadEntryPoint("glVertexAttribL1d");
		glVertexAttribL2d = (PFNGLVERTEXATTRIBL2DPROC) loadEntryPoint("glVertexAttribL2d");
		glVertexAttribL3d = (PFNGLVERTEXATTRIBL3DPROC) loadEntryPoint("glVertexAttribL3d");
		glVertexAttribL4d = (PFNGLVERTEXATTRIBL4DPROC) loadEntryPoint("glVertexAttribL4d");
		glVertexAttribL1dv = (PFNGLVERTEXATTRIBL1DVPROC) loadEntryPoint("glVertexAttribL1dv");
		glVertexAttribL2dv = (PFNGLVERTEXATTRIBL2DVPROC) loadEntryPoint("glVertexAttribL2dv");
		glVertexAttribL3dv = (PFNGLVERTEXATTRIBL3DVPROC) loadEntryPoint("glVertexAttribL3dv");
		glVertexAttribL4dv = (PFNGLVERTEXATTRIBL4DVPROC) loadEntryPoint("glVertexAttribL4dv");
		glVertexAttribLPointer = (PFNGLVERTEXATTRIBLPOINTERPROC) loadEntryPoint("glVertexAttribLPointer");
		glGetVertexAttribLdv = (PFNGLGETVERTEXATTRIBLDVPROC) loadEntryPoint("glGetVertexAttribLdv");
		glViewportArrayv = (PFNGLVIEWPORTARRAYVPROC) loadEntryPoint("glViewportArrayv");
		glViewportIndexedf = (PFNGLVIEWPORTINDEXEDFPROC) loadEntryPoint("glViewportIndexedf");
		glViewportIndexedfv = (PFNGLVIEWPORTINDEXEDFVPROC) loadEntryPoint("glViewportIndexedfv");
		glScissorArrayv = (PFNGLSCISSORARRAYVPROC) loadEntryPoint("glScissorArrayv");
		glScissorIndexed = (PFNGLSCISSORINDEXEDPROC) loadEntryPoint("glScissorIndexed");
		glScissorIndexedv = (PFNGLSCISSORINDEXEDVPROC) loadEntryPoint("glScissorIndexedv");
		glDepthRangeArrayv = (PFNGLDEPTHRANGEARRAYVPROC) loadEntryPoint("glDepthRangeArrayv");
		glDepthRangeIndexed = (PFNGLDEPTHRANGEINDEXEDPROC) loadEntryPoint("glDepthRangeIndexed");
		glGetFloati_v = (PFNGLGETFLOATI_VPROC) loadEntryPoint("glGetFloati_v");
		glGetDoublei_v = (PFNGLGETDOUBLEI_VPROC) loadEntryPoint("glGetDoublei_v");
	}

#endif // KGE_PLATFORM != KGE_PLATFORM_ANDROID

	return true;
}


