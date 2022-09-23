#ifndef HTTP_ADAPTER_FACTORY_HPP
#define HTTP_ADAPTER_FACTORY_HPP

#include "adapters/http/wrapper.hpp"
#include "interfaces/logger/singleton.hpp"
#include "interfaces/store/singleton.hpp"
#include "wrappers/http/request.hpp"
#include "wrappers/http/response.hpp"
#include <memory>

namespace adapter {

template <
    typename Store = interface::DefStore,
    typename Logger = interface::DefLogger>
using Request = IRequest<WrapperRequest<Store, Logger>>;
using Response = IResponse<WrapperResponse>;

} // namespace adapter

#endif

