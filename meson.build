project(
    'monitor',
    'c',
    'cpp',
    version : '1.0',
)

dpdk = dependency('libdpdk')

sources = files(
    'src/data.cpp',
    'src/main.cpp',
    'src/port.c',
    'src/process.cpp',
)

executable('monitor', sources, dependencies: dpdk, install : true)
