#
# Copyright 2010-2013 Fabric Engine Inc. All rights reserved.
#

import os, sys, platform, copy

Import('parentEnv', 'FABRIC_CAPI_DIR', 'CORE_VERSION', 'SPLICE_VERSION', 'STAGE_DIR', 'SPLICE_DEBUG', 'SPLICE_OS', 'SPLICE_ARCH', 'BOOST_DIR')

# configuration flags
if SPLICE_OS == "Windows":
  baseCPPDefines = [
    '_SCL_SECURE_NO_WARNINGS=1',
    '_ITERATOR_DEBUG_LEVEL=0',
    '_WIN32_WINNT=0x0600',
  ]
  baseCPPFlags = [
    '/EHsc', 
    '/wd4624',
  ]
  baseLinkFlags = ['chkstk.obj']
  spliceDebugFlags = {
    'CCFLAGS': baseCPPFlags + ['/Od', '/Z7', '/MTd', '/DEBUG'],
    'CPPDEFINES': baseCPPDefines + ['_ITERATOR_DEBUG_LEVEL=0', '_DEBUG', 'DEBUG'],
    'LINKFLAGS': baseLinkFlags + ['/DEBUG', '/NODEFAULTLIB:LIBCMT'],
  }
  spliceReleaseFlags = {
    'CCFLAGS': baseCPPFlags + ['/Ox', '/MT'],
    'CPPDEFINES': baseCPPDefines + ['NDEBUG'],
    'LINKFLAGS': baseLinkFlags + ['/NDEBUG', '/NODEFAULTLIB:LIBCMTD'],
  }
  if SPLICE_ARCH.endswith('64'):
    baseCPPDefines.append( 'WIN64' )
else:
  spliceDebugFlags = {
    'CCFLAGS': ['-fPIC', '-m64', '-g'],
    'LINKFLAGS': ['-m64']
  }
  spliceReleaseFlags = {
    'CCFLAGS': ['-fPIC', '-m64', '-O2'],
    'LINKFLAGS': ['-m64'],
    'CPPDEFINES': ['NDEBUG'],
  }

Export('spliceDebugFlags', 'spliceReleaseFlags')

if SPLICE_DEBUG:
  parentEnv.MergeFlags(spliceDebugFlags)
else:
  parentEnv.MergeFlags(spliceReleaseFlags)

baseCapiFlags = {
  'CPPPATH': [os.path.join(FABRIC_CAPI_DIR, 'include')],
  'CPPDEFINES': [
    'FEC_PROVIDE_STL_BINDINGS',
    '__STDC_LIMIT_MACROS',
    '__STDC_CONSTANT_MACROS',
  ],
  'LIBPATH': [],
  'LIBS': [],
}

staticCapiFlags = copy.deepcopy(baseCapiFlags)
staticCapiFlags['CPPDEFINES'] += ['FEC_STATIC']
if SPLICE_OS == 'Windows':
  staticCapiFlags['LIBS'] += [File(os.path.join(FABRIC_CAPI_DIR, 'lib', 'FabricCore-'+CORE_VERSION+'_s.lib'))]
else:
  staticCapiFlags['LIBS'] += [File(os.path.join(FABRIC_CAPI_DIR, 'lib', 'libFabricCore-'+CORE_VERSION+'_s.a'))]
Export('staticCapiFlags')

sharedCapiFlags = copy.deepcopy(baseCapiFlags)
sharedCapiFlags['CPPDEFINES'] += ['FEC_SHARED']
sharedCapiFlags['LIBPATH'] += [os.path.join(FABRIC_CAPI_DIR, 'lib')]
sharedCapiFlags['LIBS'] += ['FabricCore-'+CORE_VERSION]
Export('sharedCapiFlags')

apiVersion = SPLICE_VERSION.split('.')
for i in range(1, len(apiVersion)):
  while len(apiVersion[i]) < 3:
    apiVersion[i] = '0'+apiVersion[i]

parentEnv.Append(CPPDEFINES=['SPLICE_API_VERSION='+''.join(apiVersion)])
parentEnv.Append(CPPDEFINES=['SPLICE_MAJOR_VERSION='+SPLICE_VERSION.split('.')[0]])
parentEnv.Append(CPPDEFINES=['SPLICE_MINOR_VERSION='+SPLICE_VERSION.split('.')[1]])
parentEnv.Append(CPPDEFINES=['SPLICE_REVISION_VERSION='+SPLICE_VERSION.split('.')[2]])

if SPLICE_OS == 'Windows':
  parentEnv.Append(LIBS = ['advapi32', 'shell32'])

if BOOST_DIR is None or not os.path.exists(BOOST_DIR):
  print "BOOST_DIR not found. Please specify BOOST_DIR."
  print "Refer to README.txt for more information."
  sys.exit(1)

boostFlags = {
  'CPPPATH': [BOOST_DIR],
  'LIBPATH': [os.path.join(BOOST_DIR, 'lib')],
}
if SPLICE_OS == 'Windows':
  if SPLICE_DEBUG:
    boostFlags['LIBS'] = [
      'libboost_thread-vc100-mt-sgd-1_55.lib',
      'libboost_system-vc100-mt-sgd-1_55.lib',
      'libboost_filesystem-vc100-mt-sgd-1_55.lib'
      ]
  else:
    boostFlags['LIBS'] = [
      'libboost_thread-vc100-mt-s-1_55.lib',
      'libboost_system-vc100-mt-s-1_55.lib',
      'libboost_filesystem-vc100-mt-s-1_55.lib'
      ]
else:
  boostFlags['LIBS'] = ['boost_thread','boost_system','boost_filesystem']
Export('boostFlags')

parentEnv.MergeFlags(boostFlags)

env = parentEnv.Clone()

libNameBase = 'FabricSplice-'+SPLICE_VERSION

sharedEnv = env.Clone()
sharedEnv.Append(CPPDEFINES=['FEC_SHARED', 'FECS_SHARED', 'FECS_BUILDING'])
sharedEnv.MergeFlags(sharedCapiFlags)
sharedLibName = libNameBase
if SPLICE_OS == 'Windows':
  sharedLibName += '.dll'
else:
  sharedLibName += '.so'
sharedEnv.VariantDir('shared', env.Dir('.').srcnode().abspath)

staticEnv = env.Clone()
staticEnv.Append(CPPDEFINES=['FEC_SHARED', 'FECS_STATIC', 'FECS_BUILDING'])
staticEnv.MergeFlags(staticCapiFlags)
staticLibName = libNameBase+'_s'
if SPLICE_OS == 'Windows':
  staticLibName += '.lib'
else:
  staticLibName += '.a'

staticLibrary = staticEnv.Library(
  staticLibName, 
  staticEnv.Glob('*.cpp')
)
installedStaticLibrary = staticEnv.Install(STAGE_DIR, staticLibrary)

installedHeader = staticEnv.Install(STAGE_DIR, 'FabricSplice.h')
installedLicense = staticEnv.Install(STAGE_DIR, 'license.txt')

installedFiles = [installedStaticLibrary, installedHeader, installedLicense]

# StaticSpliceFlags = {
#   'CPPPATH': [STAGE_DIR],
#   'LIBS': [installedStaticLibrary],
#   'CPPDEFINES': ['SPLICE_API_VERSION='+str(SPLICE_VERSION).partition('-')[0].replace('.', ''), 'FECS_STATIC']
# }
# Export('StaticSpliceFlags')

# SharedSpliceFlags = {
#   'CPPPATH': [STAGE_DIR],
#   'LIBPATH': [STAGE_DIR],
#   'LIBS': [libNameBase],
#   'CPPDEFINES': ['SPLICE_API_VERSION='+str(SPLICE_VERSION).partition('-')[0].replace('.', ''), 'FECS_SHARED']
# }
# Export('SharedSpliceFlags')

# # build a zip
# spliceArchives = []
# if 'upload' in COMMAND_LINE_TARGETS and ('dist' in COMMAND_LINE_TARGETS or 'api' in COMMAND_LINE_TARGETS):
#   archiveName = 'FabricSplice'+SPLICE_VERSION+'-API-'+distOS+'-'+buildArch
#   spliceArchives.append(env.ZipBetter(os.path.join(installRoot, archiveName+'.zip'), STAGE_DIR, baseDir = installRoot))

#   if buildDCCSamples:
#     # build the sample scenes zip
#     if not cpDir:
#       raise Exception("FABRIC_SCENE_GRAPH_DIR not found.")
#     sampleSceneDir = os.path.join(cpDir, 'Python', 'Apps', 'Resources', 'Splice', 'Splice')
#     installSampleSceneDir = os.path.join(installRoot, 'Samples', 'Splice')
#     env.Install(installSampleSceneDir, Glob(os.path.join(sampleSceneDir, '*')))
#     archiveName = 'FabricSplice'+SPLICE_VERSION+'-JSON-Samples'
#     spliceArchives.append(env.ZipBetter(os.path.join(installRoot, archiveName+'.zip'), installSampleSceneDir, baseDir = installRoot))

alias = env.Alias('spliceapi', installedFiles)
spliceData = (alias, installedFiles)
Return('spliceData')
