#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
class MrEpoll {
public:
    MrEpoll(uint32_t m_ = 256) {
        events_ = new struct epoll_event[m_];
        memset(events_, 0, sizeof(struct epoll_event));

        epfd_ = epoll_create(m_);
        if (-1 == epfd_) {
            fprintf(stderr, "epoll create failed");
        }
    }

    ~MrEpoll() {
        close(epfd_);
        delete[] events_;
    }

    void Ctrl(int op, int fd, uint32_t event) {
        ev_.data.fd = fd;
        ev_.events = event;
        epoll_ctl(epfd_, op, fd, &ev_);
    }

    int Poll(int maxevents, int timeout) {
        return epoll_wait(epfd_, events_, maxevents, timeout);
    }

    struct epoll_event *GetEvents() {
        return events_;
    }

private:
    int epfd_;
    struct epoll_event *events_;
    struct epoll_event ev_;
};

int main() {
    MrEpoll epoll_(256);
    char buff[256];
    memset(buff, 0, 255);
    epoll_.Ctrl(EPOLL_CTL_ADD, STDIN_FILENO, EPOLLIN);
    struct epoll_event *events = epoll_.GetEvents();
    while (1) {
        int n = epoll_.Poll(20, 500);
        for (int i = 0; i < n; i++) {
            if (events[i].events & EPOLLIN) {
                int fd = events[i].data.fd;
                read(fd, buff, 256);
                buff[strlen(buff) - 1] = '\0';
                if (0 == strcmp(buff, "set")) {
                    printf("set someth \n");
                } else if (0 == strcmp(buff, "get")) {
                    printf("get someth \n");
                } else if (0 == strcmp(buff, "del")) {
                    printf("del someth \n");
                } else {
                    printf("wrong input \n");
                }
            }
        }
    }
    return 0;
}
