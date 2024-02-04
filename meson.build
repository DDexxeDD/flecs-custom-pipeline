project('pipeline', 'c')

cc = meson.get_compiler('c')

pipeline_dependencies = [
  cc.find_library('pthread'),
]

include = [
  include_directories('.'),
  include_directories('external/flecs'),
]

source_files = [
  'source/main.c',
]

subdir ('external/flecs')

pipeline_binary = executable (
  'pipeline',
  source_files,
  include_directories : include,
  dependencies : [pipeline_dependencies],
)
