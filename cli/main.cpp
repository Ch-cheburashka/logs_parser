#include <argparse/argparse.hpp>
#include <iostream>
#include <logs_parser/logs_parser.hpp>
#include <logs_parser/filters.hpp>

constexpr char *fileparse_arg = "--fileparse";
constexpr char *dirparse_arg = "--dirparse";
constexpr char *log_level_arg = "--level";
constexpr char *interval_arg = "--interval";
constexpr char *substring_arg = "--substring";

int main(int argc, char **argv) {

    argparse::ArgumentParser opt_desc("logs_parser");

    opt_desc.add_argument(fileparse_arg)
            .default_value(std::string{});

    opt_desc.add_argument(dirparse_arg)
            .default_value(std::string{});

    opt_desc.add_argument(log_level_arg)
            .default_value(std::string{});

    opt_desc.add_argument(interval_arg)
            .nargs(2)
            .default_value(std::vector<std::string>{});

    opt_desc.add_argument(substring_arg)
            .default_value(std::string());

    opt_desc.parse_args(argc, argv);

    std::list<log_info> logs;
    if (opt_desc.is_used(fileparse_arg)) {
        auto file = opt_desc.get<std::string>(fileparse_arg);
        if (std::filesystem::is_directory(file)) {
            throw std::logic_error("the path to the directory was provided");
        }
        if (!file.ends_with(".log")) {
            throw std::logic_error("the file extension is not .log");
        }
        logs_parser logsParser(file);
        logsParser.set_parser(std::make_unique<file_parser>());
        logs = logsParser.parse();
    } else if (opt_desc.is_used(dirparse_arg)) {
        auto dir = opt_desc.get<std::string>(dirparse_arg);
        if (std::filesystem::is_regular_file(dir)) {
            throw std::logic_error("the path to the file was provided");
        }
        logs_parser logsParser(dir);
        logsParser.set_parser(std::make_unique<dir_parser>());
        logs = logsParser.parse();
    }

    std::list<std::unique_ptr<i_log_filter>> filters{};

    if (opt_desc.is_used(log_level_arg)) {
        filters.emplace_back(
                std::make_unique<level_filter>(opt_desc.get<std::string>(log_level_arg))
        );
    }

    if (opt_desc.is_used(interval_arg)) {
        filters.emplace_back(
                std::make_unique<interval_filter>(opt_desc.get<std::vector<std::string>>(interval_arg))
        );
    }
    if (opt_desc.is_used(substring_arg)) {
        filters.emplace_back(
                std::make_unique<substring_filter>(opt_desc.get<std::string>(substring_arg))
        );
    }

    for (auto const &log_entry: logs) {
        if (std::all_of(filters.begin(), filters.end(),
                        [log_entry](auto const& filter) { return filter->filter(log_entry); })) {
            std::cout << log_entry.info << '\n';
        }
    }

    std::cout.flush();
    return 0;
}