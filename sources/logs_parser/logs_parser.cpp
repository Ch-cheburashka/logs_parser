#include <logs_parser/logs_parser.hpp>
#include <iostream>
#include <fstream>

std::vector<std::filesystem::path> read_file(const std::filesystem::path &file, std::vector<std::filesystem::path> &logs) {
    std::ifstream f;
    f.open(file);
    std::string current;

    while (!f.eof()) {
        current.clear();
        std::getline(f,current);
        auto path = std::filesystem::path(current);
        logs.emplace_back(path);
    }
    f.close();
    return logs;
}

std::vector<std::filesystem::path> file_parser::parse(const std::filesystem::path& path_to_logs) {
    std::vector <std::filesystem::path> logs;
    read_file(path_to_logs,logs);
    return logs;
}

std::vector<std::filesystem::path> dir_parser::parse(const std::filesystem::path &path_to_logs) {
    std::vector <std::filesystem::path> logs;
    for (const std::filesystem::directory_entry &x: std::filesystem::directory_iterator{path_to_logs}) {
        read_file(x.path(),logs);
    }
    return logs;
}
