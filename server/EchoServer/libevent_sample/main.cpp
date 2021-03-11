#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

static const char message[] = "hello, world!";
static const int port = 9677;
static void listener_cb(struct evconnlistener*, evutil_socket_t,
    struct sockaddr*, int socklen, void*);
static void conn_writecb(struct bufferevent*, void*);
static void conn_eventcb(struct bufferevent*, short, void*);
static void signal_cb(evutil_socket_t, short, void*);

int main(int argc, char **argv) {
    struct event_base *base;
    struct evconnlistener *listener;
    struct event *signal_event;

    struct sockaddr_in sin = {0};
    base = event_base_new();
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);

    listener = evconnlistener_new_bind(base, listener_cb, (void*)base,
        LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,
        (struct sockaddr*)&sin,
        sizeof(sin));
    signal_event = evsignal_new(base, SIGINT, signal_cb, (void*)base);
    event_add(signal_event, NULL);

    event_base_dispatch(base);
    evconnlistener_free(listener);
    event_free(signal_event);
    event_base_free(base);
}

static void listener_cb(struct evconnlistener *listener, evutil_socket_t fd,
    struct sockaddr *sa, int socklen, void *user_data) {
    struct event_base *base = (event_base*)user_data;
    struct bufferevent *bev;
    bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
    
    bufferevent_setcb(bev, NULL, conn_writecb, conn_eventcb, NULL);
    bufferevent_enable(bev, EV_WRITE);
    bufferevent_disable(bev, EV_READ);

    bufferevent_write(bev, message, strlen(message));
}

static void conn_writecb(struct bufferevent *bev, void *user_data) {
    struct evbuffer *output = bufferevent_get_output(bev);
	if (evbuffer_get_length(output) == 0) {
		printf("flushed answer\n");
		bufferevent_free(bev);
	}
}

static void
conn_eventcb(struct bufferevent *bev, short events, void *user_data)
{
	if (events & BEV_EVENT_EOF) {
		printf("Connection closed.\n");
	} else if (events & BEV_EVENT_ERROR) {
		printf("Got an error on the connection: %s\n",
		    strerror(errno));/*XXX win32*/
	}
	/* None of the other events can happen here, since we haven't enabled
	 * timeouts */
	bufferevent_free(bev);
}

static void
signal_cb(evutil_socket_t sig, short events, void *user_data)
{
	struct event_base *base = (event_base*)user_data;
	struct timeval delay = { 2, 0 };

	printf("Caught an interrupt signal; exiting cleanly in two seconds.\n");

	event_base_loopexit(base, &delay);
}