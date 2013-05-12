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
#include <string.h>

#include <hybris/internal/binding.h>

#define Handle         uint32_t
#define vresult_e      uint32_t
#define vconfig_t      void
#define vstream_info_t void
#define u8             uint8_t
#define u32            uint32_t
#define u64            uint64_t

HYBRIS_LIBRARY_INITIALIZE(cedarv_base,    "/system/lib/libcedarv_base.so");
HYBRIS_LIBRARY_INITIALIZE(cedarv_adapter, "/system/lib/libcedarv_adapter.so");
HYBRIS_LIBRARY_INITIALIZE(vecore,         "/system/lib/libve.so");

HYBRIS_IMPLEMENT_FUNCTION3(vecore, Handle,    libve_open, vconfig_t*, vstream_info_t*, void*);
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

/*
 * The libve library ABI is unfortunately unstable and suffers from
 * pointless changes from time to time. One of such changes is the
 * reshuffling of arguments for 'fbm_init_ex' function (the introduction
 * of 'dummy' argument, which seems to be always NULL). We mitigate this
 * problem by introducing a thunk function, which emulates calling
 * conventions used in the linux armhf libvecore blob.
 */

static Handle (*real_fbm_init_ex)(u32 max_frame_num,
            u32 min_frame_num, u32 size_y[2],
            u32 size_u[2], u32 size_v[2],
            u32 size_alpha[2], u32 out_3d_mode,
            u32 format, void* parent);

static Handle fbm_init_ex_thunk(u32 max_frame_num,
            u32 min_frame_num, u32 size_y[2],
            u32 size_u[2], u32 size_v[2],
            u32 size_alpha[2], u32 out_3d_mode,
            u32 format, void *dummy, void* parent)
{
    return real_fbm_init_ex(max_frame_num,
            min_frame_num, size_y,
            size_u, size_v,
            size_alpha, out_3d_mode,
            format, parent);
}

/* Overwrite the corresponding interface structures */

vresult_e libve_set_ive(void *new_ive)
{
    static void *IVE;
    HYBRIS_DLSYSM(cedarv_adapter, &IVE, "IVE");
    memcpy(IVE, new_ive, 6 * 4);
    return 0;
}

vresult_e libve_set_ios(void *new_ios)
{
    static void *IOS;
    HYBRIS_DLSYSM(cedarv_adapter, &IOS, "IOS");
    memcpy(IOS, new_ios, 10 * 4);
    return 0;
}

vresult_e libve_set_ifbm(void *new_ifbm)
{
    static void **IFBM;
    HYBRIS_DLSYSM(cedarv_base, &IFBM, "IFBM");
    memcpy(IFBM, new_ifbm, 5 * 4);
    real_fbm_init_ex = IFBM[4];
    IFBM[4] = fbm_init_ex_thunk;
    return 0;
}

vresult_e libve_set_ivbv(void *new_ivbv)
{
    static void *IVBV;
    HYBRIS_DLSYSM(cedarv_base, &IVBV, "IVBV");
    memcpy(IVBV, new_ivbv, 5 * 4);
    return 0;
}
