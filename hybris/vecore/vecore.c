/*
 * Copyright (C) 2013 Siarhei Siamashka <siarhei.siamashka@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <dlfcn.h>
#include <stddef.h>
#include <stdint.h>

#include <hybris/internal/binding.h>

#define Handle         uint32_t
#define vresult_e      uint32_t
#define vconfig_t      void
#define vstream_info_t void
#define u8             uint8_t
#define u32            uint32_t
#define u64            uint64_t

HYBRIS_LIBRARY_INITIALIZE(cedarxosal, "/system/lib/libcedarxosal.so");
HYBRIS_LIBRARY_INITIALIZE(vecore,     "/system/lib/libve.so");

/*
 * HACK: make sure that cedarx_hardware_init() is called before libve_open(),
 *       otherwise we are in a bigger trouble and libve_open() segfaults.
 */
Handle libve_open(vconfig_t* a, vstream_info_t* b, void* c)
{
    static Handle (*real_libve_open)(vconfig_t*, vstream_info_t*, void*) = NULL;
    static void (*cedarx_hardware_init)() = NULL;
    HYBRIS_DLSYSM(vecore, &real_libve_open, "libve_open");
    HYBRIS_DLSYSM(cedarxosal, &cedarx_hardware_init, "cedarx_hardware_init");
    cedarx_hardware_init();
    return real_libve_open(a, b, c);
}

HYBRIS_IMPLEMENT_FUNCTION2(vecore, vresult_e, libve_close, u8, Handle);
HYBRIS_IMPLEMENT_FUNCTION2(vecore, vresult_e, libve_reset, u8, Handle);
HYBRIS_IMPLEMENT_FUNCTION2(vecore, vresult_e, libve_flush, u8, Handle);
HYBRIS_IMPLEMENT_FUNCTION2(vecore, vresult_e, libve_set_vbv, Handle, Handle);
HYBRIS_IMPLEMENT_FUNCTION2(vecore, vresult_e, libve_set_minor_vbv, Handle, Handle);
HYBRIS_IMPLEMENT_FUNCTION1(vecore, Handle,    libve_get_fbm, Handle);
HYBRIS_IMPLEMENT_FUNCTION1(vecore, Handle,    libve_get_minor_fbm, Handle);
HYBRIS_IMPLEMENT_FUNCTION1(vecore, u32,       libve_get_fbm_num, Handle);
HYBRIS_IMPLEMENT_FUNCTION3(vecore, vresult_e, libve_io_ctrl, u32, u32, Handle);
HYBRIS_IMPLEMENT_FUNCTION4(vecore, vresult_e, libve_decode, u8, u8, u64, Handle);
HYBRIS_IMPLEMENT_FUNCTION2(vecore, vresult_e, libve_get_stream_info, vstream_info_t*, Handle);
HYBRIS_IMPLEMENT_FUNCTION1(vecore, u8*,       libve_get_version, Handle);
HYBRIS_IMPLEMENT_FUNCTION1(vecore, u8*,       libve_get_last_error, Handle);

/* These don't seem to be available in Android */
HYBRIS_IMPLEMENT_FUNCTION1(vecore, vresult_e, libve_set_ive, void*);
HYBRIS_IMPLEMENT_FUNCTION1(vecore, vresult_e, libve_set_ios, void*);
HYBRIS_IMPLEMENT_FUNCTION1(vecore, vresult_e, libve_set_ifbm, void*);
HYBRIS_IMPLEMENT_FUNCTION1(vecore, vresult_e, libve_set_ivbv, void*);
