project(
    'monitor',
    'c',
    'cpp',
    version : '1.0',
)

dpdk = dependency('libdpdk')
protobuf = dependency('protobuf')
grpc = dependency('grpc++')


sources = files(
    'src/data.cpp',
    'src/main.cpp',
    'src/port.c',
    'src/process.cpp',
    'src/proto/ns.grpc.pb.cc',
    'src/proto/ns.pb.cc',
    'src/proto/server.cpp',
)

executable('monitor', sources, dependencies: [dpdk, protobuf, grpc], install : true)
