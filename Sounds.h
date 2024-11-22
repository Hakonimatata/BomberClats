#pragma once

#include <raylib.h>
#include <map>
#include <string>

using namespace std;

class Sounds
{
    public:
        static void initSounds();  // Add sounds here
        static map<string, Sound> sounds;
};
