#pragma once

#include <vector>
#include <json.hpp>

namespace ap
{
    class Config{
    public:
        std::vector<int> key_pool;
        int ndl,
        ndh,
        sndl,
        sndh,
        updl,
        updh,
        times_max_key;

        Config(wws::Json d)
        {
            set_data(d);   
        }

        Config(){}

        void set_data(wws::Json& d)
        {
            key_pool = d.get_arr<int>("key_pool");
            ndl = d.get<int>("ndl");
            ndh = d.get<int>("ndh");
            sndl = d.get<int>("sndl");
            sndh = d.get<int>("sndh");
            updl = d.get<int>("updl");
            updh = d.get<int>("updh");
            times_max_key = d.get<int>("times_max_key");
        }
    };
}