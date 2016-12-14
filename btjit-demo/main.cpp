#include <QDebug>
#include <QFile>
#include <algorithm>

#define pa(al) __attribute__((packed, aligned(al)))

struct packet_header {
    quint32 magic;
    const char uuid[16];
    quint32 stream_id;
} pa(8);

struct event_header {
    quint32 id;
    quint64 timestamp;
} pa(8);

struct packet_context {
    quint64 timestamp_begin;
    quint64 timestamp_end;
    quint64 content_size;
    quint64 packet_size;
    quint64 events_discarded;
} pa(8);

struct events {
    union {
        struct {
            int foo_val;
        } foo;
        struct {
            int bar_val;
        } bar;
    } fields pa(8);
};

struct events_dfs {
    int id;
    const char *name;
    ulong (*parse)(struct events *);
};

ulong parse_foo(struct events *ev) {
    printf("foo_val: %d\n", ev->fields.foo.foo_val);
    return sizeof(ev->fields.foo) + 4;
}

ulong parse_bar(struct events *ev) {
    printf("bar_val: %d\n", ev->fields.bar.bar_val);
    return sizeof(ev->fields.bar) + 4;
}

static struct events_dfs evs[] = {
    { 0, "foo", parse_foo },
    { 1, "bar", parse_bar },
};

int main(int argc, char *argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    QFile trace(TOPSRCDIR "/trace/my_stream_class_0");
    if (trace.open(QIODevice::ReadOnly)) {
        uchar *buf = trace.map(0, trace.size());

        struct packet_header *pkt_hdr = (struct packet_header *)buf;
        printf("magic: %x\n", pkt_hdr->magic);

        printf("uuid: ");
        for (int i = 0; i < 16; i++) {
            printf("%02x ", pkt_hdr->uuid[i] & 0xFF);
        }
        printf("\n");

        // FIXME: does not work: always 0
        //QUuid uuid(QByteArray(pkt_hdr->uuid));

        printf("stream_id: %d\n", pkt_hdr->stream_id);

        buf += sizeof(struct packet_header);

        struct packet_context *pkt_ctx = (struct packet_context *)buf;

        printf("timestamp_begin: 0x%llx\n", pkt_ctx->timestamp_begin);
        printf("timestamp_end: 0x%llx\n", pkt_ctx->timestamp_end);
        printf("content_size: %llu\n", pkt_ctx->content_size / 8);
        printf("packet_size: %llu\n", pkt_ctx->packet_size / 8);
        printf("events_discarded: %llu\n", pkt_ctx->events_discarded);

        buf += sizeof(struct packet_context);
        uchar *end = buf + (pkt_ctx->content_size / 8);
        while (buf < end) {
            struct event_header *hdr = (struct event_header *)buf;
            printf("event.id: %d\n", hdr->id);
            printf("event.ts: %llu\n", hdr->timestamp);

            if (hdr->id > 1) {
                printf("wrong event id\n");
                break;
            }

            printf("event.name: %s\n", evs[hdr->id].name);

            buf += sizeof(struct event_header);

            struct events *ev = (struct events *)buf;
            ulong ev_size = evs[hdr->id].parse(ev);
            ev_size = std::max((long)ev_size, (long)8);
            printf("ev_size: %ld\n", ev_size);
            buf += ev_size;
        }

    }

    return 0;
}

