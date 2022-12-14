# Generated by the gRPC Python protocol compiler plugin. DO NOT EDIT!
"""Client and server classes corresponding to protobuf-defined services."""
import grpc

import grpc_pb2 as grpc__pb2


class GreeterStub(object):
    """Missing associated documentation comment in .proto file."""

    def __init__(self, channel):
        """Constructor.

        Args:
            channel: A grpc.Channel.
        """
        self.GetMsg = channel.unary_unary(
                '/Greeter/GetMsg',
                request_serializer=grpc__pb2.ReqMsg.SerializeToString,
                response_deserializer=grpc__pb2.grpc_dpdk_arp_res.FromString,
                )


class GreeterServicer(object):
    """Missing associated documentation comment in .proto file."""

    def GetMsg(self, request, context):
        """单次请求单次回应
        """
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')


def add_GreeterServicer_to_server(servicer, server):
    rpc_method_handlers = {
            'GetMsg': grpc.unary_unary_rpc_method_handler(
                    servicer.GetMsg,
                    request_deserializer=grpc__pb2.ReqMsg.FromString,
                    response_serializer=grpc__pb2.grpc_dpdk_arp_res.SerializeToString,
            ),
    }
    generic_handler = grpc.method_handlers_generic_handler(
            'Greeter', rpc_method_handlers)
    server.add_generic_rpc_handlers((generic_handler,))


 # This class is part of an EXPERIMENTAL API.
class Greeter(object):
    """Missing associated documentation comment in .proto file."""

    @staticmethod
    def GetMsg(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/Greeter/GetMsg',
            grpc__pb2.ReqMsg.SerializeToString,
            grpc__pb2.grpc_dpdk_arp_res.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)
