#include <Windows.h>
#include <iostream>
#include <serialization.hpp>
#include <json.hpp>
#include <filesystem>
#include <fstream>
#include <strstream>
#include <print_stl.hpp>
#include <dbg.hpp>

#include <config.hpp>
#include <AutoPlay.hpp>

using namespace std;
using namespace ap;
namespace fs = std::filesystem;

int main(int argc,char **argv)
{
    if(argc <= 2)
    {
        cout << "invaild parameter!\n";
        return -1;
    }
    string a1(argv[1]);
    HWND edit = reinterpret_cast<HWND>( wws::parser<long long>(a1,16));//(FindWindowA("GxWindowClass","Ä§ÊÞÊÀ½ç");
    dbg(edit);
    //HWND edit = FindWindowExA(h,NULL,"Edit",NULL);

    fs::path conf(argv[2]);
    if(!fs::exists(conf))
    {
        cout << "config not exists!\n";
        return -1;
    }

    ifstream iif(conf.generic_string(),std::ios::binary);
    string str;
    while(!iif.eof())
    {
        auto c = iif.get();
        if(c != -1)
            str += c;
    }

    dbg(str);
    Config cf; 
    try{
        wws::Json temp(str);
        cf.set_data(temp);
    }catch(std::runtime_error e)
    {
        cout << e.what() << "\n";
        return -1;
    }
    if(cf.key_pool.empty())
        return -1;
    dbg(cf.key_pool);
    dbg(cf.ndh);
    dbg(cf.ndl);
    dbg(cf.sndh);
    dbg(cf.sndl);
    dbg(cf.times_max_key);
    
    AutoPlay aup(cf,edit);
    aup.go(); 
    system("pause");
    return 0;
}