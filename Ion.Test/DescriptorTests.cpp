#include "pch.h"

#include "Utility/Descriptor.hpp"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <memory>
// Ensure the Winsock library is linked
#pragma comment(lib, "Ws2_32.lib")
#include <io.h>
#include<windows.h>

using namespace Ion::Utility;



using LinuxSocket = Descriptor<int, _close, -1>;
using LinuxFileDescriptor = Descriptor<int, _close, -1>;
// using StupidSocket = Descriptor<char, _close, 'a'>; //this will now fail because _close(char) is not the exact signature
using AddrInfoDescriptor = Descriptor<struct addrinfo*, freeaddrinfo, nullptr>;
TEST(DescriptorTests, BasicTest) {

    /*
    // 1. Linux/Posix Logic (Works Fine)
    {
        int rawFd = _dup(1);
        LinuxFileDescriptor fd(rawFd);

        EXPECT_TRUE(fd);
        EXPECT_EQ(fd.get(), rawFd);

        auto result = fd.close();
        EXPECT_EQ(result.value(), 0);
    }

    // 2. Windows Logic (FIXED)
    {
        // Define the type (MSVC will warn about non-standard extension, but it compiles)
        using WindowsHandle = Descriptor<HANDLE, ::CloseHandle, INVALID_HANDLE_VALUE>;

        // BAD (My previous mistake): GetStdHandle(STD_OUTPUT_HANDLE)
        // GOOD: Create a standalone, throwaway kernel object (Event)
        HANDLE rawWinHandle = CreateEvent(nullptr, TRUE, FALSE, L"IonTestEvent");

        // Ensure we actually got a handle
        ASSERT_NE(rawWinHandle, nullptr);
        ASSERT_NE(rawWinHandle, INVALID_HANDLE_VALUE);

        WindowsHandle winFd(rawWinHandle);

        EXPECT_TRUE(winFd);
        EXPECT_EQ(winFd.get(), rawWinHandle);

        // This is now safe. It closes the Event, not the Console.
        auto result = winFd.close();

        // CloseHandle returns non-zero (TRUE) on success
        EXPECT_TRUE(result.has_value());
        EXPECT_NE(result.value(), 0);
    }
    */

    



    int result;
    struct addrinfo hints, * res = NULL; // Always initialize to NULL


    // 1. You MUST initialize Winsock once per process
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        // Handle startup error
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // 2. Check for ANY non-zero return value
    result = getaddrinfo("localhost", "8080", &hints, &res);

    if (result != 0) {
        // 3. Use gai_strerror for addrinfo specific errors, 
        // or WSAGetLastError() for general socket errors.
        fprintf(stderr, "getaddrinfo failed: %s\n", gai_strerror(result));

        // Cleanup if necessary
        WSACleanup();
        return;
    }

    std::unique_ptr<addrinfo,> smart_res(res,freeaddrinfo);
  
  


}