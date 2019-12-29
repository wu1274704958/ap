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
#include <spy.hpp>

using namespace std;
using namespace ap;
using namespace spy;
using namespace token;
namespace fs = std::filesystem;

#define COUT_AND_CONTINUE(s)   \
{                                           \
    std::cout << s << "\n";\
    continue;\
}

#define COUT_DEFAULT_ERR COUT_AND_CONTINUE("invaild parameter!")


int main(int argc,char **argv)
{
    if (argc <= 1)
    {
        cout << "invaild parameter!\n";
        return -1;
    }

    Spy s = Spy::EnumWindowsByTitleAndCls("无标题 - 记事本","Notepad");
    auto ws = s.get_windows();
    dbg(ws.size());
    if (ws.empty())
    {
        std::cout << "Not Find Window!!!" << std::endl;
        return 0;
    }
    for (int i = 0;i < ws.size();++i)
    {
        printf("%d: %p\n", i, ws[i].get_hwnd());
    }

    int choose;
    

    while (true)
    {
        std::string inp;
        std::cin >> inp;
        TokenStream<std::string> tsr(inp);
        tsr.analyse();
        auto& ts = tsr.tokens;
        if(ts.empty())
            COUT_DEFAULT_ERR
        if (ts[0].body == "exit")
            return 0;
        if(ts.size() < 2)
            COUT_DEFAULT_ERR
        if (ts[0].body.empty() || ts[1].body.empty())
            COUT_DEFAULT_ERR
        try {
            if (ts[0].body == "ds")
            {
                choose = wws::parser<int>(ts[1].body);
                if (!s.has(choose))
                    COUT_DEFAULT_ERR
                    s.draw_sign(choose);
            }
            else
            if (ts[0].body == "go")
            {
                choose = wws::parser<int>(ts[1].body);
                if (!s.has(choose))
                    COUT_DEFAULT_ERR
                    break;
            }
        }catch(...)
            COUT_AND_CONTINUE("Nuknow err!!")
    }
    
    HWND edit = ws[choose].get_hwnd();//(FindWindowA("GxWindowClass","魔兽世界");
    dbg(edit);

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