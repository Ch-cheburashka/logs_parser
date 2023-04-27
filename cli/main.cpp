#include <argparse/argparse.hpp>
#include <iostream>
#include <logs_parser/logs_parser.hpp>

int main (int argc, char** argv) {

    argparse::ArgumentParser opt_desc ("logs_parser");

    opt_desc.add_argument("--fileparse")
    .default_value(std::string{});

    opt_desc.add_argument("--dirparse")
    .default_value(std::string{});

    opt_desc.add_argument("--level")
    .default_value(std::string{});

    opt_desc.add_argument("--interval")
    .nargs(2)
    .default_value(std::vector<std::string>{});

    opt_desc.add_argument("--substring")
    .default_value(std::string());

    opt_desc.parse_args(argc,argv);

    std::vector<log_info> logs;
    if (opt_desc.is_used("--fileparse")) {
        auto file = opt_desc.get<std::string>("--fileparse");
        if (std::filesystem::is_directory(file)) {
            throw std::logic_error("the path to the directory was provided");
        }
        if (!file.ends_with(".log")) {
            throw std::logic_error("the file extension is not .log");
        }
        logs_parser logsParser(file);
        logsParser.set_parser(std::make_unique<file_parser>());
        logs = logsParser.parse();
    }

    else if (opt_desc.is_used("--dirparse")) {
        auto dir = opt_desc.get<std::string>("--dirparse");
        if (std::filesystem::is_regular_file(dir)) {
            throw std::logic_error("the path to the file was provided");
        }
        logs_parser logsParser(dir);
        logsParser.set_parser(std::make_unique<dir_parser>());
        logs = logsParser.parse();
    }

    if (opt_desc.is_used("--level")) {
        auto level = opt_desc.get<std::string>("--level");
        for (auto& v : logs) {
            if (v.level == level)
                std::cout << v.info << "\n";
        }
    }

    if (opt_desc.is_used("--interval")) {
        auto interval = opt_desc.get<std::vector<std::string>>("--interval");
        for (auto& v : logs) {
            if (v.interval == interval[0]+ " " + interval[1])
                std::cout << v.info << "\n";
        }
    }
    if (opt_desc.is_used("--substring")) {
        auto str = opt_desc.get<std::string>("--substring");
        for (auto& v : logs) {
            if (v.info.find(str) != std::string::npos)
                std::cout << v.info << "\n";
        }
    }

    return 0;
}