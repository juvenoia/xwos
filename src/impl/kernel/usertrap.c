void uwrite_serial(char a) {
  // SYSCALL
  // 64-bit syscall.
  /*
   * syscall loads CS from STAR 47:32, SS .store RIP in RCX(we need to store rcx)
   * loads RIP from LSTAR.
   * */
  //syscall(); syscall should include saving regs.
  // that means, you need task_struct, and a kernel space to save current running process.
  write_serial(a);

  // SYSRET
  /*
   * loads RIP from RCX. we do not need these datas, because syscall is done every-time-called.
   * */
}