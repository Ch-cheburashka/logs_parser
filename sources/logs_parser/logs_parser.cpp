#include <logs_parser/logs_parser.hpp>
#include <iostream>
#include <fstream>
#include <ctime>

std::vector<log_info> read_file(const std::filesystem::path &file, std::vector<log_info> &logs) {
    std::ifstream f;
    f.open(file);
    std::string current;

    while (!f.eof()) {
        current.clear();
        std::getline(f,current);
        if (!current.empty()) {
            struct tm tm;
            if (strptime(current.substr(0,19).c_str(), "%F %T",&tm)) {
                log_info log{current, current.substr(0, 19), current.substr(current.find_first_of('(') + 1, 3)};
                logs.emplace_back(log);
            }
            else {
                logs[logs.size()-1].info.append("\n" + current);
            }
        }
    }
    f.close();
    return logs;
}

std::vector<log_info> file_parser::parse(const std::filesystem::path& path_to_logs) {
    std::vector <log_info> logs;
    read_file(path_to_logs,logs);
    return logs;
}

std::vector<log_info> dir_parser::parse(const std::filesystem::path &path_to_logs) {
    std::vector <log_info> logs;
    for (const std::filesystem::directory_entry &x: std::filesystem::directory_iterator{path_to_logs}) {
        read_file(x.path(),logs);
    }
    return logs;
}
