#include "jsb_hdlplayer.hpp"
#include "cocos/bindings/jswrapper/SeApi.h"
#include "cocos/bindings/manual/jsb_conversions.h"
#include "cocos/ui/hdlplayer/HDLPlayer.hpp"

se::Class* __jsb_HDLPlayer_class = nullptr;

static bool HDLPlayer_finalize(se::State& s) {
    HDLPlayer* cobj = (HDLPlayer*)s.nativeThisObject();
    delete cobj;
    return true;
}
SE_BIND_FINALIZE_FUNC(HDLPlayer_finalize)

static bool HDLPlayer_constructor(se::State& s) {
    auto cobj = new (std::nothrow) HDLPlayer();
    if (!cobj)
        return false;

    s.thisObject()->setPrivateData(cobj);

    return true;
}
SE_BIND_CTOR(HDLPlayer_constructor, __jsb_HDLPlayer_class, HDLPlayer_finalize)

static bool HDLPlayer_takeImageInfo(se::State& s) {
    HDLPlayer* cobj = (HDLPlayer*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "HDLPlayer_takeImageInfo : Invalid Native Object");

    auto photo = cobj->takeImageInfo();
    if (!photo) {
        s.rval().setNull();
        return true;
    }

    se::Value dataVal;
    dataVal.setUint64(reinterpret_cast<uintptr_t>(photo->data));

    se::HandleObject retObj(se::Object::createPlainObject());
    retObj->setProperty("data", dataVal);
    retObj->setProperty("width", se::Value(photo->width));
    retObj->setProperty("height", se::Value(photo->height));

    s.rval().setObject(retObj);

    return true;
}
SE_BIND_FUNC(HDLPlayer_takeImageInfo)

static bool HDLPlayer_init(se::State& s) {
    const auto& args = s.args();
    std::string path;
    auto        ok = seval_to_std_string(args[0], &path);
    SE_PRECONDITION2(ok, false, "Convert string failed");

    HDLPlayer* cobj = (HDLPlayer*)s.nativeThisObject();
    cobj->init(path);

    return true;
}
SE_BIND_FUNC(HDLPlayer_init)

static bool HDLPlayer_play(se::State& s) {
    HDLPlayer* cobj = (HDLPlayer*)s.nativeThisObject();
    cobj->play();

    return true;
}
SE_BIND_FUNC(HDLPlayer_play)

bool register_all_hdlplayer(se::Object* obj) {
    se::Class* cls = se::Class::create("HDLPlayer", obj, nullptr, _SE(HDLPlayer_constructor));
    cls->defineFinalizeFunction(_SE(HDLPlayer_finalize));
    cls->defineFunction("init", _SE(HDLPlayer_init));
    cls->defineFunction("play", _SE(HDLPlayer_play));
    cls->defineFunction("takeImageInfo", _SE(HDLPlayer_takeImageInfo));

    cls->install();

    //    JSBClassType::registerClass<HDLPlayer>(cls);

    __jsb_HDLPlayer_class = cls;

    se::ScriptEngine::getInstance()->clearException();

    return true;
}
