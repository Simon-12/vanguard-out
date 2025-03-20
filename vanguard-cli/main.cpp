#include <mini/ini.h>
#include <windows.h>

#include <CLI/CLI.hpp>
#include <filesystem>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

#ifdef NDEBUG
const bool DEBUG_MODE = false;
#else
const bool DEBUG_MODE = true;
#endif

std::string REGISTER_PATH = "HKLM:/Software/Microsoft/Windows/CurrentVersion/Run";
std::wstring_convert<std::codecvt_utf8<wchar_t>> CONVERTER;

class SettingsFile {
public:
    SettingsFile(const std::string& path) {
        m_file = new mINI::INIFile(path);
        m_file->read(m_ini);

        if (m_ini.has("settings")) {
            auto& settings = m_ini["settings"];

            // State
            if (settings.has("state")) {
                std::string state = settings["state"];
                m_state = state.compare("activate") == 0;
            } else
                m_state = true;

            // Tray path
            if (settings.has("tray")) {
                m_tray = settings["tray"];
                if (m_tray.length() == 0) m_tray = read_tray_path();
            } else
                m_tray = read_tray_path();
        } else {
            m_state = true;
            m_tray = read_tray_path();
        }
    }

    // Getter
    bool state() { return m_state; }
    std::string tray() { return m_tray; }

    // Setter
    void set_state(const bool state) {
        m_state = state;
        update_file();
    }
    void set_tray(const std::string& path) {
        m_tray = path;
        update_file();
    }

private:
    void update_file() {
        m_ini["settings"]["state"] = m_state ? "activate" : "deactivate";
        m_ini["settings"]["tray"] = m_tray;
        m_file->write(m_ini);
    }

    std::string read_tray_path() {
        std::string cmd = "powershell (Get-ItemProperty -Path " + REGISTER_PATH + ").'Riot Vanguard'";
        std::string out;
        char buffer[128];
        FILE* pipe = _popen(cmd.c_str(), "r");
        if (pipe) {
            while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
                out += buffer;
            }
            _pclose(pipe);
        }
        return out;
    }

    mINI::INIFile* m_file;
    mINI::INIStructure m_ini;
    bool m_state;
    std::string m_tray;
};

void run_command(const std::wstring& cmd) {
    ShellExecuteW(NULL, L"runas", L"cmd.exe", (L"/c " + cmd).c_str(), NULL, SW_HIDE);
    // https://learn.microsoft.com/de-de/windows/win32/api/shellapi/nf-shellapi-shellexecutew
}

void run_activate(const std::string& tray) {
    if (DEBUG_MODE)
        run_command(L"start calc.exe");
    else {
        run_command(L"sc config vgc start= demand & sc config vgk start= system");
        run_command(L"net start vgc");
        run_command(CONVERTER.from_bytes(tray));
        run_command(L"powershell Set-ItemProperty -Path " + CONVERTER.from_bytes(REGISTER_PATH) +
                    L" -Name 'Riot Vanguard' -Value '" + CONVERTER.from_bytes(tray) + L"'");
    }
    std::cout << "Vanguard State: Active" << std::endl;
}

void run_deactivate() {
    if (DEBUG_MODE)
        run_command(L"start calc.exe");
    else {
        run_command(
            L"sc config vgc start= disabled & sc config vgk start= disabled & net stop vgc & net stop vgk & taskkill /IM "
            L"vgtray.exe");
        run_command(L"powershell Remove-ItemProperty -Path " + CONVERTER.from_bytes(REGISTER_PATH) + L" -Name 'Riot Vanguard'");
    }
    std::cout << "Vanguard State: Inactive" << std::endl;
}

int main(int argc, char** argv) {
    // Create app
    CLI::App app("Command line tool to temporarily stop and disable Riot Vanguard \n");
    app.set_version_flag("-v,--version", "vanguard-cli tool 0.3.0");
    app.get_formatter()->column_width(50);

    // Settings file
    WCHAR buffer[255] = {0};
    GetModuleFileNameW(NULL, buffer, 255);
    std::filesystem::path app_path(buffer);
    std::filesystem::path settings_path = app_path.parent_path() / "settings.ini";
    SettingsFile settings(settings_path.string());

    // Option state
    bool state = false;
    std::map<std::string, bool> map_state{
        {"activate", true},
        {"deactivate", false},
    };
    auto option_state = app.add_option("-s,--state", state, "Set vanguard state")
                            ->transform(CLI::CheckedTransformer(map_state, CLI::ignore_case))
                            ->option_text("{activate, deactivate} or {1, 0}");

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
        bool file_state = settings.state();
        if (!file_state) run_deactivate();

        return 0;  // Exit
    }

    // Change state
    if (option_state->count() > 0) {
        if (state) {
            settings.set_state(true);
            run_activate(settings.tray());
        } else {
            settings.set_state(false);
            run_deactivate();
        }
    }

    return 0;
}
