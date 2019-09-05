project('avoider', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])

cc = meson.get_compiler('c')
m_dep = cc.find_library('m', required : false)

deps = [
  dependency('lapack'),
  dependency('sdl2'),
  dependency('SDL2_gfx'),
  m_dep,
]

srcs = [
  'src/main.c',

  'src/ai/common.c',
  'src/ai/simple.c',
  'src/ai/simple2.c',
  'src/engine.c',
  'src/math_util.c',
  'src/object.c',
]

executable('avoider',
           srcs,
           install : true,
           dependencies : deps)
