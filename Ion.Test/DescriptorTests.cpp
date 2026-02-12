#include "pch.h"

#include "Utility/Descriptor.hpp"

#include <io.h>

using namespace Ion::Utility;

using LinuxSocket = Descriptor<int, _close, -1>;
using LinuxFileDescriptor = Descriptor<int, _close, -1>;

TEST(DescriptorTests, BasicTest) {
	
    
    auto mockDeleter = [](int d) { SUCCEED() << "close ran"; };


    



    



}