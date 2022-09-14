#include <curses.h>
#include <signal.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <libgen.h>
#include <errno.h>

#include "generated/anims.h"

#define OPTSTR "n:s:hv"
#define USAGE_FMT  "Usage: %s [-n N] [-s M] [-v] [-h]\n"
#define ERR_NARG_FMT "Error: invalid %s option, N should be in [0-%d]\n"
#define USAGE_DESC                                                      \
    "A cruel program made to punish users who mispell ls\n\n"           \
    "Available options:\n"                                              \
    "\t-h\n"                                                            \
    "\t\tDisplay this help\n"                                           \
    "\t-v\n"                                                            \
    "\t\tShow the current version\n"                                    \
    "\t-n N\n"                                                          \
    "\t\tSelect the Nth animation (instead of a random one)\n"          \
    "\t-s M\n"                                                          \
    "\t\tSelect the speed mode: 0, 1, 2 (default = 1)\n"

# define SPEED_MODE_0 20000
# define SPEED_MODE_1 40000
# define SPEED_MODE_2 100000
# define DEFAULT_SPEED_MODE 1

extern int errno;
extern char *optarg;
extern int opterr, optind;

struct sl_options {
    /* Shows the nth animation */
    int n;
    /* Speed mode 1, 2 or 3 */
    int speed_mode;
};

static void usage(const char *progname) {
    fprintf(stderr, USAGE_FMT, progname);
    fprintf(stderr, USAGE_DESC);
    exit(EXIT_FAILURE);
}

static char get_frame_char(struct animation *anim, int x, int y, int frame)
{
    assert(frame >= 0 && frame < anim->frame_count);

    if (frame > 0) {
        struct animation_frame_update_list fr_list = anim->frames[frame - 1];
        struct animation_frame_update *updates = fr_list.updates;

        for (int i = 0; i < fr_list.len; ++i) {
            if (x >= updates[i].pos.x &&
                x < (updates[i].pos.x + updates[i].pos.len) &&
                y == updates[i].pos.y) {
                return updates[i].content[x - updates[i].pos.x];
            }
        }
    }
    /*
       if we're displaying the frame 0 or if there is no changes here
       we can just return the original frame char
    */
    return anim->orig_frame[y][x];
}

static int display_row(struct animation *anim, int y, int x, int y_offset, int frame)
{
    int i = 0;
    char c;

    assert(frame >= 0 && frame < anim->frame_count);

    /* If the animation exceeds the left side, start with an offset */
    if (x < 0) {
        if (-x >= anim->width) {
            return ERR;
        }
        i += -x;
        x = 0;
    }
    /* Display frame */
    c = get_frame_char(anim, i, y, frame);
    for (; c != '\0'; ++i, ++x) {
        if (mvaddch(y + y_offset, x, c) == ERR) {
            return ERR;
        }
        c = get_frame_char(anim, i, y, frame);
    }
    /* Clean up remaining space */
    for (; i <= anim->width; ++i) {
        if (mvaddch(y + y_offset, x, ' ') == ERR) {
            return ERR;
        }
    }

    return OK;
}

static int display_animation(struct animation *anim, int x)
{
    if (x < - anim->width) {
        /* The animation has been displayed entirely */
        return -1;
    }
    int y_offset = LINES / 2 - anim->height / 2;

    for (int y = 0; y < anim->height; ++y) {
        display_row(anim, y, x, y_offset, (anim->width + x) % anim->frame_count);
    }
    return 0;
}

static int start_animation(struct sl_options *options)
{
    struct animation *anim = NULL;
    int selected;
    unsigned int usec;

    switch (options->speed_mode) {
    case 0:
        usec = SPEED_MODE_0;
        break;
    case 2:
        usec = SPEED_MODE_2;
        break;
    default:
    case 1:
        usec = SPEED_MODE_1;
        break;
    }

    if (options->n < 0) {
        /* Select a random animation */
        srand(time(NULL));
        selected = rand() % ANIMATION_COUNT;
    } else {
        selected = options->n;
    }
    anim = __animations_list[selected];

    initscr();
#ifdef NDEBUG
    signal(SIGINT, SIG_IGN);
#endif
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    leaveok(stdscr, TRUE);
    scrollok(stdscr, FALSE);

    for (int x = COLS - 1; ; --x) {
        if (display_animation(anim, x) != 0) {
            break;
        }
        getch();
        refresh();
        usleep(usec);
    }
    mvcur(0, COLS - 1, LINES - 1, 0);
    endwin();

    return 0;
}

int main(int argc, char **argv)
{
    int opt;
    char *invalid_digits = NULL;
    struct sl_options options = {
        .n = -1,
        .speed_mode = DEFAULT_SPEED_MODE,
    };

    opterr = 0;

    while ((opt = getopt(argc, argv, OPTSTR)) != EOF) {
        switch (opt) {
        case 'n':
            options.n = (int)strtoul(optarg, &invalid_digits, 10);
            if (optarg == invalid_digits || options.n < 0 || options.n > ANIMATION_COUNT) {
                fprintf(stderr, ERR_NARG_FMT, "-n", ANIMATION_COUNT - 1);
                return EXIT_FAILURE;
            }
            break;
        case 's':
            options.speed_mode = (int)strtoul(optarg, &invalid_digits, 10);
            if (optarg == invalid_digits || options.speed_mode < 0 || options.speed_mode > 2) {
                fprintf(stderr, ERR_NARG_FMT, "-s", 2);
                return EXIT_FAILURE;
            }
            break;
        case 'v':
            /* VERSION should be passed at compile time */
            fprintf(stderr, "v%s\n", VERSION);
            exit(0);
        case 'h':
        default:
            usage(basename(argv[0]));
            /* exits here */
            break;
        }
    }

    return start_animation(&options);
}
