#include <windows.h>

#include <CLI/CLI.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;
fs::path STATE_FILE;

#ifdef NDEBUG
const bool DEBUG_MODE = false;
#else
const bool DEBUG_MODE = true;
#endif

void file_write(const bool state) {
    // Create file
    std::ofstream file(STATE_FILE, std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "Error: unable to create state file" << std::endl;
        return;
    }

    // Write state
    std::string content = state ? "activate" : "deactivate";
    file << content;
    file.close();
    return;
}

bool file_read() {
    // Check file exist
    if (!fs::exists(STATE_FILE)) {
        file_write(false);
        return false;
    }

    // Read file
    std::ifstream file(STATE_FILE);
    if (!file.is_open()) {
        std::cerr << "Error: unable to read state file" << std::endl;
        return false;
    }
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    return content.compare("activate") == 0;
}

void run_command(const std::wstring& cmd) {
    ShellExecuteW(NULL, L"runas", L"cmd.exe", (L"/c " + cmd).c_str(), NULL, SW_HIDE);
    // https://learn.microsoft.com/de-de/windows/win32/api/shellapi/nf-shellapi-shellexecutew
}

void run_activate() {
    if (DEBUG_MODE)
        run_command(L"start calc.exe");
    else
        run_command(L"sc config vgc start= demand & sc config vgk start= system");

    std::cout << "Vanguard State: Active" << std::endl;
}

void run_deactivate() {
    if (DEBUG_MODE)
        run_command(L"start calc.exe");
    else
        run_command(
            L"sc config vgc start= disabled & sc config vgk start= disabled & net stop vgc & net stop vgk & taskkill /IM "
            L"vgtray.exe");

    std::cout << "Vanguard State: Inactive" << std::endl;
}

int main(int argc, char** argv) {
    // Create app
    CLI::App app("Command line tool to temporarily stop and disable Riot Vanguard \n");
    app.set_version_flag("-v,--version", "vanguard-cli tool 1.0.0");
    app.get_formatter()->column_width(50);

    // Set state file path
    WCHAR buffer[255] = {0};
    GetModuleFileNameW(NULL, buffer, 255);
    std::filesystem::path app_path(buffer);
    STATE_FILE = app_path.parent_path() / "state";

    // Option state
    bool state = false;
    std::map<std::string, bool> map_state{
        {"activate", true},
        {"deactivate", false},
    };
    auto option_state = app.add_option("-s,--state", state, "Set vanguard state")
                            ->transform(CLI::CheckedTransformer(map_state, CLI::ignore_case))
                            ->option_text("{activate, deactivate} or {1, 0}");

    // Option restart
    std::vector<int> int_vec;
    app.add_option("-r,--restart", int_vec, "Restart system after N seconds (default: 0)")
        ->expected(0, 1)
        ->check(CLI::PositiveNumber);

    // Option check
    bool check = false;
    app.add_flag("-c,--check", check, "Check current state and activate or deactivate vanguard \n");

    // Start parse arguments
    CLI11_PARSE(app, argc, argv);

    // If no arguments are provided, display the help string
    if (argc == 1) {
        std::cout << app.help() << std::endl;
        return 0;  // Exit
    }

    // Run check
    if (check) {
        bool file_state = file_read();
        if (!file_state) run_deactivate();

        return 0;  // Exit
    }

    // Change state
    if (option_state->count() > 0) {
        if (state) {
            file_write(true);
            run_activate();
        } else {
            file_write(false);
            run_deactivate();
        }
    }

    // Restart system
    if (!int_vec.empty() && state) {
        int seconds = int_vec[0];
        std::cout << "Restart: " << std::to_string(seconds) << "s" << std::endl;

        if (!DEBUG_MODE) run_command(L"shutdown /r /f /t " + std::to_wstring(seconds));

    } else if (state)
        std::cout << "System restart required" << std::endl;

    return 0;
}
