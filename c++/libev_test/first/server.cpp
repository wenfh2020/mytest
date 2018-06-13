#include <stdio.h>
#include <ev.h>

ev_io stdin_watcher;
ev_timer timeout_watcher;
ev_signal signal_watcher;
struct ev_loop* g_loop = EV_DEFAULT;

static void stdin_cb(EV_P_ ev_io* w, int revents) {
    printf("stdin ready, event = %d", revents);

    ev_io_stop(EV_A_ w);
    ev_break(EV_A_ EVBACKEND_ALL);
}

static void timeout_cb(EV_P_ ev_timer* w, int revents) {
    puts("timeout");
    //printf("timeout, event = %d", revents);
    //ev_break(EV_A_ EVBREAK_ONE);
    //ev_timer_stop(g_loop, w);
    //ev_timer_init(&timeout_watcher, timeout_cb, 5.5, 0.);
    //ev_timer_start(g_loop, &timeout_watcher);
}

static void sigint_cb(struct ev_loop* loop, ev_signal* w, int revernts) {
    printf("signal info : %d, event = %d", w->signum, revernts);
    ev_break(loop, EVBREAK_ALL);
}

int main() {
    ev_io_init(&stdin_watcher, stdin_cb, 0, EV_READ);
    ev_io_start(g_loop, &stdin_watcher);

    ev_timer_init(&timeout_watcher, timeout_cb, 5.5, 1.);
    ev_timer_start(g_loop, &timeout_watcher);

    ev_signal_init(&signal_watcher, sigint_cb, SIGINT);
    ev_signal_start(g_loop, &signal_watcher);

    ev_run(g_loop, 0);
    return 0;
}
