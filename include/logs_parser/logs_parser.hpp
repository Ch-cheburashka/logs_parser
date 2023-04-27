#ifndef LOGS_PARSER_LOGS_PARSER_HPP
#define LOGS_PARSER_LOGS_PARSER_HPP

#include <string>
#include <filesystem>
#include <utility>
#include <vector>
#include <memory>

struct log_info {
    std::string info;
    std::string interval;
    std::string level;
};

class i_logs_parser {
public:
    virtual ~i_logs_parser() = default;
    virtual std::vector<log_info> parse (const std::filesystem::path& path_to_logs) = 0;
};

class logs_parser {
public:
    explicit logs_parser(std::filesystem::path path) : _path(std::move(path)) {}
    void set_parser(std::unique_ptr<i_logs_parser> p) {
        _parser.swap(p);
    }
    std::vector<log_info> parse () {
        return _parser->parse(_path);
    }
    ~logs_parser() = default;
private:
    std::filesystem::path _path;
    std::unique_ptr<i_logs_parser> _parser;
};

class file_parser : public i_logs_parser {
    std::vector<log_info> parse (const std::filesystem::path& path_to_logs) override;
};

class dir_parser : public i_logs_parser {
    std::vector<log_info> parse (const std::filesystem::path& path_to_logs) override;
};

#endif //LOGS_PARSER_LOGS_PARSER_HPP
