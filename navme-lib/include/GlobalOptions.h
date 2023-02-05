/*
 * Copyright 2023 Norbert Takacs
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once
#include <string>
#include <map>
#include <list>
#include <filesystem>
#include "NavMeLibExport.h"

#define OPTION_KEY_ANGLE_FORMAT "angle_format"

struct EXPORT GlobalOptions {
public:
    static GlobalOptions* get_instance();
    bool set_option(std::string option_name, std::string option_value);
    bool get_option(std::string option_name, std::string& option_value);

    std::list<std::string> get_all_option_name();
    bool save_options_to_file(std::string options_file_name);
    bool load_options_from_file(std::string options_file_name);
private:
    static GlobalOptions* instance;
    std::map<std::string, std::string> key_values;
    GlobalOptions()
    {
        instance = this;
    }
    std::filesystem::path normalize_file_path(std::string file_name);
};
