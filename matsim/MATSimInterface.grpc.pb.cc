// Generated by the gRPC protobuf plugin.
// If you make any local change, they will be lost.
// source: MATSimInterface.proto

#include "MATSimInterface.pb.h"
#include "MATSimInterface.grpc.pb.h"

#include <grpc++/async_unary_call.h>
#include <grpc++/channel_interface.h>
#include <grpc++/impl/client_unary_call.h>
#include <grpc++/impl/rpc_service_method.h>
#include <grpc++/impl/service_type.h>
#include <grpc++/stream.h>
namespace hybrid {

static const char* MATSimInterfaceService_method_names[] = {
  "/hybrid.MATSimInterfaceService/reqExtern2MATSim",
  "/hybrid.MATSimInterfaceService/reqAgentStuck",
  "/hybrid.MATSimInterfaceService/reqExternalConnect",
  "/hybrid.MATSimInterfaceService/reqExternSimStepFinished",
};

std::unique_ptr< MATSimInterfaceService::Stub> MATSimInterfaceService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel) {
  std::unique_ptr< MATSimInterfaceService::Stub> stub(new MATSimInterfaceService::Stub(channel));
  return stub;
}

MATSimInterfaceService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : ::grpc::InternalStub(channel), rpcmethod_reqExtern2MATSim_(MATSimInterfaceService_method_names[0], ::grpc::RpcMethod::NORMAL_RPC, channel->RegisterMethod(MATSimInterfaceService_method_names[0]))
  , rpcmethod_reqAgentStuck_(MATSimInterfaceService_method_names[1], ::grpc::RpcMethod::NORMAL_RPC, channel->RegisterMethod(MATSimInterfaceService_method_names[1]))
  , rpcmethod_reqExternalConnect_(MATSimInterfaceService_method_names[2], ::grpc::RpcMethod::NORMAL_RPC, channel->RegisterMethod(MATSimInterfaceService_method_names[2]))
  , rpcmethod_reqExternSimStepFinished_(MATSimInterfaceService_method_names[3], ::grpc::RpcMethod::NORMAL_RPC, channel->RegisterMethod(MATSimInterfaceService_method_names[3]))
  {}

::grpc::Status MATSimInterfaceService::Stub::reqExtern2MATSim(::grpc::ClientContext* context, const ::hybrid::Extern2MATSim& request, ::hybrid::Extern2MATSimConfirmed* response) {
  return ::grpc::BlockingUnaryCall(channel(), rpcmethod_reqExtern2MATSim_, context, request, response);
}

std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::hybrid::Extern2MATSimConfirmed>> MATSimInterfaceService::Stub::AsyncreqExtern2MATSim(::grpc::ClientContext* context, const ::hybrid::Extern2MATSim& request, ::grpc::CompletionQueue* cq, void* tag) {
  return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::hybrid::Extern2MATSimConfirmed>>(new ::grpc::ClientAsyncResponseReader< ::hybrid::Extern2MATSimConfirmed>(channel(), cq, rpcmethod_reqExtern2MATSim_, context, request, tag));
}

::grpc::Status MATSimInterfaceService::Stub::reqAgentStuck(::grpc::ClientContext* context, const ::hybrid::AgentsStuck& request, ::hybrid::AgentsStuckConfirmed* response) {
  return ::grpc::BlockingUnaryCall(channel(), rpcmethod_reqAgentStuck_, context, request, response);
}

std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::hybrid::AgentsStuckConfirmed>> MATSimInterfaceService::Stub::AsyncreqAgentStuck(::grpc::ClientContext* context, const ::hybrid::AgentsStuck& request, ::grpc::CompletionQueue* cq, void* tag) {
  return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::hybrid::AgentsStuckConfirmed>>(new ::grpc::ClientAsyncResponseReader< ::hybrid::AgentsStuckConfirmed>(channel(), cq, rpcmethod_reqAgentStuck_, context, request, tag));
}

::grpc::Status MATSimInterfaceService::Stub::reqExternalConnect(::grpc::ClientContext* context, const ::hybrid::ExternalConnect& request, ::hybrid::ExternalConnectConfirmed* response) {
  return ::grpc::BlockingUnaryCall(channel(), rpcmethod_reqExternalConnect_, context, request, response);
}

std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::hybrid::ExternalConnectConfirmed>> MATSimInterfaceService::Stub::AsyncreqExternalConnect(::grpc::ClientContext* context, const ::hybrid::ExternalConnect& request, ::grpc::CompletionQueue* cq, void* tag) {
  return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::hybrid::ExternalConnectConfirmed>>(new ::grpc::ClientAsyncResponseReader< ::hybrid::ExternalConnectConfirmed>(channel(), cq, rpcmethod_reqExternalConnect_, context, request, tag));
}

::grpc::Status MATSimInterfaceService::Stub::reqExternSimStepFinished(::grpc::ClientContext* context, const ::hybrid::ExternSimStepFinished& request, ::hybrid::ExternSimStepFinishedReceived* response) {
  return ::grpc::BlockingUnaryCall(channel(), rpcmethod_reqExternSimStepFinished_, context, request, response);
}

std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::hybrid::ExternSimStepFinishedReceived>> MATSimInterfaceService::Stub::AsyncreqExternSimStepFinished(::grpc::ClientContext* context, const ::hybrid::ExternSimStepFinished& request, ::grpc::CompletionQueue* cq, void* tag) {
  return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::hybrid::ExternSimStepFinishedReceived>>(new ::grpc::ClientAsyncResponseReader< ::hybrid::ExternSimStepFinishedReceived>(channel(), cq, rpcmethod_reqExternSimStepFinished_, context, request, tag));
}

MATSimInterfaceService::AsyncService::AsyncService(::grpc::CompletionQueue* cq) : ::grpc::AsynchronousService(cq, MATSimInterfaceService_method_names, 4) {}

MATSimInterfaceService::Service::~Service() {
  delete service_;
}

::grpc::Status MATSimInterfaceService::Service::reqExtern2MATSim(::grpc::ServerContext* context, const ::hybrid::Extern2MATSim* request, ::hybrid::Extern2MATSimConfirmed* response) {
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void MATSimInterfaceService::AsyncService::RequestreqExtern2MATSim(::grpc::ServerContext* context, ::hybrid::Extern2MATSim* request, ::grpc::ServerAsyncResponseWriter< ::hybrid::Extern2MATSimConfirmed>* response, ::grpc::CompletionQueue* cq, void* tag) {
  AsynchronousService::RequestAsyncUnary(0, context, request, response, cq, tag);
}

::grpc::Status MATSimInterfaceService::Service::reqAgentStuck(::grpc::ServerContext* context, const ::hybrid::AgentsStuck* request, ::hybrid::AgentsStuckConfirmed* response) {
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void MATSimInterfaceService::AsyncService::RequestreqAgentStuck(::grpc::ServerContext* context, ::hybrid::AgentsStuck* request, ::grpc::ServerAsyncResponseWriter< ::hybrid::AgentsStuckConfirmed>* response, ::grpc::CompletionQueue* cq, void* tag) {
  AsynchronousService::RequestAsyncUnary(1, context, request, response, cq, tag);
}

::grpc::Status MATSimInterfaceService::Service::reqExternalConnect(::grpc::ServerContext* context, const ::hybrid::ExternalConnect* request, ::hybrid::ExternalConnectConfirmed* response) {
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void MATSimInterfaceService::AsyncService::RequestreqExternalConnect(::grpc::ServerContext* context, ::hybrid::ExternalConnect* request, ::grpc::ServerAsyncResponseWriter< ::hybrid::ExternalConnectConfirmed>* response, ::grpc::CompletionQueue* cq, void* tag) {
  AsynchronousService::RequestAsyncUnary(2, context, request, response, cq, tag);
}

::grpc::Status MATSimInterfaceService::Service::reqExternSimStepFinished(::grpc::ServerContext* context, const ::hybrid::ExternSimStepFinished* request, ::hybrid::ExternSimStepFinishedReceived* response) {
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void MATSimInterfaceService::AsyncService::RequestreqExternSimStepFinished(::grpc::ServerContext* context, ::hybrid::ExternSimStepFinished* request, ::grpc::ServerAsyncResponseWriter< ::hybrid::ExternSimStepFinishedReceived>* response, ::grpc::CompletionQueue* cq, void* tag) {
  AsynchronousService::RequestAsyncUnary(3, context, request, response, cq, tag);
}

::grpc::RpcService* MATSimInterfaceService::Service::service() {
  if (service_ != nullptr) {
    return service_;
  }
  service_ = new ::grpc::RpcService();
  service_->AddMethod(new ::grpc::RpcServiceMethod(
      MATSimInterfaceService_method_names[0],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< MATSimInterfaceService::Service, ::hybrid::Extern2MATSim, ::hybrid::Extern2MATSimConfirmed>(
          std::function< ::grpc::Status(MATSimInterfaceService::Service*, ::grpc::ServerContext*, const ::hybrid::Extern2MATSim*, ::hybrid::Extern2MATSimConfirmed*)>(&MATSimInterfaceService::Service::reqExtern2MATSim), this),
      new ::hybrid::Extern2MATSim, new ::hybrid::Extern2MATSimConfirmed));
  service_->AddMethod(new ::grpc::RpcServiceMethod(
      MATSimInterfaceService_method_names[1],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< MATSimInterfaceService::Service, ::hybrid::AgentsStuck, ::hybrid::AgentsStuckConfirmed>(
          std::function< ::grpc::Status(MATSimInterfaceService::Service*, ::grpc::ServerContext*, const ::hybrid::AgentsStuck*, ::hybrid::AgentsStuckConfirmed*)>(&MATSimInterfaceService::Service::reqAgentStuck), this),
      new ::hybrid::AgentsStuck, new ::hybrid::AgentsStuckConfirmed));
  service_->AddMethod(new ::grpc::RpcServiceMethod(
      MATSimInterfaceService_method_names[2],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< MATSimInterfaceService::Service, ::hybrid::ExternalConnect, ::hybrid::ExternalConnectConfirmed>(
          std::function< ::grpc::Status(MATSimInterfaceService::Service*, ::grpc::ServerContext*, const ::hybrid::ExternalConnect*, ::hybrid::ExternalConnectConfirmed*)>(&MATSimInterfaceService::Service::reqExternalConnect), this),
      new ::hybrid::ExternalConnect, new ::hybrid::ExternalConnectConfirmed));
  service_->AddMethod(new ::grpc::RpcServiceMethod(
      MATSimInterfaceService_method_names[3],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< MATSimInterfaceService::Service, ::hybrid::ExternSimStepFinished, ::hybrid::ExternSimStepFinishedReceived>(
          std::function< ::grpc::Status(MATSimInterfaceService::Service*, ::grpc::ServerContext*, const ::hybrid::ExternSimStepFinished*, ::hybrid::ExternSimStepFinishedReceived*)>(&MATSimInterfaceService::Service::reqExternSimStepFinished), this),
      new ::hybrid::ExternSimStepFinished, new ::hybrid::ExternSimStepFinishedReceived));
  return service_;
}


static const char* ExternInterfaceService_method_names[] = {
  "/hybrid.ExternInterfaceService/reqMATSim2ExternHasSpace",
  "/hybrid.ExternInterfaceService/reqMATSim2ExternPutAgent",
  "/hybrid.ExternInterfaceService/reqExternDoSimStep",
  "/hybrid.ExternInterfaceService/reqExternOnPrepareSim",
  "/hybrid.ExternInterfaceService/reqExternAfterSim",
};

std::unique_ptr< ExternInterfaceService::Stub> ExternInterfaceService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel) {
  std::unique_ptr< ExternInterfaceService::Stub> stub(new ExternInterfaceService::Stub(channel));
  return stub;
}

ExternInterfaceService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : ::grpc::InternalStub(channel), rpcmethod_reqMATSim2ExternHasSpace_(ExternInterfaceService_method_names[0], ::grpc::RpcMethod::NORMAL_RPC, channel->RegisterMethod(ExternInterfaceService_method_names[0]))
  , rpcmethod_reqMATSim2ExternPutAgent_(ExternInterfaceService_method_names[1], ::grpc::RpcMethod::NORMAL_RPC, channel->RegisterMethod(ExternInterfaceService_method_names[1]))
  , rpcmethod_reqExternDoSimStep_(ExternInterfaceService_method_names[2], ::grpc::RpcMethod::NORMAL_RPC, channel->RegisterMethod(ExternInterfaceService_method_names[2]))
  , rpcmethod_reqExternOnPrepareSim_(ExternInterfaceService_method_names[3], ::grpc::RpcMethod::NORMAL_RPC, channel->RegisterMethod(ExternInterfaceService_method_names[3]))
  , rpcmethod_reqExternAfterSim_(ExternInterfaceService_method_names[4], ::grpc::RpcMethod::NORMAL_RPC, channel->RegisterMethod(ExternInterfaceService_method_names[4]))
  {}

::grpc::Status ExternInterfaceService::Stub::reqMATSim2ExternHasSpace(::grpc::ClientContext* context, const ::hybrid::MATSim2ExternHasSpace& request, ::hybrid::MATSim2ExternHasSpaceConfirmed* response) {
  return ::grpc::BlockingUnaryCall(channel(), rpcmethod_reqMATSim2ExternHasSpace_, context, request, response);
}

std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::hybrid::MATSim2ExternHasSpaceConfirmed>> ExternInterfaceService::Stub::AsyncreqMATSim2ExternHasSpace(::grpc::ClientContext* context, const ::hybrid::MATSim2ExternHasSpace& request, ::grpc::CompletionQueue* cq, void* tag) {
  return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::hybrid::MATSim2ExternHasSpaceConfirmed>>(new ::grpc::ClientAsyncResponseReader< ::hybrid::MATSim2ExternHasSpaceConfirmed>(channel(), cq, rpcmethod_reqMATSim2ExternHasSpace_, context, request, tag));
}

::grpc::Status ExternInterfaceService::Stub::reqMATSim2ExternPutAgent(::grpc::ClientContext* context, const ::hybrid::MATSim2ExternPutAgent& request, ::hybrid::MATSim2ExternPutAgentConfirmed* response) {
  return ::grpc::BlockingUnaryCall(channel(), rpcmethod_reqMATSim2ExternPutAgent_, context, request, response);
}

std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::hybrid::MATSim2ExternPutAgentConfirmed>> ExternInterfaceService::Stub::AsyncreqMATSim2ExternPutAgent(::grpc::ClientContext* context, const ::hybrid::MATSim2ExternPutAgent& request, ::grpc::CompletionQueue* cq, void* tag) {
  return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::hybrid::MATSim2ExternPutAgentConfirmed>>(new ::grpc::ClientAsyncResponseReader< ::hybrid::MATSim2ExternPutAgentConfirmed>(channel(), cq, rpcmethod_reqMATSim2ExternPutAgent_, context, request, tag));
}

::grpc::Status ExternInterfaceService::Stub::reqExternDoSimStep(::grpc::ClientContext* context, const ::hybrid::ExternDoSimStep& request, ::hybrid::ExternDoSimStepReceived* response) {
  return ::grpc::BlockingUnaryCall(channel(), rpcmethod_reqExternDoSimStep_, context, request, response);
}

std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::hybrid::ExternDoSimStepReceived>> ExternInterfaceService::Stub::AsyncreqExternDoSimStep(::grpc::ClientContext* context, const ::hybrid::ExternDoSimStep& request, ::grpc::CompletionQueue* cq, void* tag) {
  return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::hybrid::ExternDoSimStepReceived>>(new ::grpc::ClientAsyncResponseReader< ::hybrid::ExternDoSimStepReceived>(channel(), cq, rpcmethod_reqExternDoSimStep_, context, request, tag));
}

::grpc::Status ExternInterfaceService::Stub::reqExternOnPrepareSim(::grpc::ClientContext* context, const ::hybrid::ExternOnPrepareSim& request, ::hybrid::ExternOnPrepareSimConfirmed* response) {
  return ::grpc::BlockingUnaryCall(channel(), rpcmethod_reqExternOnPrepareSim_, context, request, response);
}

std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::hybrid::ExternOnPrepareSimConfirmed>> ExternInterfaceService::Stub::AsyncreqExternOnPrepareSim(::grpc::ClientContext* context, const ::hybrid::ExternOnPrepareSim& request, ::grpc::CompletionQueue* cq, void* tag) {
  return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::hybrid::ExternOnPrepareSimConfirmed>>(new ::grpc::ClientAsyncResponseReader< ::hybrid::ExternOnPrepareSimConfirmed>(channel(), cq, rpcmethod_reqExternOnPrepareSim_, context, request, tag));
}

::grpc::Status ExternInterfaceService::Stub::reqExternAfterSim(::grpc::ClientContext* context, const ::hybrid::ExternAfterSim& request, ::hybrid::ExternAfterSimConfirmed* response) {
  return ::grpc::BlockingUnaryCall(channel(), rpcmethod_reqExternAfterSim_, context, request, response);
}

std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::hybrid::ExternAfterSimConfirmed>> ExternInterfaceService::Stub::AsyncreqExternAfterSim(::grpc::ClientContext* context, const ::hybrid::ExternAfterSim& request, ::grpc::CompletionQueue* cq, void* tag) {
  return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::hybrid::ExternAfterSimConfirmed>>(new ::grpc::ClientAsyncResponseReader< ::hybrid::ExternAfterSimConfirmed>(channel(), cq, rpcmethod_reqExternAfterSim_, context, request, tag));
}

ExternInterfaceService::AsyncService::AsyncService(::grpc::CompletionQueue* cq) : ::grpc::AsynchronousService(cq, ExternInterfaceService_method_names, 5) {}

ExternInterfaceService::Service::~Service() {
  delete service_;
}

::grpc::Status ExternInterfaceService::Service::reqMATSim2ExternHasSpace(::grpc::ServerContext* context, const ::hybrid::MATSim2ExternHasSpace* request, ::hybrid::MATSim2ExternHasSpaceConfirmed* response) {
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void ExternInterfaceService::AsyncService::RequestreqMATSim2ExternHasSpace(::grpc::ServerContext* context, ::hybrid::MATSim2ExternHasSpace* request, ::grpc::ServerAsyncResponseWriter< ::hybrid::MATSim2ExternHasSpaceConfirmed>* response, ::grpc::CompletionQueue* cq, void* tag) {
  AsynchronousService::RequestAsyncUnary(0, context, request, response, cq, tag);
}

::grpc::Status ExternInterfaceService::Service::reqMATSim2ExternPutAgent(::grpc::ServerContext* context, const ::hybrid::MATSim2ExternPutAgent* request, ::hybrid::MATSim2ExternPutAgentConfirmed* response) {
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void ExternInterfaceService::AsyncService::RequestreqMATSim2ExternPutAgent(::grpc::ServerContext* context, ::hybrid::MATSim2ExternPutAgent* request, ::grpc::ServerAsyncResponseWriter< ::hybrid::MATSim2ExternPutAgentConfirmed>* response, ::grpc::CompletionQueue* cq, void* tag) {
  AsynchronousService::RequestAsyncUnary(1, context, request, response, cq, tag);
}

::grpc::Status ExternInterfaceService::Service::reqExternDoSimStep(::grpc::ServerContext* context, const ::hybrid::ExternDoSimStep* request, ::hybrid::ExternDoSimStepReceived* response) {
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void ExternInterfaceService::AsyncService::RequestreqExternDoSimStep(::grpc::ServerContext* context, ::hybrid::ExternDoSimStep* request, ::grpc::ServerAsyncResponseWriter< ::hybrid::ExternDoSimStepReceived>* response, ::grpc::CompletionQueue* cq, void* tag) {
  AsynchronousService::RequestAsyncUnary(2, context, request, response, cq, tag);
}

::grpc::Status ExternInterfaceService::Service::reqExternOnPrepareSim(::grpc::ServerContext* context, const ::hybrid::ExternOnPrepareSim* request, ::hybrid::ExternOnPrepareSimConfirmed* response) {
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void ExternInterfaceService::AsyncService::RequestreqExternOnPrepareSim(::grpc::ServerContext* context, ::hybrid::ExternOnPrepareSim* request, ::grpc::ServerAsyncResponseWriter< ::hybrid::ExternOnPrepareSimConfirmed>* response, ::grpc::CompletionQueue* cq, void* tag) {
  AsynchronousService::RequestAsyncUnary(3, context, request, response, cq, tag);
}

::grpc::Status ExternInterfaceService::Service::reqExternAfterSim(::grpc::ServerContext* context, const ::hybrid::ExternAfterSim* request, ::hybrid::ExternAfterSimConfirmed* response) {
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void ExternInterfaceService::AsyncService::RequestreqExternAfterSim(::grpc::ServerContext* context, ::hybrid::ExternAfterSim* request, ::grpc::ServerAsyncResponseWriter< ::hybrid::ExternAfterSimConfirmed>* response, ::grpc::CompletionQueue* cq, void* tag) {
  AsynchronousService::RequestAsyncUnary(4, context, request, response, cq, tag);
}

::grpc::RpcService* ExternInterfaceService::Service::service() {
  if (service_ != nullptr) {
    return service_;
  }
  service_ = new ::grpc::RpcService();
  service_->AddMethod(new ::grpc::RpcServiceMethod(
      ExternInterfaceService_method_names[0],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< ExternInterfaceService::Service, ::hybrid::MATSim2ExternHasSpace, ::hybrid::MATSim2ExternHasSpaceConfirmed>(
          std::function< ::grpc::Status(ExternInterfaceService::Service*, ::grpc::ServerContext*, const ::hybrid::MATSim2ExternHasSpace*, ::hybrid::MATSim2ExternHasSpaceConfirmed*)>(&ExternInterfaceService::Service::reqMATSim2ExternHasSpace), this),
      new ::hybrid::MATSim2ExternHasSpace, new ::hybrid::MATSim2ExternHasSpaceConfirmed));
  service_->AddMethod(new ::grpc::RpcServiceMethod(
      ExternInterfaceService_method_names[1],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< ExternInterfaceService::Service, ::hybrid::MATSim2ExternPutAgent, ::hybrid::MATSim2ExternPutAgentConfirmed>(
          std::function< ::grpc::Status(ExternInterfaceService::Service*, ::grpc::ServerContext*, const ::hybrid::MATSim2ExternPutAgent*, ::hybrid::MATSim2ExternPutAgentConfirmed*)>(&ExternInterfaceService::Service::reqMATSim2ExternPutAgent), this),
      new ::hybrid::MATSim2ExternPutAgent, new ::hybrid::MATSim2ExternPutAgentConfirmed));
  service_->AddMethod(new ::grpc::RpcServiceMethod(
      ExternInterfaceService_method_names[2],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< ExternInterfaceService::Service, ::hybrid::ExternDoSimStep, ::hybrid::ExternDoSimStepReceived>(
          std::function< ::grpc::Status(ExternInterfaceService::Service*, ::grpc::ServerContext*, const ::hybrid::ExternDoSimStep*, ::hybrid::ExternDoSimStepReceived*)>(&ExternInterfaceService::Service::reqExternDoSimStep), this),
      new ::hybrid::ExternDoSimStep, new ::hybrid::ExternDoSimStepReceived));
  service_->AddMethod(new ::grpc::RpcServiceMethod(
      ExternInterfaceService_method_names[3],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< ExternInterfaceService::Service, ::hybrid::ExternOnPrepareSim, ::hybrid::ExternOnPrepareSimConfirmed>(
          std::function< ::grpc::Status(ExternInterfaceService::Service*, ::grpc::ServerContext*, const ::hybrid::ExternOnPrepareSim*, ::hybrid::ExternOnPrepareSimConfirmed*)>(&ExternInterfaceService::Service::reqExternOnPrepareSim), this),
      new ::hybrid::ExternOnPrepareSim, new ::hybrid::ExternOnPrepareSimConfirmed));
  service_->AddMethod(new ::grpc::RpcServiceMethod(
      ExternInterfaceService_method_names[4],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< ExternInterfaceService::Service, ::hybrid::ExternAfterSim, ::hybrid::ExternAfterSimConfirmed>(
          std::function< ::grpc::Status(ExternInterfaceService::Service*, ::grpc::ServerContext*, const ::hybrid::ExternAfterSim*, ::hybrid::ExternAfterSimConfirmed*)>(&ExternInterfaceService::Service::reqExternAfterSim), this),
      new ::hybrid::ExternAfterSim, new ::hybrid::ExternAfterSimConfirmed));
  return service_;
}


}  // namespace hybrid

