#include "testlib.h"

#include <string>

int main(int argc, char * argv[]) {
  registerChecker("sequence", argc, argv);

  std::string output_secret = "nfp39szm23aa01pcmyosi03slwpeksnfjri3opqp";

  readBothSecrets(output_secret);
  readBothGraderResults();

  compareRemainingLines();
}
