// Created by Chuilian Kong April 30, 2018

#ifndef CORE_SERIALIZATION_H
#define CORE_SERIALIZATION_H

#include <cstring>

namespace serialization{
  //*********************************
  // write type to the byte array
  //*********************************
  template<typename T>
  static void WriteToByteArrayImplBitOpts(unsigned char* arr, std::size_t offset, T data){
    // an implementation of converting typed data to byte array -- using bit operations
    std::size_t type_size = sizeof(T);
    T mask = 0xFF; // 0000 0000 0000 0000 0000 0000 1111 1111 for 4-byte int
    std::size_t mask_offset = 0;
    for(std::size_t i = 0; i < type_size; i++){
      arr[offset + type_size - 1 - i] = static_cast<unsigned char>((data & (mask << mask_offset)) >> mask_offset);
      mask_offset += 8;
    }
  }

  template<typename T>
  static void WriteToByteArrayImplMemcpy(unsigned char* arr, std::size_t offset, T data){
    // an implementation of converting typed data to byte array -- using memcpy
    std::memcpy(arr + offset, &data, sizeof(T));
  }

  template<typename T>
  static void WriteToByteArray(unsigned char* arr, std::size_t offset, T data){
    //WriteToByteArrayImplBitOpts<T>(arr, offset, data);
    WriteToByteArrayImplMemcpy<T>(arr, offset, data);
  }

  //*********************************
  // read type from the byte array
  //*********************************
  template<typename T>
  static void ReadFromByteArrayImplBitOpts(unsigned char* arr, std::size_t offset, T* data){
    // an implementation of converting byte array to typed data -- using bit operations
    std::size_t type_size = sizeof(T);
    std::memset(data, 0, type_size);
    std::size_t mask_offset = 0;
    for(std::size_t i = 0; i < type_size; i++){
      *data |= (static_cast<T>(arr[offset + type_size - 1 - i]) << mask_offset);
      mask_offset += 8;
    }
  }

  template<typename T>
  static void ReadFromByteArrayImplMemcpy(unsigned char* arr, std::size_t offset, T* data){
    // an implementation of converting byte array to typed data -- using memcpy
    std::memcpy(data, arr + offset, sizeof(T));
  }

  template<typename T>
  static void ReadFromByteArray(unsigned char* arr, std::size_t offset, T* data){
    //ReadFromByteArrayImplBitOpts<T>(arr, offset, data);
    ReadFromByteArrayImplMemcpy<T>(arr, offset, data);
  }
}

#endif  // CORE_SERIALIZATION_H
