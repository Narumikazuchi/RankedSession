#pragma once
#include <bakkesmod/wrappers/cvarmanagerwrapper.h>

namespace RankedSession
{
	inline std::shared_ptr<CVarManagerWrapper> manager;

	void Log(std::string logtext);
}

inline void RankedSession::Log(std::string logtext)
{
	if (RankedSession::manager == nullptr)
	{
		return;
	}
	RankedSession::manager->log(logtext);
}