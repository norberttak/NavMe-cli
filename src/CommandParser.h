/*
 * Copyright 2023 Norbert Takacs
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once
#include <vector>
#include <list>
#include <string>
#include <map>
#include "NavMeLib.h"

class CommandParser;

typedef bool (CommandParser::*f_command_handler)(std::string main_cmd, std::string sub_cmd, std::vector<std::string> parameters);

class CommandParser {
private:
	XPlaneParser& navdata_parser;
	FlightRoute flight_route;
	std::map<std::string, f_command_handler> command_handlers;
	bool get_and_remove_parameter_value(std::string param_name, std::vector<std::string>& parameters, std::string& out_value);
	std::string strip_nav_point_name(std::string nav_point_name);

	bool handle_show_flight_plan(std::string main_cmd, std::string sub_cmd, std::vector<std::string> parameters);
	bool handle_show_direct(std::string main_cmd, std::string sub_cmd, std::vector<std::string> parameters);
	bool handle_show_option(std::string main_cmd, std::string sub_cmd, std::vector<std::string> parameters);
	bool handle_show_info(std::string main_cmd, std::string sub_cmd, std::vector<std::string> parameters);

	bool handle_set_option(std::string main_cmd, std::string sub_cmd, std::vector<std::string> parameters);
	bool handle_set_departure(std::string main_cmd, std::string sub_cmd, std::vector<std::string> parameters);
	bool handle_set_dest(std::string main_cmd, std::string sub_cmd, std::vector<std::string> parameters);
	bool handle_set_sid(std::string main_cmd, std::string sub_cmd, std::vector<std::string> parameters);
	bool handle_set_star(std::string main_cmd, std::string sub_cmd, std::vector<std::string> parameters);
	bool handle_set_app(std::string main_cmd, std::string sub_cmd, std::vector<std::string> parameters);

	bool handle_route_insert(std::string main_cmd, std::string sub_cmd, std::vector<std::string> parameters);
	bool handle_route_add(std::string main_cmd, std::string sub_cmd, std::vector<std::string> parameters);
	bool handle_route_remove(std::string main_cmd, std::string sub_cmd, std::vector<std::string> parameters);

	bool handle_list_rnav(std::string main_cmd, std::string sub_cmd, std::vector<std::string> parameters);
public:
	CommandParser(XPlaneParser& _navdata_parser);
	bool parse_and_dispatch_command(std::string line);
};