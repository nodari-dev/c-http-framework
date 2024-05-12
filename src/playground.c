#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test(int *count) { count += 2; }

int main() {

  int *count = 1;
  printf("%d\n", count);
  test(count);
	count += 3;
  printf("%d\n", count);
  return 0;
}
