#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <chrono>
#include <string>
#include <cmath>
#include <thread>

#define FRAMERATE 5

int main()
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int fb_w = w.ws_col;
    int fb_h = w.ws_row;
    char fb [fb_w][fb_h];
    auto frame_t = std::chrono::microseconds((long) floor(1000000 / FRAMERATE));
    bool flip = false;

    system("clear");
    while (true) {
        // TESTING
        flip = !flip;

        // Grab reference time for frame
        auto start_t = std::chrono::steady_clock::now();
        
        // Populate frame buffer
        for (int i_h = 0; i_h < fb_h; i_h++) {
            for (int i_w = 0; i_w < fb_w; i_w++) {
                if (i_h % 2 == 0 && flip) {
                    fb[i_w][i_h] = '-';
                }
                else {
                    fb[i_w][i_h] = '#'; 
                }
            }
        }

        // Build stdout string from frame buffer
        int frame_len = (fb_w * fb_h) + fb_h;
        char frame [frame_len];
        int c_i = 0;
        for (int i_h = 0; i_h < fb_h; i_h++) {
            for (int i_w = 0; i_w < fb_w; i_w++) {
                frame[c_i] = fb[i_w][i_h];
                c_i++;
            }
            frame[c_i] = '\n';
            c_i++;
        }

        // Build framestring and write to stdout
        std::string frame_str(frame);
        std::cout << frame_str;

        // TODO: Sleep until next frame
        auto end_t = std::chrono::steady_clock::now();
        auto elapsed_t = std::chrono::duration_cast<std::chrono::microseconds>(end_t - start_t);
        std::this_thread::sleep_for(std::chrono::microseconds(frame_t - elapsed_t));
    }
}
