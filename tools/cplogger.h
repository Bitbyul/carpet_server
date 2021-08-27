#ifndef CARPET_SERVER_CPLOGGER_H
#define CARPET_SERVER_CPLOGGER_H

#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup.hpp>
#include <iostream>

static const std::string COMMON_FMT("[%TimeStamp%][%Severity%]:  %Message%");

class Logger {

public:

    static void init_log();

    template <typename T>
    static void concat_all(std::ostringstream& ss, T arg) {
        ss << arg;
    }

    template <typename T, typename... Types>
    static void concat_all(std::ostringstream& ss, T arg, Types... args) {
        ss << arg;
        concat_all(ss, args...);
    }

    template <typename... Types>
    static void i(Types... args){
        std::ostringstream ss;

        concat_all(ss, args...);
        std::cout << ss.str() << "\n";

        //BOOST_LOG_TRIVIAL(info) <<ss.str();
    }

    template <typename... Types>
    static void e(Types... args){
        std::ostringstream ss;

        concat_all(ss, args...);
        std::cout << ss.str() << "\n";

        //BOOST_LOG_TRIVIAL(error) <<ss.str();
    }

private:
    Logger(){

    };
};



#endif //CAR_CPLOGGER_H
