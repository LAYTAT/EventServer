#include "SeFNet.h"
#include "SeNet.h"

void SeFNet::Execute(LOOP_RUN_TYPE run)
{
	mNet->StartLoop(run);
}

void SeFNet::RemoveReceiveCallBack(const int nMsgID)
{
	auto it = mReceiveCallBack.find(nMsgID);
	if (it != mReceiveCallBack.end())
	{
		mReceiveCallBack.erase(it);
	}
}

void SeFNet::AddReceiveCallBack(const int nMsgID, const NET_RECEIVE_FUNCTOR_PTR& cb)
{
	auto it = mReceiveCallBack.find(nMsgID);
	if (it != mReceiveCallBack.end())
	{
		return;
	}
	mReceiveCallBack.emplace(nMsgID, cb);
}

void SeFNet::AddReceiveCallBack(const NET_RECEIVE_FUNCTOR_PTR& cb)
{
	mReceiveCallBackList.emplace_back(cb);
}

void SeFNet::AddEventCallBack(const NET_EVENT_FUNCTOR_PTR& cb)
{
	mEventCallBackList.emplace_back(cb);
}

bool SeFNet::InitNet(const char* ip, uint32_t port)
{
	mNet = new SeNet(this, &SeFNet::OnReceiveNetPack, &SeFNet::OnSocketNetEvent);
	return mNet->InitClient(ip, port);
}

bool SeFNet::InitNet(uint32_t port)
{
	mNet = new SeNet(this, &SeFNet::OnReceiveNetPack, &SeFNet::OnSocketNetEvent);
	return mNet->InitServer(port);
}

void SeFNet::OnReceiveNetPack(const socket_t nFd, const int nMsgId, const char* pMsg, const uint32_t nLen)
{
	auto it = mReceiveCallBack.find(nMsgId);
	if (it != mReceiveCallBack.end())
	{
		it->second->operator()(nFd, nMsgId, pMsg, nLen);
	}
	else
	{
		for (auto& it : mReceiveCallBackList)
		{
			it->operator()(nFd, nMsgId, pMsg, nLen);
		}
	}
}

void SeFNet::OnSocketNetEvent(const socket_t nFd, const SE_NET_EVENT nEvent, SeNet* pNet)
{
	for (auto& it : mEventCallBackList)
	{
		it->operator()(nFd, nEvent, pNet);
	}
}

void SeFNet::SendMsg(const socket_t fd, const int nMsgID, const char* msg, int len)
{
	mNet->SendProtoMsg(fd, nMsgID, msg, len);
}
void SeFNet::SendMsg(std::vector<socket_t>& fdlist, const int nMsgID, const char* msg, int len)
{
	mNet->SendProtoMsg(fdlist, nMsgID, msg, len);
}
void SeFNet::SendToAll(const int nMsgID, const char* msg, int len)
{
	mNet->SendProtoMsg(nMsgID, msg, len);
}

void SeFNet::SendPbMsg(const socket_t fd, const int nMsgID, ::google::protobuf::Message* pMsg)
{
	std::string strMsg = pMsg->SerializeAsString();
	if (strMsg.empty()) return;
	mNet->SendProtoMsg(fd, nMsgID, strMsg.c_str(), strMsg.length());
}
void SeFNet::SendPbMsg(std::vector<socket_t>& fdlist, const int nMsgID, ::google::protobuf::Message* pMsg)
{
	std::string strMsg = pMsg->SerializeAsString();
	if (strMsg.empty()) return;

	mNet->SendProtoMsg(fdlist, nMsgID, strMsg.c_str(), strMsg.length());
}
void SeFNet::SendPBToAllMsg(const int nMsgID, ::google::protobuf::Message* pMsg)
{
	std::string strMsg = pMsg->SerializeAsString();
	if (strMsg.empty()) return;
	mNet->SendProtoMsg(nMsgID, strMsg.c_str(), strMsg.length());
}
