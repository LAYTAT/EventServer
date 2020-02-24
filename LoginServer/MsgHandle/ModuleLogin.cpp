#include "clientmodule/ModuleLogin.h"
#include "LoginPlayer.h"
#include "LogUtil.h"
#include "Assertx.h"
#include "Connection/ConnectionPool.h"
#include "NodeNet/LoginServerThread.h"

int ModuleLogin::LoginReq(Player* player, Packet* packet)
{
	Assert(player && packet);
	auto pb_msg = (Login_LoginReq*)packet->msg;
	auto login_player = (LoginPlayer*)player;
	// unity 登录 用户名密码验证
	// 手机客户端登录 gameid token(openid) channelid 发送到上游验证
	//std::thread::id tid = std::this_thread::get_id();
	//if (tid == g_pServerThread->GetThreadId())
	if (pb_msg->game_id().empty())
	{
		// 本地数据验证
		ConnThread* conn_thrd = g_conn_pool->Malloc();
		std::string sql = "select loginname,loginpwd from tb_account where loginname=" + pb_msg->login_name() \
			+ ",loginpwd=" + pb_msg->login_pwd();
		conn_thrd->AddSqlReq(player->m_playerid, sql);
	}
	else
	{
		// 通过sdk 验证
	}

	Login_LoginReply reply;
	//reply.set_ret(0);

	return 0;
}
int ModuleLogin::CreateRoleReq(Player* player, Packet* packet)
{
	return 0;
}
int ModuleLogin::SelectRoleReq(Player* player, Packet* packet)
{
	return 0;
}
int ModuleLogin::DelRoleReq(Player* player, Packet* packet)
{
	return 0;
}