#include "cyberland.h"

#include <cassert>
#include <cstdio>
#include <iostream>
#include <vector>
#include <cstring>

int main() {
  int T;
  const std::string sECReT_oUtPUT = "a9100fda0a7059a979d560b8550f715d4ee391ff9b8b680b2f87b26a69ee5a5e";
	std::cout << sECReT_oUtPUT << std::endl;
  assert(1 == scanf("%d", &T));
  while (T--){
    int N,M,K,H;
    assert(4 == scanf("%d %d %d\n%d", &N, &M, &K, &H));
    std::vector<int> x(M);
    std::vector<int> y(M);
    std::vector<int> c(M);
    std::vector<int> arr(N);
    for (int i=0;i<N;i++)
      assert(1 == scanf("%d", &arr[i]));
    for (int i=0;i<M;i++)
      assert(3 == scanf("%d %d %d", &x[i], &y[i], &c[i]));
    printf("%.12lf\n", solve(N, M, K, H, x, y, c, arr));
  }
  std::cout << sECReT_oUtPUT << std::endl;
}
