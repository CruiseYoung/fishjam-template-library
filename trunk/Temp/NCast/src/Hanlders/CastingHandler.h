#ifndef _CASTING_HANDLER_H_
#define _CASTING_HANDLER_H_

//JNI �ӿ�
//public static native int startMediaRender(byte[] friendname ,byte[] uuid);
//public static native int stopMediaRender();  
//1.���ⲿ��action�¼�ͨ����������׸�java�㴦�� -- JNIEnv::CallStaticVoidMethod
//2.ͨ��jni��һЩ�¼�״ֵ̬���������ڷ����б� -- 
//  FindServiceByType("urn:schemas-upnp-org:service:AVTransport:1") => SetStateVariable
//
//��jni��ת��C++������java����ʱһ��Ҫע�⼰ʱ�ͷ���Դ����Ҫ����ڴ�й©��


#include "CastingDefine.h"

//#if defined(__cplusplus)
//extern "C" {
//#endif 


class CASTING_HANDLER_API IDeviceInfo{
public:

};

class CASTING_HANDLER_API IBrowserCallback{
public:
    virtual void OnDeviceAdd(IDeviceInfo* pDeviceInfo) = 0;
    virtual void OnDeviceRemove(IDeviceInfo* pDeviceInfo) = 0;
};


class CASTING_HANDLER_API ICastingHandler{
public:
    virtual NPT_Int32   Init() = 0;
    virtual NPT_Int32   Release() = 0;
    virtual NPT_Int32   RefreshDevice() = 0;

public:
    virtual ~ICastingHandler() {}
};



//#if defined(__cplusplus)
//}
//#endif 



#endif //_CASTING_HANDLER_H_