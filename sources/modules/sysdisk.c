#include "../utils.c"

int
main()
{
  struct statvfs sysdisk_status;
  statvfs("/", &sysdisk_status);
  unsigned long total = sysdisk_status.f_blocks * sysdisk_status.f_bsize;
  unsigned long free = sysdisk_status.f_bfree * sysdisk_status.f_bsize;
  unsigned long used = total - free;
  unsigned int percentage = (unsigned int) (((float) used / total) * 100);
  printf("%%F{green}%s%%f%u%%%%\n", choose_symbol(" ", "DISK "), percentage);
  return 0;
}
