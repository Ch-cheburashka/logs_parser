#include <argparse/argparse.hpp>
#include <iostream>
#include <logs_parser/logs_parser.hpp>

std::vector<std::filesystem::path> sort(const std::string& param, std::vector<std::filesystem::path>&& logs) {
    std::vector<std::filesystem::path> new_logs;
    for (auto& i : logs) {
        if (i.string().find(param) != std::string::npos)
            new_logs.emplace_back(i);
    }
    return new_logs;
}


int main (int argc, char** argv) {

    argparse::ArgumentParser opt_desc ("logs_parser");

    opt_desc.add_argument("--fileparse")
    .default_value(std::string{});

    opt_desc.add_argument("--dirparse")
    .default_value(std::string{});

    opt_desc.add_argument("--level")
    .default_value(std::string{});

    opt_desc.add_argument("--interval")
    .default_value(std::vector<std::string>{})
    .nargs(1,2);

    opt_desc.add_argument("--substring")
    .default_value(std::string());

    opt_desc.parse_args(argc,argv);

    std::vector<std::filesystem::path> logs;
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
        logs = sort(level,std::move(logs));
        for (auto& v : logs)
            std::cout << v << "\n";
    }
    if (opt_desc.is_used("--interval")) {
        auto interval = opt_desc.get<std::vector<std::string>>("--interval");
        if (interval.size() > 1)
            logs = sort(interval[0] + " " + interval[1],std::move(logs));
        else
            logs = sort(interval[0], std::move(logs));
        for (auto& v : logs)
            std::cout << v << "\n";
    }
    if (opt_desc.is_used("--substring")) {
        auto str = opt_desc.get<std::string>("--substring");
        logs = sort(str,std::move(logs));
        for (auto& v : logs)
            std::cout << v << "\n";
    }

    return 0;
}