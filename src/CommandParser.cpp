/*
 * Copyright 2023 Norbert Takacs
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include <regex>
#include "CommandParser.h"

#define KM_TO_NM 0.5399568

bool CommandParser::handle_show_flight_plan(std::string main_cmd, std::string sub_cmd, std::vector<std::string> parameters)
{
	std::cout << "#########################################" << std::endl;
	std::cout << "departure:  " << flight_route.departure_airport.get_name() << flight_route.departure_airport.get_icao_id() << std::endl;
	std::cout << "destination:" << flight_route.destination_airport.get_name() << flight_route.destination_airport.get_icao_id() << std::endl;

	if (flight_route.departure_airport.get_icao_id() != "" && flight_route.destination_airport.get_icao_id() != "") {
		RelativePos rel_pos;
		Coordinate dest_coordinate = flight_route.destination_airport.get_coordinate();
		flight_route.departure_airport.get_coordinate().get_relative_pos_to(dest_coordinate, rel_pos);
		std::cout << "direct distance orthodrom: " << (int)rel_pos.dist_ortho << " km, " << (int)(rel_pos.dist_ortho * KM_TO_NM) << " nm" << std::endl;
		std::cout << "direct distance loxodrom: " << (int)rel_pos.dist_loxo << " km, " << (int)(rel_pos.dist_loxo * KM_TO_NM) << " nm" << std::endl;
	}
	std::cout << "#########################################" << std::endl;

	std::cout << "sid: " << flight_route.sid.get_name() << " " << flight_route.sid.get_runway_name() << std::endl;
	for (auto& np : flight_route.sid.get_nav_point_ids())
	{
		std::list<NavPoint> nav_points = navdata_parser.get_nav_points_by_icao_id(flight_route.sid.get_region(), np);
		if (nav_points.size() == 0)
			continue;
		std::cout << "  " << nav_points.front().get_icao_id() << " " << nav_points.front().get_coordinate().to_string() << std::endl;
	}

	std::cout << "enroute: " << std::endl;
	for (auto& np : flight_route.enroute_points)
	{
		std::cout << "  " << np.get_icao_id() << " "
			<< np.get_coordinate().to_string() << " "
			<< np.get_name() << " "
			<< (np.get_radio_frequency() == 0 ? "" : std::to_string(np.get_radio_frequency()))
			<< std::endl;
	}

	std::cout << "star: " << flight_route.star.get_name() << " " << flight_route.star.get_runway_name() << std::endl;
	for (auto& np : flight_route.star.get_nav_point_ids())
	{
		std::list<NavPoint> nav_points = navdata_parser.get_nav_points_by_icao_id(flight_route.star.get_region(), np);
		if (nav_points.size() == 0)
			continue;
		std::cout << "  " << nav_points.front().get_icao_id() << " " << nav_points.front().get_coordinate().to_string() << std::endl;
	}

	std::cout << "approach: " << flight_route.approach.get_name() << " " << flight_route.approach.get_runway_name() << std::endl;
	for (auto& np : flight_route.approach.get_nav_point_ids())
	{
		std::list<NavPoint> nav_points = navdata_parser.get_nav_points_by_icao_id(flight_route.approach.get_region(), np);
		if (nav_points.size() == 0)
			continue;
		std::cout << "  " << nav_points.front().get_icao_id() << " " << nav_points.front().get_coordinate().to_string() << std::endl;
	}

	return true;
}

bool CommandParser::handle_show_direct(std::string main_cmd, std::string sub_cmd, std::vector<std::string> parameters)
{
	if (parameters.size() < 2)
	{
		std::cout << "error: invalid command syntax. show direct <id1> <id2>" << std::endl;
		return false;
	}

	Coordinate coord1, coord2;
	std::list<NavPoint> nav_1_points = navdata_parser.get_nav_points_by_icao_id(parameters[0]);
	if (nav_1_points.size() == 0)
	{
		Airport airport;
		if (!navdata_parser.get_airport_by_icao_id(parameters[0], airport))
		{
			std::cout << "unknown airport icao id " << parameters[0] << std::endl;
			return false;
		}
		coord1 = airport.get_coordinate();
	}
	else
	{
		coord1 = nav_1_points.front().get_coordinate();
	}

	std::list<NavPoint> nav_2_points = navdata_parser.get_nav_points_by_icao_id(parameters[1]);
	if (nav_2_points.size() == 0)
	{
		Airport airport;
		if (!navdata_parser.get_airport_by_icao_id(parameters[1], airport))
		{
			std::cout << "unknown airport icao id " << parameters[1] << std::endl;
			return false;
		}
		coord2 = airport.get_coordinate();
	}
	else
	{
		coord2 = nav_2_points.front().get_coordinate();
	}

	RelativePos realative_pos;
	coord1.get_relative_pos_to(coord2, realative_pos);
	std::cout << "loxodrom (rhumb line) route:" << std::endl;
	std::cout << "  heading " << realative_pos.heading_loxo.to_string(false) << std::endl;
	std::cout << "  distance " << realative_pos.dist_loxo << " km   " << (int)(KM_TO_NM * realative_pos.dist_loxo) << " nm" << std::endl;
	std::cout << "orthodrom (great circle) route:" << std::endl;
	std::cout << "  departure heading " << realative_pos.heading_ortho_departure.to_string(false) << "  arrival heading " << realative_pos.heading_ortho_arrival.to_string(false) << std::endl;
	std::cout << "  distance " << realative_pos.dist_ortho << " km   " << (int)(KM_TO_NM * realative_pos.dist_ortho) << " nm" << std::endl;
	return true;
}

bool CommandParser::handle_show_option(std::string main_cmd, std::string sub_cmd, std::vector<std::string> parameters)
{
	if (parameters.size() == 0)
	{
		std::cout << "error: show option <option_name>" << std::endl;
		return false;
	}

	std::string value;

	if (parameters[0] == "--ALL")
	{
		std::list<std::string> option_names = GlobalOptions::get_instance()->get_all_option_name();
		for (auto& key : option_names)
		{
			GlobalOptions::get_instance()->get_option(key, value);
			std::cout << key << " : " << value << std::endl;
		}
		return true;
	}

	if (!GlobalOptions::get_instance()->get_option(parameters[0], value))
	{
		std::cout << "error: unknown option name " << parameters[0] << std::endl;
		return false;
	}

	std::cout << parameters[0] << " : " << value << std::endl;
	return true;
}

bool CommandParser::handle_show_info(std::string main_cmd, std::string sub_cmd, std::vector<std::string> parameters)
{
	if (parameters.size() == 0)
	{
		std::cout << "error: invalid command syntax. show info <id>" << std::endl;
		return false;
	}

	std::list<NavPoint> nav_points = navdata_parser.get_nav_points_by_icao_id(parameters[0]);
	for (NavPoint& np : nav_points)
	{
		std::cout << "Nav point: " << np.get_icao_id() << " " << np.get_icao_region() << " " << np.get_coordinate().to_string() << std::endl;
		switch (np.get_radio_type())
		{
		case NavPoint::RadioNavType::VOR:
			std::cout << "  VOR: " << np.get_radio_frequency() << " MHz" << std::endl;
			break;
		case NavPoint::RadioNavType::VOR_DME:
			std::cout << "  VOR+DME: " << np.get_radio_frequency() << " MHz" << std::endl;
			break;
		case NavPoint::RadioNavType::NDB:
			std::cout << "  NDB: " << np.get_radio_frequency() << " kHz" << std::endl;
			break;
		case NavPoint::RadioNavType::DME:
			std::cout << "  DME: " << np.get_radio_frequency() << " MHz" << std::endl;
			break;
		default:
			break;
		}
	}

	Airport airport;
	if (navdata_parser.get_airport_by_icao_id(parameters[0], airport))
	{
		std::cout << "airport: " << airport.get_icao_id() << " " << airport.get_icao_region() << " " << airport.get_coordinate().to_string() << std::endl;
		for (Runway& rwy : airport.get_runways())
		{
			std::cout << "  " << rwy.get_name() << " " << rwy.get_course() << " " << rwy.get_length() << " ils: " << rwy.get_ils_freq() << std::endl;
		}
	}
	return true;
}

bool CommandParser::handle_set_option(std::string main_cmd, std::string sub_cmd, std::vector<std::string> parameters)
{
	if (parameters.size() < 2)
	{
		std::cout << "error: not enough parameter" << std::endl;
		return false;
	}

	if (!GlobalOptions::get_instance()->set_option(parameters[0], parameters[1]))
	{
		std::cout << "error: set option" << std::endl;
		return false;
	}

	return true;
}

bool CommandParser::handle_set_departure(std::string main_cmd, std::string sub_cmd, std::vector<std::string> parameters)
{
	if (parameters.size() < 1)
	{
		std::cout << "wrong command. syntax: set dep <airport_icao>" << std::endl;
		return false;
	}
	std::cout << "departure airport: " << parameters[0] << std::endl;

	if (navdata_parser.get_airport_by_icao_id(parameters[0], flight_route.departure_airport) == true)
	{
		std::cout << "  name: " << flight_route.departure_airport.get_name() << ", region: " << flight_route.departure_airport.get_icao_region() << std::endl;
		std::cout << "  magnetic variation: " << flight_route.departure_airport.get_magnetic_variation().to_string(false) << std::endl;
		std::cout << "  " << flight_route.departure_airport.get_coordinate().to_string() << std::endl;
		return true;
	}

	std::cout << "unknown airport: " << parameters[0] << std::endl;
	return false;
}

bool CommandParser::handle_set_dest(std::string main_cmd, std::string sub_cmd, std::vector<std::string> parameters)
{
	if (parameters.size() < 1)
	{
		std::cout << "wrong command. syntax: set dest <airport_icao>" << std::endl;
		return false;
	}
	std::cout << "destination airport: " << parameters[0] << std::endl;

	if (navdata_parser.get_airport_by_icao_id(parameters[0], flight_route.destination_airport) == true)
	{
		std::cout << "  name: " << flight_route.destination_airport.get_name() << ", region: " << flight_route.destination_airport.get_icao_region() << std::endl;
		std::cout << "  magnetic variation: " << flight_route.destination_airport.get_magnetic_variation().to_string(false) << std::endl;
		std::cout << "  " << flight_route.destination_airport.get_coordinate().to_string() << std::endl;

		return true;
	}

	std::cout << "unknown airport: " << parameters[0] << std::endl;
	return false;
}

bool CommandParser::handle_set_sid(std::string main_cmd, std::string sub_cmd, std::vector<std::string> parameters)
{
	if (parameters.size() < 1)
	{
		std::cout << "wrong command. syntax: set sid <sid id>" << std::endl;
		return false;
	}

	if (flight_route.departure_airport.get_icao_id() == "")
	{
		std::cout << "error: no departure airport set" << std::endl;
		return false;
	}

	std::cout << "SID: " << parameters[0] << std::endl;
	RNAVProc sid;

	if (navdata_parser.get_procedure_by_id(parameters[0], flight_route.departure_airport.get_icao_id(), sid))
	{
		if (sid.get_type() != RNAVProc::RNAV_SID)
		{
			std::cout << "error: " << parameters[0] << " is not a SID procedure!" << std::endl;
			return false;
		}
		flight_route.sid = sid;

		std::vector<std::string> nav_points = flight_route.sid.get_nav_point_ids();
		for (auto& navp : nav_points)
		{
			std::cout << "  " << navp << ":";
			std::list<NavPoint> sid_points = navdata_parser.get_nav_points_by_icao_id(navp);
			if (sid_points.size() > 0)
				std::cout << sid_points.front().get_coordinate().to_string();
			std::cout << std::endl;
		}
		return true;
	}

	std::cout << "unknown SID " << parameters[0] << std::endl;
	return false;
}

bool CommandParser::handle_set_star(std::string main_cmd, std::string sub_cmd, std::vector<std::string> parameters)
{
	if (parameters.size() < 1)
	{
		std::cout << "wrong command. syntax: set star <star id>" << std::endl;
		return false;
	}

	if (flight_route.destination_airport.get_icao_id() == "")
	{
		std::cout << "error: no destination airport set" << std::endl;
		return false;
	}

	std::cout << "STAR: " << parameters[0] << std::endl;
	RNAVProc star;

	if (navdata_parser.get_procedure_by_id(parameters[0], flight_route.destination_airport.get_icao_id(), star))
	{
		if (star.get_type() != RNAVProc::RNAV_STAR)
		{
			std::cout << "error: " << parameters[0] << " is not a STAR procedure!" << std::endl;
			return false;
		}
		flight_route.star = star;

		std::vector<std::string> nav_points = flight_route.star.get_nav_point_ids();
		for (auto& navp : nav_points)
		{
			std::cout << "  " << navp << ":";
			std::list<NavPoint> star_points = navdata_parser.get_nav_points_by_icao_id(navp);
			if (star_points.size() > 0)
				std::cout << star_points.front().get_coordinate().to_string();
			std::cout << std::endl;
		}
		return true;
	}

	std::cout << "unknown STAR " << parameters[0] << std::endl;
	return false;
}

bool CommandParser::handle_set_app(std::string main_cmd, std::string sub_cmd, std::vector<std::string> parameters)
{
	if (parameters.size() < 1)
	{
		std::cout << "wrong command. syntax: set app <approach id>" << std::endl;
		return false;
	}

	if (flight_route.destination_airport.get_icao_id() == "")
	{
		std::cout << "error: no destination airport set" << std::endl;
		return false;
	}

	std::cout << "APP: " << parameters[0] << std::endl;
	RNAVProc app;

	if (navdata_parser.get_procedure_by_id(parameters[0], flight_route.destination_airport.get_icao_id(), app))
	{
		if (app.get_type() != RNAVProc::RNAV_APPROACH)
		{
			std::cout << "error: " << parameters[0] << " is not a APPROACH procedure!" << std::endl;
			return false;
		}
		flight_route.approach = app;

		std::vector<std::string> nav_points = flight_route.approach.get_nav_point_ids();
		for (auto& navp : nav_points)
		{
			std::cout << "  " << navp << ":";
			std::list<NavPoint> app_points = navdata_parser.get_nav_points_by_icao_id(navp);
			if (app_points.size() > 0)
				std::cout << app_points.front().get_coordinate().to_string();
			std::cout << std::endl;
		}
		return true;
	}

	std::cout << "unknown APPROACH " << parameters[0] << std::endl;
	return false;
}

bool CommandParser::get_and_remove_parameter_value(std::string param_name, std::vector<std::string>& parameters, std::string& out_value)
{
	for (std::vector<std::string>::iterator it = parameters.begin(); it < parameters.end(); it++)
	{
		if (*it == param_name && (it + 1) != parameters.end())
		{
			out_value = *(it + 1);

			parameters.erase(it, it + 2);

			return true;
		}
	}
	return false;
}

bool CommandParser::handle_route_add(std::string main_cmd, std::string sub_cmd, std::vector<std::string> parameters)
{
	// remove any --after or --before option from parameter list
	std::string insert_pos_before_string = "";
	std::string insert_pos_after_string = "";
	get_and_remove_parameter_value("--BEFORE", parameters, insert_pos_before_string);
	get_and_remove_parameter_value("--AFTER", parameters, insert_pos_after_string);

	return handle_route_insert(main_cmd, sub_cmd, parameters);
}

std::string CommandParser::strip_nav_point_name(std::string nav_point_name)
{
	size_t index_of_slash = nav_point_name.find_first_of('/');
	if (index_of_slash == std::string::npos)
		return nav_point_name;
	else
		return nav_point_name.substr(0, index_of_slash);
}

bool CommandParser::handle_route_insert(std::string main_cmd, std::string sub_cmd, std::vector<std::string> parameters)
{
	std::string region = "";
	std::string insert_pos_before_string = "";
	std::string insert_pos_after_string = "";
	int insert_index = -1;

	get_and_remove_parameter_value("--REGION", parameters, region);
	get_and_remove_parameter_value("--BEFORE", parameters, insert_pos_before_string);
	get_and_remove_parameter_value("--AFTER", parameters, insert_pos_after_string);

	if (insert_pos_before_string != "")
	{
		int insert_pos_before = std::stoi(insert_pos_before_string);
		if (insert_pos_before >= flight_route.enroute_points.size())
		{
			std::cout << "error: invalid index " << insert_pos_after_string << std::endl;
			return false;
		}
		insert_index = insert_pos_before;

	}
	else if (insert_pos_after_string != "")
	{
		int insert_pos_after = std::stoi(insert_pos_after_string);
		if (insert_pos_after >= flight_route.enroute_points.size())
		{
			std::cout << "error: invalid index " << insert_pos_after_string << std::endl;
			return false;
		}
		insert_index = insert_pos_after + 1;
	}
	else
	{
		insert_index = -1; // use append instead of insert
	}

	std::vector<NavPoint> nav_points_to_add;
	for (std::string nav_point_id : parameters)
	{
		if (nav_point_id == "DCT")
			continue;

		nav_point_id = strip_nav_point_name(nav_point_id);
		std::list<NavPoint> nav_points;
		if (region != "")
			nav_points = navdata_parser.get_nav_points_by_icao_id(region, nav_point_id);
		else
			nav_points = navdata_parser.get_nav_points_by_icao_id(nav_point_id);

		if (nav_points.size() == 0)
		{
			std::cout << "error: unknown nav point " << nav_point_id << std::endl;
			return false;
		}

		if (nav_points.size() > 1)
		{
			std::cout << "multiple nav points found. try to use \'--region <code>\' " << std::endl;
			for (auto& np : nav_points)
			{
				std::cout << "id:" << np.get_icao_id() << " name:" << np.get_name() << " region:" << np.get_icao_region() << std::endl;
			}
			return true;
		}

		nav_points_to_add.emplace_back(nav_points.front());
	}

	std::vector<NavPoint>::iterator insert_iterator;
	if (insert_index == -1)
		insert_iterator = flight_route.enroute_points.end();
	else
		insert_iterator = std::next(flight_route.enroute_points.begin(), insert_index);

	flight_route.enroute_points.insert(insert_iterator, nav_points_to_add.begin(), nav_points_to_add.end());

	return true;
}

bool CommandParser::handle_route_remove(std::string main_cmd, std::string sub_cmd, std::vector<std::string> parameters)
{
	if (parameters.size() != 1)
	{
		std::cout << "error: use 'route remove index' or 'route remove index1:index2'" << std::endl;
		return false;
	}

	std::vector<NavPoint>::iterator erase_iterator_start;
	std::vector<NavPoint>::iterator erase_iterator_end;
	size_t index_of_colon = parameters[0].find_first_of(':');

	if (index_of_colon != std::string::npos)
	{
		erase_iterator_start = std::next(flight_route.enroute_points.begin(), std::stoi(parameters[0].substr(0, index_of_colon)));
		erase_iterator_end = std::next(flight_route.enroute_points.begin(), std::stoi(parameters[0].substr(index_of_colon + 1)) + 1);
	}
	else
	{
		erase_iterator_start = std::next(flight_route.enroute_points.begin(), std::stoi(parameters[0]));
		erase_iterator_end = std::next(flight_route.enroute_points.begin(), std::stoi(parameters[0]) + 1);
	}

	flight_route.enroute_points.erase(erase_iterator_start, erase_iterator_end);

	return true;
}

bool CommandParser::handle_list_rnav(std::string main_cmd, std::string sub_cmd, std::vector<std::string> parameters)
{
	if (parameters.size() < 1)
	{
		// if departure or destination airport is set AND no airport in the parameters try to use
		// the dep or dest airport as default
		if (flight_route.departure_airport.get_icao_id() != "" && sub_cmd == "SID")
			parameters.push_back(flight_route.departure_airport.get_icao_id());
		else if (flight_route.destination_airport.get_icao_id() != "" && (sub_cmd == "STAR" || sub_cmd == "APP"))
			parameters.push_back(flight_route.destination_airport.get_icao_id());
		else
		{
			std::cout << "wrong command. syntax: list star <icao_id>" << std::endl;
			return false;
		}
	}

	std::list<RNAVProc> rnav_procs = navdata_parser.get_rnav_procs_by_airport_icao_id(parameters[0]);
	if (rnav_procs.size() == 0)
	{
		std::cout << "error: can't find rnav proc for airport " << parameters[0];
		return false;
	}

	for (auto& proc : rnav_procs) {
		if (sub_cmd == "SID" && proc.get_type() != RNAVProc::RNAVProcType::RNAV_SID)
			continue;
		if (sub_cmd == "STAR" && proc.get_type() != RNAVProc::RNAVProcType::RNAV_STAR)
			continue;
		if (sub_cmd == "APP" && proc.get_type() != RNAVProc::RNAVProcType::RNAV_APPROACH)
			continue;

		std::cout << proc.get_name() << " " << proc.get_runway_name() << std::endl;
	}

	return true;
}

CommandParser::CommandParser(XPlaneParser& _navdata_parser) :
	navdata_parser(_navdata_parser), flight_route(FlightRoute(""))
{
	command_handlers["SHOW__FLIGHT_PLAN"] = &CommandParser::handle_show_flight_plan;
	command_handlers["SHOW__DIRECT"] = &CommandParser::handle_show_direct;
	command_handlers["SHOW__INFO"] = &CommandParser::handle_show_info;
	command_handlers["SHOW__OPTION"] = &CommandParser::handle_show_option;

	command_handlers["SET__OPTION"] = &CommandParser::handle_set_option;
	command_handlers["SET__DEP"] = &CommandParser::handle_set_departure;
	command_handlers["SET__DEST"] = &CommandParser::handle_set_dest;
	command_handlers["SET__SID"] = &CommandParser::handle_set_sid;
	command_handlers["SET__STAR"] = &CommandParser::handle_set_star;
	command_handlers["SET__APP"] = &CommandParser::handle_set_app;

	command_handlers["ROUTE__INSERT"] = &CommandParser::handle_route_insert;
	command_handlers["ROUTE__ADD"] = &CommandParser::handle_route_add;
	command_handlers["ROUTE__REMOVE"] = &CommandParser::handle_route_remove;

	command_handlers["LIST__SID"] = &CommandParser::handle_list_rnav;
	command_handlers["LIST__STAR"] = &CommandParser::handle_list_rnav;
	command_handlers["LIST__APP"] = &CommandParser::handle_list_rnav;
}

bool CommandParser::parse_and_dispatch_command(std::string line)
{
	std::transform(line.begin(), line.end(), line.begin(), ::toupper);

	std::regex re("\\s+");
	std::sregex_token_iterator it{ line.begin(), line.end(), re, -1 };
	std::vector<std::string> tokenized{ it, {} };

	// remove empty tokens
	for (std::vector<std::string>::iterator st_it = tokenized.begin(); st_it != tokenized.end(); st_it++)
	{
		if (st_it->size() == 0)
			tokenized.erase(st_it);
	}

	if (tokenized.size() > 1)
	{
		std::vector<std::string> parameters(tokenized.begin() + 2, tokenized.end());

		f_command_handler f_handler = command_handlers[tokenized[0] + "__" + tokenized[1]];

		if (f_handler == NULL)
		{
			std::cout << "unknown command: " << line << std::endl;
			return false;
		}

		(this->*f_handler)(tokenized[0], tokenized[1], parameters);
	}
	return true;
}