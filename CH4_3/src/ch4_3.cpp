/*
 * ch4_3.cpp
 *
 *  Created on: 2024. 4. 8.
 *      Author: USER
 *
 *  + class Memo 추가: string 클래스의 멤버함수 및 operator 활용법
 *  + Person::memo_c_str[] 멤버 및 set, get 함수 추가
 *  + CurrentUser::memo 멤버, 함수 및 메뉴 항목 추가
 *  + PersonManager::~PersonManager()에서 display() 문장 삭제
 */

#include <iostream>
#include <cstring>
using namespace std;

#define AUTOMATIC_ERROR_CHECK true // false: 자동 오류 체크, true: 키보드에서 직접 입력하여 프로그램 실행


/******************************************************************************
 * Person class
 ******************************************************************************/

class Person
{
    string   name;        // 이름
    string passwd;          // 비번
    int    id;              // Identifier
    double weight;          // 체중
    bool   married;         // 결혼여부
    char   address[60];     // 주소 40?
    char   memo_c_str[1024]; // 메모장

protected:
    void inputMembers(istream* in);
    void printMembers(ostream* out);

public:
    Person(): Person("") {};
    Person(const string name): Person(name, 0, 0, 0, ""){};
    Person(const string name, int id, double weight, bool married, const char *address);
    ~Person();

    void set(const string name, int id, double weight, bool married, const char *address);
    void setName(const string name)       { this->name = name; }
    void setPasswd(const string passwd)   { this->passwd = passwd; }
    void setId(int id)                   { this->id = id; }
    void setWeight(double weight)        { this->weight = weight; }
    void setMarried(bool married)        { this->married = married; }
    void setAddress(const char* address) { strcpy(this->address, address); }
    void setMemo(const char* c_str)      { /*TODO: C 스트링 문자열 c_str을 memo_c_str로 복사하라.*/
    										strcpy(memo_c_str, c_str);}

    string getName()    { return name; }
    string      getPasswd()  { return passwd; }
    int         getId()      { return id; }
    double      getWeight()  { /* TODO: [문제 2] */ return weight; }  // 구현 시
    bool        getMarried() { /* TODO: [문제 2] */ return married; }  // 리턴 값들을
    const char* getAddress() { /* TODO: [문제 2] */ return address; } // 수정하시오.
    const char* getMemo()    { /*TODO: C 스트링 문자열(memo_c_str)를 리턴하라.*/
    							return memo_c_str;}

    void input(istream* pin)  { inputMembers(pin); } // ch3_2에서 추가
    void print(ostream* pout) { printMembers(pout); }
    void println()            { print(&cout); cout << endl; }
    void whatAreYouDoing();                          // ch3_2에서 추가
    bool isSame(const string name, int id);         // ch3_2에서 추가
};

Person::Person(const string name, int id, double weight, bool married,
               const char *address) :
        id{id}, weight{weight}, married{married}, memo_c_str{} {
    // 위에서 각 멤버를 초기화하는 {}는 각 매개변수 값을 객체의 상응하는 멤버에 설정하는 것이다. 즉,
    // this->id=id, this->weight=weight, this->married=married와 동일하다.
    // 여기서 this는 해당 객체를 포인터하는 포인터 변수이며, (다음 장에서 설명될 예정임)
    // this->id는 해당 객체의 멤버 id를 지칭하며 =의 우측변수 id는 함수의 매개변수이다.
    // 값을 간단히 대입하는 것은 위처럼 하면 되지만 name[], address[]와 같은 배열 복사 함수
    // 호출 또는 다른 함수를 호출할 때는 생성자 함수의 몸체 { } 내에서 호출해야 한다.
    // 위 memo_c_str{}는 배열 memo_c_str[]의 모든 원소를 기본 값인 널 문자('\0')로 초기화하는 코드임
    setName(name);
    setAddress(address);
    cout << "Person::Person(...):"; println();
}

Person::~Person() {
    cout << "Person::~Person():"; println();
}

void Person::printMembers(ostream* pout)   {
	*pout << name << " " << id << " " << weight << " " << married
	    		<< " " << ":" << address << ":";
}

void Person::set(const string name, int pid, double pweight,
    bool pmarried, const char *paddress) {
    // TODO: [문제 4]
	this->name = name;
	id = pid;
	weight = pweight;
	married = pmarried;
	setAddress(paddress);
}

void Person::inputMembers(istream* pin)   {
    // TODO: [문제 7]
	*pin >> name >> id >> weight >> married;

	    if (!(*pin)) return;
	    pin->getline(address, sizeof(address), ':');
	    pin->getline(address, sizeof(address), ':');

}

void Person::whatAreYouDoing() {
    // TODO: [문제 5]
	cout << name << " is taking a rest.";
}

bool Person::isSame(const string name, int pid) {
    // TODO: [문제 6]
	return ((this->name == name) && (id == pid));
}

/******************************************************************************
 * User Interface
 ******************************************************************************/
// 기본적인 입력과 관련된 전역 함수들을 UI라는 이름공간 내부에 정의함

namespace UI {

bool echo_input = false;
string line, emptyLine;

// 입력에서 정수 대신 일반 문자가 입력되었는지 체크하고 에러 발생시 에러 메시지 출력
bool checkInputError(istream* pin, const string msg) {
    if (!(*pin)) { // 에러가 발생했다면
        cout << msg;  // 에러 메시지를 출력
        pin->clear(); // 에러 발생 상태정보를 리셋함; 그래야 다음 문장에서 읽을 수 있음
        getline(*pin, emptyLine); // 에러가 발생한 행 전체를 읽어 데이터를 버림
        return true;
    }
    return false;
}

// 정수나 실수를 입력해야 하는 곳에 일반 문자열을 입력한 경우의 에러 체크
bool checkDataFormatError(istream* pin) {
    return checkInputError(pin, "Input-data format MISMATCHED\n");
}

// 한 사람의 정보 즉, 각 멤버 데이터를 순서적으로 입력 받아 p에 저장하고
// 입력 중 입력 데이터에 오류가 있는지 확인하고 오류가 있을 시 에러 메시지를 출력한다.
bool inputPerson(Person* p) {
    cout << "input person information:" << endl;
    p->input(&cin);
    if (checkDataFormatError(&cin)) return false;
    if (echo_input) p->println(); // 자동체크에서 사용됨
    return true;
}

// 입력장치에서 하나의 단어로 구성된 문자열을 입력 받음
string getNext(const string msg) {
    cout << msg; // 입력용 메시지를 출력
    cin >> line; // 하나의 단어를 읽어 들임
    if (echo_input) cout << line << endl; // 자동체크 시 출력됨
    getline(cin, emptyLine); // 입력받은 한 단어 외 그 행의 나머지 데이타(엔터포함)는 읽어서 버림
    return line;             // 이유는 여기서 [엔터]를 제거하지 않으면
}                            // 다음의 getNextLine()에서 엔터만 읽어 들일 수 있기 때문에

// 입력장치에서 한 행을 입력 받음
string getNextLine(const string msg) {
    cout << msg; // 입력용 메시지를 출력
    getline(cin, line); // 한 행을 읽어 들임
    if (echo_input) cout << line << endl; // 자동체크 시 출력됨
    return line;
}

// 하나의 정수를 입력 받음; 정수가 아닌 아닌 문자열 입력시 에러 메시지 출력 후 재입력 받음
int getInt(const string msg) {
    for (int value; true; ) {
        cout << msg;
        cin >> value;
        if (echo_input) cout << value << endl; // 자동체크 시 출력됨
        if (checkInputError(&cin, "Input an INTEGER.\n"))
            continue;
        getline(cin, emptyLine); // skip [enter] after the number
        return value;
    }
}

// 하나의 양의 정수를 입력 받음; 음수 입력시 에러 메시지 출력 후 재입력 받음
int getPositiveInt(const string msg) {
    int value;
    while ((value = getInt(msg)) < 0)
        cout << "Input a positive INTEGER." << endl;
    return value;
}

// 0~(size-1)사이의 선택된 메뉴 항목 또는 리스트의 항목의 인덱스 값을 리턴함
// 존재하지 않는 메뉴항목을 선택한 경우 에러 출력
int getIndex(const string msg, int size) {
    while (true) {
        int index = getPositiveInt(msg);
        if (0 <= index  && index < size) return index;
        cout << index << ": OUT of selection range(0 ~ "
             << size-1 << ")" << endl;
    }
}

// 사용자에게 메뉴를 보여주고 사용자가 선택한 메뉴항목의 인덱스를 리턴함
int selectMenu(const string menuStr, int menuItemCount) {
    cout << endl << menuStr;
    return getIndex("Menu item number? ", menuItemCount);
}

} // namespace UI

/******************************************************************************
 * ch4_3: string and Memo class
 ******************************************************************************/

class Memo
{
    string mStr; // 메모를 저장해 두는 문자열

    string get_next_line(size_t* ppos);
    bool find_line(int line, size_t* start, size_t* next);
    size_t find_last_line();

public:
    string getNext(size_t* ppos);
    void displayMemo();
    const char *get_c_str() { return mStr.c_str(); }
    void set_c_str(const char *c_str) { this->mStr = c_str; }
    // = 연산자의 좌우 데이타 타입이 달라도 C 스트링 문자열인 c_str이 string mStr 내부로 복사된다.

    void findString();
    void compareWord();
    void dispByLine();
    void deleteLine();
    void replaceLine();
    void scrollUp();
    void scrollDown();
    void inputMemo();
    void run();
};

void Memo::displayMemo() { // Menu item 1
    cout << "------- Memo -------" << endl;
    cout << mStr;
    if (mStr.length() > 0 && mStr[mStr.length()-1] != '\n')
        cout << endl; // 메모 끝에 줄바꾸기 문자가 없을 경우 출력
    cout << "--------------------" << endl;
}

// 사용자로부터 찾을 단어를 입력받고 메모 문자열에서 해당 단어의 출현 회수를 세어서 출력한다.
// 찾을 단어가 다른 단어의 일부분일지라도 모두 카운트하라.
void Memo::findString() {
    string word = UI::getNext("Word to find? ");
    int count = 0, len = word.length();
    /*
    TODO: for 문을 사용하여 반복적으로 string::find() 함수를 호출하여 찾은 단어의
          출현 회수(count)를 세어라. find()를 통해 찾은 위치에서 len을 더하면
          찾은 단어의 끝 위치가 되는데 이것이 다음 번에 찾기 시작할 위치이다.
          string::find()의 리턴 값이 string::npos와 같으면
          단어를 찾지 못했다는 것을 의미한다. 못 찾을 때까지 반복 수행하면 됨.
          찾기 시작 위치 또는 리턴 값의 타입은 size_t를 사용하라.
    */
    size_t pos=0;
    for(pos = mStr.find(word, pos); pos != std::string::npos; pos = mStr.find(word, pos)){
    	count++;
    	pos += len;
    }
    cout << "Found count: " << count << endl;
}

// 메모의 현 위치(pos = *ppos)에서 하나의 단어를 찾아 해당 단어를 별도의 string으로 구성해서 리턴함
// 스페이스, 탭, 엔터와 같은 공백 문자로 시작할 경우 이들을 먼저 모두 스킵함
// 단어의 끝은 공백, 탭, 엔터, 구두점(punctuation)으로 구분함
// 구두점이란 숫자, 영문자를 제외한 문자들을 의미한다. ,.';:"][{}()*&^%$#@!~`?>< 등
// 각 구두점 글자는 단어를 구분하는 구분자로 사용되고 그 글자 하나만으로도 하나의 단어로 취급받음
// isspace()와 ispunct() 함수는 www.cplusplus.com/reference/에서 검색해 보기 바란다.

string Memo::getNext(size_t* ppos) {
    size_t pos = *ppos, end;
    for ( ; pos < mStr.size() && isspace(mStr[pos]); ++pos) ; // 단어 앞의 공백 문자들 스킵(있을 경우)
    end = pos; // pos는 단어의 시작 위치이고 end는 단어의 끝 다음 위치이다.
    if (end < mStr.size() && ispunct(mStr[end])) // 첫 글자가 구두점일 경우
        ++end;
    else { // 단어의 끝을 찾음
        for ( ; end < mStr.size() && !isspace(mStr[end]) &&
                !ispunct(mStr[end]); ++end) ;
    }
    *ppos = end; // 다음 단어의 시작 위치를 기록해 둠
    /*
    TODO: string::substr()을 이용해서 찾은 단어를 발췌해서 별도의 string으로 구성하여 리턴하라.
          mStr의 끝에 도착하여 더 이상 찾을 단어가 없을 경우 "" 문자열을 반환하게 된다.
          발췌할 단어의 길이는 pos와 end의 간단한 계산으로 구할 수 있다.
    */
    return mStr.substr(pos, end-pos);
}

// 사용자로부터 비교할 단어를 입력받고 메모 문자열에서
// 해당 단어와 동일한 단어의 출현 회수 same,
// 사전적 순서에서 이 단어보다 앞쪽에 있는 단어의 출현 회수 less,
// 그리고 이 단어보다 뒤쪽에 있는 단어의 출현 회수 larger를 세어서 각각을 출력한다.
void Memo::compareWord() {
    string next, word = UI::getNext("Word to compare? ");
    int less = 0, same = 0, larger = 0;
    /*
    아래 pos는 getNext(&pos)를 호출할 때 다음 단어를 찾아야 할 시작 위치임
    TODO: for(size_t pos = 0; getNext(&pos)를 호출하여 mStr의 끝까지 반복 수행; )
             위 getNext(&pos)를 호출시 리턴된 다음 단어를 next에 저장한 후
             next가 ""일 경우 mStr의 끝을 의미하므로 for문 종료
             ""가 아닌 경우 찾을 단어인 word와 비교(<, >, ==)하여
             적절한 less, same, larger 변수의 값을 증가시킨다.
    */
    size_t pos = 0;
    for(pos = 0; ; ) {
            next = getNext(&pos);
            if(next == "") break;

            if(next < word) {
                ++less;
            } else if(next == word) {
                ++same;
            } else {
                ++larger;
            }
        }
    cout << "less: "   << less   << endl;
    cout << "same: "   << same   << endl;
    cout << "larger: " << larger << endl;
}

// 메모의 현 위치(pos = *ppos)에서 그 행의 끝을 찾은 후 행 전체를 별도의 string으로 구성해서 리턴함
// string::npos는 해당 문자를 찾지 못했을 경우의 리턴 값이며 (-1)과 동일 값임
string Memo::get_next_line(size_t* ppos) {
    size_t pos = *ppos, end;

    /*
    TODO: string::find()를 이용해 행의 끝('\n') 위치를 찾아서(현재 행의 시작 위치는 pos임) end에 저장
    */
    end = mStr.find('\n', pos);

    // 메모의 끝에 '\n'이 없을 경우: (end == string::npos)
    end = (end == string::npos)? mStr.length() : end+1;
    *ppos = end; // 다음 행의 시작 위치를 기록해 둠

    /*
    TODO: 찾은 현재 행을 string::substr()으로 발췌해서 별도의 string으로 구성하여 리턴하라.
          발췌할 단어 길이는 pos와 end로 간단히 계산할 수 있음
    */
    return mStr.substr(pos, end-pos);
}

// 메모 텍스트 mStr를 한 행씩 잘라서 행 번호와 함께 화면에 보여즘; 행 번호는 0부터 시작함
void Memo::dispByLine() {
    cout << "--- Memo by line ---" << endl;
    /*
    아래 pos는 get_next_line(&pos)를 호출할 때 다음 행의 시작 위치임
    TODO: for(size_t pos = 0, ... 문을 이용하여 pos가 mStr의 길이보다 작을 동안 반복 수행
             get_next_line(&pos)를 호출하여 반환된 다음 행 문자열을 line에 저장하고
             적절한 행 번호와 함께 해당 행(line)을 출력(행번호 출력은 PersonManager::display() 참조)
             행의 끝에 줄바꾸기 문자 '\n'가 없을 경우 endl 출력 (displayMemo() 참조)
    */
    size_t pos = 0; string line; int i = 0;
    for(pos = 0; pos < mStr.length(); ) {
          line = get_next_line(&pos);
          if(line.empty() || line == "\n") break; // 더 이상 행이 없으므로 for문 종료
          cout << "[" << i << "] ";
          cout << line;
          ++i;
          if (mStr.length() > 0 && mStr[mStr.length()-1] != '\n')
                  cout << endl; // 메모 끝에 줄바꾸기 문자가 없을 경우 출력
          }
    cout << "--------------------" << endl;
}

// 행번호가 line_num(0부터 시작)인 행을 찾아 행의 시작 위치 *start와 행 길이 *plen를 계산함
// 해당 행 번호를 찾았으면 true, 찾지 못했으면 false 반환
bool Memo::find_line(int line_num, size_t* pstart, size_t* plen) {
    size_t start = 0;
    size_t end;
    /*TODO: for 문을 이용해 line_num번 반복 수행 (0 행에서 line_num-1 행까지 반복)
              string::find()를 이용해 start에서부터 행의 끝('\n')을 찾고,
              find()의 리턴된 값을 다시 start에 저장
              행의 끝을 찾지 못했을 경우(line_num 행까지 진행하지 못한 경우) 즉,
              find()의 반환된 값 start가 string::npos이면
                  여기서 false 리턴함 (line_num행을 찾지 못한 경우)
              start 값을 1 증가시켜 다음 행의 시작 위치로 조정*/
    for(int i = 0;i < line_num;i++){
    	end = mStr.find('\n', start);

    	if(end==string::npos){
    		return false;
    	}

    	start = end + 1;
    }
    *pstart = start; // line_num 행의 시작 위치를 기록

    /*TODO: line_num 행의 끝 위치를 찾고,
          찾았으면 그 행의 길이를 계산하여 *plen에 저장
          찾지 못한 경우 string::npos를 *plen에 저장  예를들어, 실제로 행이
              [0], [1], [2] 행까지 있는데 [3] 행의 시작(메모의 끝 위치)을 찾을 경우에 해당함 */
    end = mStr.find('\n', start);
    if(end != string::npos){
    	*plen = end - start;
    }
    else
    	*plen = string::npos;

    return true; // line_num 행을 찾았다는 것을 의미함
}

// 사용자로부터 삭제할 행 번호를 입력 받은 후 해당 행 전체를 삭제함
void Memo::deleteLine() {
    size_t start, len, line_num = UI::getPositiveInt("Line number to delete? ");

    /*TODO: 만약 line_num 행을 찾지 못한 경우
          ( 즉, mStr이 비어 있거나 또는
            find_line(line_num,&start,&len) 호출하여 line_num 행을 못 찾았거나(false) 또는
            찾았지만(true) 행의 시작 위치인 start가 메모 텍스트의 끝인 경우(start == mStr.size()) )
              "Out of line range" 문장 출력
          line_num 행을 찾은 경우 mStr에서 해당 행을 삭제한 후 dispByLine() 호출*/
    bool result = find_line(line_num, &start, &len);
    if( result == false ||  mStr.empty() == true || (result == true && start == mStr.size()) ) {
    	cout << "Out of line range" << endl;
    }
    else {
    	mStr.erase(start, len+1);
    	dispByLine();
    }
}

// 교체할 행 번호와 교체할 문자열을 입력 받은 후 해당 행을 입력 받은 행으로 교체함
void Memo::replaceLine() {
    size_t start, len, line_num = UI::getPositiveInt("Line number to replace? ");

    /*TODO: find_line(line_num, &start, &len) 호출하여 line_num 행을 찾음
          만약 해당 행을 찾지 못한 경우 "Out of line range"를 출력하고 리턴함*/
    bool result = find_line(line_num, &start, &len);
    if(result == false){
    	cout << "Out of line range" << endl;
    	return;
    }

    string line = UI::getNextLine("Input a line to replace:\n");

    /*TODO: 읽은 line의 끝에 '\n'을 추가하라.
          찾은 행을 line 문자열로 교체한 후 dispByLine() 호출*/
    //line.push_back('\n');
    mStr.replace(start, len, line);
    dispByLine();
}

// 모든 행을 한 행씩 앞으로 옮김 (즉, 첫행을 마지막으로 옮김)
// 즉, 첫 행(행 번호 0)을 삭제한 후 삭제된 행을 맨 마지막에 다시 추가함
void Memo::scrollUp() {
    size_t start, len;
    string saved;
    /*TODO: find_line()를 이용해 첫 행을 찾음
          mStr의 첫 행을 mStr의 맨 마지막에 추가함
          mStr의 첫 행을 삭제한 후 dispByLine() 호출*/
    find_line(0, &start, &len);
    saved = mStr.substr(start, len);
    saved.push_back('\n');
    mStr.append(saved);
    mStr.erase(0, len+1);
    dispByLine();

}

// 마지막 행의 시작 위치를 찾아서 반환함
size_t Memo::find_last_line() {
    for (size_t start = 0, pos = 0; true; start = pos) {

        /*TODO: start 위치 이후로 행 끝을 찾아 pos에 저장
              행 끝을 못 찾았거나 또는 ++pos가 mStr의 길이와 같으면(마지막 행) start 값 리턴*/
    	pos = mStr.find('\n', start);
    	if(pos == string::npos || (++pos == mStr.length()))
    		return start;

    }
}

void Memo::scrollDown() {
    size_t last = find_last_line();//(find_last_line()을 호출하여 마지막 행의 시작 위치를 찾음)
    size_t len = mStr.length() - last;//(last와 mStr의 길이를 이용하여 마지막 행의 길이 계산)
  /*마지막 행을 서브 string으로 발췌하여 별도 저장
    마지막 행 삭제
    발췌했던 마지막 행을 맨 처음에 삽입한 후 dispByLine() 호출*/
    string saved;
    saved = mStr.substr(last, len);
    mStr.erase(last, len+1);
    mStr = saved + mStr;
    dispByLine();
}

/*
In war, he is daring, boastful, cunning, ruthless, self-denying,
and self-devoted; in peace, just, generous, hospitable, revengeful,
superstitious, modest, and commonly chaste.
These are qualities, it is true, which do not distinguish all alike;
but they are so far the predominating traits of these remarkable people
as to be characteristic.
It is generally believed that the Aborigines of the American continent
have an Asiatic origin.

*/
// 사용자로부터 메모 데이타를 행 단위로 입력받아 기존의 mStr에 계속 추가함
// 입력의 끝은 빈 줄을 입력하면 된다.
void Memo::inputMemo() {

    //TODO: 기존 mStr의 문자열을 모두 삭제하고, 크기를 0으로 만듦
	mStr.clear();

    string line;
    cout << "--- Input memo lines, and then input empty line at the end ---" << endl;
    while (true) {

     /*TODO:*/getline(cin, line); //을 사용하여 키보드로부터 한 줄을 입력 받아 line에 저장
              if (UI::echo_input) cout << line << endl; // 자동체크 때 실행됨
            /*입력 받은 한 행이 빈 줄이면 리턴함
              입력 받은 행 끝에 "\n"을 추가
              입력 받은 행을 mStr의 끝에 추가*/
              if(line.empty()) return;
              line.push_back('\n');
              mStr.append(line);

    }
}

// 아래 R"( 와 )"는 그 사이에 있는 모든 문자를 하나의 문자열로 취급하라는 의미이다.
// 따라서 행과 행 사이에 있는 줄바꾸기 \n 문자도 문자열에 그대로 포함된다.
// 이런 방식을 사용하지 않으면 여러 행에 걸친 문자열을 만들려면 복잡해진다.
const char* memoData = R"(The Last of the Mohicans
James Fenimore Cooper
Author's Introduction
It is believed that the scene of this tale, and most of the information
necessary to understand its allusions, are rendered sufficiently 
obvious to the reader in the text itself, or in the accompanying notes.
Still there is so much obscurity in the Indian traditions, and so much
confusion in the Indian names, as to render some explanation useful.
Few men exhibit greater diversity, or, if we may so express it, 
greater antithesis of character, 
than the native warrior of North America.
)";
//const char* memoData = "The Last of the Mohicans"; // 행의 끝에 줄바꾸기 문자가 없다.

void Memo::run() {

    // TODO 문제 [1]: func_arr[], menuCount 선언
	using func_t = void (Memo::*)();
	func_t func_arr[] = {
			nullptr, &Memo::displayMemo, &Memo::findString, &Memo::compareWord,
			&Memo::dispByLine, &Memo::deleteLine, &Memo::replaceLine, &Memo::scrollUp,
			&Memo::scrollDown, &Memo::inputMemo,
	};

	int menuCount = sizeof(func_arr)/sizeof(func_arr[0]);

    string menuStr =
        "++++++++++++++++++++++ Memo Management Menu +++++++++++++++++++++\n"
        "+ 0.Exit 1.DisplayMemo 2.FindString 3.CompareWord 4.DispByLine  +\n"
        "+ 5.DeleteLine 6.RepaceLine 7.ScrollUp 8.ScrollDown 9.InputMemo +\n"
        "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";

    if (mStr == "") mStr = memoData;// 멤버 mStr이 비었을 경우 위 memoData로 초기화한다.

    // TODO 문제 [1]: while 문장 삽입하여 선택된 메뉴항목 실행하는 함수 호출
    while(true){
    	int menuItem = UI::selectMenu(menuStr, menuCount);
    	        if (menuItem == 0) return;
    	        (this->*func_arr[menuItem])();
    }
}

/******************************************************************************
 * ch3_2: CurrentUser class
 ******************************************************************************/

class CurrentUser
{
    Person* pUser;
    Memo    memo; // ch4_3에서 추가

public:
    CurrentUser(Person* pUser): pUser(pUser) { }  // user(u)는 this->user = u 와 동일한 기능
    void display();
    void setter();
    void getter();
    void set();
    void whatAreYouDoing();
    void isSame();
    void inputPerson();
    void changePasswd();
    void manageMemo();
    void run();
};

void CurrentUser::display() { // Menu item 1
    pUser->println();
}

void CurrentUser::getter() { // Menu item 2
    cout << "name:" << pUser->getName() << ", id:" << pUser->getId() << ", weight:" <<
            pUser->getWeight() << ", married:" << pUser->getMarried() <<
            ", address:" << pUser->getAddress() << endl;
}

void CurrentUser::setter() { // Menu item 3
    Person* pp = new Person {"pp"};
    pp->setName(pp->getName());
    pp->setId(pUser->getId());
    pp->setWeight(pUser->getWeight());
    pp->setMarried(pUser->getMarried());
    pp->setAddress(pUser->getAddress());
    cout << "pp->setMembers():"; pp->println();
    delete pp;
}

void CurrentUser::set() { // Menu item 4
    Person* pp = new Person {"pp"};
    pp->set(pp->getName(), pUser->getId(), pUser->getWeight(),
              !pUser->getMarried(), pUser->getAddress());
    cout << "pp->set():"; pp->println();
    delete pp;
}

void CurrentUser::whatAreYouDoing() {  // Menu item 5
    pUser->whatAreYouDoing();
}

void CurrentUser::isSame() { // Menu item 6
    pUser->println();
    cout << "isSame(\"user\", 1): " << pUser->isSame("user", 1) << endl;
}

void CurrentUser::inputPerson() { // Menu item 7
    if (UI::inputPerson(pUser)) // GilDong 1 70.5 true :Jongno-gu, Seoul:
        display();              // user 1 71.1 true :Gwangju Nam-ro 21:
}

void CurrentUser::changePasswd() {
    /*UI::getNext("New password: ")를 사용해 비번을 입력 받고,
    pUser가 포인트하는 객체의 비번을 변경하라.*/
	string passwd = UI::getNext("New password: ");
	pUser->setPasswd(passwd);
    cout << "Password changed" << endl;
}

// Person 객체에 저장되어 있던 메모 내용을 memo 객체로 복사하고
// 메모 메뉴에서 메모의 추가, 삭제, 교체 등의 조작이 끝나고 나면 (memo.run())
// 반대로 memo에 있던 메모 내용을 다시 Person 객체로 복사한다.
void CurrentUser::manageMemo() { // Menu item 9
    memo.set_c_str(pUser->getMemo());
    memo.run();
    pUser->setMemo(memo.get_c_str());
}

void CurrentUser::run() {
    using func_t = void (CurrentUser::*)();
    using CU = CurrentUser;
    func_t func_arr[] = {
        nullptr, &CU::display, &CU::getter, &CU::setter,
        &CU::set, &CU::whatAreYouDoing,
        &CU::isSame, &CU::inputPerson, &CU::changePasswd, &CU::manageMemo,
    };
    int menuCount = sizeof(func_arr) / sizeof(func_arr[0]); // func_arr[] 배열의 길이
    string menuStr =
            "+++++++++++++++++++++ Current User Menu ++++++++++++++++++++++++\n"
            "+ 0.Logout 1.Display 2.Getter 3.Setter 4.Set 5.WhatAreYouDoing +\n"
    		"+ 6.IsSame 7.InputPerson 8.ChangePasswd(4_2) 9.ManageMemo(4_3) +\n"
            "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";

    while (true) {
        int menuItem = UI::selectMenu(menuStr, menuCount);
        if (menuItem == 0) return;
        (this->*func_arr[menuItem])();
    }
}
// ch3_2: CurrentUser class

/******************************************************************************
 * ch4_2: VectorPerson class
 ******************************************************************************/
// VectorPerson는 n개의 Person * 포인터를 pVector[] 배열에 담아 관리하는 클래스이다.
// 필요에 따라 포인터를 삽입, 삭제, 추가 등을 할 수 있다.
// 삽입하는 포인터 개수가 점점 많아지면 pVector[] 배열을 위한 메모리를 자동 확장하기도 한다.

class VectorPerson
{
    static const int DEFAULT_SIZE = 10; // pVector의 디폴트 배열 원소 개수

    Person **pVector; // Person *pVector[]; Person에 대한 포인터들의 배열, 배열에 저장될 값이 Person *이다.
    int count;        // pVector 배열에 현재 삽입된 객체 포인터의 개수
    int allocSize;    // 할당 받의 pVector의 총 배열 원소의 개수

    void extend_capacity(); /* TODO 문제 [7] */

public:
    VectorPerson(): VectorPerson(DEFAULT_SIZE) {  /* TODO 문제 [2]: 위임 생성자로 변환 */}
    VectorPerson(int capacity);
    ~VectorPerson();

    // 아래 긱 함수이름 뒤의 const는 그 함수가 클래스 멤버 변수들을 수정하지 않고 읽기만 한다는 의미임
    // pVector[index]의 포인터 값을 반환
    Person* at(int index) const { /* TODO 문제 [1] */ return pVector[index]; }

    // 할당 받의 pVector의 총 배열 원소의 개수를 반환
    int     capacity()    const { /* TODO 문제 [1] */ return allocSize; }

    // pVector 배열에 현재 삽입된 객체 포인터의 개수를 0으로 설정
    void    clear()             { /* TODO 문제 [1] */ count = 0; }

    // 현재 삽입된 객체 포인터가 하나도 없으면 true, 있으면 false
    bool    empty()       const { /* TODO 문제 [1] */if(count==0) return true;
    											    else return false;}

    // 현재 삽입된 객체 포인터의 개수를 반환
    int     size()        const { /* TODO 문제 [1] */ return count; }

    // pVector 배열에 마지막 삽입된 원소 뒤에 새로운 원소 p를 삽입하고 현재 삽입된 객체 개수를 증가
    void    push_back(Person* p); /* TODO 문제 [4, 7] */

};

// capacity는 할당해야 할 배열 원소의 개수
VectorPerson::VectorPerson(int capacity): count(), allocSize(capacity) /* : TODO 문제 [2]: 멤버 초기화 */ {
    // allocSize = capacity, count = 0; 초기화를 위 함수 서두(위 /* */ 주석 사이)에서 할 것
    // 함수 서두에서 초기화하는 방법은 Person 클래스 참고할 것
    cout << "VectorPerson::VectorPerson(" << allocSize << ")" << endl;
    pVector = new Person*[allocSize]; // Person* 들의 배열을 위한 동적 메모리 할당
}

VectorPerson::~VectorPerson() {
    /* TODO 문제 [2]: 동적으로 할당된 배열 pVector 반납: pVector가 배열임을 명심하라. */
	 delete [] pVector;
     cout << "VectorPerson::~VectorPerson(): pVector deleted" << endl;
}

void VectorPerson::push_back(Person *p) {
	if(count==allocSize)
		extend_capacity();
	pVector[count++] = p;
}

void VectorPerson::extend_capacity() {
    Person **saved_persons = pVector; // 기존의 pVector를 백업함
    /*allocSize 값을 두 배로 늘린 후 새로 pVector를 할당 받는다.
    for 문을 이용하여 count 개수만큼만
        saved_persons[i]에 저장된 포인터를 새로 할당 받은 pVector[i]에 저장한다.
    saved_persons [배열] 메모리를 반납한다. (배열임을 명심할 것)*/
    allocSize*=2;
    pVector = new Person* [allocSize];
    for(int i=0;i<count;i++){
    	pVector[i] = saved_persons[i];
    }
    delete [] saved_persons;
    cout << "VectorPerson: capacity extended to " << allocSize << endl;
}

/******************************************************************************
 * ch4_2: Factory class
 ******************************************************************************/

class Factory
{
public:
    // 동적으로 Person 객체를 할당 받은 후 키보드로부터 새로 추가하고자 하는 사람의
    // 인적정보를 읽어 들여 해당 객체에 저장한 후 그 객체의 포인터를 반환한다.
    Person* inputPerson(istream* in) {
        Person* p = new Person();
        p->input(in);  // 멤버들을 입력 받음
        if (UI::checkDataFormatError(in)) { // 정수입력할 곳에 일반 문자 입력한 경우
            delete p;         // 할당한 메모리 반납
            return nullptr;   // nullptr 반환은 에러가 발생했다는 의미임
        }
        if (UI::echo_input) p->println(); // 자동체크에서 사용됨
        return p;
    }
};

/******************************************************************************
 * ch4_2: PersonManager class
 ******************************************************************************/

class PersonManager
{
    VectorPerson persons;
    Factory factory;

    void deleteElemets();
    void printNotice(const string preMessage, const string postMessage);
    Person* findByName(const string name);

public:
    PersonManager(Person* array[], int len); // 6장에서 default 매개변수 설정
    ~PersonManager();
    void display();
    void append();
    void clear();
    void login();
    void run();
};

PersonManager::PersonManager(Person* array[], int len) {
    cout << "PersonManager::PersonManager(array[], len)" << endl;
    /* TODO 문제 [4] */
    for(int i=0;i<len;i++){
    	Person *s = array[i];
    	Person *copied = new Person(s->getName(), s->getId(), s->getWeight(), s->getMarried(), s->getAddress());
    	persons.push_back(copied);
    }
    display();
}

PersonManager::~PersonManager() {
    deleteElemets();
}

void PersonManager::deleteElemets() {
    /* TODO 문제 [5] */
	for(int i=0;i<persons.size();i++){
		delete persons.at(i);
	}
	persons.clear();
}

void PersonManager::display() { // Menu item 1
    int count = persons.size();
    cout << "display(): count " << count << endl;
    for (int i = 0; i < count; ++i) {
        cout << "[" << i << "] ";
        persons.at(i)->println();
    }
    cout << "empty():" << persons.empty() << ", size():" << persons.size()
         << ", capacity():" << persons.capacity() << endl;
}

void PersonManager::printNotice(const string preMessage, const string postMessage) {
    cout << preMessage;
    cout << " [person information] ";
    cout << postMessage << endl;
}

// 아래 함수는 사용자로부터 새로 추가할 Person 객체의 수를 입력 받고 for문을 이용하여
// 그 개수만큼의 Person 객체를 생성하고 인적정보를 입력받은 후 (factory.inputPerson(&cin)을 통해)
// 그 객체 포인터를 VectorPerson persons의 맨 끝 원소로 추가한다.

/* append() 실행 시 아래 항목들을 복사해서 순서적으로 입력하면 편하게 인적정보를 입력할 수 있음
3
HongGilDong 0 71.5 false :Gwangju Nam-gu Bongseon-dong 21:
LeeMongRyong 1 65 true :Jong-ro 1-gil, Jongno-gu, Seoul:
LeeSoonShin 2 80 true :1001, Jungang-daero, Yeonje-gu, Busan:
*/
void PersonManager::append() { // Menu item 2
    int count = UI::getPositiveInt("The number of persons to append? ");
    // to_string(10) 함수: 정수 10을 문자열 "10"으로 변환
    // stoi("10") 함수: 문자열 "10"을 정수 10으로 변환
    printNotice("Input "+to_string(count), ":");
    for (int i = 0; i < count; ++i) {
        Person* p = factory.inputPerson(&cin); // 한 사람 정보 입력 받음
        if (p) persons.push_back(p); // if (p != nullptr) 와 동일;
        // 만약 p가 nullptr이면 이는 입력 시 에러가 발생한 것임
        // (즉, 정수를 입력해야 하는 곳에 일반 문자를 입력한 경우)
    }
    display();
}

void PersonManager::clear() { // Menu item 3
    deleteElemets();
    display();
}

// VectorPerson에 저장된 사람들 중 매개변수 name과 동일한 이름을 가진 사람을 찾는다.
Person* PersonManager::findByName(const string name) {
    /*for 문을 이용하여 persons에 저장된 각각(i)의 사람 객체에 대해
        매개변수 name과 비교해서 동일한 이름을 가진 객체이면 해당 객체의 포인터(Person*)를 반환한다.
        persons의 size(), at(i), at(i)->getName() 등의 멤버함수 활용하라.*/
	for(int i=0;i<persons.size();i++){
		if(name == persons.at(i)->getName())
					return persons.at(i);
	}
    // 찾지 못한 경우
    cout << name + ": NOT found" << endl;
    return nullptr;
}

// 사용자로부터 로그인할 사람의 이름과 비번을 입력받고 동일한 이름을 가진 Person 객체를
// persons에서 찾고 비번이 일치하는지 체크한 후 CurrentUser(p).run() 실행한다.
// 기존(ch4-1)의 Main Menu에서 호출하던 CurrentUser를 메뉴항목에서 삭제한 후
// 대신 여기서 호출함(사용자가 선택한 인적정보 p를 인자로 넘겨 주면서)
void PersonManager::login() {
    string name = UI::getNext("user name: ");
    Person* p = findByName(name); // 해당 사람을 VectorPerson에서 찾는다.
    if (p == nullptr) return;     // 해당 사람 존재하지 않음
    string passwd = UI::getNextLine("password: ");
    if (passwd != p->getPasswd()) // 비번 불일치
        cout << "WRONG password!!" << endl;
    else
        CurrentUser(p).run();
}

void PersonManager::run() {
    using func_t = void (PersonManager::*)();
    using PM = PersonManager; // 코딩 길이를 줄이기 위해
    func_t func_arr[] = {
        nullptr, &PM::display, &PM::append, &PM::clear, &PM::login,
    };
    int menuCount = sizeof(func_arr) / sizeof(func_arr[0]); // func_arr[] 길이
    string menuStr =
        "====================== Person Management Menu ===================\n"
        "= 0.Exit 1.Display 2.Append 3.Clear 4.Login(CurrentUser, ch4) =\n"
        "=================================================================\n";

    while (true) {
        int menuItem = UI::selectMenu(menuStr, menuCount);
        if (menuItem == 0) return;
        (this->*func_arr[menuItem])();
    }
}

/******************************************************************************
 * ch3_2, 4_1, 4_2: MultiManager class
 ******************************************************************************/
class MultiManager
{
    static const int personCount = 5;
    Person persons[personCount] = {
        Person("p0", 10, 70.0, false, "Gwangju Nam-gu Bongseon-dong 21"),
        Person("p1", 11, 61.1, true,  "Jong-ro 1-gil, Jongno-gu, Seoul"),
        Person("p2", 12, 52.2, false, "1001, Jungang-daero, Yeonje-gu, Busan"),
        Person("p3", 13, 83.3, true,  "100 Dunsan-ro Seo-gu Daejeon"),
        Person("p4", 14, 64.4, false, "88 Gongpyeong-ro, Jung-gu, Daegu"),
    };
    // new를 이용해 동적으로 할당할 경우 소멸자 함수를 만들어 거기서 delete 해 주어야 함

    static const int allPersonCount = personCount;
    Person* allPersons[allPersonCount] = {
        &persons[0], &persons[1], &persons[2], &persons[3], &persons[4],
    };

    PersonManager personMng { allPersons, allPersonCount };
    // 위 문장은 PersonManager personMng(allPersons, allPersonCount);와 동일함

public:
    void run() {
        cout << "PersonManager::run() starts" << endl;
        personMng.run();
        cout << "PersonManager::run() returned" << endl;
    }
}; // ch3_2, 4_1, 4_2: MultiManager class

/******************************************************************************
 * ch3_1: Class And Object
 ******************************************************************************/

//Person gps("gps", 1000, 100, true, "gps adress");

class ClassAndObject
{

	void defualConstructor() { // Menu item 1
	        Person ps;    // Person::Person() 기본 생성자 및 소멸자 실행
	    }


	void constructor() { // Menu item 2: 지역객체의 다양한 생성자 및 소멸자 실행
	        Person ps1;
	        Person ps2("ps2");
	        Person ps3("ps3", 3, 70.3, true, "ps3 address");
	    }


	void construcor_destructor_inner_inner() {
	        cout << "\n--- construcor_destructor_inner_inner() begins ---" << endl;
	        Person ps3("ps3");
	        cout << "\n- if begins -" << endl;
	        if (true) {
	            Person ps4("ps4");
	        }
	        cout << "- if ends -\n" << endl;
	        cout << "- temporary object begins -" << endl;
	        Person("temp_ps_1"); // 임시객체(이름 없는 객체) 성성 및 소멸
	        cout << endl;
	        Person("temp_ps_2").setName("TEMP_PS_2"); // 임시객체 생성,
	                                                  // setName() 호출, 임시객체 소멸
	        cout << "- temporary object ends -\n" << endl;
	        Person ps5("ps5");
	    }


	    void construcor_destructor_inner() {
	        cout << "\n--- construcor_destructor_inner() begins ---" << endl;
	        Person ps2("ps2");
	        construcor_destructor_inner_inner();
	        cout << "--- construcor_destructor_inner_inner() returned ---\n" << endl;
	        Person ps6("ps6");
	    }


	    void construcorDestructor() { // Menu item 3
	        Person ps1("ps1");
	        construcor_destructor_inner();
	        cout << "--- construcor_destructor_inner() returned ---\n" << endl;
	    }


	    void global_static_local_objects_inner() {
	            cout << "\n--- global_static_local_objects_inner() begins ---" << endl;
	            //static Person sps("sps", 2000, 200, false, "sps adress");
	        }


	        void globalStaticLocalObjects() { // Menu item 4
	            global_static_local_objects_inner();
	            cout << "--- global_static_local_objects_inner() returned ---" << endl;
	            global_static_local_objects_inner();
	            cout << "--- global_static_local_objects_inner() returned ---" << endl;
	        }


public:
    // 사용자가 선택한 메뉴 항목을 실행하는 멤버 함수(func_arr[menuItem]에 등록된 함수)를 호출
    void run() {
        // ClassAndObject의 멤버 함수에 대한 포인터 타입인 새로운 데이타 타입 func_t를 정의함
        using func_t = void (ClassAndObject::*)();
        // 위 using 문은 ClassAndObject 클래스의 멤버 함수(리턴 타입이 void 이면서 매개변수가 없는)에
        // 대한 포인터 타입을 앞으로는 간단히 func_t로 사용하겠다는 의미이다.

        // ClassAndObject의 멤버 함수 포인터들의 배열
        // 나중에 호출하기 위해 멤버 함수의 이름을 미리 배열에 저장(등록)
        //     함수 이름이 곧 함수에 대한 포인터임: 즉, 함수의 첫번째 명령어의 주소를 저장함
        // 등록된 이 멤버 함수들은 나중에 run()에서 호출됨
        //     (즉, 함수 주소로 jump 해 가서 함수를 실행함)

        func_t func_arr[] = { // 메뉴항목을 실행하는 멤버 함수를 배열에 미리 저장(등록)해 둠
            nullptr, &ClassAndObject::defualConstructor, &ClassAndObject::constructor,
            &ClassAndObject::construcorDestructor, &ClassAndObject::globalStaticLocalObjects,
        };
        int menuCount = sizeof(func_arr) / sizeof(func_arr[0]);
        // func_arr[]의 원소의 개수 = 배열 전체 크기 / 한 배열 원소의 크기

        // 화면에 보여 줄 메뉴
        string menuStr =
            "+++++++++++ Person Class And Object Menu ++++++++++++\n"
            "+ 0.Exit 1.DefualConstructor 2.Constructor          +\n"
            "+ 3.ConstrucorDestructor 4.GlobalStaticLocalObjects +\n"
            "+++++++++++++++++++++++++++++++++++++++++++++++++++++\n";

        while (true) {
            int menuItem = UI::selectMenu(menuStr, menuCount); // 메뉴 번호 입력 받음
            if (menuItem == 0) return;
            (this->*func_arr[menuItem])(); // 선택된 메뉴 항목을 실행할 멤버 함수를 호출함
            /* 위 문장은 다음을 간단히 한 것이다. 앞으로 계속 나올테니 잘 이해하기 바란다.
               func_t f = func_arr[menuItem];// 배열에 저장된 멤버 함수 포인터
               (this->*f)();                 // 멤버함수 포인터를 이용하여 해당 함수를 호출함
            */
        }
    }
}; // ch3_1: class ClassAndObject

/******************************************************************************
 * Main Menu
 ******************************************************************************/

class MainMenu
{
public:
    void run() {
        int menuCount = 3; // 상수 정의
        string menuStr =
        "******************************* Main Menu *********************************\n"
        "* 0.Exit 1.PersonManager(ch3_2, 4)                                        *\n"
        "* 2.Class:Object(ch3_1)                                                   *\n"
        "***************************************************************************\n";

        while (true) {
            int menuItem = UI::selectMenu(menuStr, menuCount);
            if (menuItem == 0) break;

            switch(menuItem) {
            case 1: MultiManager().run();     break;
            case 2: ClassAndObject().run();           break;
            }
        }
        cout << "Good bye!!" << endl;
    }
}; // class MainMenu

/******************************************************************************
 * run() 함수: 메인 메뉴를 시작함
 ******************************************************************************/
void run() {
    MainMenu().run();
    //Memo().run();
    // MainMenu 타입의 이름 없는 임시객체를 생성한 후
    // 그 객체의 run() 멤버함수를 호출함; run()에서 리턴한 후에는 임시객체가 자동 소멸됨
    // 즉, 위 문장은 아래 두 문장과 동일한 기능임
    // MainMenu mm;
    // mm.run();
}

/******************************************************************************
 * Program Test: Automatic Error Check
 ******************************************************************************/
#if AUTOMATIC_ERROR_CHECK
#include "check_error.h"
#endif

/******************************************************************************
 * main() 함수
 ******************************************************************************/
int main() {

	cout << boolalpha;  // 11장에서 배움; bool 타입 값을 0, 1 대신 true, false로 출력하도록 설정
	    cin >> boolalpha;   // bool 타입 값을 0, 1 대신 true, false로 입력 받도록 설정

#if AUTOMATIC_ERROR_CHECK
    evaluate(false);            // 함수 호출
#else
    run();
#endif
}

//-----------------------------------------------------------------------------
// ch2.cpp 소스 끝
//-----------------------------------------------------------------------------




