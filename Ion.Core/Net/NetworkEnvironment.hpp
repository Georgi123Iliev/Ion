#pragma once
#include <memory>
#include <atomic>
#include <csignal>
#include <thread>
#include <stop_token>
#include <chrono>


namespace Ion::Net
{

	

	class NetworkEnvironment
	{
	public:
		NetworkEnvironment();
		~NetworkEnvironment();

		auto aquireStopToken() const noexcept
		{
			return m_stopSource.get_token();
		}


		NetworkEnvironment(const NetworkEnvironment&) = delete;
		NetworkEnvironment& operator=(const NetworkEnvironment&) = delete;
		NetworkEnvironment(NetworkEnvironment&&) = delete;
		NetworkEnvironment& operator=(NetworkEnvironment&&) = delete;

	private:



		inline static std::atomic<bool> m_exists {false};
		class EnvironmentImpl;
		std::unique_ptr<EnvironmentImpl> m_impl;

		//Graceful shutdown mechanics

		inline static std::atomic<bool> m_sigintReceived{ false };

		static_assert(std::atomic<bool>::is_always_lock_free,
			"Pointless assert really, I don't know any platform that does not support lock free atomic bools");

		static void m_sigintHandler(int sig)
		{
			m_sigintReceived.store(true);
		}


		/*
		* m_stopSource clearly must outlive watchDog
		*/
		std::stop_source m_stopSource;

		std::jthread watchDog{ [&stopSource = m_stopSource]() {

			using namespace std::literals::chrono_literals;

			std::signal(SIGINT,m_sigintHandler);

			// Deliberately using a loop here.
			// The standard seemingly allows atomic<T>.wait() + atomic<T>.notify_one() for lock free atomics.
			// This however has been acknoledged as false.
			// See paper P3255R0.
			// Sleep loop seems to be the most cross platform solution, that is also forgiving on perfomance

			while (!m_sigintReceived)
			{
				std::this_thread::sleep_for(50ms);
			}

			stopSource.request_stop();

			//maybe should give it a bit of time before I allow sigint again?
			
			std::signal(SIGINT, SIG_DFL);

			} };
		
	};

	


}

