bool deamo()
{
    //step 1: fork
    pid_t ret;
    ret = fork();
    if(ret < 0)
    {
        return false;
    }
    else if (ret > 0)
    {
        //setp 2: close parent pid
        exit(0);
    }

    //setp 3 : creat section
    pid_t sid = setsid();
    if (sid < 0)
    {
        return false;
    }

    //setp 4: chdir to "/"
    if(chdir("/") < 0)
    {
        return false;
    }

    // setp 5: close terminal
    close(STDIN_FILENO);
    close(STDOOUT_FILENO);
    close(STDERR_FILENO);
    open("/dev/null", O_RDONLY);
    open("/dev/null", O_RDWR);
    open("/dev/null", O_RDWR);
    return true;
}
