#include "SeFClientBase.h"
#include "LogHelper.h"
#include "SeFNetClient.h"
#include "JsonConfig.h"
#include "SeNet.h"

bool SeFClientBase::Init()
{
	mNetCliModule = new SeFNetClient();
	mNetCliModule->AddReceiveCallBack(ServerType::SERVER_TYPE_MASTER, MASTER_REPORT_SERVER_INFO_TO_SERVER, this, &SeFClientBase::OnMasterMessage);
	
	return true;
}

ConnectDataPtr SeFClientBase::GetServerNetInfo(const int& nServerID)
{
	return mNetCliModule->GetServerNetInfo(nServerID);
}

ConnectDataPtr SeFClientBase::GetServerNetInfo(const SeNet* pNet)
{
	return mNetCliModule->GetServerNetInfo(pNet);
}

ConnectDataPtr SeFClientBase::GetServerNetInfo(const socket_t& sock_fd)
{
	return mNetCliModule->GetServerNetInfo(sock_fd);
}

void SeFClientBase::OnSocketNodeEvent(const socket_t sock_fd, const SE_NET_EVENT nEvent, SeNet* pNet)
{
	ConnectDataPtr pConnData = mNetCliModule->GetServerNetInfo(pNet);
	if (nEvent == SE_NET_EVENT_CONNECTED && pConnData)
	{
		pConnData->SockFd = sock_fd;
		mNetCliModule->SendPbByServId(pConnData->ServerId, REPORT_CLIENT_INFO_TO_SERVER, &mServerInfo);
		LOG_INFO("(%d : %s) report info to %s server", mServerInfo.server_id(), mServerInfo.server_name().c_str(), (pConnData->name).c_str());
	}
	else
	{
		LOG_ERR("%s server client disconnected!!!", mServerInfo.server_name().c_str());
	}
}

void SeFClientBase::OnMasterMessage(const socket_t sock_fd, const int nMsgID, const char* msg, const uint32_t msg_len)
{
	if (mConnectType.empty()) return;
	ServerReportList report_list;
	if (!report_list.ParseFromArray(msg, msg_len))
	{
		LOG_ERR("report list parse error.....");
		return;
	}

	for (int i = 0; i < report_list.server_info_size(); ++i)
	{
		const ServerReport& server_info = report_list.server_info(i);
		for (auto& serverType : mConnectType)
		{
			if (server_info.server_type() == serverType)
			{
				ConnectDataPtr ServerData = std::make_shared<ConnectData>();
				ServerData->ServerId = server_info.server_id();
				ServerData->Port = server_info.server_port();
				ServerData->name = server_info.server_name();
				ServerData->Ip = server_info.server_ip();
				ServerData->ServerType = ServerType(server_info.server_type());
				ServerData->ConnState = ConnectState::CONNECTING;
				mNetCliModule->AddServer(ServerData);
			}
		}
	}
}


void SeFClientBase::Loop()
{
	mNetCliModule->Execute(LOOP_RUN_NONBLOCK);
}


void SeFClientBase::AddConnectMaster()
{
	if (this->GetServerType() != ServerType::SERVER_TYPE_MASTER)
	{
		g_JsonConfig->Load("../Config/ServerConf.json");
		ConnectDataPtr ServerData = std::make_shared<ConnectData>();
		ServerData->ServerId = g_JsonConfig->m_Root["MasterServer"]["NodeId"].asInt();
		ServerData->ServerType = ServerType::SERVER_TYPE_MASTER;
		ServerData->Ip = g_JsonConfig->m_Root["MasterServer"]["NodeIp"].asString();
		ServerData->Port = g_JsonConfig->m_Root["MasterServer"]["NodePort"].asInt();
		ServerData->name = g_JsonConfig->m_Root["MasterServer"]["NodeName"].asString();
		ServerData->ConnState = ConnectState::CONNECTING;
		mNetCliModule->AddServer(ServerData);
	}
}
