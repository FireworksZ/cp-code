#include<bits/stdc++.h>

using namespace std;

#define double long double

string test = "\"C:/Data/cpp-workspace/cp-code/1/Debug/1.exe\"";
string solution = "\"C:/Data/cpp-workspace/cp-code/1_Solution/Debug/1_Solution.exe\"";

//string data_file = "\"E:/FileTransfer/ICPCèµ„æ–™/2021/hduå¤šæ ¡è�”å�ˆèµ›/01/Another thief in a Shop\"";
string data_file = "C:/Data/cpp-workspace/cp-code/10_checker/input.txt";

string checker_folder = "\"C:/Data/cpp-workspace/cp-code/10_checker/";

char test_cmd[100000];
char judge_cmd[100000];
char fc_cmd[100000];

void preprocess(){

	string user_output = checker_folder+"user_out.txt\"";
	string judge_output = checker_folder+"judge_out.txt\"";

	string test_temp = test+" < "+data_file+" > "+user_output;
//	string test_temp = test+" < "+data_file;

	string judge_temp = solution+" < "+data_file+" > "+judge_output;



	test_temp = ""+test_temp;
	judge_temp = ""+judge_temp;

	int pt = 0;
	for(int i=0;i<(int)test_temp.length();i++){
		test_cmd[pt++] = test_temp[i];
	}
	test_cmd[pt++] = '\0';
	pt = 0;
	for(int i=0;i<(int)judge_temp.length();i++){
		judge_cmd[pt++] = judge_temp[i];
	}
	judge_cmd[pt++] = '\0';


}
int main(){

    preprocess();

    cout<<"Generating judge output..."<<endl;
//    system(judge_cmd);
    cout<<"Generating user output..."<<endl;
    system(test_cmd);



    system("fc.exe user_out.txt judge_out.txt");

    return 0;
}
