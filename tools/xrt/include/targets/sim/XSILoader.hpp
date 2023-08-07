/*******************************************************************************
#  Copyright (C) 2021 Xilinx, Inc
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#
# *******************************************************************************/

#pragma once
#include <dlfcn.h>
#include <string>
#include <stdexcept>
#include <exception>
#include "xsi.h"

namespace Xsi
{
    class SharedLibrary
    {
    public:
        typedef void * handle_type;
        typedef void * symbol_type;

        SharedLibrary():
            _lib(0),
            _retain(false) {

        }

        ~SharedLibrary() {
            unload();
        }

        operator bool() const {
            return (_lib != 0);
        }

        bool loaded() const {
            return (_lib != 0);
        }

        handle_type handle() const {
            return _lib;
        }

        const std::string& path() const {
            return _path;
        }

        const std::string& error() const {
            return _err;
        }

        bool load(const std::string& path)
        {
            unload();
            // reset the retain flag
            _retain = false;

            if (path.empty()) {
                _err = "[XSI_LOADER] Failed to load shared library. Path of the shared library is not specified.";
                return false;
            }

            std::string msg;
            bool ok = load_impl(path, msg);

            if (ok) {
                _path = path;
                _err.clear();
            }
            else {
                _err = "\n[XSI_LOADER] Failed to load shared library \"" + path + "\". " + "\n" + msg;
            }
            return ok;
        }

        bool load_impl(const std::string& path, std::string& errmsg)
        {
            bool ok = true;

            _lib = dlopen(path.c_str(), RTLD_LAZY | RTLD_GLOBAL);
            char *err = dlerror();
            if (err != NULL) {
                errmsg = err;
                ok = false;
            }
            return ok;
        }

        void unload()
        {
            if (_lib) {
                if (!_retain) {
                    dlclose(_lib);
                }
                _lib = 0;
            }
            _err.clear();
        }

        void retain()
        {
            _retain = true;
        }

        bool getsymbol(const std::string& name, symbol_type& sym)
        {
            std::string msg;
            bool ok = true;

            if (_lib == 0) {
                msg = "The shared library is not loaded.";
                ok = false;
            }
            else {
                dlerror(); // clear error
                sym = (void *) dlsym(_lib, name.c_str());
                char *err = dlerror();
                if (err != NULL) {
                    msg = err;
                    ok = false;
                }
            }

            if (ok) {
                _err.clear();
            }
            else {
                _err = "Failed to obtain symbol \""
                        + name +
                       "\" from shared library. " + msg;
            }

            return ok;
        }

        symbol_type getfunction(const std::string& name)
        {
            symbol_type sym = NULL;
            return getsymbol(name, sym) ? sym : NULL;
        }

    private:
        // shared library is non-copyable
        SharedLibrary(const SharedLibrary&);
        const SharedLibrary& operator=(const SharedLibrary&);
        handle_type _lib;
        std::string _path;
        std::string _err;
        bool _retain;
    };

    class LoaderException : public std::exception
    {
    public:
        LoaderException(const std::string& msg):
        _msg("ISim engine error: " + msg)
        {

        }

        virtual ~LoaderException() throw() { }

        virtual const char * what() const throw()
        {
            return _msg.c_str();
        }

    private:
        std::string _msg;
    };

    class Loader
    {
    public:
        Loader();
        Loader(const std::string_view& dll_name, const std::string_view& simkernel_libname);
        ~Loader();

        bool isopen() const;
        void open(p_xsi_setup_info setup_info);
        void close();
        void run(XSI_INT64 step);
        void restart();
        int get_num_ports();
        float get_time_precision();
        int get_value(int port_number, void* value);
        int get_port_number(const char* port_name);
        int get_port_bits(int port_number);
        bool port_is_input(int port_number);
        const char *get_port_name(int port_number);
        void put_value(int port_number, const void* value);
        int get_status();
        const char* get_error_info();
        void trace_all();
        XSI_INT64 get_time();
        void generate_clock(XSI_INT32 port_number,  XSI_UINT64 timeLow,  XSI_UINT64 timeHigh);

    private:
        bool initialize();

        Xsi::SharedLibrary _design_lib;
        Xsi::SharedLibrary _simkernel_lib;
        std::string _design_libname;
        std::string _simkernel_libname;
        //handle
        xsiHandle _design_handle;
        t_fp_xsi_open _xsi_open;
        t_fp_xsi_close _xsi_close;
        t_fp_xsi_run _xsi_run;
        t_fp_xsi_get_value _xsi_get_value;
        t_fp_xsi_put_value _xsi_put_value;
        t_fp_xsi_get_status _xsi_get_status;
        t_fp_xsi_get_error_info _xsi_get_error_info;
        t_fp_xsi_restart _xsi_restart;
        t_fp_xsi_get_port_number _xsi_get_port_number;
        t_fp_xsi_get_port_name _xsi_get_port_name;
        t_fp_xsi_trace_all _xsi_trace_all;
        t_fp_xsi_get_time _xsi_get_time;
        t_fp_xsi_get_int _get_int_property;
        t_fp_xsi_get_int_port _get_int_port_property;
        t_fp_xsi_generate_clock _xsi_generate_clock;

    };
}
