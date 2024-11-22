#include "Sounds.h"

map<string, Sound> Sounds::sounds;

void Sounds::initSounds() {
    // Init audio
    InitAudioDevice();

    // Add sounds
    sounds["jump"] = LoadSound("assets/Sounds/jump.wav");

}