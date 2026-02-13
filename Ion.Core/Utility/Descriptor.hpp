#pragma once
#include <concepts>
#include <type_traits>
#include <utility>
#include <optional>

#include "Utility/nttp_callable_traits.hpp"



namespace Ion::Utility
{





	
	 


	
	template<auto F, typename T>
	concept CallableWithExactlyT =
	std::invocable<decltype(F), T> && std::same_as<T, inspector_t<F, 0>>;

	
	template<typename T, auto closeRawDescriptor, T InvalidValue>
	requires CallableWithExactlyT<closeRawDescriptor,T>
	class Descriptor
	{
	private:
		using CloseResult_t = std::invoke_result_t<decltype(closeRawDescriptor), T>;

	public:
		Descriptor operator=(const Descriptor&) = delete;
		Descriptor(const Descriptor&) = delete;

		
		Descriptor() : m_rawDescriptor(InvalidValue) {}

		Descriptor& operator=(Descriptor&& other) noexcept
		{
			if (this != &other)
			{
				
				if (m_rawDescriptor != InvalidValue)
					closeRawDescriptor(m_rawDescriptor);

				
				m_rawDescriptor = std::exchange(other.m_rawDescriptor, InvalidValue);
				
			}
			return *this;
		}

		Descriptor(Descriptor&& other) noexcept
			: m_rawDescriptor(std::exchange(other.m_rawDescriptor, InvalidValue))
		{
		}

		explicit Descriptor(T descriptor)
		{
			m_rawDescriptor = descriptor;
			
		}

		[[nodiscard]] T get() const noexcept
		{
			return m_rawDescriptor;
		}

		
		explicit operator bool() const { return m_rawDescriptor != InvalidValue; }

		
		T release()
		{
			return std::exchange(m_rawDescriptor, InvalidValue);
		}



		//Use if you want to examine the close result of the internal function
		//Implicitly called in the destructor
		//Returns an empty optional upon a second close attempt
		[[nodiscard]] std::optional<CloseResult_t> close() requires (!std::is_void_v<CloseResult_t>)
		{
			if (m_rawDescriptor != InvalidValue)
			{
				T temp = std::exchange(m_rawDescriptor, InvalidValue);

				return closeRawDescriptor(temp);
			}

			return std::nullopt;

		}


		void close() requires std::is_void_v<CloseResult_t>
		{
			if (m_rawDescriptor != InvalidValue)
			{
				T temp = std::exchange(m_rawDescriptor, InvalidValue);

				closeRawDescriptor(temp);
			}

		}

		~Descriptor()
		{
			(void) this->close();
		}

	private:
		T m_rawDescriptor;
		
	};

	

	//using WindowsFileDescriptor = Descriptor<HANDLE, CloseHandle,INVALID_HANDLE_VALUE>;

	// CHANGE: Updated aliases to include the platform-specific InvalidValue.
	// using LinuxDescriptor = Descriptor<int, close, -1>;
	// using WindowsFileDescriptor = Descriptor<HANDLE, CloseHandle, INVALID_HANDLE_VALUE>;
}

