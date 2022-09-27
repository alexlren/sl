#ifndef ANIMATION_H
# define ANIMATION_H

# include <stdint.h>

struct animation_frame_update {
    struct {
        uint16_t x;
        uint16_t y;
        uint16_t len;
    } pos;
    /* list of rows of the frame update - size is w * h */
    const char *content;
};

struct animation_frame_update_list {
    uint8_t len;
    struct animation_frame_update *updates;
};

struct animation {
    /* Width (max) for this animation */
    uint16_t width;
    /* Height (max) for this animation */
    uint16_t height;
    /* Number of frames */
    uint8_t frame_count;
    /* Original frame split in rows */
    const char **orig_frame;
    /* List of frame updates to apply on top of each frame */
    struct animation_frame_update_list *frames;
};

#endif /* !ANIMATION_H */
