/*
 * Copyright 2023 Norbert Takacs
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once
#include <string>
#include <list>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <regex>
#include "../NavPoint.h"
#include "../Airport.h"
#include "../RNAVProc.h"

const std::string RE_FLOAT = "([+-]*[0-9\\.]+)";
const std::string RE_INT = "([+-]*[0-9]+)";
const std::string RE_ID = "([a-zA-Z0-9\\/_\\-]+)";
const std::string RE_ID_S = "([a-zA-Z0-9\\/_\\-\\s]+)"; // ID with space
const std::string RE_TEXT = "(.+)";

//RWY:RW13L,     ,      ,00496, ,BPL ,2,   ;N47264352,E019152718,0000;
//RWY:RW17L,+0080,      ,00283, ,    , ,   ;N46412110,E021094215,0000;
//RWY:RW13R,     ,      ,00448, ,FER ,3,   ;N47265534,E019131473,0000;
//RWY:RW04R,+0080,      ,00355, ,DCN ,1,   ;N47285299,E021361079,0000;
//RWY:RW08 ,-0198,      ,01907, ,OEJ ,0,   ;N47153197,E011195411,0197;
//RWY:RW26, +0198, , 01894, , OEV, 1, ; N47154182, E011212524, 0000;
const std::string APT_RWY_LINE ="RWY:RW" +
								RE_ID_S + "," + //[1] RWY name
								".+" + "," +
								".+" + "," +
								"\\s*"+RE_INT + "," + //[2] elevation
								".+" + "," +
								RE_ID_S + "," + //[3] ???
								".+" + "," +
								".+" + ";" +
								RE_ID_S + "," + //[4] lat
								RE_ID_S + "," + //[5] lng
								".+;";

//SID:010,5,NARK6D,RW04R,DC008,LH,P,C,EY  , ,   ,DF, , , , , ,      ,    ,    ,    ,    ,-,03000,     ,10000,-,230,    ,   ,LHDC,LH,P,A, , , , ;
//SID:010,5,BADO2J,RW13R,BP711,LH,P,C,EY  , ,   ,DF, , , , , ,      ,    ,    ,    ,    , ,     ,     ,10000,-,230,    ,   ,LHBP,LH,P,A, , , , ;
//STAR:020,5,NARK6A,ALL,DC016,LH,P,C,E   , ,   ,TF, , , , , ,      ,    ,    ,    ,    ,B,09000,05000,     , ,   ,    ,   , , , , , , , , ;
//SID:040,5,DUZL2B,RW13L,DUZLA,LH,E,A,EEC , ,   ,TF, , , , , ,      ,    ,    ,    ,    , ,     ,     ,     , ,   ,    ,   , , , , , , , , ;
//APPCH:030, I, I04R, , RW04R, LH, P, G, G  M, , , CF, , DCN, LH, P, I, , 2229, 0015, 0430, 0064, , 00410, , , , , -300, , , , , , , 0, D, S;
//APPCH:010,A,I13L,CATUZ,CATUZ,LH,P,C,E  A, ,   ,IF, , , , , ,      ,    ,    ,    ,    ,+,05000,     ,     ,-,230,    ,   , , , , , ,0,N,S;
//APPCH:010,A,I31L,ATICO,ATICO,LH,P,C,E  A, ,   ,IF, , , , , ,      ,    ,    ,    ,    ,+,04000,     ,     ,-,230,    ,   , , , , , ,0,D,S;
const std::string APT_PROC_LINE = "(SID|STAR|APPCH):([0-9]+)," +
									RE_ID_S + "," + //3: not used/approach type
									RE_ID_S + "," + //4: Proc ID
									RE_ID_S + "," + //5: RWY name/Transition
									RE_ID_S + "," + //6: Nav point ID
									RE_ID_S + "," + //7: Region ID
									".+;";

class XPlaneParser {
private:
	std::list<NavPoint> _nav_points;
	std::list<Airport> _airports;
	std::list<RNAVProc> _rnav_procs;
	std::string xplane_root_folder;
	std::list<std::string> _airport_files_parsed;
	std::filesystem::path absolute_path(std::string root_folder, std::string nav_folder, std::string file_name);
	void trim_line(std::string& line);
	void parse_rwy_line(std::cmatch& m, Airport* apt_ptr);
	void parse_proc_line(std::cmatch& m, std::string airport_iaco_id);
	void parse_approach_proc_line(std::cmatch& m, std::string airport_iaco_id);
	bool parse_airport_file(std::string airport_icao_code);
	Airport* get_airport_ptr(std::string airport_icao_code);
public:
	XPlaneParser(std::string _xplane_root_folder);
	bool parse_earth_fix_dat_file();
	bool parse_earth_nav_dat_file();
	std::list<std::string> get_list_of_airport_iaco_codes();
	std::list<NavPoint> get_nav_points_by_icao_id(std::string icao_id);
	std::list<NavPoint> get_nav_points_by_icao_id(std::string region, std::string icao_id);
	bool get_airport_by_icao_id(std::string icao_id, Airport& _airport);
	bool get_procedure_by_id(std::string proc_name, std::string airport_icao, RNAVProc& proc);
	std::list<RNAVProc> get_rnav_procs_by_airport_icao_id(std::string icao_id);
	std::list<NavPoint>& get_nav_points();
};
