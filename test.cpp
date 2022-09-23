#include "src/fbs/auth.hpp"
#include <flatbuffers/buffer.h>
#include <flatbuffers/flatbuffer_builder.h>
#include <iostream>

int main() {
  /* auto req = flatbuffers::GetMutableRoot<fb::LoginRequest>(); */
  flatbuffers::FlatBufferBuilder builder(1024);
  fb::LoginRequestBuilder lr_builder(builder);
  auto username = builder.CreateString("username");
  auto password = builder.CreateString("password");
  auto login_request = fb::CreateLoginRequest(builder, username, password);

  /* auto offset = lr_builder.Finish(); */
  /* std::cout << offset.o << "\n"; */
  /* flatbuffers::GetMutableRoot<fb::LoginRequest>(offset); */
  return 0;
}
