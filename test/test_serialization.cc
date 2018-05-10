#include <iostream>
#include "core/networking.h"
#include "test_timer.h"

void test_serialization_general(){
  ClientId cli_id = 123456;
  GameId game_id = 654321;
  unsigned char request[kMaxBufferLength];
  std::size_t length;
  MakeRequestJoinGame(request, &length, cli_id, game_id);

  ClientId new_cli_id = 0;
  GameId new_game_id = 0;
  ResolveRequestJoinGame(request + 2, length - 2, &new_cli_id, &new_game_id);

  std::cout << "cli_id: " << cli_id << " >> " << new_cli_id << std::endl
            << "game_id: " << game_id << " >> " << new_game_id << std::endl;

  assert(cli_id == new_cli_id);
  assert(game_id == new_game_id);


}

// memcpy implementation 5x faster than bit manipulation implementation for both
// serialization and deserialization.

// but the serialized byte array does not look like the original data, for instance,
// unsigned integer 123456 - [00000000 00000001 11100010 01000000]
// after memcpy to an unsigned char array
// [01000000 11100010 00000001 00000000]
// so you can imagine that the memcpy will treat any type as an byte array,
// and it reads
// ** from the Least Significant Bit to the Most **

void test_serialization_impl(){
  std::size_t times = 100000;
  std::size_t size = sizeof(unsigned int);
  unsigned char buf[kMaxBufferLength];
  unsigned int content = 123456;

  {
    TestTimer timer("WriteToByteArrayImplBitOpts");
    for(int t=0; t<times; t++){
      std::size_t offset = 0;
      for(int i=0; i< 10; i++){
        WriteToByteArrayImplBitOpts<unsigned int>(buf, offset, content);
        offset += sizeof(unsigned int);
      }
    }
  }

  for(int i=0; i< size; i++){
    std::cout << std::bitset<8>(buf[i]) << " ";
  }
  std::cout << std::endl;


  {
    TestTimer timer("WriteToByteArrayImplMemcpy");
    for(int t=0; t<times; t++){
      std::size_t offset = 0;
      for(int i=0; i< 10; i++){
        WriteToByteArrayImplMemcpy<unsigned int>(buf, offset, content);
        offset += sizeof(unsigned int);
      }

    }
  }

  for(int i=0; i< size; i++){
    std::cout << std::bitset<8>(buf[i]) << " ";
  }
  std::cout << std::endl;

  unsigned int new_content = 0;

  {
    TestTimer timer("ReadFromByteArrayImplBitOpts");
    for(int t=0; t<times; t++){
      std::size_t offset = 0;
      for(int i=0; i< 10; i++){
        ReadFromByteArrayImplBitOpts<unsigned int>(buf, offset, &new_content);
        assert(new_content != content);
        offset += sizeof(unsigned int);
      }
    }
  }

  {
    TestTimer timer("ReadFromByteArrayImplMemcpy");
    for(int t=0; t<times; t++){
      std::size_t offset = 0;
      for(int i=0; i< 10; i++){
        ReadFromByteArrayImplMemcpy<unsigned int>(buf, offset, &new_content);
        offset += sizeof(unsigned int);
      }
    }
  }

}

int main(int argc, char** argv){
  test_serialization_general();
  //test_serialization_impl();
  return 0;
}
