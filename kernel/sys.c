#include <n7OS/sys.h>
#include <n7OS/syscall_defs.h>
#include <n7OS/console.h>
#include <n7OS/irq.h>
#include <n7OS/cpu.h>
#include <unistd.h>

extern void handler_syscall();

void init_syscall() {
  // ajout de la fonction de traitement de l'appel systeme
  add_syscall(NR_example, sys_example);
  add_syscall(NR_shutdown, sys_shutdown);
  add_syscall(NR_write, sys_write);

  // initialisation de l'IT soft qui gère les appels systeme
  init_irq_entry(0x80, (uint32_t) handler_syscall);
}

/**
 * @brief Handler of the example syscall.
 */
int sys_example() {
  // on ne fait que retourner 1
  return 1;
}

/**
 * @brief Handler of the shutdown syscall.
 * 
 * @param n indicate the poweroff (1) or nothing (other)
 */
int sys_shutdown(int n) {
  if (n == 1) {
    outw(0x2000, 0x604); // Poweroff QEMU > 2.0
    return -1;
  } else {
    return 1;
  }
}

/**
 * @brief Handler of the write syscall.
 * 
 * @param s the string to write
 * @param len the length of the string
 */
int sys_write(const char *s, int len) {
  console_putbytes(s, len);
  return 1;
}
