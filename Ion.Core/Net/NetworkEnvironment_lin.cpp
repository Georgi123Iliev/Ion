#include "pch.h"
#include "NetworkEnvironment.hpp"


#if false
#include<stdexcept>
#include <signal.h>

namespace Ion::Net
{


	//linux doesnt need to actually any setup for socket usage so this is a no-op class
	class NetworkEnvironment::EnvironmentImpl
	{
	public:

		EnvironmentImpl() = default;

		~EnvironmentImpl() = default;
	};

	NetworkEnvironment::NetworkEnvironment()
	{
		if (m_exists)
		{
			throw(std::logic_error("Only one environment can exist at a time"));
		}

		m_impl = std::make_unique<EnvironmentImpl>();
		m_exists.store(true);
	}

	NetworkEnvironment::~NetworkEnvironment() = default;


}

#endif