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
#include <fstream>
#include <filesystem>
#include <regex>
#include "NavMeLib.h"

class CommandParser;

#define RE_HTML_TEMPLATE_TITLE "%%_TITLE_%%"
#define RE_HTML_TEMPLATE_DEPARTURE "%%_DEPARTURE_%%"
#define RE_HTML_TEMPLATE_DESTINATION "%%_DESTINATION_%%"
#define RE_HTML_TEMPLATE_DIRECT_DIST_GREAT "%%_DIRECT_DIST_GREAT_%%"
#define RE_HTML_TEMPLATE_DIRECT_DIST_RUMB "%%_DIRECT_DIST_RUMB_%%"
#define RE_HTML_TEMPLATE_TOTAL_ROUTE_LENGTH "%%_TOTAL_ROUTE_LENGTH_%%"
#define RE_HTML_TEMPLATE_TABLE_BODY_NAV_POINTS "%%_TABLE_BODY_NAV_POINTS_%%"
#define RE_HTML_TEMPLATE_NAV_POINT_COORDS "%%_NAV_POINT_COORDS_%%"
#define RE_HTML_TEMPLATE_DATE_TIME "%%_DATE_TIME_%%"

struct HtmlTemplateParameters {
	std::string title;
	std::string departure;
	std::string destination;
	std::string direct_dist_great;
	std::string direct_dist_rumb;
	std::string total_route_length;
	std::string table_body_nav_points;
	std::string table_nav_point_coords;
};

typedef bool (CommandParser::*f_command_handler)(std::string main_cmd, std::string sub_cmd, std::vector<std::string> parameters);

class CommandParser {
private:
	XPlaneParser& navdata_parser;
	FlightRoute flight_route;
	std::map<std::string, f_command_handler> command_handlers;
	bool get_and_remove_parameter_name_value(std::string param_name, std::vector<std::string>& parameters, std::string& out_value);
	bool get_and_remove_parameter_name(std::string param_name, std::vector<std::string>& parameters);
	std::string strip_nav_point_name(std::string nav_point_name);
	bool create_html_based_on_template(std::string template_file_name, std::string html_file_name, HtmlTemplateParameters& template_params);
	bool handle_export_flight_plan_html(std::string main_cmd, std::string sub_cmd, std::vector<std::string> parameters);

	bool handle_help(std::string sub_command);

	bool handle_show_flight_plan(std::string main_cmd, std::string sub_cmd, std::vector<std::string> parameters);
	bool handle_show_direct(std::string main_cmd, std::string sub_cmd, std::vector<std::string> parameters);
	bool handle_show_option(std::string main_cmd, std::string sub_cmd, std::vector<std::string> parameters);
	bool handle_show_info(std::string main_cmd, std::string sub_cmd, std::vector<std::string> parameters);
	bool handle_show_metar_taf(std::string main_cmd, std::string sub_cmd, std::vector<std::string> parameters);

	bool handle_export_flight_plan(std::string main_cmd, std::string sub_cmd, std::vector<std::string> parameters);
	bool handle_save_flight_plan(std::string main_cmd, std::string sub_cmd, std::vector<std::string> parameters);
	bool handle_load_flight_plan(std::string main_cmd, std::string sub_cmd, std::vector<std::string> parameters);
	bool handle_load_simbrief(std::string main_cmd, std::string sub_cmd, std::vector<std::string> parameters);

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