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

class CASTING_HANDLER_API INMediaObject{
public:

};

class CASTING_HANDLER_API INDevice{
public:
    virtual const char* GetFriendlyName() = 0;
};

class CASTING_HANDLER_API INCastingEventCallback{
public:
    virtual void OnDeviceAdd(INDevice* pDevice) = 0;
    virtual void OnDeviceRemove(INDevice* pDevice) = 0;
};


class CASTING_HANDLER_API INCastingHandler{
public:
    virtual int Init(INCastingEventCallback* pCallback) = 0;
    virtual int Release() = 0;
    virtual int RefreshDevice() = 0;
    virtual int PlayMediaObjectOnDevice(INMediaObject* pMediaObject, INDevice* pDevice) = 0;

public:
    virtual ~INCastingHandler() {}
};



//#if defined(__cplusplus)
//}
//#endif 



#endif //_CASTING_HANDLER_H_