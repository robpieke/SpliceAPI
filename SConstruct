import os, platform

AddOption(
  '--debug-build',
  action='store_true',
  help='debug build',
  default=False
)

# check environment variables
for var in ['FABRIC_CAPI_DIR', 'FABRIC_SPLICE_VERSION', 'BOOST_DIR']:
  if not os.environ.has_key(var):
    print 'The environment variable %s is not defined.' % var
    exit(0)
  if var.lower().endswith('_dir'):
    if not os.path.exists(os.environ[var]):
      print 'The path for environment variable %s does not exist.' % var
      exit(0)

spliceEnv = Environment()

if not os.path.exists(spliceEnv.Dir('.stage').abspath):
  os.makedirs(spliceEnv.Dir('.stage').abspath)

(spliceApiAlias, spliceApiFiles) = SConscript(
  dirs = ['.'],
  exports = {
    'parentEnv': spliceEnv,
    'FABRIC_CAPI_DIR': os.environ['FABRIC_CAPI_DIR'],
    'CORE_VERSION': os.environ['FABRIC_SPLICE_VERSION'].rpartition('.')[0],
    'SPLICE_VERSION': os.environ['FABRIC_SPLICE_VERSION'],
    'STAGE_DIR': spliceEnv.Dir('.stage').Dir('API').Dir('FabricSpliceAPI'),
    'SPLICE_DEBUG': GetOption('debug_build'),
    'SPLICE_OS': platform.system(),
    'SPLICE_ARCH': 'x86_64',
    'BOOST_DIR': os.environ['BOOST_DIR']
  },
  variant_dir = spliceEnv.Dir('.build')
)

allAliases = [spliceApiAlias]
spliceEnv.Alias('all', allAliases)
