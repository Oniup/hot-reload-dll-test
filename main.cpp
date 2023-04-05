#include <iostream>
#include <string>
#include <string_view>
#include <ctime>
#include <sys/stat.h>
#ifndef WIN32
#   include <unistd.h>
#endif

#include <Windows.h>

class LoadSharedLibrary {
public:
    LoadSharedLibrary(std::string_view path) {
#ifndef WIN32
        m_handle = dlopen(path.data(), RTLD_LAZY);
#else
        m_handle = LoadLibrary(path.data());
#endif
        if (is_nullptr()) {
            std::cout << "failed to load dll with path: " << path.data() << "\n";
        }

        m_path = std::string(path);
    }

    ~LoadSharedLibrary() {
        if (!is_nullptr()) {
#ifndef WIN32
            dlclose(m_handle);
#else
            if (!FreeLibrary(m_handle)) {
                std::cout << "failed to free dll with path: " << m_path.data() << "\n";
            }
            
#endif
            m_handle = nullptr;
        }
    }

    auto load_fnptr(std::string_view function_name) {
#ifndef WIN32
        return dlsym(m_handle, function_name.data());
#else
        return GetProcAddress(m_handle, function_name.data());
#endif
    }

    inline bool is_nullptr() {
        return m_handle == nullptr;
    }

private:
#ifndef WIN32
    void* m_handle{ nullptr };
#else
    HINSTANCE m_handle{ nullptr };
#endif
    std::string m_path;
};

typedef void (*fnptr_print_message)();
typedef bool (*fnptr_is_running)();

int main(int argc, char** argv) {
    bool running = true;
    size_t count = 0;
    std::time_t last_modified{};
    while (running) {
        Sleep(500);
        struct stat file_results;

        if (stat("dll_test.dll", &file_results) == 0) {
            if (last_modified != file_results.st_mtime) {
                LoadSharedLibrary lib("dll_test.dll");
                if (lib.is_nullptr()) {
                    continue;
                }

                last_modified = file_results.st_mtime;
                std::cout << "loaded dll, last modified: " << last_modified << "\n";

                fnptr_print_message print_message = (fnptr_print_message)lib.load_fnptr("print_message");
                fnptr_is_running is_running = (fnptr_is_running)lib.load_fnptr("is_running");

                if (print_message == nullptr) {
                    std::cout << "failed to load function pointer (fnptr_print_message)\n";

                    std::getchar();
                    return -1;
                }
                if (is_running == nullptr) {
                    std::cout << "failed to load function pointer (fnptr_is_running)\n";

                    std::getchar();
                    return -1;
                }

                std::cout << "loaded functions from dll\nptrfn_print_message():\n\t";
                print_message();
                running = is_running();

                std::cout << "freed dll\n\n\n";
            }
        }
    }

    std::cout << "application closed\n";
    std::getchar();

    return 0;
}