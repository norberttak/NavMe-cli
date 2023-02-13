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
#include "NavMeLib.h"
#include "CommandParser.h"

int main(void) {
    std::string line;
    std::cout << "NavMe CLI. built at " << __DATE__ <<" " << __TIME__ << std::endl;
    std::cmatch m;

    const std::string config_file_name = "navme-cli.ini";
    if (!GlobalOptions::get_instance()->load_options_from_file(config_file_name))
    {
        GlobalOptions::get_instance()->set_option("ANGLE_FORMAT", "ANGLE_DEG_DECMIN");
        GlobalOptions::get_instance()->set_option("XPLANE_ROOT", "C:\\XPlane 12");
        std::cout << config_file_name << " config file doesn't exist. creating one." << std::endl;
        GlobalOptions::get_instance()->save_options_to_file(config_file_name);
    }
    
    std::string xplane_root;
    GlobalOptions::get_instance()->get_option("XPLANE_ROOT", xplane_root);
    std::cout << "Navigation data source: " << xplane_root << "\\Custom Data" << std::endl;

    XPlaneParser navdata_parser(xplane_root);

    std::cout << "Parse earth nav data: ";
    navdata_parser.parse_earth_nav_dat_file();
    std::cout << "DONE" << std::endl;

    std::cout << "Parse earth fix data: ";
    navdata_parser.parse_earth_fix_dat_file();
    std::cout << "DONE" << std::endl;
    
    std::cout << "Nr of navigation point: " << navdata_parser.get_nav_points().size() << std::endl;

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
            cmd_parser.parse_and_dispatch_command(line);
        }
        catch (...) {
            std::cout << "error occurred while parsing command: " << line << std::endl;
        }

        std::cout << "# ";        
    }
    return EXIT_SUCCESS;
}