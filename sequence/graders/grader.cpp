#include "sequence.h"

#include <cassert>
#include <cstdio>

// BEGIN SECRET
#include <string>
// END SECRET
#include <vector>

int main() {
  // BEGIN SECRET
  {
    std::string in_secret = "8wq90di9812978rqwiok0k0o21klklm21oiwi121";
    std::string out_secret = "nfp39szm23aa01pcmyosi03slwpeksnfjri3opqp";
    char secret[1000];
    assert(1 == scanf("%s", secret));
    if (std::string(secret) != in_secret) {
      printf("%s\n", out_secret.c_str());
      printf("SV\n");
      fclose(stdout);
      return 0;
    }
  }
  // END SECRET
  int N, K;
  assert(1 == scanf("%d", &N));

  std::vector<int> A(N);
  for (int i = 0; i < N; ++i) {
    assert(1 == scanf("%d", &A[i]));
  }

  int result = sequence(N, A);
  // BEGIN SECRET
  {
    std::string out_secret = "nfp39szm23aa01pcmyosi03slwpeksnfjri3opqp";
    printf("%s\n", out_secret.c_str());
    printf("OK\n");
  }
  // END SECRET
  printf("%d\n", result);
  return 0;
}
