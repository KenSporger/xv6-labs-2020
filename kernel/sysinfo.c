#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "defs.h"
#include "proc.h"
#include "sysinfo.h"

int
sysinfo(uint64 addr)
{
  struct proc *p = myproc();
  struct sysinfo info;
  
  info.freemem = freemem();
  info.nproc = nproc();

  if (copyout(p->pagetable, addr, (char *)&info, sizeof(info)) < 0)
    return -1;
  
  return 0;
}

// sysinfo system call
// get sysinfo struct address
// call sysinfo()
uint64
sys_sysinfo(void)
{
  uint64 info;
  if (argaddr(0, &info) < 0)
    return -1;
    
  return sysinfo(info);
}