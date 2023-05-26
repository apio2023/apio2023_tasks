#include <bits/stdc++.h>
#include <signal.h>
using namespace std;

void report_result(double score, string report_message){
	cout << score << endl;
	cerr << report_message << endl;
	exit(0);
}

static constexpr int TESTCASE = 60;
static constexpr int MAX_LA = 100000;
static constexpr int MAX_LB = 100000;
static constexpr char CIRCUIT_WORKING_CORRECT_OUTPUT[] = "Circuit Output Finished.\n";
static constexpr char CIRCUIT_SCORING_CORRECT_OUTPUT[] = "235fc77fd78da210d6b5a61b92603c4f\n";
static char buffer[10000003];
struct CONTENT{
	vector < string > content; bool aliceorbob;
	CONTENT(vector < string > _cont = vector < string > {}, bool f = 0){content = _cont; aliceorbob = f;}
	int numofline(){return content.size();}
	CONTENT split(int L , int R){
		if(L < 0 || R > numofline() || L > R)
			report_result(0, "Grader Error: split() argument invalid. Please Contact Admin.");
		return CONTENT(vector < string > (content.begin() + L, content.begin() + R));
	}
	void write(FILE* file){
		for(auto t : content){
            assert(t.back() == '\n');
            // fprintf(file, t.c_str());
            fputs(t.c_str(), file);
            // cerr << "Manager: written " << t << flush;
        }
		fflush(file);
	}
	void read(FILE* file, int line, bool check_for_binary_output = false){
		for(int i = 0 ; i < line ; ++i){
			char* res = fgets(buffer, 1e7, file);
			if(res == NULL)
				report_result(0, (string("Cannot get output from ") + (aliceorbob ? "Bob" : "Alice") + ". Possible problems: segmential fault / manual exit(0)").c_str());
			string str(res);
            // cerr << "Manager: read line " << str << flush;
			if(str.back() != '\n')
				report_result(0, "Grader Error: there exists a line longer than 10^7. If you do not print anything to stdout, please contact admin.");
            if(check_for_binary_output && i){
				bool binary = 1;
				for(int i = 0 ; i < (int)str.size() - 1 ; ++i)
					binary &= str[i] == '0' || str[i] == '1';
				if(!binary){
					// This line reports an error.
					report_result(0, str);
				}
			}
            if(check_for_binary_output && !i) {
                if (strncmp(res, "235fc77fd78da210d6b5a61b92603c4f", 32))
					report_result(0, str);
            }
			content.push_back(str);
		}
	}
	bool checkSameLength(){
        assert (content.size() >= 2);
        for (int i = 1; i < content.size(); ++i)
            if (content[i].size() != content[1].size())
                return false;
        return true;
	}
};

struct FIFO{
	FILE *USERTOMANAGER, *MANAGERTOUSER;
	void InitializePipe(char* UserToManagerFile, char* ManagerToUserFile){
		MANAGERTOUSER = fopen(ManagerToUserFile, "w");
		USERTOMANAGER = fopen(UserToManagerFile, "r");
		if(USERTOMANAGER == NULL || MANAGERTOUSER == NULL)
			report_result(0, "Grader Error: Cannot open pipe. Please Contact Admin.");
	}
	void Initialize(){
        fputs("Hello\n", MANAGERTOUSER);
		fflush(MANAGERTOUSER);
	}
    ~FIFO() {
        fputs("Goodbye\n", MANAGERTOUSER);
        fflush(MANAGERTOUSER);
        fclose(MANAGERTOUSER);
        fclose(USERTOMANAGER);
    }
	FILE* write_file(){return MANAGERTOUSER;}
	FILE* read_file(){return USERTOMANAGER;}
}Alice, Bob, Circuit;

int main(int argc, char** argv){
    signal(SIGPIPE, SIG_IGN);
    //cerr << "Manager: Hello world!" << endl;
	if(argc != 7){report_result(0, "Grader Error: argc should be 7 but get " + to_string(argc));}
	Alice.InitializePipe(argv[1], argv[2]);
	Bob.InitializePipe(argv[3], argv[4]);
	Circuit.InitializePipe(argv[5], argv[6]);
    //cerr << "Manager: initialize FIFO ok" << endl;
	
	CONTENT InputFile; InputFile.read(stdin, 2 * (TESTCASE + 1));
    //cerr << "Manager: read input ok" << endl;
	CONTENT AliceIn = InputFile.split(0, TESTCASE + 1), BobIn = InputFile.split(TESTCASE + 1 , 2 * (TESTCASE + 1));
	Alice.Initialize(); AliceIn.write(Alice.write_file()); 
	Bob.Initialize(); BobIn.write(Bob.write_file());
   // cerr << "Manager: write input to Alice and Bob ok" << endl;
	
	CONTENT AliceOut, BobOut; BobOut.aliceorbob = 1;
    //cerr << "Manager: before read from Alice" << endl;
	AliceOut.read(Alice.read_file(), TESTCASE + 1, true);
    //cerr << "Manager: read from Alice ok" << endl;
	BobOut.read(Bob.read_file(), TESTCASE + 1, true);
    //cerr << "Manager: read from Bob ok" << endl;

// final_grader_middle rephrase
	if(!AliceOut.checkSameLength())
		report_result(0, "Alice lengths are not all equal.");
	if(!BobOut.checkSameLength())
		report_result(0, "Bob lengths are not all equal.");
	int aliceLength = AliceOut.content[1].size() - 1, bobLength = BobOut.content[1].size() - 1;
	if(aliceLength < 0 || aliceLength > MAX_LA)
		report_result(0, "Alice Length exceeds " + to_string(MAX_LA));
	if(bobLength < 0 || bobLength > MAX_LB)
		report_result(0, "Bob Length exceeds " + to_string(MAX_LB));

	Circuit.Initialize();
	fprintf(Circuit.write_file(), "%d\n%d\n", aliceLength, bobLength);
    fflush(Circuit.write_file());

    //cerr << "Manager: write to Circuit ok" << endl;
	char* circuit_work_res = fgets(buffer, 1e7, Circuit.read_file());
	if(circuit_work_res == NULL || strcmp(circuit_work_res, CIRCUIT_WORKING_CORRECT_OUTPUT) != 0){
		// Circuit returns an error.
		report_result(0, circuit_work_res == NULL ? "Cannot get output from circuit. Possible problems: segmential fault / manual exit(0)" : circuit_work_res);
	}
    //cerr << "Manager: read from Circuit middle output ok" << endl;
	AliceIn.write(Circuit.write_file()); BobIn.write(Circuit.write_file());
	AliceOut.write(Circuit.write_file()); BobOut.write(Circuit.write_file());
    //cerr << "Manager: write everything to Circuit ok" << endl;

	char* scoring_res = fgets(buffer, 1e7, Circuit.read_file());
    int score = 0;
    if (scoring_res != NULL && !strcmp(scoring_res, CIRCUIT_SCORING_CORRECT_OUTPUT)) {
        score = 1;
        scoring_res = fgets(buffer, 1e7, Circuit.read_file());
    }
	report_result(score, scoring_res == NULL ? "Grader Error: Scoring Segmential Fault. Please Contact Admin." : scoring_res);
}