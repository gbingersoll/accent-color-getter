#pragma once

#include <napi.h>

class AccentColorGetter : public Napi::ObjectWrap<AccentColorGetter>
{
public:
    AccentColorGetter(const Napi::CallbackInfo &);
    Napi::Value Get(const Napi::CallbackInfo &);

    static Napi::Function GetClass(Napi::Env);
};
