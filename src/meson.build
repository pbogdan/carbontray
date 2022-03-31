carbontray_deps = [
    dependency('glib-2.0')
  , dependency('gtk+-x11-3.0')
]

executable(
    'carbontray'
  , 'carbontray.c'
  , install: true
  , dependencies: carbontray_deps
  , link_with: carbontray
  , include_directories: inc
)
