#pragma once
#ifdef _MSC_VER
#include <functional>
#include <string>
#include <tuple>
#include <Windows.h>
namespace spy{

    struct Window : public std::tuple<::HWND,std::string,std::string>
    {
        using SUPER = std::tuple<::HWND,std::string,std::string>;
        Window(){}
        Window(::HWND h) :
            SUPER(h,"","")
        {}
        Window(::HWND h,std::string title,std::string cls) :
            SUPER(h,std::move(title),std::move(cls))
        {}
        ::HWND get_hwnd()
        {
            return std::get<0>(*this);
        }
        std::string& get_title()
        {
            return std::get<1>(*this);
        }
        std::string& get_class()
        {
            return std::get<2>(*this);
        }
    };

    class Spy{
    public:
        inline static Spy EnumWindowsByTitleAndCls(const char*tit,const char* cls)
        {
            std::tuple<const char*,const char*,std::vector<Window>> cb_data(tit,cls,std::vector<Window>());
            ::EnumWindows(EnumWindowsByTitleAndClsBody,reinterpret_cast<::LPARAM>(&cb_data));
            return Spy(std::move(std::get<2>(cb_data)));
        } 
        inline static BOOL CALLBACK EnumWindowsByTitleAndClsBody(HWND h, LPARAM ptr)
        {
            if(ptr)
            {
                auto p = reinterpret_cast<std::tuple<const char*,const char*,std::vector<Window>>*>(ptr);
                auto tit = std::get<0>(*p);
                auto cls = std::get<1>(*p);
                std::vector<Window> &ws = std::get<2>(*p);
                std::string title = get_window_text(h);
                std::string clas = get_class_name(h);
                if(title == tit && clas == cls)
                {
                    ws.push_back(Window(h,std::move(title),std::move(clas)));
                }
            }
            return TRUE;
        }

        inline static std::string get_text(::HWND h,std::function<int(::HWND)> len,
         std::function<::BOOL(::HWND,std::string&,int)> set_str) noexcept(false)
        {
            std::string title;
            int l = len(h);
            if(l <= 0) return title;
            title.reserve(l + 1);
                
            if(!set_str(h,title,l))
            {
                throw std::runtime_error("get text failed!");
            }
            return title;
        }

        inline static std::string get_window_text(::HWND h)
        {
            return get_text(h,[](::HWND h){
                return ::GetWindowTextLengthA(h);
            },[](::HWND h,std::string& s,int l)->::BOOL{
                return ::GetWindowTextA(h,s.data(),l);
            });
        }
        inline static std::string get_class_name(::HWND h)
        {
            return get_text(h,[](::HWND h){
                return 100;
            },[](::HWND h,std::string& s,int l)->::BOOL{
                return ::GetClassNameA(h,s.data(),l);
            });
        }
        
        std::vector<Window>& get_windows()
        {
            return ws;
        }

        Spy(std::vector<Window> ws) : ws(std::move(ws))
        {

        }
    private:
        Spy(){}
        std::vector<Window> ws;
    };
}

#endif 