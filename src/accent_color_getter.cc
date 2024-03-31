#include "accent_color_getter.h"
#ifdef WIN32
#include <winrt/Windows.UI.ViewManagement.h>
#endif

#include <iomanip>
#include <sstream>

using namespace Napi;

#ifdef WIN32
using namespace winrt;
using namespace Windows::UI::ViewManagement;
#endif

AccentColorGetter::AccentColorGetter(const Napi::CallbackInfo &info) : ObjectWrap(info)
{
    Napi::Env env = info.Env();

    if (info.Length() > 0)
    {
        Napi::TypeError::New(env, "Wrong number of arguments. Expecting none.")
            .ThrowAsJavaScriptException();
        return;
    }
}

Napi::Value AccentColorGetter::Get(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    if (info.Length() > 0)
    {
        Napi::TypeError::New(env, "Wrong number of arguments. Expecting none.")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

#ifdef WIN32
    const char *color_names[] = {
        "Background",
        "Foreground",
        "AccentDark3",
        "AccentDark2",
        "AccentDark1",
        "Accent",
        "AccentLight1",
        "AccentLight2",
        "AccentLight3"};

    auto ui_settings = UISettings();
    Object colorSet = Object::New(env);

    for (int id = 0; id < sizeof(color_names) / sizeof(*color_names); id++)
    {
        auto color_value = ui_settings.GetColorValue(static_cast<UIColorType>(id));
        auto red = static_cast<int>(color_value.R);
        auto green = static_cast<int>(color_value.G);
        auto blue = static_cast<int>(color_value.B);
        auto alpha = static_cast<int>(color_value.A);

        Object color = Object::New(env);
        color.Set("id", id);
        color.Set("R", red);
        color.Set("G", green);
        color.Set("B", blue);
        color.Set("A", alpha);

        std::stringstream ss_hex;
        ss_hex << "#" << std::setbase(16) << std::setfill('0');
        ss_hex << std::setw(2) << red;
        ss_hex << std::setw(2) << green;
        ss_hex << std::setw(2) << blue;
        color.Set("hexRGB", ss_hex.str());
        ss_hex << std::setw(2) << alpha;
        color.Set("hexRGBA", ss_hex.str());

        std::stringstream ss_rgba;
        ss_rgba << "rgba(";
        ss_rgba << red << ", ";
        ss_rgba << green << ", ";
        ss_rgba << blue << ", ";
        ss_rgba << static_cast<float>(alpha) / 255.0;
        if (alpha == 255)
        {
            ss_rgba << ".0";
        }
        ss_rgba << ")";
        color.Set("rgba", ss_rgba.str());

        colorSet.Set(color_names[id], color);
    }

    return colorSet;
#else
    Napi::TypeError::New(env, "Getting system accent colors only works on Windows.")
        .ThrowAsJavaScriptException();
    return env.Null();
#endif
}

Napi::Function AccentColorGetter::GetClass(Napi::Env env)
{
    return DefineClass(env, "AccentColorGetter", {
                                                     AccentColorGetter::InstanceMethod("get", &AccentColorGetter::Get),
                                                 });
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    Napi::String name = Napi::String::New(env, "AccentColorGetter");
    exports.Set(name, AccentColorGetter::GetClass(env));
    return exports;
}

NODE_API_MODULE(addon, Init)
