//
// Created by dell on 2017/11/29.
//

#include <jni.h>
#include <string>
#include "android/log.h"
extern "C"

#define  LOG_TAG    "jni"
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO,  LOG_TAG, fmt, ##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##args)

JNICALL jobject Java_echo_textndk_NativeItfs_testArgs(
        JNIEnv *env,
        jobject thiz, 
        jint a, 
        jbyteArray bytes, 
        jstring nameStr, 
        jobject poiList) {
   
    // 1. 定义字段
    jclass cls_poi;
    jfieldID fld_name;
   	jfieldID fld_age;
    jfieldID fld_is_hot;
   	jfieldID fld_data;

    jobject obj_poi;
    jobject obj_ArrayList;
    jmethodID poi_construct;


    LOGI("111111111111");
    
    // 2. 实例化, 
    // 获得签名方式：javap -s xxx.class
    // 注意签名除了java类要带分号，别的基本类型，数组都不带分号
   	cls_poi = env->FindClass("echo/textndk/Poi");
   	fld_name = env->GetFieldID( cls_poi, "name", "Ljava.lang.String;");
    fld_age = env->GetFieldID( cls_poi, "age", "I");
    fld_is_hot = env->GetFieldID( cls_poi, "isHot", "Z");
    fld_data = env->GetFieldID( cls_poi, "data", "[B");
    poi_construct = env->GetMethodID(cls_poi, "<init>","()V");

    LOGI("2222222222222");
    
    // 3.arrayList
    jclass cls_list = env->GetObjectClass(poiList);    
    jmethodID arraylist_get = env->GetMethodID(cls_list,"get","(I)Ljava/lang/Object;");
    jmethodID arraylist_size = env->GetMethodID(cls_list,"size","()I");
    jmethodID arrayList_construct = env->GetMethodID(cls_list,"<init>","()V");
    jmethodID arrayList_add = env->GetMethodID(cls_list,"add","(Ljava/lang/Object;)Z");
    
    jint len = env->CallIntMethod(poiList, arraylist_size);
    obj_ArrayList = env->NewObject(cls_list, arrayList_construct);

    LOGI("3333333333333");
    
    
    // 4. 获取String
    const char* str;
    str = env->GetStringUTFChars(nameStr, false);
    LOGI("name:%s", str);
    env->ReleaseStringUTFChars(nameStr, str);
    
    LOGI("44444444444444");
    
    // 5. 解析javaBean
    for(int i=0;i<len;i++){
        
        // 构造Obj
        jobject obj_add = env->NewObject(cls_poi, poi_construct);
        
        // 从ArrayList获取obj
        obj_poi = env->CallObjectMethod(poiList, arraylist_get,i);

        LOGI("55555555555555");
        
        // 从bean中获取jstring，并改变其值
        jstring nameObj =  (jstring)env->GetObjectField(obj_poi, fld_name);
        const char* str;
        char buf[20]={0};
        str = env->GetStringUTFChars(nameObj, false);
        LOGI("poi.name: %s", str);
        sprintf(buf,"%s%s",str, "_jni");
        env->ReleaseStringUTFChars(nameObj, str);
        LOGI("poi.name.new: %s", buf);

        LOGI("66666666666666");
        
        // 向obj设置成员变量
        env->SetObjectField(obj_add, fld_name, env->NewStringUTF(buf));
        env->SetIntField(obj_add, fld_age, env->GetIntField(obj_poi, fld_age) + 100);
        env->SetBooleanField(obj_add, fld_is_hot, env->GetBooleanField(obj_poi, fld_is_hot)==JNI_TRUE ? JNI_FALSE : JNI_TRUE );

        LOGI("77777777777777");
        
        jobject bytes_obj = env->GetObjectField (obj_poi, fld_data);
        jbyteArray *arr = reinterpret_cast<jbyteArray *>(&bytes_obj);

        LOGI("88888888888888");
        
        // 解析byte数组，并改变其值
        jint length = env->GetArrayLength(*arr);
        if(length>0){
            jbyte * data = env->GetByteArrayElements(*arr, NULL);
            for(int j=0; j< length; j++){
                data[j] += 100;
            }
            
           jbyteArray resultArray = env->NewByteArray(length);
           env->SetByteArrayRegion(resultArray, 0, length, data);

            LOGI("999999999999999");
            
            env->SetObjectField(obj_add, fld_data, resultArray);
            env->ReleaseByteArrayElements(*arr, data, 0);
            env->DeleteLocalRef(resultArray);
        }
        
        LOGI("aaaaaaaaaaaaaaaaaaaa");
        
        env->CallBooleanMethod(obj_ArrayList, arrayList_add, obj_add);
        
        env->DeleteLocalRef(bytes_obj);
        env->DeleteLocalRef(obj_add);
    }
    
    LOGI("bbbbbbbbbbbbbbbbbbbbb");
    
    // 传入的参数不用主动删除
//    env->DeleteLocalRef(obj_poi);
//    env->DeleteLocalRef(obj_ArrayList);
    
    return obj_ArrayList;
}