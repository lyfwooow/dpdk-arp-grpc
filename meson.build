project(
    'monitor',
    'c',
    'cpp',
    version : '1.0',
)

dpdk = declare_dependency(
    dependencies: dependency('libdpdk'),
    link_args: [
        '-L/usr/local/lib64',
        '-lrte_eal',
        '-lrte_ethdev',
        '-lrte_mbuf',
        '-lrte_mempool',
        '-lrte_mempool_ring',
        '-lrte_net_vmxnet3',
        '-lrte_net_ixgbe',
        '-lrte_net_i40e',
        ],
     required: true,static: false)
#dpdk_lib = static_library('libdpdk')
#dpdk = declare_dependency(link_with:dpdk_lib)
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

dpdk_cflags = run_command('pkg-config', '--cflags-only-I', 'libdpdk').stdout().split()

#default_cflags = []

#default_cflags += dpdk_cflags
#message(default_cflags)

executable('monitor', sources,
c_args: [dpdk_cflags],
cpp_args: [dpdk_cflags],
dependencies: [dpdk, protobuf, grpc], install : true)
