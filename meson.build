project(
    'monitor',
    'c',
    'cpp',
    version : '1.0',
)

dpdk = dependency('libdpdk')

sources = files(
    'src/main.cpp',
    'src/port.c',
)

executable('monitor', sources, dependencies: dpdk, install : true)
