//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <sources/cmd/ANSI-color-codes.hpp>
#include <sources/cmd/CmdSource.hpp>
#include <sources/cmd/rxterm/components/flowlayout.hpp>
#include <sources/cmd/rxterm/components/maxwidth.hpp>
#include <sources/cmd/rxterm/components/progress.hpp>
#include <sources/cmd/rxterm/style.hpp>
#include <sources/cmd/rxterm/terminal.hpp>
#include <sources/mux/MuxSource.hpp>
#include <common/resources/ResourceIdentifier.hpp>

#include <cstdlib>
#include <exception>
#include <filesystem>
#include <thread>

#include <fmt/core.h>
#include <readline/history.h>
#include <readline/readline.h>

char* username;

using namespace rxterm;

auto renderToTerm = [](auto const& vt, unsigned const w, Component const& c) {
    return vt.flip(c.render(w).toString());
};

std::vector<std::string> terminalCommands{"", "ll", "ls", "pwd", "cd", "exit", "quit", "q"};

//-------------------------------------------------------------------------------------
CmdSource::CmdSource(MuxSource* _muxSource) {
    muxSource = _muxSource;
    initShell();
    progress();
    std::thread t(&CmdSource::run, this);
    t.detach();
}

//-------------------------------------------------------------------------------------
void CmdSource::initShell() {
    //    clear();
    std::cout << RED
              << "***********************************************************************"
                 "*************************************************"
              << std::endl;
    std::cout << RED << "******                                            " << YEL << XRT_LOGO << " Command Line"
              << RED << "                                           ******" << std::endl;
    std::cout << RED << "******                                          " << YEL << "Xpu RunTime © 2022-2023" << RED
              << "                                           ******" << std::endl;
    std::cout << RED
              << "***********************************************************************"
                 "*************************************************"
              << COLOR_RESET << std::endl;
    username = getenv("USER");
}

//-------------------------------------------------------------------------------------
void CmdSource::progress() {
    using namespace std::chrono_literals;
    using namespace std::string_literals;

    VirtualTerminal vt;

    //  std::string _message = "Loading "+ _name + "...";

    auto superProgressBar = [](auto x) -> FlowLayout<> {
        return {//      Text(">"),
                FlowLayout<>{
                    MaxWidth(120, Progress(x)),
                }};
    };

    auto w = VirtualTerminal::width();
    if (!w)
        w = 80;
    for (int i = 0; i < 101; ++i) {
        vt = renderToTerm(vt, w, superProgressBar(0.01 * i));
        std::this_thread::sleep_for(5ms);
    }
}

//-------------------------------------------------------------------------------------
std::string CmdSource::get_input(const std::string& p) {
    std::string _line;
    //  std::cout << ">2.["<<p<<"]" << std::endl;
    _line = readline(p.c_str());
    //  std::cout << p;
    //  std::getline(std::cin, _line);
    //  std::cout << ">3.["<< _line << "]" << std::endl;
    if (!_line.empty()) {
        add_history(_line.c_str());
    }
    return _line;
}

//-------------------------------------------------------------------------------------
[[noreturn]] void CmdSource::run() {
    std::string _homedir = string(getenv("HOME"));
    std::string _user    = string(getenv("USER"));
    while (true) {
        std::string _pwd = std::filesystem::current_path().string();

        if (_pwd.compare(0, _homedir.length(), _homedir) == 0) {
            _pwd.replace(0, _homedir.length(), "~");
        }

        std::string _prompt;
        _prompt.append(HGRN).append(_user).append("@").append(_pwd).append(">").append(CRESET);

        std::string _line = get_input(_prompt);
        runCommand(_line);
    }
}

//-------------------------------------------------------------------------------------

void CmdSource::runCommand(std::string_view _line) {
    try {
        strTokenizer(_line);

        if (argv.at(0) == "run") {
            // TODO: muxSource->run(ResourceIdentifier::fromString(argv.at(1)), {}, {});
        } else if (argv.at(0) == "source") {
            std::filesystem::path _path = argv.at(1);
            // TODO: muxSource->load(_path);
        } else if (argv.at(0) == "debug-set-breakpoint") {
            unsigned _i = std::stoi(argv.at(2));
            fmt::println("{}", muxSource->debugSetBreakpoint(argv.at(1), _i));
        }
    } catch (const std::exception& e) {
    }
}

//-------------------------------------------------------------------------------------
