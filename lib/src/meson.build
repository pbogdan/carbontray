gnome = import('gnome')

libcarbontray_deps = [
    dependency('gtk+-x11-3.0')
  , dependency('x11')
  , dependency('xcomposite')
]

libcarbontray_sources = [
    'tray.c'
  , 'child.c'
]

libcarbontray_marshal_files = gnome.genmarshal('marshal', sources: 'marshal.list')

carbontray = shared_library(
    'carbontray'
  , libcarbontray_sources + libcarbontray_marshal_files
  , include_directories : inc
  , dependencies : libcarbontray_deps
  , install : true
)
