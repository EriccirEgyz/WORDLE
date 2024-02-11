#include <set>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <chrono>
#include <iomanip>
#include <cmath>
#include <unordered_map>
#include <utility>
#include <map>
#include <ncurses.h>
using namespace std;
// 此类读入并存储了候选词和可用词集合
// 调用此类的两个成员函数可以判断一个单词是否属于候选词/可用词集合
class WordSet {
public:
    // 在构造时，从文件读入候选集和可用集。
    // 你需要在代码中实例化一个 WordSet 类型变量。
    WordSet() {
        std::ifstream FINAL_FILE;
        FINAL_FILE.open("./FINAL.txt", std::ifstream::in);
        while(!FINAL_FILE.eof()) {
            std::string str;
            FINAL_FILE >> str;
            final_set.insert(str);
        }
        std::ifstream ACCEPTABLE_FILE;
        ACCEPTABLE_FILE.open("./ACC.txt", std::ifstream::in);
        while(!ACCEPTABLE_FILE.eof()) {
            std::string str;
            ACCEPTABLE_FILE >> str;
            acceptable_set.insert(str);
        }
    }
    // 判断输入参数 word 是否属于候选集合
    bool in_final_set(std::string word) {
        return final_set.find(word) != final_set.end();
    }
    // 判断输入参数 word 是否属于可用集合
    bool in_acceptable_set(std::string word) {
        return acceptable_set.find(word) != acceptable_set.end();
    }
//private: 下面两个成员变量是否可以设成公共成员
    std::set<std::string> final_set;
    std::set<std::string> acceptable_set;
};

char alphabet[26]={'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
int answer_letter_count[26]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int guess_letter_count[26]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
char alphabet_status[26]={'X','X','X','X','X','X','X','X','X','X','X','X','X','X','X','X','X','X','X','X','X','X','X','X','X','X'};
char guess_status[5]={'R','R','R','R','R'};

int success=0, failure=0;
int totaltrying=0;
double averagetrying=0;

void answer_analyze (string word){ //用answer_letter_count记录answer中字母的出现情况
    for (int i=0; i<=4; i++){
        for (int j=0; j<=25; j++){
            if (alphabet[j]==word[i]){
                answer_letter_count[j]+=1;
            }
        }
    }
}

void input_analyze (string word){  //用guess_letter_count记录猜测中字母的出现情况
    for (int i=0; i<=4; i++){
        for (int j=0; j<=25; j++){
            if (alphabet[j]==word[i]){
                guess_letter_count[j]+=1;
            }
        }
    }
}

void outputlater (string answer, string guess){ //判断所有字母的状态
    for (int i=0; i<=25; i++){
        if (guess_letter_count[i]>0){
            if (answer_letter_count[i]==0){ //猜测出现但答案没有出现的字母是red
                alphabet_status[i]='R';
            }
            else{
                if (alphabet_status[i]!='G'){ //猜测中出现但没有被判断为red和green的就是yellow
                    alphabet_status[i]='Y';
                }
                for (int j=0; j<=4; j++){
                    if ((answer[j]==alphabet[i])&&(guess[j]==alphabet[i])){
                        alphabet_status[i]='G'; //某个位置guess和answer一样就是green
                    }
                }
            }
        }
    }
}

void outputformer (string answer, string guess){ //判断用户猜测结果中每个字母的状态
    for (int i=0; i<=4; i++){
        bool isbreakloop = true;
        for (int j=0; j<=4 && isbreakloop; j++){
            if (answer[j]==guess[i]){
                if (i==j){
                    guess_status[i]='G';
                    isbreakloop=false; //当一个字母被判断为green因为优先级最高，就不可能被改变了
                }
            }
        }
    }
    for (int i=0; i<=4; i++){  //将可能的red改为yellow
        if (guess_status[i]=='R') {
            for (int j=0; j<=4; j++){
                if (answer[j]==guess[i]){
                    int charcaterinansweravailable=0;
                    int characteringuess=0;
                    for (int k=0; k<=4; k++){
                        if ((answer[k]==answer[j])&&(guess_status[k]!='G')){
                            charcaterinansweravailable+=1;
                        }
                        if ((guess[k]==guess[i])&&(guess_status[k]=='Y')){
                            characteringuess+=1;
                        }
                    }
                    if (charcaterinansweravailable>characteringuess){ //答案中该字母有足够没有被记做green和yellow的，否则还是red状态
                        guess_status[i]='Y';
                    }
                }
            }
        }
    }
}

void local_initialization(){ //数组初始化：（数组4 猜测字母状态；数组2 猜测中字母出现状态）
    for (int k = 0; k <= 4; k++) {
        guess_status[k] = 'R';
    }
    for (int m = 0; m <= 25; m++) {
        guess_letter_count[m] = 0;
    }
}

void basic_analyze_output(string answer, string guess){ //在基础要求中一直沿用的输出模式：前五个加后26个
    answer_analyze(answer);
    input_analyze(guess);
    outputlater(answer, guess);
    outputformer(answer, guess);
    for (int i = 0; i <= 4; i++) {
        cout << guess_status[i];
    }
    cout << " ";
    for (int j = 0; j <= 25; j++) {
        if (j == 25) {
            cout << alphabet_status[j] << endl; //26个字母判断完之后换行
        } else {
            cout << alphabet_status[j];
        }
    }
}

void singlegame(){ //task1 单局游戏
    string answer;
    cin.ignore(); //为了防止之前cin输入的影响
    getline(cin,answer);
    WordSet answerobject;
    if (answerobject.in_final_set(answer)){ //answer在final_set中
        bool isbreakloop1=true; //用于猜对后退出循环
        for (int times=1; times<=6 && isbreakloop1; ) {
            string guess;
            getline(cin,guess);
            WordSet guessobject;
            if (!(guessobject.in_acceptable_set(guess))) { //猜测要在acceptable_set中
                cout << "INVALID" << endl;
            } else {
                basic_analyze_output(answer,guess);
                //检测是否获得了正确猜测
                if (guess == answer) {
                    cout << "CORRECT" << " " << times << endl;
                    success+=1;
                    totaltrying+=times;
                    isbreakloop1 = false;
                } else {
                    if (times == 6) { //6次全猜错情况
                        cout << "FAILED" << " ";
                        failure+=1;
                        for (int i = 0; i <= 4; i++) {
                            answer[i] = toupper(answer[i]); //toupper变大写
                            cout << answer[i];
                            if (i==4){
                                cout << endl;
                            }
                        }
                    }
                }
                times++; //正确输入才消耗次数
            }
            local_initialization();
        }
    }
}

void multiplegame() { //task2 多局游戏
    for (char whethercontinue = 'Y'; whethercontinue == 'Y';) {
        memset(answer_letter_count,0,sizeof(int)*26);
        memset(guess_letter_count,0,sizeof(int)*26);
        memset(alphabet_status,'X',sizeof(char)*26);
        memset(guess_status,'R',sizeof(char)*5);
        singlegame();
        cin >> whethercontinue;
    }
}

char recordguess[5]={'R','R','R','R','R'}; //用于记录之前的猜测
string recordguess1;
bool validity=true;

void difficulty(){ //困难模式的单局游戏
    string answer;
    cin.ignore();
    getline(cin,answer);
    WordSet answerobject;
    if (answerobject.in_final_set(answer)){
        bool isbreakloop1=true;
        for (int times=1; times<=6 && isbreakloop1; ) {
            validity=true; //用于判断输入是否符合条件
            string guess;
            getline(cin,guess);
            for (int i=0; i<=4; i++){
                if (recordguess[i]=='G'){ //如果原来猜测一个位置是green 则不能变动
                    if (guess[i]!=answer[i]){
                        validity=false;
                    }
                }
                if (recordguess[i]=='Y'){ // 检测是否用到了存在但位置不正确的字母
                    bool whether_use_yellow_chars=false;
                    for (int a=0; a<=4; a++){
                        if ((guess[a]==recordguess1[i])&&(recordguess[i]!='G')){
                            whether_use_yellow_chars=true;
                        }
                    }
                    if (whether_use_yellow_chars==false){
                        validity=false;
                    }
                }
            }
            WordSet guessobject;
            if (guessobject.in_acceptable_set(guess) == false) {
                cout << "INVALID" << endl;
            }
            else if((validity==false)&&(times>=2)){ //新增了一种invalid的情况
                cout << "INVALID" << endl;
            }
            else {
                recordguess1=guess;
                basic_analyze_output(answer,guess);
                for (int k=0; k<=4; k++){
                    recordguess[k]=guess_status[k];
                } //将猜测的内容和字母状态都储存下来，用于检测下一次猜测是否符合条件
                if (guess == answer) {
                    cout << "CORRECT" << " " << times << endl;
                    success+=1;
                    totaltrying+=times;
                    isbreakloop1 = false;
                } else {
                    if (times == 6) {
                        cout << "FAILED" << " ";
                        failure+=1;
                        for (int i = 0; i <= 4; i++) {
                            answer[i] = toupper(answer[i]);
                            cout << answer[i];
                            if (i==4){
                                cout << endl;
                            }
                        }
                    }
                }
                times++;
            }
            local_initialization();
        }
    }
}

void initialization(){ //一些受影响的数组进行初始化
    memset(answer_letter_count,0,sizeof(int)*26);
    memset(guess_letter_count,0,sizeof(int)*26);
    memset(alphabet_status,'X',sizeof(char)*26);
    memset(guess_status,'R',sizeof(char)*5);
    memset(recordguess,'R',sizeof(char)*5);
}

void difficultmode(){ //task3,启动困难模式
    for (char whethercontinue = 'Y'; whethercontinue == 'Y';) {
        initialization();
        difficulty();
        cin >> whethercontinue;
    }
}

void statistics(){ //task4, 增加统计信息
    for (char whethercontinue = 'Y'; whethercontinue == 'Y';) {
        initialization();
        difficulty();
        cout << success << " " << failure << " ";
        if (success!=0){
            averagetrying=(float)totaltrying/success; //防止整除
        }
        cout.setf(ios::fixed);
        cout << setprecision(2) << averagetrying << endl; //将浮点数截断到小数点后两位，与上一行共同起作用
        cin >> whethercontinue;
    }
}

void check_for_hints(WordSet answerobject, vector <string> &hint_list,string guess){ //遍历acceptable_set,寻找可能的答案
    vector<string>hint_list2;
    std::set<std::string>::iterator it = answerobject.acceptable_set.begin();
    std::advance(it,1); //避免set中第一个是空格的影响，重要！！
    int count=1;//检测
    int guess_letter_count_copy[26]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int answer_letter_count_copy[26]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    char alphabet_status_copy[26]={'X','X','X','X','X','X','X','X','X','X','X','X','X','X','X','X','X','X','X','X','X','X','X','X','X','X'};
    char guess_status_copy[5]={'R','R','R','R','R'};
    for (int j=0; j<26; j++){
        guess_letter_count_copy[j]=guess_letter_count[j];
        answer_letter_count_copy[j]=answer_letter_count[j];
        alphabet_status_copy[j]=alphabet_status[j];
    }
    for (int i=0; i<5; i++){
        guess_status_copy[i]=guess_status[i];
    }//将这些数组复制出一份，因为接下来遍历可选词判断是否是可能的答案时，需要用到这些数组
    memset(answer_letter_count,0,sizeof(int)*26);
    memset(guess_status,'R',sizeof(char)*5); //下面的步骤需要用到answer_letter_count和guess_status数组，先要复制一份出来并初始化
    for ( ;it != answerobject.acceptable_set.end(); it++) {
        string possible_hint = *it;
        bool possible_hint_or_not=true;
        answer_analyze(possible_hint);
        outputformer(possible_hint, guess);
        for (int i = 0; i < 5; i++) {
            if (guess_status[i] != guess_status_copy[i]) {
                possible_hint_or_not = false;
            }
        }//要与之前的猜测词五个字母的状态一致
        memset(answer_letter_count, 0, sizeof(int) * 26);
        memset(guess_status, 'R', sizeof(char) * 5);//每次检验是不是可选词汇都需要！
        bool in_hint_list=false;
        if (find(hint_list.begin(),hint_list.end(),possible_hint)!=hint_list.end()){
            in_hint_list=true;
        }
        if (in_hint_list==true & possible_hint_or_not==true){
            hint_list2.push_back(possible_hint);
        }
    }
    for (int j=0; j<26; j++){
        guess_letter_count[j]=guess_letter_count_copy[j];
        answer_letter_count[j]=answer_letter_count_copy[j];
        alphabet_status[j]=alphabet_status_copy[j];
    }
    for (int i=0; i<5; i++){
        guess_status[i]=guess_status_copy[i];
    } //把数组恢复到原来状态
    hint_list.clear();
    for (int m=0; m<hint_list2.size(); m++){
        hint_list.push_back(hint_list2[m]);
    }//用hint_list记录筛选出的可能答案
}//重点查看！！

void ten_words_a_line_output(vector<string> &hint_list){ //将vector中内容输出，每十个换行
    int count=1;
    for (int a=0; a<hint_list.size(); a++) {
        if (count == 10) { //每十个换行输出
            cout << hint_list[a] << endl;
            count = 0;
        } else {
            cout << hint_list[a] << " ";
        }
        count += 1;
    }
    if (count>1 && count<10){
        cout << endl;
    }
}

void hint(){ //基于现有的猜测结果给出提示 提高要求1
    string answer;
    cin.ignore();
    getline(cin,answer);
    WordSet answerobject;
    if (answerobject.in_final_set(answer)){
        bool isbreakloop1=true;
        vector <string> hint_list;
        std::set<std::string>::iterator it = answerobject.acceptable_set.begin();
        std::advance(it,1); //避免set中第一个是空格的影响，重要！！
        for ( ;it != answerobject.acceptable_set.end(); it++) {
            string possible_hint = *it;
            hint_list.push_back(possible_hint);
        }
        for (int times=1; times<=6 && isbreakloop1; ) {
            string guess;
            getline(cin,guess);
            WordSet guessobject;
            if (!(guessobject.in_acceptable_set(guess))) {
                cout << "INVALID" << endl;
            } else {
                basic_analyze_output(answer,guess);
                //检测是否获得了正确猜测
                if (guess == answer) {
                    cout << "CORRECT" << " " << times << endl;
                    success+=1;
                    totaltrying+=times;
                    isbreakloop1 = false;
                } else {
                    if (times == 6) { //6次全猜错情况
                        cout << "FAILED" << " ";
                        failure += 1;
                        for (int i = 0; i <= 4; i++) {
                            answer[i] = toupper(answer[i]); //toupper变大写
                            cout << answer[i];
                            if (i == 4) {
                                cout << endl;
                            }
                        }
                    }
                }
                if ((guess!=answer) && (times<6)) {
                    cout << "Do you want to see some hints (bases on the status of letters in the alphabet)? Please type yes or no" << endl;
                    string hint_or_not;
                    getline(cin, hint_or_not);
                    if (hint_or_not == "yes") {
                        check_for_hints(answerobject,hint_list,guess);
                        ten_words_a_line_output(hint_list);
                        if (hint_list.size()==1){
                            cout << "great! you have got the ultimate answer"<<endl;
                        }
                        else {
                            cout << "That's all I can provide." << endl;
                        }
                    }
                }
                times++; //正确输入才消耗次数
            }
            local_initialization();
        }
    }
}

long double calculate_entropy(string hint, vector<string>range){  //计算信息熵，依赖于单词和所需考察的范围 p(x)中的分母会根据vector的size而变化
    int arr[243];
    memset(arr,0,sizeof(arr)); //初始化一个全为0的243位数组
    for (int i=0; i<range.size(); i++){
        string possible_hint = range[i];
        outputformer(possible_hint, hint);
        int what_position_in_arr=1;
        for (int i=0; i<=4; i++){ //将结果RRRRR作为243位数组的第一位，GGGGG作为最后一位 着重检查！！！
            if (guess_status[i]=='Y'){
                what_position_in_arr+=1*pow(3,i);
            }
            if (guess_status[i]=='G'){
                what_position_in_arr+=2*pow(3,i);
            }
        } //判断possible_hint作为answer,hint作为guess会带来什么guess字母情况（五位）
        arr[what_position_in_arr]+=1;
        for (int n=0; n<=4; n++){ //将outputfomer用过的数组初始化
            guess_status[n]='R';
        }
    }
    long double entropy=0.0;
    for (int j=0; j<243; j++){
        if (arr[j]!=0){
            long double a=range.size();
            long double p=arr[j]/a;
            entropy+=p*(log2(1/p)); //3blue1brown中给出的信息熵计算公式
        }
    }
    return entropy;
}

bool cmp(const pair<string,long double> a,pair<string,long double>b){
    return a.second>b.second;
} //将vector pair中根据第二位的大小排序

void single_step_optimize(){ //在剩余词的基础上，给出若干推荐词 提高要求2
    string answer;
    cin.ignore();
    getline(cin,answer);
    WordSet answerobject;
    WordSet guessobject;
    if (answerobject.in_final_set(answer)){
        bool isbreakloop1=true;
        vector <string> hint_list;
        std::set<std::string>::iterator it = answerobject.acceptable_set.begin();
        std::advance(it,1); //避免set中第一个是空格的影响，重要！！
        for ( ;it != answerobject.acceptable_set.end(); it++) {
            string possible_hint = *it;
            hint_list.push_back(possible_hint);
        }//上面这7行将acceptable set中所有元素加入到hint_list中
        for (int times=1; times<=6 && isbreakloop1; ) {
            string guess;
            getline(cin,guess);
            if (!(guessobject.in_acceptable_set(guess))) {
                cout << "INVALID" << endl;
            } else {
                basic_analyze_output(answer,guess);
                //检测是否获得了正确猜测
                if (guess == answer) {
                    cout << "CORRECT" << " " << times << endl;
                    success+=1;
                    totaltrying+=times;
                    isbreakloop1 = false;
                } else {
                    if (times == 6) {
                        cout << "FAILED" << " ";
                        failure += 1;
                        for (int i = 0; i <= 4; i++) {
                            answer[i] = toupper(answer[i]);
                            cout << answer[i];
                            if (i == 4) {
                                cout << endl;
                            }
                        }
                    }
                }
                if ((guess!=answer) && (times<6)) {
                    cout << "Do you want to see some hints (based on single-step optimization)? Please type yes or no" << endl;
                    string hint_or_not;
                    getline(cin, hint_or_not);
                    if (hint_or_not == "yes") {
                        //计算hint_list中所有词的entropy并进行排序
                        check_for_hints(answerobject,hint_list, guess);
                        map<string,long double>mymap;
                        for (int n=0; n<hint_list.size(); n++){
                            string word=hint_list[n];
                            long double entropy=calculate_entropy(word, hint_list);
                            mymap[word]=entropy;
                        }
                        vector<pair<string,long double> >v(mymap.begin(),mymap.end());
                        sort(v.begin(),v.end(),cmp);
                        map<string,long double>::iterator it;
                        for (int i = 0; i < v.size(); i++) {
                            cout << "word: " << v[i].first << "  " << "entorpy: " << v[i].second << endl;
                        }
                        /*cout << "How many hints do you want to see?" << endl;
                        int number=0;
                        cin >> number;
                        if (number<=v.size()) {
                            for (int i = 0; i < number; i++) {
                                cout << "word: " << v[i].first << "  " << "entorpy: " << v[i].second << endl;
                            }
                        }
                        else{
                            cout << "Sorry, that is more than I can provide." << endl;
                            for (int i = 0; i < v.size(); i++) {
                                cout << "word: " << v[i].first << "  " << "entorpy: " << v[i].second << endl;
                            }
                            cout << "That is all the possible hints sorted by entropy." << endl;
                        }
                        cout << "(the theoretical largest entropy is about 7.92)" << endl;*/
                    }
                }
                times++; //正确输入才消耗次数
            }
            local_initialization();
        }
    }
}

void interaction(){ //交互解决wordle 提高要求3
    WordSet answerobject;
    WordSet guessobject;
    bool isbreakloop = true;
    vector <string> hint_list; //先建立一个包含所有acceptable_set单词的vector
    std::set<std::string>::iterator it = answerobject.acceptable_set.begin();
    std::advance(it,1); //避免set中第一个是空格的影响，重要！！
    for ( ;it != answerobject.acceptable_set.end(); it++) {
        string possible_hint = *it;
        hint_list.push_back(possible_hint);
    }
    string guess;
    for (int times=1; times<=6 && isbreakloop; times++) {
        if (times==1) {
            cout << "What's your first guess?" << endl;
            cout << "some recommended words are: tares lares rales rates teras nares soare tales reais tears" << endl;
            cin >> guess;
        }
        else{
            cout << "The next optimal guess is: "<<guess<<"."<<endl;
        }
        cout << "What's the state of the five letters in this guess? (in terms of R ,Y, G)";
        string guess_state;
        cin >> guess_state;
        char guess_state2[5]={'R','R','R','R','R'};
        for (int i=0; i<5; i++){
            guess_state2[i]=guess_state[i];
        }
        int arr[243];
        memset(arr,0,sizeof(arr)); //初始化一个全为0的243位数组
        for (int i=0; i<hint_list.size(); i++) {
            string possible_hint = hint_list[i];
            outputformer(possible_hint, guess);
            int what_position_in_arr = 1;
            for (int i = 0; i <= 4; i++) { //将结果RRRRR作为243位数组的第一位，GGGGG作为最后一位 着重检查！！！
                if (guess_status[i] == 'Y') {
                    what_position_in_arr += 1 * pow(3, i);
                }
                if (guess_status[i] == 'G') {
                    what_position_in_arr += 2 * pow(3, i);
                }
            } //判断possible_hint作为answer,hint作为guess会带来什么guess字母情况（五位）
            arr[what_position_in_arr] += 1;
            for (int n = 0; n <= 4; n++) { //将outputfomer用过的数组初始化
                guess_status[n] = 'R';
            }
        }
        int coloring_number=0;
        long double entropy=0.0;
        cout << "clorings are: "<<endl;
        int count_coloring_in_a_line=0;
        for (int j=0; j<243; j++){
            if (arr[j]!=0){
                coloring_number+=1;
                //十进制转回三进制
                char coloring_display[5]={'R','R','R','R','R'};
                int a1=j;
                for (int i=0; i<5; i++){
                    int a2=a1/3;
                    if (a1%3==1){
                        coloring_display[i]='Y';
                    }
                    if (a1%3==2){
                        coloring_display[i]='G';
                    }
                    a1=a2;
                }
                for (int j=0; j<5;j++){
                    cout << coloring_display[j];
                }
                cout << " ";
                count_coloring_in_a_line+=1;
                if (count_coloring_in_a_line==10){
                    cout << endl;
                    count_coloring_in_a_line=0;
                }
                //展示colorings的结果有哪些
                long double a=hint_list.size();
                long double p=arr[j]/a;
                entropy+=p*(log2(1/p)); //3blue1brown中给出的信息熵计算公式
            }
        }
        if (count_coloring_in_a_line>0 && count_coloring_in_a_line<10){
            cout <<endl;
        }
        cout << "There are total of "<<coloring_number<<" coloring."<<endl;
        vector <string> hint_list2;
        std::set<std::string>::iterator it2 = guessobject.acceptable_set.begin();
        std::advance(it2,1); //避免set中第一个是空格的影响，重要！！
        for (; it2!=guessobject.acceptable_set.end();it2++){
            string possible_hint=*it2;
            bool possible_hint_or_not =true;
            answer_analyze(possible_hint);
            outputformer(possible_hint,guess);
            for (int i=0; i<5; i++){
                if (guess_status[i]!=guess_state2[i]){
                    possible_hint_or_not = false;
                }
            }
            memset(answer_letter_count,0,sizeof(int)*26);
            memset(guess_status,'R',sizeof(char)*5);
            bool in_hint_list = false;
            if (find(hint_list.begin(),hint_list.end(),possible_hint)!=hint_list.end()){
                in_hint_list = true;
            }
            if (in_hint_list==true && possible_hint_or_not == true){
                hint_list2.push_back(possible_hint);
            }
        }
        hint_list.clear();
        for (int m=0; m<hint_list2.size(); m++){
            hint_list.push_back(hint_list2[m]);
        }
        if (hint_list.size()==1){
            cout << "great! you have got the ultimate answer: "<<hint_list[0];
            isbreakloop=false;
        }
        else{
            cout << "There are a total of "<<hint_list.size()<<" possible answers"<<endl;
        }

        map<string, long double> mymap;
        std::set<std::string>::iterator it3 = guessobject.acceptable_set.begin();
        std::advance(it3, 1);
        for (;it3!=guessobject.acceptable_set.end();it3++) {
            string word = *it3;
            long double entropy = calculate_entropy(word, hint_list);
            mymap[word] = entropy;
        }
        vector<pair<string, long double> > v(mymap.begin(), mymap.end());
        sort(v.begin(), v.end(), cmp);
        map<string, long double>::iterator it4;
        guess = v[0].first; //更新guess为entropy最大的一个
    }
}

void test_average_guess(){ //测试算法平均猜词次数 提高要求4 - 其中it it2 it3三个迭代器都通过advance操作去除空格影响
    WordSet answerobject;
    WordSet guessobject;
    double count=0.0;
    int totaltrying2=0;
    std::set<std::string>::iterator it = answerobject.final_set.begin();
    std::advance(it,1);
    for (; it != answerobject.final_set.end(); it++) {
        string answer=*it;
        std::cout << answer << std::endl; //检测
        bool isbreakloop1=true;
        vector <string> hint_list;
        std::set<std::string>::iterator it = answerobject.acceptable_set.begin();
        std::advance(it,1); //避免set中第一个是空格的影响，重要！！
        for ( ;it != answerobject.acceptable_set.end(); it++) {
            string possible_hint = *it;
            hint_list.push_back(possible_hint);
        }//上面这7行将acceptable set中所有元素加入到hint_list中

        vector <string>guess_list;
        for (int times=1; times<=6 && isbreakloop1; ) {
            string guess;

            if (times>1) {
                map<string, long double> mymap;
                std::set<std::string>::iterator it3 = guessobject.acceptable_set.begin();
                std::advance(it3, 1);
                for (int m = 0; m < hint_list.size(); m++) {
                    string word = hint_list[m];
                    long double entropy = calculate_entropy(word, hint_list);
                    mymap[word] = entropy;
                }
                vector<pair<string, long double> > v(mymap.begin(), mymap.end());
                sort(v.begin(), v.end(), cmp);
                map<string, long double>::iterator it;
                guess = v[0].first; //其实这部是对所有进行了排序，如何选取最高的一个，用stack? (运行更快)
                for (int i = 0; i < 1; i++) {
                    cout << "word: " << v[i].first << "  " << "entorpy: " << v[i].second << endl;
                } //这三行用于检测
            }
            else{
                guess="tares";
                cout << "word: " << "tares" << "  " << "entorpy: " << "6.19" << endl;
            }
            guess_list.push_back(guess);
            answer_analyze(answer);
            input_analyze(guess);
            outputlater(answer, guess);
            outputformer(answer, guess);
            if (guess == answer) {
                cout << "CORRECT" << " " << times << endl;
                success+=1;
                totaltrying+=times;
                isbreakloop1 = false;
            } else {
                if (times == 6) { //6次全猜错情况
                    cout << "FAILED" << " ";
                    failure += 1;
                    for (int i = 0; i <= 4; i++) {
                        answer[i] = toupper(answer[i]); //toupper变大写
                        cout << answer[i];
                        if (i == 4) {
                            cout << endl;
                        }
                    }
                    totaltrying=6; //猜不出来，则猜测次数为6次
                }
            }
            check_for_hints(answerobject,hint_list,guess);
            times++;
            local_initialization();
        }
        totaltrying2+=totaltrying;
        totaltrying=0;
        count+=1;
        std::cout<<count<<std::endl; //检测速度
        initialization();
    }
    double averagetrying=totaltrying2/count;
    std::cout<<"the average guessing of this algorithm is "<<averagetrying<<" times."<<std::endl;
}

void best_initial_guesses(){ //给出若干全局最优的起始猜测词
    WordSet answerobject;
    int count=0;//检查
    vector <string> range;
    std::set<std::string>::iterator it = answerobject.acceptable_set.begin();
    std::advance(it,1); //避免set中第一个元素为空的影响
    for ( ; it != answerobject.acceptable_set.end(); it++) {
        string word = *it;
        std::cout<<word<<std::endl;//检查
        range.push_back(word);
    }
    map<string,long double>mymap;
    std::set<std::string>::iterator it2 = answerobject.acceptable_set.begin();
    std::advance(it2,1);
    for ( ; it2 != answerobject.acceptable_set.end(); it2++) {
        string word = *it2;
        long double possible_entropy=calculate_entropy(word,range);
        mymap[word]=possible_entropy;
        //检查运行速度
        count+=1;
        std::cout<<count<<std::endl;
    }
    vector<pair<string,long double> >v(mymap.begin(),mymap.end());
    sort(v.begin(),v.end(),cmp);
    map<string,long double>::iterator it3;
    for (int i = 0; i < 10; i++) {
        cout << "word: " << v[i].first << "  " << "entorpy: " << v[i].second << endl;
    }
    std::cout<<"here are ten best initial guesses based on entropy"<<std::endl;
}

int tasknumber=0;
int main() {
    cin>>tasknumber;
    switch(tasknumber){
        case 1: singlegame();break;
        case 2: multiplegame();break;
        case 3: difficultmode();break;
        case 4: statistics();break;
        case 5: hint();break;
        case 6: single_step_optimize();break;
        case 7: interaction();break;
        case 8:test_average_guess();break;
        case 9:best_initial_guesses();break;
    }
    return 0;
}
//重要：候选词列表对于求解器来说是未知的！！！
//现在找出的最大信息熵是6.193 与3blue1brown视频一致
