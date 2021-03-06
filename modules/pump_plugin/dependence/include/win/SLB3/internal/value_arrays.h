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
#ifndef __SLB3_VALUE_ARRAYS__
#define __SLB3_VALUE_ARRAYS__

namespace SLB3 {

  // -- Value specialization for array of elements
  template<class T, unsigned N>
  struct Value<T[N]>  {
    static void Push(lua_State *L,T const (&array)[N]) {
      lua_createtable(L,N,0);
      for(unsigned int i = 0; i < N; ++i) {
        Value<T>::Push(L, array[i]);
        lua_rawseti(L,-2, i+1);
      }
    }
    // returns true on success
    static bool GetArray(lua_State *L, int index, T (*where)[N]) {
      if (lua_istable(L,index) && (lua_rawlen(L, index) == N)) {
        for(unsigned int i = 0; i < N; ++i) {
          lua_rawgeti(L,index, i+1);
          (*where)[i] = Value<T>::Get(L,-1);
          lua_pop(L,1);
        }
        return true;
      }
      return false;
    }
  };

  template<class T, unsigned N>
  struct Value<T(&)[N]> : public Value<T[N]> {};
  template<class T, unsigned N>
  struct Value<T const (&)[N]> : public Value<T[N]> {};
}

#endif
