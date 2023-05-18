//
// Created by teufel on 17.05.23.
//

#ifndef INCLUDE_LOGS_PARSER_FILTERS_HPP
#define INCLUDE_LOGS_PARSER_FILTERS_HPP

struct log_info;

class i_log_filter {
public:
    virtual ~i_log_filter() = default;

    virtual bool filter(const log_info &info) = 0;
};

class level_filter : public i_log_filter {
public:
    level_filter(const std::string &log_level) : log_level(log_level) {}

    bool filter(const log_info &info) override {
        return info.level == log_level;
    }

private:
    std::string log_level;
};

class substring_filter : public i_log_filter {
public:
    substring_filter(const std::string &substring) : substring(substring) {}

    bool filter(const log_info &info) override {
        return info.info.find(substring) == std::string::npos;
    }

private:
    std::string substring;
};

class interval_filter : public i_log_filter {
public:
    interval_filter(const std::string &interval) : interval(interval) {
    }

    bool filter(const log_info &info) override {
        return info.interval == interval;
    }

private:
    std::string interval;
};

#endif //INCLUDE_LOGS_PARSER_FILTERS_HPP
