// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// See www.openfst.org for extensive documentation on this weighted
// finite-state transducer library.

#ifndef FST_LIB_COMPAT_H_
#define FST_LIB_COMPAT_H_

#ifdef _MSC_VER //AddedPD
#include <BaseTsd.h>
//#include <intrin.h>
//#include <bitset>

typedef SSIZE_T ssize_t;
//#define snprintf _snprintf
//#define strtoll _strtoi64
//#define __builtin_popcount __popcnt

//unsigned long long __builtin_ctzll(unsigned long long x);

//unsigned long long  __builtin_popcountll(unsigned long long  w);

#ifndef OPENFSTEXPORT
#ifdef _DEBUG
#ifdef _M_X64
#pragma comment (lib, "openfst64-gd.lib")
#else
#pragma comment (lib, "openfst-gd.lib")
#endif
#else
#ifdef _M_X64
#pragma comment (lib, "openfst64.lib")
#else
#pragma comment (lib, "openfst.lib")
#endif
#endif
#endif
#else
#include <dlfcn.h>
#endif

#include <climits>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

// Makes copy constructor and operator= private
// Deprecated: now just use =delete.
#define DISALLOW_COPY_AND_ASSIGN(type)    \
  type(const type&);                      \
  void operator=(const type&)

#if defined(__GNUC__) || defined(__clang__)
#define OPENFST_DEPRECATED(message) __attribute__((deprecated(message)))
#elif defined(_MSC_VER)
#define OPENFST_DEPRECATED(message) __declspec(deprecated(message))
#else
#define OPENFST_DEPRECATED(message)
#endif

#include <fst/config.h>
#include <fst/types.h>
#include <fst/lock.h>
#include <fst/flags.h>
#include <fst/log.h>
#include <fst/icu.h>

using std::string;

void OPENFSTDLL FailedNewHandler();

namespace fst {

// Downcasting.
template<typename To, typename From>
inline To down_cast(From* f) { return static_cast<To>(f); }

// Bitcasting.
template <class Dest, class Source>
inline Dest bit_cast(const Source &source) {
  static_assert(sizeof(Dest) == sizeof(Source),
                "Bitcasting unsafe for specified types");
  Dest dest;
  memcpy(&dest, &source, sizeof(dest));
  return dest;
}

// Check sums
class CheckSummer {
 public:
  CheckSummer() : count_(0) {
    check_sum_.resize(kCheckSumLength, '\0');
  }

  void Reset() {
    count_ = 0;
    for (int i = 0; i < kCheckSumLength; ++i) check_sum_[i] = '\0';
  }

  void Update(void const *data, int size) {
    const char *p = reinterpret_cast<const char *>(data);
    for (int i = 0; i < size; ++i) {
      check_sum_[(count_++) % kCheckSumLength] ^= p[i];
    }
  }

  void Update(string const &data) {
    for (int i = 0; i < data.size(); ++i) {
      check_sum_[(count_++) % kCheckSumLength] ^= data[i];
    }
  }

  string Digest() { return check_sum_; }

 private:
  static const int kCheckSumLength = 32;
  int count_;
  string check_sum_;

  CheckSummer(const CheckSummer &) = delete;
  CheckSummer &operator=(const CheckSummer &) = delete;
};

}  // namespace fst

#endif  // FST_LIB_COMPAT_H_
