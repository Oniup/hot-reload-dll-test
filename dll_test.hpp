#ifndef __DLL_TEST_HPP__
#define __DLL_TEST_HPP__

extern "C" {

__declspec(dllexport) void print_message();
__declspec(dllexport) bool is_running();

}


#endif