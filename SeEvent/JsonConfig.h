#pragma once

#include <memory>
#include "json.h"

class JsonConfig {
public:
	bool Load(const char* jsonFile);

	Json::Value m_Root;
	Json::Value m_ServerConf;

	Json::Value m_dbConf;
};

extern std::unique_ptr<JsonConfig>  g_pJsonConfig;