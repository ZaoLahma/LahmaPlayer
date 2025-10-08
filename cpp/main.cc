#include "AudioStream.h"
#include "AudioWavFile.h"

#include <iostream>
#include <string>
#include <thread>

int main(int argc, char *argv[])
{
    // Extract file name to open from the provided arguments
    if (argc < 2)
    {
        std::cout<<"Please provide a file name."<<std::endl;
        return 1;
    }
    const std::string fileName = argv[1];
    std::cout<<"Opening file: "<<fileName<<std::endl;

    LahmaPlayer::AudioStream::AudioStream stream;
    stream.start(std::make_shared<LahmaPlayer::AudioFile::AudioWavFile>(fileName));

    stream.waitUntilFinished();

    return 0;
}