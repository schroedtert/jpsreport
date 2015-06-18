// Generated by the gRPC protobuf plugin.
// If you make any local change, they will be lost.
// source: MATSimInterface.proto
#ifndef GRPC_MATSimInterface_2eproto__INCLUDED
#define GRPC_MATSimInterface_2eproto__INCLUDED

#include "MATSimInterface.pb.h"

#include <grpc++/impl/internal_stub.h>
#include <grpc++/impl/rpc_method.h>
#include <grpc++/impl/service_type.h>
#include <grpc++/status.h>

namespace grpc {
class CompletionQueue;
class ChannelInterface;
class RpcService;
class ServerContext;
template <class OutMessage> class ClientAsyncResponseReader;
template <class OutMessage> class ServerAsyncResponseWriter;
}  // namespace grpc

namespace org {
namespace matsim {
namespace hybrid {

class MATSimInterfaceService GRPC_FINAL {
 public:
  class Stub GRPC_FINAL : public ::grpc::InternalStub {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel);
    ::grpc::Status reqExternalConnect(::grpc::ClientContext* context, const ::org::matsim::hybrid::ExternalConnect& request, ::org::matsim::hybrid::ExternalConnectConfirmed* response);
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::org::matsim::hybrid::ExternalConnectConfirmed>> AsyncreqExternalConnect(::grpc::ClientContext* context, const ::org::matsim::hybrid::ExternalConnect& request, ::grpc::CompletionQueue* cq, void* tag);
    ::grpc::Status reqExtern2MATSim(::grpc::ClientContext* context, const ::org::matsim::hybrid::Extern2MATSim& request, ::org::matsim::hybrid::Extern2MATSimConfirmed* response);
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::org::matsim::hybrid::Extern2MATSimConfirmed>> AsyncreqExtern2MATSim(::grpc::ClientContext* context, const ::org::matsim::hybrid::Extern2MATSim& request, ::grpc::CompletionQueue* cq, void* tag);
    ::grpc::Status reqAgentStuck(::grpc::ClientContext* context, const ::org::matsim::hybrid::AgentsStuck& request, ::org::matsim::hybrid::AgentsStuckConfirmed* response);
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::org::matsim::hybrid::AgentsStuckConfirmed>> AsyncreqAgentStuck(::grpc::ClientContext* context, const ::org::matsim::hybrid::AgentsStuck& request, ::grpc::CompletionQueue* cq, void* tag);
    ::grpc::Status reqExternSimStepFinished(::grpc::ClientContext* context, const ::org::matsim::hybrid::ExternSimStepFinished& request, ::org::matsim::hybrid::ExternSimStepFinishedReceived* response);
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::org::matsim::hybrid::ExternSimStepFinishedReceived>> AsyncreqExternSimStepFinished(::grpc::ClientContext* context, const ::org::matsim::hybrid::ExternSimStepFinished& request, ::grpc::CompletionQueue* cq, void* tag);
    ::grpc::Status reqMaximumNumberOfAgents(::grpc::ClientContext* context, const ::org::matsim::hybrid::MaximumNumberOfAgentsConfirmed& request, ::org::matsim::hybrid::MaximumNumberOfAgents* response);
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::org::matsim::hybrid::MaximumNumberOfAgents>> AsyncreqMaximumNumberOfAgents(::grpc::ClientContext* context, const ::org::matsim::hybrid::MaximumNumberOfAgentsConfirmed& request, ::grpc::CompletionQueue* cq, void* tag);
    ::grpc::Status reqSendTrajectories(::grpc::ClientContext* context, const ::org::matsim::hybrid::Extern2MATSimTrajectories& request, ::org::matsim::hybrid::MATSim2ExternTrajectoriesReceived* response);
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::org::matsim::hybrid::MATSim2ExternTrajectoriesReceived>> AsyncreqSendTrajectories(::grpc::ClientContext* context, const ::org::matsim::hybrid::Extern2MATSimTrajectories& request, ::grpc::CompletionQueue* cq, void* tag);
   private:
    const ::grpc::RpcMethod rpcmethod_reqExternalConnect_;
    const ::grpc::RpcMethod rpcmethod_reqExtern2MATSim_;
    const ::grpc::RpcMethod rpcmethod_reqAgentStuck_;
    const ::grpc::RpcMethod rpcmethod_reqExternSimStepFinished_;
    const ::grpc::RpcMethod rpcmethod_reqMaximumNumberOfAgents_;
    const ::grpc::RpcMethod rpcmethod_reqSendTrajectories_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel);

  class Service : public ::grpc::SynchronousService {
   public:
    Service() : service_(nullptr) {}
    virtual ~Service();
    virtual ::grpc::Status reqExternalConnect(::grpc::ServerContext* context, const ::org::matsim::hybrid::ExternalConnect* request, ::org::matsim::hybrid::ExternalConnectConfirmed* response);
    virtual ::grpc::Status reqExtern2MATSim(::grpc::ServerContext* context, const ::org::matsim::hybrid::Extern2MATSim* request, ::org::matsim::hybrid::Extern2MATSimConfirmed* response);
    virtual ::grpc::Status reqAgentStuck(::grpc::ServerContext* context, const ::org::matsim::hybrid::AgentsStuck* request, ::org::matsim::hybrid::AgentsStuckConfirmed* response);
    virtual ::grpc::Status reqExternSimStepFinished(::grpc::ServerContext* context, const ::org::matsim::hybrid::ExternSimStepFinished* request, ::org::matsim::hybrid::ExternSimStepFinishedReceived* response);
    virtual ::grpc::Status reqMaximumNumberOfAgents(::grpc::ServerContext* context, const ::org::matsim::hybrid::MaximumNumberOfAgentsConfirmed* request, ::org::matsim::hybrid::MaximumNumberOfAgents* response);
    virtual ::grpc::Status reqSendTrajectories(::grpc::ServerContext* context, const ::org::matsim::hybrid::Extern2MATSimTrajectories* request, ::org::matsim::hybrid::MATSim2ExternTrajectoriesReceived* response);
    ::grpc::RpcService* service() GRPC_OVERRIDE GRPC_FINAL;
   private:
    ::grpc::RpcService* service_;
  };
  class AsyncService GRPC_FINAL : public ::grpc::AsynchronousService {
   public:
    explicit AsyncService(::grpc::CompletionQueue* cq);
    ~AsyncService() {};
    void RequestreqExternalConnect(::grpc::ServerContext* context, ::org::matsim::hybrid::ExternalConnect* request, ::grpc::ServerAsyncResponseWriter< ::org::matsim::hybrid::ExternalConnectConfirmed>* response, ::grpc::CompletionQueue* cq, void *tag);
    void RequestreqExtern2MATSim(::grpc::ServerContext* context, ::org::matsim::hybrid::Extern2MATSim* request, ::grpc::ServerAsyncResponseWriter< ::org::matsim::hybrid::Extern2MATSimConfirmed>* response, ::grpc::CompletionQueue* cq, void *tag);
    void RequestreqAgentStuck(::grpc::ServerContext* context, ::org::matsim::hybrid::AgentsStuck* request, ::grpc::ServerAsyncResponseWriter< ::org::matsim::hybrid::AgentsStuckConfirmed>* response, ::grpc::CompletionQueue* cq, void *tag);
    void RequestreqExternSimStepFinished(::grpc::ServerContext* context, ::org::matsim::hybrid::ExternSimStepFinished* request, ::grpc::ServerAsyncResponseWriter< ::org::matsim::hybrid::ExternSimStepFinishedReceived>* response, ::grpc::CompletionQueue* cq, void *tag);
    void RequestreqMaximumNumberOfAgents(::grpc::ServerContext* context, ::org::matsim::hybrid::MaximumNumberOfAgentsConfirmed* request, ::grpc::ServerAsyncResponseWriter< ::org::matsim::hybrid::MaximumNumberOfAgents>* response, ::grpc::CompletionQueue* cq, void *tag);
    void RequestreqSendTrajectories(::grpc::ServerContext* context, ::org::matsim::hybrid::Extern2MATSimTrajectories* request, ::grpc::ServerAsyncResponseWriter< ::org::matsim::hybrid::MATSim2ExternTrajectoriesReceived>* response, ::grpc::CompletionQueue* cq, void *tag);
  };
};

class ExternInterfaceService GRPC_FINAL {
 public:
  class Stub GRPC_FINAL : public ::grpc::InternalStub {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel);
    ::grpc::Status reqMATSim2ExternHasSpace(::grpc::ClientContext* context, const ::org::matsim::hybrid::MATSim2ExternHasSpace& request, ::org::matsim::hybrid::MATSim2ExternHasSpaceConfirmed* response);
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::org::matsim::hybrid::MATSim2ExternHasSpaceConfirmed>> AsyncreqMATSim2ExternHasSpace(::grpc::ClientContext* context, const ::org::matsim::hybrid::MATSim2ExternHasSpace& request, ::grpc::CompletionQueue* cq, void* tag);
    ::grpc::Status reqMATSim2ExternPutAgent(::grpc::ClientContext* context, const ::org::matsim::hybrid::MATSim2ExternPutAgent& request, ::org::matsim::hybrid::MATSim2ExternPutAgentConfirmed* response);
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::org::matsim::hybrid::MATSim2ExternPutAgentConfirmed>> AsyncreqMATSim2ExternPutAgent(::grpc::ClientContext* context, const ::org::matsim::hybrid::MATSim2ExternPutAgent& request, ::grpc::CompletionQueue* cq, void* tag);
    ::grpc::Status reqExternDoSimStep(::grpc::ClientContext* context, const ::org::matsim::hybrid::ExternDoSimStep& request, ::org::matsim::hybrid::ExternDoSimStepReceived* response);
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::org::matsim::hybrid::ExternDoSimStepReceived>> AsyncreqExternDoSimStep(::grpc::ClientContext* context, const ::org::matsim::hybrid::ExternDoSimStep& request, ::grpc::CompletionQueue* cq, void* tag);
    ::grpc::Status reqExternOnPrepareSim(::grpc::ClientContext* context, const ::org::matsim::hybrid::ExternOnPrepareSim& request, ::org::matsim::hybrid::ExternOnPrepareSimConfirmed* response);
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::org::matsim::hybrid::ExternOnPrepareSimConfirmed>> AsyncreqExternOnPrepareSim(::grpc::ClientContext* context, const ::org::matsim::hybrid::ExternOnPrepareSim& request, ::grpc::CompletionQueue* cq, void* tag);
    ::grpc::Status reqExternAfterSim(::grpc::ClientContext* context, const ::org::matsim::hybrid::ExternAfterSim& request, ::org::matsim::hybrid::ExternAfterSimConfirmed* response);
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::org::matsim::hybrid::ExternAfterSimConfirmed>> AsyncreqExternAfterSim(::grpc::ClientContext* context, const ::org::matsim::hybrid::ExternAfterSim& request, ::grpc::CompletionQueue* cq, void* tag);
   private:
    const ::grpc::RpcMethod rpcmethod_reqMATSim2ExternHasSpace_;
    const ::grpc::RpcMethod rpcmethod_reqMATSim2ExternPutAgent_;
    const ::grpc::RpcMethod rpcmethod_reqExternDoSimStep_;
    const ::grpc::RpcMethod rpcmethod_reqExternOnPrepareSim_;
    const ::grpc::RpcMethod rpcmethod_reqExternAfterSim_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel);

  class Service : public ::grpc::SynchronousService {
   public:
    Service() : service_(nullptr) {}
    virtual ~Service();
    virtual ::grpc::Status reqMATSim2ExternHasSpace(::grpc::ServerContext* context, const ::org::matsim::hybrid::MATSim2ExternHasSpace* request, ::org::matsim::hybrid::MATSim2ExternHasSpaceConfirmed* response);
    virtual ::grpc::Status reqMATSim2ExternPutAgent(::grpc::ServerContext* context, const ::org::matsim::hybrid::MATSim2ExternPutAgent* request, ::org::matsim::hybrid::MATSim2ExternPutAgentConfirmed* response);
    virtual ::grpc::Status reqExternDoSimStep(::grpc::ServerContext* context, const ::org::matsim::hybrid::ExternDoSimStep* request, ::org::matsim::hybrid::ExternDoSimStepReceived* response);
    virtual ::grpc::Status reqExternOnPrepareSim(::grpc::ServerContext* context, const ::org::matsim::hybrid::ExternOnPrepareSim* request, ::org::matsim::hybrid::ExternOnPrepareSimConfirmed* response);
    virtual ::grpc::Status reqExternAfterSim(::grpc::ServerContext* context, const ::org::matsim::hybrid::ExternAfterSim* request, ::org::matsim::hybrid::ExternAfterSimConfirmed* response);
    ::grpc::RpcService* service() GRPC_OVERRIDE GRPC_FINAL;
   private:
    ::grpc::RpcService* service_;
  };
  class AsyncService GRPC_FINAL : public ::grpc::AsynchronousService {
   public:
    explicit AsyncService(::grpc::CompletionQueue* cq);
    ~AsyncService() {};
    void RequestreqMATSim2ExternHasSpace(::grpc::ServerContext* context, ::org::matsim::hybrid::MATSim2ExternHasSpace* request, ::grpc::ServerAsyncResponseWriter< ::org::matsim::hybrid::MATSim2ExternHasSpaceConfirmed>* response, ::grpc::CompletionQueue* cq, void *tag);
    void RequestreqMATSim2ExternPutAgent(::grpc::ServerContext* context, ::org::matsim::hybrid::MATSim2ExternPutAgent* request, ::grpc::ServerAsyncResponseWriter< ::org::matsim::hybrid::MATSim2ExternPutAgentConfirmed>* response, ::grpc::CompletionQueue* cq, void *tag);
    void RequestreqExternDoSimStep(::grpc::ServerContext* context, ::org::matsim::hybrid::ExternDoSimStep* request, ::grpc::ServerAsyncResponseWriter< ::org::matsim::hybrid::ExternDoSimStepReceived>* response, ::grpc::CompletionQueue* cq, void *tag);
    void RequestreqExternOnPrepareSim(::grpc::ServerContext* context, ::org::matsim::hybrid::ExternOnPrepareSim* request, ::grpc::ServerAsyncResponseWriter< ::org::matsim::hybrid::ExternOnPrepareSimConfirmed>* response, ::grpc::CompletionQueue* cq, void *tag);
    void RequestreqExternAfterSim(::grpc::ServerContext* context, ::org::matsim::hybrid::ExternAfterSim* request, ::grpc::ServerAsyncResponseWriter< ::org::matsim::hybrid::ExternAfterSimConfirmed>* response, ::grpc::CompletionQueue* cq, void *tag);
  };
};

}  // namespace hybrid
}  // namespace matsim
}  // namespace org


#endif  // GRPC_MATSimInterface_2eproto__INCLUDED
