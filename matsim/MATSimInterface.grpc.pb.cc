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
namespace org {
namespace matsim {
namespace hybrid {

static const char* MATSimInterfaceService_method_names[] = {
  "/org.matsim.hybrid.MATSimInterfaceService/reqExternalConnect",
  "/org.matsim.hybrid.MATSimInterfaceService/reqExtern2MATSim",
  "/org.matsim.hybrid.MATSimInterfaceService/reqAgentStuck",
  "/org.matsim.hybrid.MATSimInterfaceService/reqExternSimStepFinished",
  "/org.matsim.hybrid.MATSimInterfaceService/reqMaximumNumberOfAgents",
  "/org.matsim.hybrid.MATSimInterfaceService/reqSendTrajectories",
};

std::unique_ptr< MATSimInterfaceService::Stub> MATSimInterfaceService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel) {
  std::unique_ptr< MATSimInterfaceService::Stub> stub(new MATSimInterfaceService::Stub(channel));
  return stub;
}

MATSimInterfaceService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : ::grpc::InternalStub(channel), rpcmethod_reqExternalConnect_(MATSimInterfaceService_method_names[0], ::grpc::RpcMethod::NORMAL_RPC, channel->RegisterMethod(MATSimInterfaceService_method_names[0]))
  , rpcmethod_reqExtern2MATSim_(MATSimInterfaceService_method_names[1], ::grpc::RpcMethod::NORMAL_RPC, channel->RegisterMethod(MATSimInterfaceService_method_names[1]))
  , rpcmethod_reqAgentStuck_(MATSimInterfaceService_method_names[2], ::grpc::RpcMethod::NORMAL_RPC, channel->RegisterMethod(MATSimInterfaceService_method_names[2]))
  , rpcmethod_reqExternSimStepFinished_(MATSimInterfaceService_method_names[3], ::grpc::RpcMethod::NORMAL_RPC, channel->RegisterMethod(MATSimInterfaceService_method_names[3]))
  , rpcmethod_reqMaximumNumberOfAgents_(MATSimInterfaceService_method_names[4], ::grpc::RpcMethod::NORMAL_RPC, channel->RegisterMethod(MATSimInterfaceService_method_names[4]))
  , rpcmethod_reqSendTrajectories_(MATSimInterfaceService_method_names[5], ::grpc::RpcMethod::NORMAL_RPC, channel->RegisterMethod(MATSimInterfaceService_method_names[5]))
  {}

::grpc::Status MATSimInterfaceService::Stub::reqExternalConnect(::grpc::ClientContext* context, const ::org::matsim::hybrid::ExternalConnect& request, ::org::matsim::hybrid::ExternalConnectConfirmed* response) {
  return ::grpc::BlockingUnaryCall(channel(), rpcmethod_reqExternalConnect_, context, request, response);
}

std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::org::matsim::hybrid::ExternalConnectConfirmed>> MATSimInterfaceService::Stub::AsyncreqExternalConnect(::grpc::ClientContext* context, const ::org::matsim::hybrid::ExternalConnect& request, ::grpc::CompletionQueue* cq, void* tag) {
  return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::org::matsim::hybrid::ExternalConnectConfirmed>>(new ::grpc::ClientAsyncResponseReader< ::org::matsim::hybrid::ExternalConnectConfirmed>(channel(), cq, rpcmethod_reqExternalConnect_, context, request, tag));
}

::grpc::Status MATSimInterfaceService::Stub::reqExtern2MATSim(::grpc::ClientContext* context, const ::org::matsim::hybrid::Extern2MATSim& request, ::org::matsim::hybrid::Extern2MATSimConfirmed* response) {
  return ::grpc::BlockingUnaryCall(channel(), rpcmethod_reqExtern2MATSim_, context, request, response);
}

std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::org::matsim::hybrid::Extern2MATSimConfirmed>> MATSimInterfaceService::Stub::AsyncreqExtern2MATSim(::grpc::ClientContext* context, const ::org::matsim::hybrid::Extern2MATSim& request, ::grpc::CompletionQueue* cq, void* tag) {
  return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::org::matsim::hybrid::Extern2MATSimConfirmed>>(new ::grpc::ClientAsyncResponseReader< ::org::matsim::hybrid::Extern2MATSimConfirmed>(channel(), cq, rpcmethod_reqExtern2MATSim_, context, request, tag));
}

::grpc::Status MATSimInterfaceService::Stub::reqAgentStuck(::grpc::ClientContext* context, const ::org::matsim::hybrid::AgentsStuck& request, ::org::matsim::hybrid::AgentsStuckConfirmed* response) {
  return ::grpc::BlockingUnaryCall(channel(), rpcmethod_reqAgentStuck_, context, request, response);
}

std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::org::matsim::hybrid::AgentsStuckConfirmed>> MATSimInterfaceService::Stub::AsyncreqAgentStuck(::grpc::ClientContext* context, const ::org::matsim::hybrid::AgentsStuck& request, ::grpc::CompletionQueue* cq, void* tag) {
  return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::org::matsim::hybrid::AgentsStuckConfirmed>>(new ::grpc::ClientAsyncResponseReader< ::org::matsim::hybrid::AgentsStuckConfirmed>(channel(), cq, rpcmethod_reqAgentStuck_, context, request, tag));
}

::grpc::Status MATSimInterfaceService::Stub::reqExternSimStepFinished(::grpc::ClientContext* context, const ::org::matsim::hybrid::ExternSimStepFinished& request, ::org::matsim::hybrid::ExternSimStepFinishedReceived* response) {
  return ::grpc::BlockingUnaryCall(channel(), rpcmethod_reqExternSimStepFinished_, context, request, response);
}

std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::org::matsim::hybrid::ExternSimStepFinishedReceived>> MATSimInterfaceService::Stub::AsyncreqExternSimStepFinished(::grpc::ClientContext* context, const ::org::matsim::hybrid::ExternSimStepFinished& request, ::grpc::CompletionQueue* cq, void* tag) {
  return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::org::matsim::hybrid::ExternSimStepFinishedReceived>>(new ::grpc::ClientAsyncResponseReader< ::org::matsim::hybrid::ExternSimStepFinishedReceived>(channel(), cq, rpcmethod_reqExternSimStepFinished_, context, request, tag));
}

::grpc::Status MATSimInterfaceService::Stub::reqMaximumNumberOfAgents(::grpc::ClientContext* context, const ::org::matsim::hybrid::MaximumNumberOfAgentsConfirmed& request, ::org::matsim::hybrid::MaximumNumberOfAgents* response) {
  return ::grpc::BlockingUnaryCall(channel(), rpcmethod_reqMaximumNumberOfAgents_, context, request, response);
}

std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::org::matsim::hybrid::MaximumNumberOfAgents>> MATSimInterfaceService::Stub::AsyncreqMaximumNumberOfAgents(::grpc::ClientContext* context, const ::org::matsim::hybrid::MaximumNumberOfAgentsConfirmed& request, ::grpc::CompletionQueue* cq, void* tag) {
  return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::org::matsim::hybrid::MaximumNumberOfAgents>>(new ::grpc::ClientAsyncResponseReader< ::org::matsim::hybrid::MaximumNumberOfAgents>(channel(), cq, rpcmethod_reqMaximumNumberOfAgents_, context, request, tag));
}

::grpc::Status MATSimInterfaceService::Stub::reqSendTrajectories(::grpc::ClientContext* context, const ::org::matsim::hybrid::Extern2MATSimTrajectories& request, ::org::matsim::hybrid::MATSim2ExternTrajectoriesReceived* response) {
  return ::grpc::BlockingUnaryCall(channel(), rpcmethod_reqSendTrajectories_, context, request, response);
}

std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::org::matsim::hybrid::MATSim2ExternTrajectoriesReceived>> MATSimInterfaceService::Stub::AsyncreqSendTrajectories(::grpc::ClientContext* context, const ::org::matsim::hybrid::Extern2MATSimTrajectories& request, ::grpc::CompletionQueue* cq, void* tag) {
  return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::org::matsim::hybrid::MATSim2ExternTrajectoriesReceived>>(new ::grpc::ClientAsyncResponseReader< ::org::matsim::hybrid::MATSim2ExternTrajectoriesReceived>(channel(), cq, rpcmethod_reqSendTrajectories_, context, request, tag));
}

MATSimInterfaceService::AsyncService::AsyncService(::grpc::CompletionQueue* cq) : ::grpc::AsynchronousService(cq, MATSimInterfaceService_method_names, 6) {}

MATSimInterfaceService::Service::~Service() {
  delete service_;
}

::grpc::Status MATSimInterfaceService::Service::reqExternalConnect(::grpc::ServerContext* context, const ::org::matsim::hybrid::ExternalConnect* request, ::org::matsim::hybrid::ExternalConnectConfirmed* response) {
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void MATSimInterfaceService::AsyncService::RequestreqExternalConnect(::grpc::ServerContext* context, ::org::matsim::hybrid::ExternalConnect* request, ::grpc::ServerAsyncResponseWriter< ::org::matsim::hybrid::ExternalConnectConfirmed>* response, ::grpc::CompletionQueue* cq, void* tag) {
  AsynchronousService::RequestAsyncUnary(0, context, request, response, cq, tag);
}

::grpc::Status MATSimInterfaceService::Service::reqExtern2MATSim(::grpc::ServerContext* context, const ::org::matsim::hybrid::Extern2MATSim* request, ::org::matsim::hybrid::Extern2MATSimConfirmed* response) {
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void MATSimInterfaceService::AsyncService::RequestreqExtern2MATSim(::grpc::ServerContext* context, ::org::matsim::hybrid::Extern2MATSim* request, ::grpc::ServerAsyncResponseWriter< ::org::matsim::hybrid::Extern2MATSimConfirmed>* response, ::grpc::CompletionQueue* cq, void* tag) {
  AsynchronousService::RequestAsyncUnary(1, context, request, response, cq, tag);
}

::grpc::Status MATSimInterfaceService::Service::reqAgentStuck(::grpc::ServerContext* context, const ::org::matsim::hybrid::AgentsStuck* request, ::org::matsim::hybrid::AgentsStuckConfirmed* response) {
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void MATSimInterfaceService::AsyncService::RequestreqAgentStuck(::grpc::ServerContext* context, ::org::matsim::hybrid::AgentsStuck* request, ::grpc::ServerAsyncResponseWriter< ::org::matsim::hybrid::AgentsStuckConfirmed>* response, ::grpc::CompletionQueue* cq, void* tag) {
  AsynchronousService::RequestAsyncUnary(2, context, request, response, cq, tag);
}

::grpc::Status MATSimInterfaceService::Service::reqExternSimStepFinished(::grpc::ServerContext* context, const ::org::matsim::hybrid::ExternSimStepFinished* request, ::org::matsim::hybrid::ExternSimStepFinishedReceived* response) {
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void MATSimInterfaceService::AsyncService::RequestreqExternSimStepFinished(::grpc::ServerContext* context, ::org::matsim::hybrid::ExternSimStepFinished* request, ::grpc::ServerAsyncResponseWriter< ::org::matsim::hybrid::ExternSimStepFinishedReceived>* response, ::grpc::CompletionQueue* cq, void* tag) {
  AsynchronousService::RequestAsyncUnary(3, context, request, response, cq, tag);
}

::grpc::Status MATSimInterfaceService::Service::reqMaximumNumberOfAgents(::grpc::ServerContext* context, const ::org::matsim::hybrid::MaximumNumberOfAgentsConfirmed* request, ::org::matsim::hybrid::MaximumNumberOfAgents* response) {
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void MATSimInterfaceService::AsyncService::RequestreqMaximumNumberOfAgents(::grpc::ServerContext* context, ::org::matsim::hybrid::MaximumNumberOfAgentsConfirmed* request, ::grpc::ServerAsyncResponseWriter< ::org::matsim::hybrid::MaximumNumberOfAgents>* response, ::grpc::CompletionQueue* cq, void* tag) {
  AsynchronousService::RequestAsyncUnary(4, context, request, response, cq, tag);
}

::grpc::Status MATSimInterfaceService::Service::reqSendTrajectories(::grpc::ServerContext* context, const ::org::matsim::hybrid::Extern2MATSimTrajectories* request, ::org::matsim::hybrid::MATSim2ExternTrajectoriesReceived* response) {
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void MATSimInterfaceService::AsyncService::RequestreqSendTrajectories(::grpc::ServerContext* context, ::org::matsim::hybrid::Extern2MATSimTrajectories* request, ::grpc::ServerAsyncResponseWriter< ::org::matsim::hybrid::MATSim2ExternTrajectoriesReceived>* response, ::grpc::CompletionQueue* cq, void* tag) {
  AsynchronousService::RequestAsyncUnary(5, context, request, response, cq, tag);
}

::grpc::RpcService* MATSimInterfaceService::Service::service() {
  if (service_ != nullptr) {
    return service_;
  }
  service_ = new ::grpc::RpcService();
  service_->AddMethod(new ::grpc::RpcServiceMethod(
      MATSimInterfaceService_method_names[0],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< MATSimInterfaceService::Service, ::org::matsim::hybrid::ExternalConnect, ::org::matsim::hybrid::ExternalConnectConfirmed>(
          std::mem_fn(&MATSimInterfaceService::Service::reqExternalConnect), this),
      new ::org::matsim::hybrid::ExternalConnect, new ::org::matsim::hybrid::ExternalConnectConfirmed));
  service_->AddMethod(new ::grpc::RpcServiceMethod(
      MATSimInterfaceService_method_names[1],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< MATSimInterfaceService::Service, ::org::matsim::hybrid::Extern2MATSim, ::org::matsim::hybrid::Extern2MATSimConfirmed>(
          std::mem_fn(&MATSimInterfaceService::Service::reqExtern2MATSim), this),
      new ::org::matsim::hybrid::Extern2MATSim, new ::org::matsim::hybrid::Extern2MATSimConfirmed));
  service_->AddMethod(new ::grpc::RpcServiceMethod(
      MATSimInterfaceService_method_names[2],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< MATSimInterfaceService::Service, ::org::matsim::hybrid::AgentsStuck, ::org::matsim::hybrid::AgentsStuckConfirmed>(
          std::mem_fn(&MATSimInterfaceService::Service::reqAgentStuck), this),
      new ::org::matsim::hybrid::AgentsStuck, new ::org::matsim::hybrid::AgentsStuckConfirmed));
  service_->AddMethod(new ::grpc::RpcServiceMethod(
      MATSimInterfaceService_method_names[3],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< MATSimInterfaceService::Service, ::org::matsim::hybrid::ExternSimStepFinished, ::org::matsim::hybrid::ExternSimStepFinishedReceived>(
          std::mem_fn(&MATSimInterfaceService::Service::reqExternSimStepFinished), this),
      new ::org::matsim::hybrid::ExternSimStepFinished, new ::org::matsim::hybrid::ExternSimStepFinishedReceived));
  service_->AddMethod(new ::grpc::RpcServiceMethod(
      MATSimInterfaceService_method_names[4],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< MATSimInterfaceService::Service, ::org::matsim::hybrid::MaximumNumberOfAgentsConfirmed, ::org::matsim::hybrid::MaximumNumberOfAgents>(
          std::mem_fn(&MATSimInterfaceService::Service::reqMaximumNumberOfAgents), this),
      new ::org::matsim::hybrid::MaximumNumberOfAgentsConfirmed, new ::org::matsim::hybrid::MaximumNumberOfAgents));
  service_->AddMethod(new ::grpc::RpcServiceMethod(
      MATSimInterfaceService_method_names[5],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< MATSimInterfaceService::Service, ::org::matsim::hybrid::Extern2MATSimTrajectories, ::org::matsim::hybrid::MATSim2ExternTrajectoriesReceived>(
          std::mem_fn(&MATSimInterfaceService::Service::reqSendTrajectories), this),
      new ::org::matsim::hybrid::Extern2MATSimTrajectories, new ::org::matsim::hybrid::MATSim2ExternTrajectoriesReceived));
  return service_;
}


static const char* ExternInterfaceService_method_names[] = {
  "/org.matsim.hybrid.ExternInterfaceService/reqMATSim2ExternHasSpace",
  "/org.matsim.hybrid.ExternInterfaceService/reqMATSim2ExternPutAgent",
  "/org.matsim.hybrid.ExternInterfaceService/reqExternDoSimStep",
  "/org.matsim.hybrid.ExternInterfaceService/reqExternOnPrepareSim",
  "/org.matsim.hybrid.ExternInterfaceService/reqExternAfterSim",
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

::grpc::Status ExternInterfaceService::Stub::reqMATSim2ExternHasSpace(::grpc::ClientContext* context, const ::org::matsim::hybrid::MATSim2ExternHasSpace& request, ::org::matsim::hybrid::MATSim2ExternHasSpaceConfirmed* response) {
  return ::grpc::BlockingUnaryCall(channel(), rpcmethod_reqMATSim2ExternHasSpace_, context, request, response);
}

std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::org::matsim::hybrid::MATSim2ExternHasSpaceConfirmed>> ExternInterfaceService::Stub::AsyncreqMATSim2ExternHasSpace(::grpc::ClientContext* context, const ::org::matsim::hybrid::MATSim2ExternHasSpace& request, ::grpc::CompletionQueue* cq, void* tag) {
  return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::org::matsim::hybrid::MATSim2ExternHasSpaceConfirmed>>(new ::grpc::ClientAsyncResponseReader< ::org::matsim::hybrid::MATSim2ExternHasSpaceConfirmed>(channel(), cq, rpcmethod_reqMATSim2ExternHasSpace_, context, request, tag));
}

::grpc::Status ExternInterfaceService::Stub::reqMATSim2ExternPutAgent(::grpc::ClientContext* context, const ::org::matsim::hybrid::MATSim2ExternPutAgent& request, ::org::matsim::hybrid::MATSim2ExternPutAgentConfirmed* response) {
  return ::grpc::BlockingUnaryCall(channel(), rpcmethod_reqMATSim2ExternPutAgent_, context, request, response);
}

std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::org::matsim::hybrid::MATSim2ExternPutAgentConfirmed>> ExternInterfaceService::Stub::AsyncreqMATSim2ExternPutAgent(::grpc::ClientContext* context, const ::org::matsim::hybrid::MATSim2ExternPutAgent& request, ::grpc::CompletionQueue* cq, void* tag) {
  return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::org::matsim::hybrid::MATSim2ExternPutAgentConfirmed>>(new ::grpc::ClientAsyncResponseReader< ::org::matsim::hybrid::MATSim2ExternPutAgentConfirmed>(channel(), cq, rpcmethod_reqMATSim2ExternPutAgent_, context, request, tag));
}

::grpc::Status ExternInterfaceService::Stub::reqExternDoSimStep(::grpc::ClientContext* context, const ::org::matsim::hybrid::ExternDoSimStep& request, ::org::matsim::hybrid::ExternDoSimStepReceived* response) {
  return ::grpc::BlockingUnaryCall(channel(), rpcmethod_reqExternDoSimStep_, context, request, response);
}

std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::org::matsim::hybrid::ExternDoSimStepReceived>> ExternInterfaceService::Stub::AsyncreqExternDoSimStep(::grpc::ClientContext* context, const ::org::matsim::hybrid::ExternDoSimStep& request, ::grpc::CompletionQueue* cq, void* tag) {
  return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::org::matsim::hybrid::ExternDoSimStepReceived>>(new ::grpc::ClientAsyncResponseReader< ::org::matsim::hybrid::ExternDoSimStepReceived>(channel(), cq, rpcmethod_reqExternDoSimStep_, context, request, tag));
}

::grpc::Status ExternInterfaceService::Stub::reqExternOnPrepareSim(::grpc::ClientContext* context, const ::org::matsim::hybrid::ExternOnPrepareSim& request, ::org::matsim::hybrid::ExternOnPrepareSimConfirmed* response) {
  return ::grpc::BlockingUnaryCall(channel(), rpcmethod_reqExternOnPrepareSim_, context, request, response);
}

std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::org::matsim::hybrid::ExternOnPrepareSimConfirmed>> ExternInterfaceService::Stub::AsyncreqExternOnPrepareSim(::grpc::ClientContext* context, const ::org::matsim::hybrid::ExternOnPrepareSim& request, ::grpc::CompletionQueue* cq, void* tag) {
  return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::org::matsim::hybrid::ExternOnPrepareSimConfirmed>>(new ::grpc::ClientAsyncResponseReader< ::org::matsim::hybrid::ExternOnPrepareSimConfirmed>(channel(), cq, rpcmethod_reqExternOnPrepareSim_, context, request, tag));
}

::grpc::Status ExternInterfaceService::Stub::reqExternAfterSim(::grpc::ClientContext* context, const ::org::matsim::hybrid::ExternAfterSim& request, ::org::matsim::hybrid::ExternAfterSimConfirmed* response) {
  return ::grpc::BlockingUnaryCall(channel(), rpcmethod_reqExternAfterSim_, context, request, response);
}

std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::org::matsim::hybrid::ExternAfterSimConfirmed>> ExternInterfaceService::Stub::AsyncreqExternAfterSim(::grpc::ClientContext* context, const ::org::matsim::hybrid::ExternAfterSim& request, ::grpc::CompletionQueue* cq, void* tag) {
  return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::org::matsim::hybrid::ExternAfterSimConfirmed>>(new ::grpc::ClientAsyncResponseReader< ::org::matsim::hybrid::ExternAfterSimConfirmed>(channel(), cq, rpcmethod_reqExternAfterSim_, context, request, tag));
}

ExternInterfaceService::AsyncService::AsyncService(::grpc::CompletionQueue* cq) : ::grpc::AsynchronousService(cq, ExternInterfaceService_method_names, 5) {}

ExternInterfaceService::Service::~Service() {
  delete service_;
}

::grpc::Status ExternInterfaceService::Service::reqMATSim2ExternHasSpace(::grpc::ServerContext* context, const ::org::matsim::hybrid::MATSim2ExternHasSpace* request, ::org::matsim::hybrid::MATSim2ExternHasSpaceConfirmed* response) {
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void ExternInterfaceService::AsyncService::RequestreqMATSim2ExternHasSpace(::grpc::ServerContext* context, ::org::matsim::hybrid::MATSim2ExternHasSpace* request, ::grpc::ServerAsyncResponseWriter< ::org::matsim::hybrid::MATSim2ExternHasSpaceConfirmed>* response, ::grpc::CompletionQueue* cq, void* tag) {
  AsynchronousService::RequestAsyncUnary(0, context, request, response, cq, tag);
}

::grpc::Status ExternInterfaceService::Service::reqMATSim2ExternPutAgent(::grpc::ServerContext* context, const ::org::matsim::hybrid::MATSim2ExternPutAgent* request, ::org::matsim::hybrid::MATSim2ExternPutAgentConfirmed* response) {
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void ExternInterfaceService::AsyncService::RequestreqMATSim2ExternPutAgent(::grpc::ServerContext* context, ::org::matsim::hybrid::MATSim2ExternPutAgent* request, ::grpc::ServerAsyncResponseWriter< ::org::matsim::hybrid::MATSim2ExternPutAgentConfirmed>* response, ::grpc::CompletionQueue* cq, void* tag) {
  AsynchronousService::RequestAsyncUnary(1, context, request, response, cq, tag);
}

::grpc::Status ExternInterfaceService::Service::reqExternDoSimStep(::grpc::ServerContext* context, const ::org::matsim::hybrid::ExternDoSimStep* request, ::org::matsim::hybrid::ExternDoSimStepReceived* response) {
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void ExternInterfaceService::AsyncService::RequestreqExternDoSimStep(::grpc::ServerContext* context, ::org::matsim::hybrid::ExternDoSimStep* request, ::grpc::ServerAsyncResponseWriter< ::org::matsim::hybrid::ExternDoSimStepReceived>* response, ::grpc::CompletionQueue* cq, void* tag) {
  AsynchronousService::RequestAsyncUnary(2, context, request, response, cq, tag);
}

::grpc::Status ExternInterfaceService::Service::reqExternOnPrepareSim(::grpc::ServerContext* context, const ::org::matsim::hybrid::ExternOnPrepareSim* request, ::org::matsim::hybrid::ExternOnPrepareSimConfirmed* response) {
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void ExternInterfaceService::AsyncService::RequestreqExternOnPrepareSim(::grpc::ServerContext* context, ::org::matsim::hybrid::ExternOnPrepareSim* request, ::grpc::ServerAsyncResponseWriter< ::org::matsim::hybrid::ExternOnPrepareSimConfirmed>* response, ::grpc::CompletionQueue* cq, void* tag) {
  AsynchronousService::RequestAsyncUnary(3, context, request, response, cq, tag);
}

::grpc::Status ExternInterfaceService::Service::reqExternAfterSim(::grpc::ServerContext* context, const ::org::matsim::hybrid::ExternAfterSim* request, ::org::matsim::hybrid::ExternAfterSimConfirmed* response) {
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void ExternInterfaceService::AsyncService::RequestreqExternAfterSim(::grpc::ServerContext* context, ::org::matsim::hybrid::ExternAfterSim* request, ::grpc::ServerAsyncResponseWriter< ::org::matsim::hybrid::ExternAfterSimConfirmed>* response, ::grpc::CompletionQueue* cq, void* tag) {
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
      new ::grpc::RpcMethodHandler< ExternInterfaceService::Service, ::org::matsim::hybrid::MATSim2ExternHasSpace, ::org::matsim::hybrid::MATSim2ExternHasSpaceConfirmed>(
          std::mem_fn(&ExternInterfaceService::Service::reqMATSim2ExternHasSpace), this),
      new ::org::matsim::hybrid::MATSim2ExternHasSpace, new ::org::matsim::hybrid::MATSim2ExternHasSpaceConfirmed));
  service_->AddMethod(new ::grpc::RpcServiceMethod(
      ExternInterfaceService_method_names[1],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< ExternInterfaceService::Service, ::org::matsim::hybrid::MATSim2ExternPutAgent, ::org::matsim::hybrid::MATSim2ExternPutAgentConfirmed>(
          std::mem_fn(&ExternInterfaceService::Service::reqMATSim2ExternPutAgent), this),
      new ::org::matsim::hybrid::MATSim2ExternPutAgent, new ::org::matsim::hybrid::MATSim2ExternPutAgentConfirmed));
  service_->AddMethod(new ::grpc::RpcServiceMethod(
      ExternInterfaceService_method_names[2],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< ExternInterfaceService::Service, ::org::matsim::hybrid::ExternDoSimStep, ::org::matsim::hybrid::ExternDoSimStepReceived>(
          std::mem_fn(&ExternInterfaceService::Service::reqExternDoSimStep), this),
      new ::org::matsim::hybrid::ExternDoSimStep, new ::org::matsim::hybrid::ExternDoSimStepReceived));
  service_->AddMethod(new ::grpc::RpcServiceMethod(
      ExternInterfaceService_method_names[3],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< ExternInterfaceService::Service, ::org::matsim::hybrid::ExternOnPrepareSim, ::org::matsim::hybrid::ExternOnPrepareSimConfirmed>(
          std::mem_fn(&ExternInterfaceService::Service::reqExternOnPrepareSim), this),
      new ::org::matsim::hybrid::ExternOnPrepareSim, new ::org::matsim::hybrid::ExternOnPrepareSimConfirmed));
  service_->AddMethod(new ::grpc::RpcServiceMethod(
      ExternInterfaceService_method_names[4],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< ExternInterfaceService::Service, ::org::matsim::hybrid::ExternAfterSim, ::org::matsim::hybrid::ExternAfterSimConfirmed>(
          std::mem_fn(&ExternInterfaceService::Service::reqExternAfterSim), this),
      new ::org::matsim::hybrid::ExternAfterSim, new ::org::matsim::hybrid::ExternAfterSimConfirmed));
  return service_;
}


}  // namespace org
}  // namespace matsim
}  // namespace hybrid

