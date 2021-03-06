#pragma once

#include <unistd.h>
#include <string>
#include <vector>

static int Write(int socket, const std::string &str)
{
    unsigned long length = str.length();
    if(write(socket, &length, sizeof(length)) == -1)
    {
        return -1;
    }

    if(write(socket, str.c_str(), length) == -1)
    {
        return -1;
    }

    return 0;
}

static int Read(int socket, std::string &str)
{
    unsigned long length;
    if(read(socket, &length, sizeof(length)) == -1)
    {
        return -1;
    }

    std::vector<char> buffer(length + 1);
    if(read(socket, buffer.data(), length) == -1)
    {
        return -1;
    }

    buffer[length] = '\0';
    str = buffer.data();

    return 0;
}

static void Close(int socket)
{
    close(socket);
}