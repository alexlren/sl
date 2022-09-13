#ifndef TRAIN_H
# define TRAIN_H

# include "animation.h"

# define FRAME_WIDTH                    83
# define FRAME_HEIGHT                   18
# define FRAME_COUNT                    6

static const char *__train_orig_frame[FRAME_HEIGHT] = {
    "                       (@@) (  ) (@)  ( )  @@    ()    @     O     @     O      @  ",
    "                  (   )                                                            ",
    "              (@@@@)                                                               ",
    "           (    )                                                                  ",
    "                                                                                   ",
    "         (@@@)                                                                     ",
    "      ====        ________                ___________                              ",
    "  _D _|  |_______/        \\__I_I_____===__|_________|                              ",
    "   |(_)---  |   H\\________/ |   |        =|___ ___|      _________________         ",
    "   /     |  |   H  |  |     |   |         ||_| |_||     _|                \\_____A  ",
    "  |      |  |   H  |__--------------------| [___] |   =|                        |  ",
    "  | ________|___H__/__|_____/[][]~\\_______|       |   -|                        |  ",
    "  |/ |   |-----------I_____I [][] []  D   |=======|____|________________________|_ ",
    "__/ =| o |=-~~\\  /~~\\  /~~\\  /~~\\ ____Y___________|__|__________________________|_ ",
    " |/-=|___|=    ||    ||    ||    |_____/~\\___/          |_D__D__D_|  |_D__D__D_|   ",
    "  \\_/      \\O=====O=====O=====O_/      \\_/               \\_/   \\_/    \\_/   \\_/    ",
    "                                                                                   ",
    "                                                                                   "
};

/* Updates for frame 1 */
# define __TRAIN_UPDATES_FR_1__ 7
static struct animation_frame_update __train_fu_list_1__[__TRAIN_UPDATES_FR_1__] = {
{
    .pos = { .x = 23, .y = 0, .len = 59 },
    .content = " (  ) (@@) ( )  (@)  ()    @@    O     @     O     @      O",
},
{
    .pos = { .x = 18, .y = 1, .len = 6 },
    .content = " (@@@)",
},
{
    .pos = { .x = 14, .y = 2, .len = 7 },
    .content = " (    )",
},
{
    .pos = { .x = 11, .y = 3, .len = 7 },
    .content = " (@@@@)",
},
{
    .pos = { .x = 9, .y = 5, .len = 6 },
    .content = " (   )",
},
{
    .pos = { .x = 11, .y = 14, .len = 69 },
    .content = "O=====O=====O=====O   |_____/~\\___/          |_D__D__D_|  |_D__D__D_|",
},
{
    .pos = { .x = 12, .y = 15, .len = 67 },
    .content = "__/  \\__/  \\__/  \\__/      \\_/               \\_/   \\_/    \\_/   \\_/",
}
};


/* Updates for frame 2 */
# define __TRAIN_UPDATES_FR_2__ 7
static struct animation_frame_update __train_fu_list_2__[__TRAIN_UPDATES_FR_2__] = {
{
    .pos = { .x = 24, .y = 0, .len = 57 },
    .content = "  ) (@@) ( )  (@)  ()    @@    O     @     O     @      O",
},
{
    .pos = { .x = 19, .y = 1, .len = 4 },
    .content = "@@@)",
},
{
    .pos = { .x = 15, .y = 2, .len = 5 },
    .content = "    )",
},
{
    .pos = { .x = 12, .y = 3, .len = 5 },
    .content = "@@@@)",
},
{
    .pos = { .x = 10, .y = 5, .len = 4 },
    .content = "   )",
},
{
    .pos = { .x = 12, .y = 13, .len = 70 },
    .content = "O=====O=====O=====O \\ ____Y___________|__|__________________________|_",
},
{
    .pos = { .x = 12, .y = 15, .len = 67 },
    .content = "__/  \\__/  \\__/  \\__/      \\_/               \\_/   \\_/    \\_/   \\_/",
}
};


/* Updates for frame 3 */
# define __TRAIN_UPDATES_FR_3__ 7
static struct animation_frame_update __train_fu_list_3__[__TRAIN_UPDATES_FR_3__] = {
{
    .pos = { .x = 22, .y = 0, .len = 58 },
    .content = "(  ) (@@) ( )  (@)  ()    @@    O     @     O     @      O",
},
{
    .pos = { .x = 17, .y = 1, .len = 5 },
    .content = "(@@@)",
},
{
    .pos = { .x = 13, .y = 2, .len = 6 },
    .content = "(    )",
},
{
    .pos = { .x = 10, .y = 3, .len = 6 },
    .content = "(@@@@)",
},
{
    .pos = { .x = 8, .y = 5, .len = 5 },
    .content = "(   )",
},
{
    .pos = { .x = 13, .y = 13, .len = 69 },
    .content = "O=====O=====O=====O\\ ____Y___________|__|__________________________|_",
},
{
    .pos = { .x = 12, .y = 15, .len = 67 },
    .content = "__/  \\__/  \\__/  \\__/      \\_/               \\_/   \\_/    \\_/   \\_/",
}
};


/* Updates for frame 4 */
# define __TRAIN_UPDATES_FR_4__ 7
static struct animation_frame_update __train_fu_list_4__[__TRAIN_UPDATES_FR_4__] = {
{
    .pos = { .x = 21, .y = 0, .len = 58 },
    .content = "(  ) (@@) ( )  (@)  ()    @@    O     @     O     @      O",
},
{
    .pos = { .x = 16, .y = 1, .len = 5 },
    .content = "(@@@)",
},
{
    .pos = { .x = 12, .y = 2, .len = 6 },
    .content = "(    )",
},
{
    .pos = { .x = 9, .y = 3, .len = 6 },
    .content = "(@@@@)",
},
{
    .pos = { .x = 7, .y = 5, .len = 5 },
    .content = "(   )",
},
{
    .pos = { .x = 14, .y = 14, .len = 66 },
    .content = "O=====O=====O=====O|_____/~\\___/          |_D__D__D_|  |_D__D__D_|",
},
{
    .pos = { .x = 12, .y = 15, .len = 67 },
    .content = "__/  \\__/  \\__/  \\__/      \\_/               \\_/   \\_/    \\_/   \\_/",
}
};


/* Updates for frame 5 */
# define __TRAIN_UPDATES_FR_5__ 6
static struct animation_frame_update __train_fu_list_5__[__TRAIN_UPDATES_FR_5__] = {
{
    .pos = { .x = 23, .y = 0, .len = 59 },
    .content = " (@@) (  ) (@)  ( )  @@    ()    @     O     @     O      @",
},
{
    .pos = { .x = 18, .y = 1, .len = 6 },
    .content = " (   )",
},
{
    .pos = { .x = 14, .y = 2, .len = 7 },
    .content = " (@@@@)",
},
{
    .pos = { .x = 11, .y = 3, .len = 7 },
    .content = " (    )",
},
{
    .pos = { .x = 9, .y = 5, .len = 6 },
    .content = " (@@@)",
},
{
    .pos = { .x = 12, .y = 15, .len = 67 },
    .content = "_O=====O=====O=====O/      \\_/               \\_/   \\_/    \\_/   \\_/",
}
};


static struct animation_frame_update_list __train_fru_list__[FRAME_COUNT - 1] = {
{ .len = 7, .updates = (struct animation_frame_update *)&__train_fu_list_1__ },
{ .len = 7, .updates = (struct animation_frame_update *)&__train_fu_list_2__ },
{ .len = 7, .updates = (struct animation_frame_update *)&__train_fu_list_3__ },
{ .len = 7, .updates = (struct animation_frame_update *)&__train_fu_list_4__ },
{ .len = 6, .updates = (struct animation_frame_update *)&__train_fu_list_5__ }
};


static struct animation __train_animation = {
    .width = FRAME_WIDTH,
    .height = FRAME_HEIGHT,
    .frame_count = FRAME_COUNT,
    .orig_frame = __train_orig_frame,
    .frames = (struct animation_frame_update_list *)__train_fru_list__,
};

# endif
