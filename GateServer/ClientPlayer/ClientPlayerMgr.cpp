#include "ClientPlayerMgr.h"
#include "ClientPlayer.h"
#include "Session.h"

std::unique_ptr<ClientPlayerMgr> g_pClientPlayerMgr = nullptr;


ClientPlayer* ClientPlayerMgr::NewPlayer(Session* pSession)
{
	if (pSession == nullptr) return nullptr;

	ClientPlayer* pPlayer = mClientPool.NewElem();
	if (pPlayer == nullptr) return nullptr;
	pPlayer->SetSession(pSession);
	pPlayer->SetPlayerId(pPlayer->GetMemId()); // todo 设置id为玩家对象内存id
	return pPlayer;
}

void ClientPlayerMgr::DelPlayer(ClientPlayer* player)
{
	mClientPool.DelElem(player->GetId());
	mPlayerSockMap.erase(player->GetSockFd());
	m_playeridMap.erase(player->GetPlayerId());
}

ClientPlayer* ClientPlayerMgr::GetPlayer(int memId)
{
	return mClientPool.GetElem(memId);
}

void ClientPlayerMgr::AddPlayerIDMap(uint64_t playerId, ClientPlayer* player)
{
	if (player == nullptr) return;

	auto it = m_playeridMap.find(playerId);
	if (it == m_playeridMap.end())
	{
		m_playeridMap.emplace(playerId, player);
	}
}
ClientPlayer* ClientPlayerMgr::GetPlayerByID(uint64_t playerId)
{
	auto it = m_playeridMap.find(playerId);
	if (it == m_playeridMap.end()) return nullptr;

	return it->second;
}

void ClientPlayerMgr::AddPlayerSockMap(socket_t sock_fd, ClientPlayer* player)
{
	if (player == nullptr) return;
	auto it = mPlayerSockMap.find(sock_fd);
	if (it == mPlayerSockMap.end())
	{
		mPlayerSockMap.emplace(sock_fd, player);
	}
}

ClientPlayer* ClientPlayerMgr::GetPlayerByFd(socket_t sock_fd)
{
	auto it = mPlayerSockMap.find(sock_fd);
	if (it == mPlayerSockMap.end()) return nullptr;

	return it->second;
}