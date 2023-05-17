#include <logs_parser/logs_parser.hpp>
#include <iostream>
#include <fstream>
#include <ctime>

template<typename Str = std::string>
class string_builder {
public:
    string_builder() = default;

    string_builder(const Str &str) : _parts{str} {}

    void emplace_back(const Str &str) {
        _parts.emplace_back(str);
    }

    void clear() {
        _parts.clear();
    }

    [[nodiscard]] Str to_string(std::string_view delimiter = "\n") const {
        return format("{}", join(_parts, delimiter));
    }

private:
    std::list<Str> _parts{};
};

// Try-Catch надо написать будет. От ошибок никто не застрахован
void i_logs_parser::read_file(const std::filesystem::path &file, std::list<log_info> &logs) {
    std::ifstream f;
    f.open(file);
    std::unique_ptr<log_info> current_log {new log_info()};
    std::string current;
    string_builder current_string;

    while (!f.eof()) {
        current.clear();
        std::getline(f, current);
        if (!current.empty()) {
            struct tm tm;
            auto current_date = current.substr(0, 19);
            if (strptime(current_date.c_str(), "%F %T", &tm)) {
                logs.emplace_back(*current_log);
                current_log = std::make_unique<log_info>(
                        current_string.to_string(),
                        current_date,
                        current.substr(current.find_first_of('(') + 1, 3)
                );
                current_string.clear();
            } else {
                current_string.emplace_back(current);
            }
        }
    }
    f.close();
}

std::list<log_info> file_parser::parse(const std::filesystem::path &path_to_logs) {
    std::list<log_info> logs;
    read_file(path_to_logs, logs);
    return logs;
}

std::list<log_info> dir_parser::parse(const std::filesystem::path &path_to_logs) {
    std::list<log_info> logs;
    for (const std::filesystem::directory_entry &x: std::filesystem::directory_iterator{path_to_logs}) {
        read_file(x.path(), logs);
    }
    return logs;
}
