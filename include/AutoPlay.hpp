#pragma once

#include <config.hpp>
#include <dbg.hpp>
#include <random>
#include <Windows.h>
namespace ap{
    struct AutoPlay{
        Config& cf;
        bool running;
        HWND h;
        std::random_device r;
        AutoPlay(Config& conf,HWND hwnd) : 
            cf(conf),h(hwnd)
        {
            running = true;
        }

        void go()
        {
            while(running)
            {
                step();
            }
        }

        void stop()
        {
            running = false;
        }

        virtual void step()
        {
            int kn = rand(1,cf.times_max_key);
            while(kn > 0)
            {
                int key = rand(0,static_cast<int>(cf.key_pool.size() - 1));
                sendKey(cf.key_pool[key],rand(cf.updl,cf.updh));
                --kn;
                sleep(rand(cf.sndl,cf.sndh));
            }
            sleep(rand(cf.ndl,cf.ndh));
        }

        void sleep(int ms)
        {
            if(ms > 0)
                ::Sleep(ms);
        }

        int rand(int b,int e)
        {
            std::default_random_engine e1(r());
            std::uniform_int_distribution<int> uniform_dist(b, e);
            return uniform_dist(e1);
        }

        virtual void sendKeyEvent(int key,int up_dur=1000)
        {
            SendMessageA(h,WM_KEYDOWN,static_cast<WPARAM>(key),NULL);
            sleep(up_dur);
            SendMessageA(h,WM_KEYUP,static_cast<WPARAM>(key),NULL);
        }

        virtual void sendMouseKeyEvent(int key, int up_dur = 1000)
        {
            auto pos = MAKELONG(10,10);
            SendMessageA(h, WM_LBUTTONDOWN, static_cast<WPARAM>(key), pos);
            sleep(up_dur);
            SendMessageA(h, WM_LBUTTONUP, static_cast<WPARAM>(key), pos);
        }

        virtual void sendKey(int key, int up_dur = 1000)
        {
            dbg(key);
            if(key < 1000)
                sendKeyEvent(key,up_dur);
            else
                sendMouseKeyEvent(key,up_dur);
        }
    };
}