#pragma once

#include "SingleTon.hpp"
#include "packet/PacketMgr.h"

class Player;
class Packet;

enum EWorld
{
	MODULE_ID_WORLD = 15,
};
extern std::unique_ptr<PacketMgr> g_packetmgr;
class ModuleWorld : public SingleTon<ModuleWorld>
{
public:
	ModuleWorld()
	{

	}
private:
};