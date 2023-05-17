#ifndef LOGS_PARSER_LOGS_PARSER_HPP
#define LOGS_PARSER_LOGS_PARSER_HPP

#include <string>
#include <filesystem>
#include <utility>
#include <list>
#include <memory>
#include <list>
#include <fmt/format.h>

using fmt::format;
using fmt::join;

struct log_info {
    std::string info;
    std::string interval;
    std::string level;

    log_info() = default;

    log_info(const std::string &info, const std::string &interval, const std::string &level) :
            info(info),
            interval(interval),
            level(level) {}
};


class i_logs_parser {
public:
    virtual ~i_logs_parser() = default;

    virtual std::list<log_info> parse(const std::filesystem::path &path_to_logs) = 0;

    static void read_file(const std::filesystem::path &file, std::list<log_info> &logs);
};

class logs_parser {
public:
    explicit logs_parser(std::filesystem::path path) : _path(std::move(path)) {}

    void set_parser(std::unique_ptr<i_logs_parser> p) {
        _parser.swap(p);
    }

    std::list<log_info> parse() {
        return _parser->parse(_path);
    }

    ~logs_parser() = default;

private:
    std::filesystem::path _path;
    std::unique_ptr<i_logs_parser> _parser;
};


class file_parser : public i_logs_parser {
public:
    ~file_parser() override = default;

    std::list<log_info> parse(const std::filesystem::path &path_to_logs) override;
};

class dir_parser : public i_logs_parser {
public:
    ~dir_parser() override = default;

    std::list<log_info> parse(const std::filesystem::path &path_to_logs) override;
};

#endif //LOGS_PARSER_LOGS_PARSER_HPP
