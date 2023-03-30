#include "../utils.c"

int main() {
  struct statvfs sysdisk_status;
  statvfs("/", &sysdisk_status);
  int total = to_gigabytes(sysdisk_status.f_blocks * sysdisk_status.f_frsize);
  int free = to_gigabytes(sysdisk_status.f_bfree * sysdisk_status.f_frsize);
  int used = total - free;
  int percentage = (int) (((float) used / total) * 100);
  printf("%%F{green}%s%%f%d%%%%\n", choose_symbol(" ", "DISK "), percentage);
}
