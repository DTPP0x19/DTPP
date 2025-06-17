#ifndef DTPP_HOOK_H
#define DTPP_HOOK_H
#include "symbol_cache.h"
#include "tlog.h"
static SymbolCache sym;

#define install_hook(name, fn_ret_t, fn_args_t...)                                                                     \
fn_ret_t (*orig_##name)(fn_args_t);                                                                                  \
fn_ret_t fake_##name(fn_args_t);                                                                                     \
/* __attribute__((constructor)) */ static void install_hook_##name() {                                               \
  void *sym_addr = sym.get(#name);                                                                                  \
  DobbyHook(sym_addr, (dobby_dummy_func_t)fake_##name, (dobby_dummy_func_t *)&orig_##name);                          \
  tlog_info("install hook %s:%p:%p\n", #name, sym_addr, orig_##name);                                           \
}                                                                                                                    \
fn_ret_t fake_##name(fn_args_t)

void load_resources();
void hook();


#endif
