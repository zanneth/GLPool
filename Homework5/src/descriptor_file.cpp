/*
 * descriptor_file.cpp
 *
 * Author: Charles Magahern <charles.magahern@arizona.edu>
 * Date Created: 03/16/2012
 */
 
#include "descriptor_file.h"

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <sstream>

DescriptorFile::DescriptorFile(const char *filename)
{
    if (filename != NULL) {
        loadDescriptorFile(filename);
    }
}

void DescriptorFile::loadDescriptorFile(const char *filename)
{
    using namespace std;
    
    ifstream infile(filename, ifstream::in);
    char buf[512];
    
    while (infile.good()) {
        infile.getline(buf, 512);
        if (buf[0] == '#') {
            continue;
        } else {
            if (strlen(buf) > 0) {
                stringstream ss(buf);
                string key, values;
                
                getline(ss, key, ' ');
                getline(ss, values);
                _configurations.push_back(Configuration(key, values));
            }
        }
    }
}