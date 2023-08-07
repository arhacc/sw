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

#include <iostream>
#include <cmath>
#include "targets/sim/XSILoader.hpp"
using namespace Xsi;

Loader::Loader(const std::string_view& design_libname, const std::string_view& simkernel_libname):
        _design_libname(design_libname),
        _simkernel_libname(simkernel_libname),
        _design_handle{nullptr},
        _xsi_open{nullptr},
        _xsi_close{nullptr},
        _xsi_run{nullptr},
        _xsi_get_value{nullptr},
        _xsi_put_value{nullptr},
        _xsi_get_status{nullptr},
        _xsi_get_error_info{nullptr},
        _xsi_restart{nullptr},
        _xsi_get_port_number{nullptr},
        _xsi_get_port_name{nullptr},
        _xsi_trace_all{nullptr},
        _xsi_get_time{nullptr},
        _get_int_property{nullptr},
        _get_int_port_property{nullptr},
        _xsi_generate_clock{nullptr}
{
    if (!initialize()) {
        throw LoaderException("Failed to Load up XSI.");
    }
}

Loader::~Loader()
{
    close();
}

bool Loader::isopen() const
{
    return (_design_handle != nullptr);
}

void Loader::open(p_xsi_setup_info setup_info)
{
    _design_handle = _xsi_open(setup_info);
}

void Loader::close()
{
    if (_design_handle)
    {
        _xsi_close(_design_handle);
        _design_handle = nullptr;
    }
}

void Loader::run(XSI_INT64 step)
{
    _xsi_run(_design_handle, step);
}

void Loader::restart()
{
    _xsi_restart(_design_handle);
}

int Loader::get_value(int port_number, void* value)
{
    return _xsi_get_value(_design_handle, port_number, value);
}

int Loader::get_port_number(const char* port_name)
{
    return _xsi_get_port_number(_design_handle, port_name);
}

const char* Loader::get_port_name(int port_number)
{
    return _xsi_get_port_name(_design_handle, port_number);
}

void Loader::put_value(int port_number, const void* value)
{
    _xsi_put_value(_design_handle, port_number, const_cast<void*>(value));
}

int Loader::get_status()
{
    return _xsi_get_status(_design_handle);
}

const char* Loader::get_error_info()
{
    return _xsi_get_error_info(_design_handle);
}

void Loader::trace_all()
{
    _xsi_trace_all(_design_handle);
}

int Loader::get_num_ports()
{
    return _get_int_property(_design_handle, xsiNumTopPorts);
}

float Loader::get_time_precision()
{
    return std::pow(10.0, _get_int_property(_design_handle, xsiTimePrecisionKernel));
}

int Loader::get_port_bits(int port_number)
{
    return _get_int_port_property(_design_handle, port_number, xsiHDLValueSize);
}

bool Loader::port_is_input(int port_number)
{
    return (_get_int_port_property(_design_handle, port_number, xsiDirectionTopPort) == xsiInputPort);
}

XSI_INT64 Loader::get_time()
{
    return _xsi_get_time(_design_handle);
}

void Loader::generate_clock(XSI_INT32 port_number,  XSI_UINT64 timeLow,  XSI_UINT64 timeHigh)
{
    _xsi_generate_clock(_design_handle, port_number, timeLow, timeHigh);
}

bool Loader::initialize()
{
    // Load ISim design shared library
    if (!_design_lib.load(_design_libname)) {
        std::cerr << "[DESIGN LIB ERROR] Could not load XSI simulation shared library (" << _design_libname  << "): "  << _design_lib.error()  << std::endl;
        return false;
    }

    // Load Simulator Kernel shared library
    if (!_simkernel_lib.load(_simkernel_libname)) {
        std::cerr << "[KERNEL LIB ERROR] Could not load simulation kernel library ("  << _simkernel_libname << "): "  << _simkernel_lib.error()  << std::endl;
        return false;
    }

    // Get function pointer for getting an ISIM design handle
    _xsi_open = (t_fp_xsi_open) _design_lib.getfunction("xsi_open");
    if (!_xsi_open)
        return false;

    // Get function pointer for running ISIM simulation
    _xsi_run = (t_fp_xsi_run) _simkernel_lib.getfunction("xsi_run");
    if (!_xsi_run)
        return false;

    // Get function pointer for terminating ISIM simulation
    _xsi_close = (t_fp_xsi_close) _simkernel_lib.getfunction("xsi_close");
    if (!_xsi_close)
        return false;

    // Get function pointer for running ISIM simulation
    _xsi_restart = (t_fp_xsi_restart) _simkernel_lib.getfunction("xsi_restart");
    if (!_xsi_restart)
        return false;

    // Get function pointer for reading data from ISIM
    _xsi_get_value = (t_fp_xsi_get_value) _simkernel_lib.getfunction("xsi_get_value");
    if (!_xsi_get_value)
        return false;

    // Get function pointer for reading data from ISIM
    _xsi_get_port_number = (t_fp_xsi_get_port_number) _simkernel_lib.getfunction("xsi_get_port_number");
    if (!_xsi_get_port_number)
        return false;

    // Get function pointer for reading data from ISIM
    _xsi_get_port_name = (t_fp_xsi_get_port_name) _simkernel_lib.getfunction("xsi_get_port_name");
    if (!_xsi_get_port_name)
        return false;

    // Get function pointer for passing data to ISIM
    _xsi_put_value = (t_fp_xsi_put_value) _simkernel_lib.getfunction("xsi_put_value");
    if (!_xsi_put_value)
        return false;

    // Get function pointer for checking error status
    _xsi_get_status = (t_fp_xsi_get_status) _simkernel_lib.getfunction("xsi_get_status");
    if (!_xsi_get_status)
        return false;

    // Get function pointer for getting error message
    _xsi_get_error_info = (t_fp_xsi_get_error_info) _simkernel_lib.getfunction("xsi_get_error_info");
    if (!_xsi_get_error_info)
        return false;

    // Get function pointer for tracing all signals to WDB
    _xsi_trace_all = (t_fp_xsi_trace_all) _simkernel_lib.getfunction("xsi_trace_all");
    if (!_xsi_trace_all)
        return false;

    // Get function pointer for querying design properties
    _get_int_property = (t_fp_xsi_get_int) _simkernel_lib.getfunction("xsi_get_int");
    if (!_get_int_property)
        return false;

    // Get function pointer for querying port properties
    _get_int_port_property = (t_fp_xsi_get_int_port) _simkernel_lib.getfunction("xsi_get_int_port");
    if (!_get_int_port_property)
        return false;

    // Get function pointer for querying actual time of design
    _xsi_get_time = (t_fp_xsi_get_time) _simkernel_lib.getfunction("xsi_get_time");
    if (!_xsi_get_time)
        return false;

    // Get function pointer for generating clock of design
    _xsi_generate_clock = (t_fp_xsi_generate_clock) _simkernel_lib.getfunction("xsi_generate_clock");
    if (!_xsi_generate_clock)
        return false;

    return true;
}


