static int countDigits(int number) {
  int totalDigits;
  for (totalDigits = !number; number; number /= 10) {
    ++totalDigits;
  }
  return totalDigits;
}
