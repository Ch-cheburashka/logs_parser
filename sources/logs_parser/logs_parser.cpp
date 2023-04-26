#include <logs_parser/logs_parser.hpp>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::vector<json> read_file(const std::filesystem::path &file, std::vector<json> &logs) {
    std::ifstream f;
    f.open(file);
    std::string current;

    while (!f.eof()) {
        current.clear();
        std::getline(f,current);
        if (!current.empty()) {
            json log;
            log["interval"] = current.substr(0, 19);
            log["info"] = current;
            log["level"] = current.substr(39, 3);

            logs.emplace_back(log);
        }
    }
    f.close();
    return logs;
}

std::vector<json> file_parser::parse(const std::filesystem::path& path_to_logs) {
    std::vector <json> logs;
    read_file(path_to_logs,logs);
    return logs;
}

std::vector<json> dir_parser::parse(const std::filesystem::path &path_to_logs) {
    std::vector <json> logs;
    for (const std::filesystem::directory_entry &x: std::filesystem::directory_iterator{path_to_logs}) {
        read_file(x.path(),logs);
    }
    return logs;
}
