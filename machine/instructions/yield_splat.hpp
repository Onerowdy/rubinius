#include "interpreter/instructions.hpp"

#include "builtin/block_environment.hpp"
#include "builtin/proc.hpp"

namespace rubinius {
  namespace instructions {
    inline bool yield_splat(STATE, CallFrame* call_frame, intptr_t count) {
      Object* ary = stack_pop();
      Object* t1 = call_frame->scope->block();

      Arguments args(G(sym_call), t1, count, stack_back_position(count));

      if(!ary->nil_p()) {
        args.append(state, as<Array>(ary));
      }

      Object* ret;
      if(BlockEnvironment *env = try_as<BlockEnvironment>(t1)) {
        ret = env->call(state, args);
      } else if(Proc* proc = try_as<Proc>(t1)) {
        ret = proc->yield(state, args);
      } else if(t1->nil_p()) {
        state->raise_exception(Exception::make_lje(state));
        ret = NULL;
      } else {
        Dispatch dispatch(G(sym_call));
        ret = dispatch.send(state, args);
      }

      stack_clear(count);

      state->vm()->checkpoint(state);

      CHECK_AND_PUSH(ret);
    }
  }
}
