/*
 * descriptor_file.h
 *
 * Author: Charles Magahern <charles.magahern@arizona.edu>
 * Date Created: 03/16/2012
 */
 
#pragma once

#include <map>
#include <string>
#include <vector>
#include <utility>

typedef std::pair<std::string, std::string> Configuration;

class DescriptorFile {
    std::vector<Configuration> _configurations;

public:
    DescriptorFile(const char *filename);
    
    // Accessors
    std::vector<Configuration> const& getConfigurations() { return _configurations; }
    
    // Loading Files
    void loadDescriptorFile(const char *filename);
};