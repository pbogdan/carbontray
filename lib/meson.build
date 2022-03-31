inc = include_directories('include')

subdir('include')
subdir('src')

pkg_mod = import('pkgconfig')
pkg_mod.generate(
    libraries : carbontray
  , version : '1.0'
  , name : 'libcarbontray'
  , filebase: 'carbontray'
  , description : 'A from-scratch implementation of the X system tray.'
)
