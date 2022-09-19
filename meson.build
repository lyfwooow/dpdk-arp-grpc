project(
    'monitor',
    'c',
    version : '1.0',
)

dpdk = dependency('libdpdk')

sources = files(
    'src/main.c',
    'src/port.c',
)

executable('monitor', sources, dependencies: dpdk, install : true)
