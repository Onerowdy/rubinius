#include "interpreter/instructions.hpp"

#include "builtin/location.hpp"

namespace rubinius {
  namespace instructions {
    inline void raise_exc(STATE, CallFrame* call_frame) {
      Object* t1 = stack_pop();
      Exception* exc = as<Exception>(t1);
      exc->locations(state, Location::from_call_stack(state));
      state->raise_exception(exc);
    }
  }
}
