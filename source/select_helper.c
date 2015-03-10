#include "select_helper.h"

void files_init(Files* files)
{
    printf("files_init()\n");
    FD_ZERO(&files->_selectFds);
    files->maxFd = 0;
}

int files_select(Files* files)
{
    printf("files_select()\n");
    files->selectFds = files->_selectFds;
    return select(files->maxFd+1 , &files->selectFds, 0, 0, 0);
}

void files_add_file(Files* files, int newFd)
{
    printf("files_add_file(%d)\n", newFd);
    // add the file to our sets
    FD_SET(newFd, &files->_selectFds);
    files->fdSet.insert(newFd);

    // update maxFd if needed
    if(newFd > files->maxFd)
    {
        files->maxFd = newFd;
    }
}

void files_rm_file(Files* files, int fd)
{
    printf("files_rm_file(%d)\n", fd);
    // remove the file from our sets
    FD_CLR(fd, &files->_selectFds);
    files->fdSet.erase(fd);

    // update maxFd if needed
    if(fd == files->maxFd)
    {
        files->maxFd = 0;
        for(auto fd = files->fdSet.begin(); fd != files->fdSet.end(); ++fd)
        {
            if(*fd > files->maxFd)
            {
                files->maxFd = *fd;
            }
        }
    }
}
