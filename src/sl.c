#include <curses.h>
#include <signal.h>
#include <unistd.h>
#include <assert.h>

#include "train.h"

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

int main(void)
{
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
        if (display_animation(&train_animation, x) != 0) {
            break;
        }
        getch();
        refresh();
        usleep(40000);
    }
    mvcur(0, COLS - 1, LINES - 1, 0);
    endwin();

    return 0;
}
