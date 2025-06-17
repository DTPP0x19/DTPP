//日志
#include "tlog.h"

#include "llmchat.h"
#include "fio.h"
#include "hook.h"
//资源文件
#include "dtpp-qrc.h"

#include <QQuickView>
#include <QQmlContext>
#include <QQmlApplicationEngine>

//替换资源文件
bool inited=false;
static LLMChat* chat=LLMChat::getInstance();
static FIO* fio=FIO::getInstance();

//Qml Context初始化
install_hook(_ZN22YGuiApplicationPrivate14initCommonQmlsEP10QQuickView,void *,void *o,QQuickView* qmlview_ptr){
    tlog_info("qmlview_ptr->%p", qmlview_ptr);
    qmlview_ptr->rootContext()->setContextProperty("chat", chat);
    qmlview_ptr->rootContext()->setContextProperty("fio", fio);
    if(!inited){
        //inited=true;
    }
    return orig__ZN22YGuiApplicationPrivate14initCommonQmlsEP10QQuickView(o,qmlview_ptr);
}
//Hook httplog
install_hook(_ZN11YLogManager11sendHttpLogERK7QString,void,void*,const QString *info){
    tlog_info("[HOOKED][HTTP LOG]%s",info->toStdString().c_str());
    return;
}

void hook(){
    install_hook__ZN22YGuiApplicationPrivate14initCommonQmlsEP10QQuickView();
    install_hook__ZN11YLogManager11sendHttpLogERK7QString();
}
//加载资源
void load_resources()
{
    sym=SymbolCache();
    // 替换插件qml文件
    int64_t (*plg_clr_qml)()=nullptr;
    plg_clr_qml = (int64_t(*)(void))sym.get("_Z25qCleanupResources_BaseQmlv");
    if (plg_clr_qml())
    {
        tlog_info( "Cleaned up plugin resources");
    }
    else
    {
        tlog(TLOG_ERROR, "Cleaned up plugin resources Error");
    }

    bool (*plg_reg_res)(int, const unsigned char *, const unsigned char *, const unsigned char *)=NULL;
    plg_reg_res = (bool (*)(int, const unsigned char *, const unsigned char *, const unsigned char *))sym.get("_Z21qRegisterResourceDataiPKhS0_S0_");
    if (plg_reg_res(3, qt_resource_struct, qt_resource_name, qt_resource_data))
    {
        tlog_info("Plugin Resources has been replaced", plg_reg_res);
    }
    else
    {
        tlog(TLOG_ERROR, "Error due to resource replacement", plg_reg_res);
    }

    // 替换主程序qml文件
    int64_t (*y_clr_qml)();

    y_clr_qml = (int64_t(*)(void))sym.get("_Z21qCleanupResources_qmlv");
    // tlog_info( "_Z21qCleanupResources_qmlv->%p", y_clr_qml);
    if (y_clr_qml())
    {
        tlog_info( "Cleaned up main process resources OK!");
    }
    else
    {
        tlog_info( "Cleaned up Error");
    }

    bool (*y_reg_res)(int, const unsigned char *, const unsigned char *, const unsigned char *)=nullptr;

    y_reg_res = (bool (*)(int, const unsigned char *, const unsigned char *, const unsigned char *))sym.get("_Z21qRegisterResourceDataiPKhS0_S0_");
    // tlog_info( "_Z21qRegisterResourceDataiPKhS0_S0_->%p", y_reg_res);
    if (y_reg_res(3, qt_resource_struct, qt_resource_name, qt_resource_data))
    {
        tlog_info( "Resource has been replaced", y_reg_res);
    }
    else
    {
        tlog(TLOG_ERROR, "Error due to resource replacement", y_reg_res);
    }
}


