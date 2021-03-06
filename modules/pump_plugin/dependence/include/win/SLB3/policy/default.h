// Copyright (c) 2013, Jose L. Hidalgo Valiño <pplux@pplux.com>
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
//   * Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
#ifndef __SLB3_POLICY_DEFAULT__
#define __SLB3_POLICY_DEFAULT__ 1

#include "common.h"

namespace SLB3 {
namespace policy {

// Common Struct policy (object that can be copied)
struct Default {

template<class T>
struct Implementation {

  static void *Unpack(const Object &obj) {
    switch(obj.header->type) {
      case kObject_Invalid: assert(!"Invalid object"); return NULL;
      case kObject_Instance: return reinterpret_cast<T*>(obj.data);
      case kObject_Ptr:
      case kObject_Const_Ptr:
        return *reinterpret_cast<T**>(obj.data);
    }
    assert(!"Invalid policy::Struct::Unpack call");
    return NULL;
  }

  static Object NewInstanceBegin(lua_State *L, void **raw_mem) {
    Object obj = PushRawObject<T>(L, kObject_Instance, sizeof(T));
    *raw_mem = obj.data;
    return obj;
  }

  static void NewInstanceEnd(const Object obj, T* /*obj*/) {
    assert(obj.header->type == kObject_Instance && "Invalid instance");
    /* Do nothing */
  }


  static void PushPtr(lua_State *L, T* src) {
    Object obj = PushRawObject<T>(L, kObject_Ptr, sizeof(T*));
    *reinterpret_cast<T**>(obj.data) = src;
  }

  static void PushConstPtr(lua_State *L, const T* src) {
    Object obj = PushRawObject<T>(L, kObject_Const_Ptr, sizeof(const T*));
    *reinterpret_cast<const T**>(obj.data) = src;
  }

  static void Destructor(const Object & obj) {
    switch(obj.header->flags) {
      case NoFlags:
        switch(obj.header->type) {
          case kObject_Instance:
            reinterpret_cast<T*>(obj.data)->~T();
            break;
        }
        break;
      case Destroy:
        switch(obj.header->type) {
          case kObject_Ptr:
          case kObject_Const_Ptr:
            (*reinterpret_cast<T**>(obj.data))->~T();
            break;
          case kObject_Instance:
            reinterpret_cast<T*>(obj.data)->~T();
            break;
        }
        break;
	  case Delete:
        switch(obj.header->type) {
          case kObject_Ptr:
          case kObject_Const_Ptr:
            delete (*reinterpret_cast<T**>(obj.data));
            break;
          case kObject_Instance:
            delete reinterpret_cast<T*>(obj.data);
            break;
        }
      case NoDestroy:
        break;
      default:
        assert(!"Invalid policy::Flags");
    }
  }

};
};

} // end of policy naespace
} // end of SLB3 namespace

#endif

