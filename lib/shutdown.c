#include <unistd.h>

/* code de la fonction
   int shutdown();
   un argument -> appel à la fonction d'enveloppe syscall1
*/
syscall1(int, shutdown, int, n)