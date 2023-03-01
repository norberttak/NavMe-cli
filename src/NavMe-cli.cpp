/*
 * Copyright 2023 Norbert Takacs
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
//#include <ftxui/dom/elements.hpp>
//#include <ftxui/screen/screen.hpp>
#include <iostream>
#include <vector>
#include <regex>
#include "Logger.h"
#include "NavMeLib.h"
#include "CommandParser.h"

int main(void) {
    std::string line;

    std::ifstream banner("banner.txt");
    if (banner.good())
    {
        std::stringstream buffer;
        buffer << banner.rdbuf();
        std::cout << buffer.str() << std::endl;
        banner.close();
    }

    std::cout << "NavMe CLI. built at " << __DATE__ <<" " << __TIME__ << std::endl;
    std::cout << "navMe LIB version: " << NAVME_LIB_VERSION << std::endl;
    std::cmatch m;

    const std::string config_file_name = "navme-cli.ini";
    if (!GlobalOptions::get_instance()->load_options_from_file(config_file_name))
    {
        GlobalOptions::get_instance()->set_option("ANGLE_FORMAT", "ANGLE_DEG_DECMIN");
        GlobalOptions::get_instance()->set_option("XPLANE_ROOT", "C:\\XPlane 12");
        GlobalOptions::get_instance()->set_option("LOG_LEVEL", "INFO");
        std::cout << config_file_name << " config file doesn't exist. creating one." << std::endl;
        GlobalOptions::get_instance()->save_options_to_file(config_file_name);
    }
    
    std::string xplane_root;
    GlobalOptions::get_instance()->get_option("XPLANE_ROOT", xplane_root);
    std::cout << "Navigation data source: " << xplane_root << "\\Custom Data" << std::endl;
    Logger(logINFO) << "CLI: Navigation data source: " << xplane_root << "\\Custom Data" << std::endl;

    if (!std::filesystem::exists(std::filesystem::path(xplane_root + "\\Custom Data")))
    {
        std::cout << "error: navigation data source can't open. Check XPLANE_ROOT in config file!" << std::endl;
    }

    std::string log_level_str;
    GlobalOptions::get_instance()->get_option("LOG_LEVEL", log_level_str);
    if (log_level_str == "ERROR")
        Logger(logINFO).set_log_level(logERROR);
    else if (log_level_str == "WARNING")
        Logger(logINFO).set_log_level(logWARNING);
    else if (log_level_str == "INFO")
        Logger(logINFO).set_log_level(logINFO);
    else if (log_level_str == "DEBUG")
        Logger(logINFO).set_log_level(logDEBUG);
    else if (log_level_str == "TRACE")
            Logger(logINFO).set_log_level(logTRACE);
    else
        Logger(logINFO).set_log_level(logINFO);


    XPlaneParser navdata_parser(xplane_root);

    std::cout << "Parse earth nav data: ";
    navdata_parser.parse_earth_nav_dat_file();
    std::cout << "DONE" << std::endl;

    std::cout << "Parse earth fix data: ";
    navdata_parser.parse_earth_fix_dat_file();
    std::cout << "DONE" << std::endl;
    
    std::cout << "Nr of navigation point: " << navdata_parser.get_nav_points().size() << std::endl;
    Logger(logINFO) << "CLI: Nr of navigation point: " << navdata_parser.get_nav_points().size() << std::endl;

    CommandParser cmd_parser(navdata_parser);

    std::cout << "# ";

    while(std::getline(std::cin, line))
    {
        if (line == "exit" || line == "EXIT")
            break;

        if (line.size() == 0)
        {
            std::cout << "# ";
            continue;
        }

        try {
            Logger(logINFO) << "CLI: command: " << line << std::endl;
            cmd_parser.parse_and_dispatch_command(line);
        }
        catch (...) {
            std::cout << "error occurred while parsing command: " << line << std::endl;
            Logger(logINFO) << "CLI:error occurred while parsing command: " << line << std::endl;
        }

        std::cout << "# ";
    }
    return EXIT_SUCCESS;
}