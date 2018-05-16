// Created by Chuilian Kong April 30, 2018

#ifndef CORE_NETWORKING_NETWORKING_H_
#define CORE_NETWORKING_NETWORKING_H_

// asio stand alone mode
#define ASIO_STANDALONE
#define ASIO_HAS_STD_ADDRESSOF
#define ASIO_HAS_STD_ARRAY
#define ASIO_HAS_CSTDINT
#define ASIO_HAS_STD_SHARED_PTR
#define ASIO_HAS_STD_TYPE_TRAITS

#include <cstring>
#include "asio.hpp"
#include "core/networking/messages.h"

#endif  // CORE_NETWORKING_NETWORKING_H_
