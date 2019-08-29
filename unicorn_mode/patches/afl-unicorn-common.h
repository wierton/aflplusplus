/*
   american fuzzy lop - high-performance binary-only instrumentation
   -----------------------------------------------------------------

   Written by Andrew Griffiths <agriffiths@google.com> and
              Michal Zalewski <lcamtuf@google.com>

   TCG instrumentation and block chaining support by Andrea Biondo
                                      <andrea.biondo965@gmail.com>
   Adapted for afl-unicorn by Dominik Maier <mail@dmnk.co>

   Idea & design very much by Andrew Griffiths.

   Copyright 2015, 2016 Google Inc. All rights reserved.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at:

     http://www.apache.org/licenses/LICENSE-2.0

   This code is a shim patched into the separately-distributed source
   code of Unicorn 1.0.1. It leverages the built-in QEMU tracing functionality
   to implement AFL-style instrumentation and to take care of the remaining
   parts of the AFL fork server logic.

   The resulting QEMU binary is essentially a standalone instrumentation
   tool; for an example of how to leverage it for other purposes, you can
   have a look at afl-showmap.c.

 */

#include "../../config.h"

/* NeverZero */ 

#if (defined(__x86_64__) || defined(__i386__)) && defined(AFL_QEMU_NOT_ZERO)
#  define INC_AFL_AREA(loc) \
    asm volatile ( \
      "incb (%0, %1, 1)\n" \
      "adcb $0, (%0, %1, 1)\n" \
      : /* no out */ \
      : "r" (afl_area_ptr), "r" (loc) \
      : "memory", "eax" \
    )
#else
#  define INC_AFL_AREA(loc) \
  afl_area_ptr[loc]++
#endif
