#include <stdio.h>
#include <syslog.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[])
{
    openlog("writer", LOG_PID, LOG_USER);

    if (argc != 3) {
        syslog(LOG_ERR, "Expected a file path and a string argument");
        fprintf(stderr, "Usage: %s <writefile> <writestr>\n", argv[0]);
        closelog();
        return 1;
    }

    int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd == -1) {
        syslog(LOG_ERR, "Cannot open %s: %s", argv[1], strerror(errno));
        closelog();
        return 1;
    }

    syslog(LOG_DEBUG, "Writing %s to %s", argv[2], argv[1]);

    size_t length = strlen(argv[2]);
    size_t total = 0;

    while (total < length) {
        ssize_t count = write(fd, argv[2] + total, length - total);

        if (count == -1 && errno == EINTR) {
            continue;
        }

        if (count <= 0) {
            syslog(LOG_ERR, "Cannot write %s: %s", argv[1],
                   count == -1 ? strerror(errno) : "No bytes written");
            if (close(fd) == -1) {
                syslog(LOG_ERR, "Cannot close %s: %s", argv[1], strerror(errno));
            }
            closelog();
            return 1;
        }

        total += (size_t)count;
    }

    if (close(fd) == -1) {
        syslog(LOG_ERR, "Cannot close %s: %s", argv[1], strerror(errno));
        closelog();
        return 1;
    }

    closelog();
    return 0;
}
