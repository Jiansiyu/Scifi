// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME RootUserDict

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "SciFi.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_SciFi(void *p = 0);
   static void *newArray_SciFi(Long_t size, void *p);
   static void delete_SciFi(void *p);
   static void deleteArray_SciFi(void *p);
   static void destruct_SciFi(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::SciFi*)
   {
      ::SciFi *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::SciFi >(0);
      static ::ROOT::TGenericClassInfo 
         instance("SciFi", ::SciFi::Class_Version(), "SciFi.h", 17,
                  typeid(::SciFi), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::SciFi::Dictionary, isa_proxy, 4,
                  sizeof(::SciFi) );
      instance.SetNew(&new_SciFi);
      instance.SetNewArray(&newArray_SciFi);
      instance.SetDelete(&delete_SciFi);
      instance.SetDeleteArray(&deleteArray_SciFi);
      instance.SetDestructor(&destruct_SciFi);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::SciFi*)
   {
      return GenerateInitInstanceLocal((::SciFi*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::SciFi*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr SciFi::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *SciFi::Class_Name()
{
   return "SciFi";
}

//______________________________________________________________________________
const char *SciFi::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::SciFi*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int SciFi::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::SciFi*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *SciFi::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::SciFi*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *SciFi::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::SciFi*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void SciFi::Streamer(TBuffer &R__b)
{
   // Stream an object of class SciFi.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(SciFi::Class(),this);
   } else {
      R__b.WriteClassBuffer(SciFi::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_SciFi(void *p) {
      return  p ? new(p) ::SciFi : new ::SciFi;
   }
   static void *newArray_SciFi(Long_t nElements, void *p) {
      return p ? new(p) ::SciFi[nElements] : new ::SciFi[nElements];
   }
   // Wrapper around operator delete
   static void delete_SciFi(void *p) {
      delete ((::SciFi*)p);
   }
   static void deleteArray_SciFi(void *p) {
      delete [] ((::SciFi*)p);
   }
   static void destruct_SciFi(void *p) {
      typedef ::SciFi current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::SciFi

namespace {
  void TriggerDictionaryInitialization_RootUserDict_Impl() {
    static const char* headers[] = {
"SciFi.h",
0
    };
    static const char* includePaths[] = {
"/home/johnw/Scifi/Scifi_class/Scifi_class",
"/home/johnw/analyzer-1.6.3",
"/home/johnw/analyzer-1.6.3/src",
"/home/johnw/analyzer-1.6.3/hana_decode",
"/home/johnw/root/include",
"/home/johnw/root/include",
"/home/johnw/Scifi/Scifi_class/Scifi_class/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "RootUserDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate(R"ATTRDUMP(Generic Scifi class)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$SciFi.h")))  SciFi;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "RootUserDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif
#ifndef NDEBUG
  #define NDEBUG 1
#endif
#ifndef WITH_DEBUG
  #define WITH_DEBUG 1
#endif
#ifndef LINUXVERS
  #define LINUXVERS 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "SciFi.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"SciFi", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("RootUserDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_RootUserDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_RootUserDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_RootUserDict() {
  TriggerDictionaryInitialization_RootUserDict_Impl();
}
