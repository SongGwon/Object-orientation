/*
 * ch6_1.cpp
 *
 *  Created on: 2024. 4. 16.
 *      Author: USER
 *  + "address, memo_c_str deleted" 관련 출력 문장 삭제
 *  + 디폴트 매개변수를 활용하여 기본 생성자 Person(), VectorPerson() 제거
 *  + Memo 클래스 디폴트 매개변수를 가진 생성자 추가
 *  + Person::set(), Memo::c_str() 등 함수 중복
 *  + UI namespace 대신 class UI로 변경: static 멤버 활용
 *  + 함수의 매개변수 또는 반환 값으로 값이나 포인터 대신 가능하면 참조자 &를 사용하도록 변경
 *     즉, call by value 나 call by address 대신 가능하면 call by reference를 활용
 *  + 모든 UI::inputPerson() 호출시 매개변수를 포인터에서 참조자로 변경
 *  + CurrentUser: 클래스 포인터 pUser -> 참조자 rUser로 모두 변환
 *  + Factory: 멤버 함수들의 포인터 인자를 참조자로 변경
 *  + CurrentUser: 멤버 defaultParameter(), staticMember() 추가
 *  + string을 매개변수, 리턴 하는 모든 함수들의 데이타 타입을 string 참조 변수로 변경
 */

#include <iostream>
#include <cstring>
using namespace std;

#define AUTOMATIC_ERROR_CHECK false // false: 자동 오류 체크, true: 키보드에서 직접 입력하여 프로그램 실행


/******************************************************************************
 * Person class
 ******************************************************************************/

class Person
{
    string   name;        	// 이름
    string passwd;          // 비번
    int    id;              // Identifier
    double weight;          // 체중
    bool   married;         // 결혼여부
    char*  address;			// 주소 40?
    char*  memo_c_str;		// 메모장

protected:
    void inputMembers(istream* in);
    void printMembers(ostream* out);
    void copyAddress(const char* address); // 5_2에서 추가
    void copyMemo(const char* c_str);      // 5_2에서 추가

public:
    //Person(): Person("") {};
    //Person(const string name): Person(name, 0, 0, 0, ""){};
    Person(const string name, int id, double weight, bool married, const char *address);
    Person(const Person& p);
    ~Person();

    void set(const string name, int id, double weight, bool married, const char *address);
    void setName(const string name)       { this->name = name; }
    void setPasswd(const string passwd)   { this->passwd = passwd; }
    void set(int id)                   { this->id = id; }
    void set(double weight)        { this->weight = weight; }
    void set(bool married)        { this->married = married; }
    void setAddress(const char* address); // 5_2에서 수정
    void setMemo(const char* c_str);      // 5_2에서 수정

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
    Person& assign(const Person& p);
    bool isSame(const string name, int id);         // ch3_2에서 추가
};

Person::Person(const string name = {}, int id = {}, double weight = {}, bool married = {},
               const char *address = {}) :
        name(name), id{id}, weight{weight}, married{married}, memo_c_str{} {
    // 위에서 각 멤버를 초기화하는 {}는 각 매개변수 값을 객체의 상응하는 멤버에 설정하는 것이다. 즉,
    // this->id=id, this->weight=weight, this->married=married와 동일하다.
    // 여기서 this는 해당 객체를 포인터하는 포인터 변수이며, (다음 장에서 설명될 예정임)
    // this->id는 해당 객체의 멤버 id를 지칭하며 =의 우측변수 id는 함수의 매개변수이다.
    // 값을 간단히 대입하는 것은 위처럼 하면 되지만 name[], address[]와 같은 배열 복사 함수
    // 호출 또는 다른 함수를 호출할 때는 생성자 함수의 몸체 { } 내에서 호출해야 한다.
    // 위 memo_c_str{}는 배열 memo_c_str[]의 모든 원소를 기본 값인 널 문자('\0')로 초기화하는 코드임
    copyAddress(address);
    cout << "Person::Person(...):"; println();
}

// Person 멤버 데이타 중에 동적으로 메모리를 할당 받는 멤버가 없기 때문에
// 복사생성자를 직접 만들지 않아도 컴파일러가 자동으로 아래와 같은 복사생성자를 만들어 준다.
// 여기서 복사생성자를 굳이 만든 이유는 실행과정을 확인하기 위한 출력문을 넣기 위해서이다.
Person::Person(const Person& p): name(p.name), id{p.id}, weight{p.weight}, married{p.married} {
  /*TODO: p 객체의 address 문자열을 멤버 address에 복사함
          (즉, setAddress() 대신 copyAddress() 사용해야 함)
          p 객체의 메모 문자열을 멤버 memo_c_str에 복사함 (copyMemo() 사용해야 함)*/
	copyAddress(p.address);
	copyMemo(p.memo_c_str);
    cout << "Person::Person(const Person&):"; println();
}

Person::~Person() {
    cout << "Person::~Person():"; println();

   /* TODO: address와 memo_c_str가 포인터하는 배열 메모리를 반납한다.
          이들 포인터가 nullptr인 경우 반납하지 말아야 한다.*/
    if(address != nullptr)
    	delete [] address;
    if(memo_c_str != nullptr)
    	delete [] memo_c_str;

}

void Person::printMembers(ostream* pout)   {
	*pout << name << " " << id << " " << weight << " " << married
	    		<< " " << ":" << (address==nullptr ? "" : address) << ":";
	// 3항 연산자를 이용하여 address가 nullptr일 경우 ""를 출력하고 아닌 경우 address를 출력하라.
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

	// 지역변수로 미리 큰 주소용 배열을 잡는다.
	char address[40];    // ch5_2에서 추가됨


	pin->getline(address, sizeof(address), ':');
	pin->getline(address, sizeof(address), ':');

	// 아래 함수를 통해 위 지역변수 address[]에 있는 주소를 멤버 address로 복사한다.
	// 아래 함수에서 address[]의 문자열 길이만큼 메모리를 할당(멤버 address용) 받은 후 복사한다.
	// 멤버 address는 이전에 이미 초기화되었기 때문에 copyAddress()가 아닌 아래 함수를 사용함
	setAddress(address); // ch5_2에서 추가됨
}

void Person::whatAreYouDoing() {
    // TODO: [문제 5]
	cout << name << " is taking a rest.";
}

// 깊은 대입(=) 연산: p의 모든 멤버 값을 this에 복사한다. 멤버가 동적으로 메모리를 할당받은 경우
// 먼저 this의 멤버의 메모리를 먼저 반납하고 새로 메모리를 할당 받은 후 복사한다.
Person& Person::assign(const Person& p) {

  /*TODO: name, passwd, id, weight, married 멤버의 경우 = 연산자를 사용하여 p의 상응하는
          멤버 값을 this의 멤버에 바로 대입시켜라. (C++의 기본 데이타 타입과 string인 경우)
          즉, this의 각 멤버에 =를 이용해 p의 멤버 값을 저장하라.*/
          this->name = p.name; this->passwd = p.passwd; this->id = p.id;
          this->weight = p.weight; this->married = p.married;

        /*p.addres와 p.memo_c_str의 경우 각각 setAddress(), setMemo()를 호출하여
          this의 addres와 memo_c_str에 복사하라. 이 함수들은 기존에 할당 받았던 this의
          address와 memo_c_str 멤버의 메모리를 먼저 반납하고 새로 할당 받은 후 복사한다.*/
		  setAddress(p.address);
		  setMemo(p.memo_c_str);

          // 복사생성자에서는 copyAddress()와 copyMemo()를 호출했는데 (처음 초기화할 때)
          // 여기서는 setAddress(), setMemo()를 호출하였다. (기존 값을 변경하고자 할 때)
          // 왜 그렇게 했는지 이해했는가?

    return *this; // this 객체 자신의 참조자를 반환한다.
    // 자신 객체의 참조자를 리턴했기 때문에 c.assign(p.assign(backup)).println() 등의
    // 연속된 함수 호출도 가능해 진다. 즉 (c = p = backup).println()과 같은 효과
}


bool Person::isSame(const string name, int pid) {
    // TODO: [문제 6]
	return ((this->name == name) && (id == pid));
}

// 처음 객체가 초기화될 때(생성자 또는 복사생성자) address 멤버를 초기화하고자 할 때 호출된다.
// 즉, 일반 생성자 또는 복사 생성자에서만 호출됨.
// 새로 할당받은 메모리에 매개변수 address 문자열을 복사한다.
void Person::copyAddress(const char* address) {
    /*TODO: 매개변수 address가 nullptr인 경우, address 멤버도 nullptr로 설정하고 리턴하라.
          매개변수 address가 포인터하는 문자열을 포함할 수 있는 배열 메모리를 할당 받은 후
          할당된 메모리 주소를 멤버 address에 저장하고,
          매개변수의 문자열을 멤버 변수의 할당받은 메모리로 복사한다. */
	if(address == nullptr) {
		this->address = nullptr;
		return;
	}
	int len = strlen(address);
	this->address = new char[len+1];
	strcpy(this->address, address);
          // [교재 예제 5-11]의 복사 생성자  참조할 것
          // strlen(), strcpy() 함수는 구글링 또는 http://www.cplusplus.com/reference/에서 검색
          // strlen()은 끝의 null 문자를 포함하지 않은 길이이므로
          // 이를 포함하기 위해 할당해야 할 메모리는 strlen()+1이어야 함
          // strcpy()는 끝의 null 문자도 함께 복사해 준다라는 사실에 유의하라.
}

// 복사 생성자에 의해 처음 메모 문자열 memo_c_str을 초기화할 때 호출된다.
// 복사 생성자에서만 호출되고 일반 생성자에서는 nullptr로 설정됨
void Person::copyMemo(const char* c_str) {
  /*TODO: 매개변수 c_str가 nullptr인 경우, memo_c_str만 nullptr로 설정하고 리턴하라.
          매개변수 c_str가 포인터하는 문자열을 포함할 수 있는 배열 메모리를 할당 받아
          멤버 memo_c_str에 주소를 저장하고 매개변수의 문자열을 멤버 변수로 복사한다.*/
	if(c_str == nullptr) {
		this->memo_c_str = nullptr;
		return;
	}
	int len = strlen(c_str);
	this->memo_c_str = new char [len+1];
	strcpy(this->memo_c_str, c_str);
}

// 생성자 또는 복사생성자에 의해 이미 한번 초기화된 주소 문자열(기존의 주소)을 다른 주소로 변경할 때 호출됨
// 이미 멤버 address용 메모리가 할당되었기 때문에 기존 메모리를 먼저 반납해야 하고 새로 할당해야 함
void Person::setAddress(const char* address) {
  /*TODO: 기존의 멤버 address가 nullptr이 아닌 경우
        cout << "old address(" << this->address << ") deleted" << endl;
        멤버 address가 포인터하는 [배열] 메모리를 반납한다. 반납시 [] 를 꼭 사용해야 한다.*/
	if(this->address != nullptr){
		delete [] this->address;
	}
	copyAddress(address);
	// 새로 메모리 할당받은 후 복사한다.
}

// 멤버 memo_c_str은 생성자에 의해 nullptr로 설정되었거나 복사생성자에 의해 이미 초기화되었 수도 있음
// 만약 멤버 memo_c_str용 메모리가 할당되었다면 먼저 반납해야 함
void Person::setMemo(const char* c_str)      {
  /*TODO: 기존의 멤버 memo_c_str가 nullptr이 아닌 경우
              cout << "old memo_c_str deleted" << endl;
              멤버 memo_c_str가 포인터하는 [배열] 메모리를 반납한다. []를 잊지 마라.*/
	if(memo_c_str != nullptr){
		delete [] this->memo_c_str;
	}
    copyMemo(c_str); // 새로 메모리 할당받아 복사한다.
}

/******************************************************************************
 * User Interface
 ******************************************************************************/
// 기본적인 입력과 관련된 전역 함수들을 UI라는 이름공간 내부에 정의함

class UI {


static string line, emptyLine;

public:
	static bool echo_input;
	static bool checkInputError(istream* pin, const string msg);
	static bool checkDataFormatError(istream* pin);
	static bool inputPerson(Person* p);
	static string getNext(const string msg);
	static string getNextLine(const string msg);
	static int getInt(const string msg);
	static int getPositiveInt(const string msg);
	static int getIndex(const string msg, int size);
	static int selectMenu(const string menuStr, int menuItemCount);
};

bool UI::echo_input = false;
string UI::line, UI::emptyLine;

// 입력에서 정수 대신 일반 문자가 입력되었는지 체크하고 에러 발생시 에러 메시지 출력
bool UI::checkInputError(istream* pin, const string msg) {
    if (!(*pin)) { // 에러가 발생했다면
        cout << msg;  // 에러 메시지를 출력
        pin->clear(); // 에러 발생 상태정보를 리셋함; 그래야 다음 문장에서 읽을 수 있음
        getline(*pin, emptyLine); // 에러가 발생한 행 전체를 읽어 데이터를 버림
        return true;
    }
    return false;
}

// 정수나 실수를 입력해야 하는 곳에 일반 문자열을 입력한 경우의 에러 체크
bool UI::checkDataFormatError(istream* pin) {
    return checkInputError(pin, "Input-data format MISMATCHED\n");
}

// 한 사람의 정보 즉, 각 멤버 데이터를 순서적으로 입력 받아 p에 저장하고
// 입력 중 입력 데이터에 오류가 있는지 확인하고 오류가 있을 시 에러 메시지를 출력한다.
bool UI::inputPerson(Person* p) {
    cout << "input person information:" << endl;
    p->input(&cin);
    if (checkDataFormatError(&cin)) return false;
    if (echo_input) p->println(); // 자동체크에서 사용됨
    return true;
}

// 입력장치에서 하나의 단어로 구성된 문자열을 입력 받음
string UI::getNext(const string msg) {
    cout << msg; // 입력용 메시지를 출력
    cin >> line; // 하나의 단어를 읽어 들임
    if (echo_input) cout << line << endl; // 자동체크 시 출력됨
    getline(cin, emptyLine); // 입력받은 한 단어 외 그 행의 나머지 데이타(엔터포함)는 읽어서 버림
    return line;             // 이유는 여기서 [엔터]를 제거하지 않으면
}                            // 다음의 getNextLine()에서 엔터만 읽어 들일 수 있기 때문에

// 입력장치에서 한 행을 입력 받음
string UI::getNextLine(const string msg) {
    cout << msg; // 입력용 메시지를 출력
    getline(cin, line); // 한 행을 읽어 들임
    if (echo_input) cout << line << endl; // 자동체크 시 출력됨
    return line;
}

// 하나의 정수를 입력 받음; 정수가 아닌 아닌 문자열 입력시 에러 메시지 출력 후 재입력 받음
int UI::getInt(const string msg) {
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
int UI::getPositiveInt(const string msg) {
    int value;
    while ((value = getInt(msg)) < 0)
        cout << "Input a positive INTEGER." << endl;
    return value;
}

// 0~(size-1)사이의 선택된 메뉴 항목 또는 리스트의 항목의 인덱스 값을 리턴함
// 존재하지 않는 메뉴항목을 선택한 경우 에러 출력
int UI::getIndex(const string msg, int size) {
    while (true) {
        int index = getPositiveInt(msg);
        if (0 <= index  && index < size) return index;
        cout << index << ": OUT of selection range(0 ~ "
             << size-1 << ")" << endl;
    }
}

// 사용자에게 메뉴를 보여주고 사용자가 선택한 메뉴항목의 인덱스를 리턴함
int UI::selectMenu(const string menuStr, int menuItemCount) {
    cout << endl << menuStr;
    return getIndex("Menu item number? ", menuItemCount);
} // class UI

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
    Memo(const char* c_str = {} ): mStr(c_str == nullptr ? "" : c_str) { }
    string getNext(size_t* ppos);
    void displayMemo();
    const char *c_str() { return mStr.c_str(); }
    void c_str(const char *c_str) {
          /*TODO: 매개변수 c_str이 nullptr일 경우 mStr을 ""로 설정하고
                  아닌 경우 mStr을 c_str로 설정하라.
                  mStr = ? : ; 3항 연산자를 사용하여 한 문장으로 완성하라.*/
    	mStr = (c_str == nullptr) ? "" : c_str;
        }  // C 문자열로 메모 mStr 초기화

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
          찾지 못한 경우 string::npos를 *plen에 저장 예를들어, 실제로 행이
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
    Person& rUser;
    Memo    memo; // ch4_3에서 추가

public:
    CurrentUser(Person& rUser): rUser(rUser) { }  // user(u)는 this->user = u 와 동일한 기능
    void display();
    void setter();
    void getter();
    void set();
    void whatAreYouDoing();
    void isSame();
    void inputPerson();
    void changePasswd();
    void manageMemo();
    void defaultParameter();
    void staticMember();
    void run();
};

void CurrentUser::display() { // Menu item 1
    rUser.println();
}

void CurrentUser::getter() { // Menu item 2
    cout << "name:" << rUser.getName() << ", id:" << rUser.getId() << ", weight:" <<
            rUser.getWeight() << ", married:" << rUser.getMarried() <<
            ", address:" << rUser.getAddress() << endl;
}

void CurrentUser::setter() { // Menu item 3
    Person* pp = new Person {"pp"};
    pp->setName(pp->getName());
    pp->set(rUser.getId());
    pp->set(rUser.getWeight());
    pp->set(rUser.getMarried());
    pp->setAddress(rUser.getAddress());
    cout << "pp->setMembers():"; pp->println();
    pp->println(); //??
    delete pp;
}

void CurrentUser::set() { // Menu item 4
    Person* pp = new Person {"pp"};
    pp->set(pp->getName(), rUser.getId(), rUser.getWeight(),
              !rUser.getMarried(), rUser.getAddress());
    cout << "pp->set():"; pp->println();
    delete pp;
}

void CurrentUser::whatAreYouDoing() {  // Menu item 5
    rUser.whatAreYouDoing();
}

void CurrentUser::isSame() { // Menu item 6
    rUser.println();
    cout << "isSame(\"user\", 1): " << rUser.isSame("user", 1) << endl;
}

void CurrentUser::inputPerson() { // Menu item 7
    if (UI::inputPerson(&rUser)) // GilDong 1 70.5 true :Jongno-gu, Seoul:
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
    memo.c_str(pUser->getMemo());
    memo.run();
    pUser->setMemo(memo.c_str());
}

void CurrentUser::defaultParameter() { // Menu item 10
    Person ps1;
    Person ps2("ps2");
    Person ps3("ps3", 3);
    Person ps4("ps4", 4, 70.4);
    Person ps5("ps5", 5, 70.5, true);
    Person ps6("ps6", 6, 70.6, true, "ps6 address");

    cout << "\nMemo m1;" << endl;
    Memo m1; // 메모 생성자에게 인자를 넘겨 주지 않은 경우
    m1.displayMemo();

    cout << "\nMemo m2(pUser->getMemo())" << endl;
    Memo m2(pUser->getMemo()); // 메모 생성자에게 인자를 넘겨 준 경우
    m2.displayMemo();
}

void CurrentUser::staticMember() { // Menu item 11

  /*TODO: string 변수 word1을 선언하고 클래스 이름을 사용하여 UI의
          getNext("Input a word: ")를 호출한 후 리턴 값으로 word1 변수를 초기화하라.*/
	string word1 = UI::getNext("Input a word: ");
    cout << "UI::getNext(): " << word1 << endl << endl;

    UI ui;

  /*TODO: string 변수 word2을 선언하고 UI의 객체인 ui 이름으로
          getNext("Input a word: ")를 호출한 후 리턴 값으로 word2 변수를 초기화하라.*/
    string word2 = ui.getNext("Input a word: ");
    cout << "ui.getNext() : " << word2 << endl;
}

void CurrentUser::run() {
    using func_t = void (CurrentUser::*)();
    using CU = CurrentUser;
    func_t func_arr[] = {
        nullptr, &CU::display, &CU::getter, &CU::setter,
        &CU::set, &CU::whatAreYouDoing,
        &CU::isSame, &CU::inputPerson, &CU::changePasswd, &CU::manageMemo,
		&CU::defaultParameter, &CU::staticMember,
    };
    int menuCount = sizeof(func_arr) / sizeof(func_arr[0]); // func_arr[] 배열의 길이
    string menuStr =
            "+++++++++++++++++++++ Current User Menu ++++++++++++++++++++++++\n"
            "+ 0.Logout 1.Display 2.Getter 3.Setter 4.Set 5.WhatAreYouDoing +\n"
    		"+ 6.IsSame 7.InputPerson 8.ChangePasswd(4_2) 9.ManageMemo(4_3) +\n"
    		"+ 10.DefaultParameter(6_1) 11.StaticMember(6_1)                +\n"
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
    //VectorPerson(): VectorPerson(DEFAULT_SIZE) {  /* TODO 문제 [2]: 위임 생성자로 변환 */}
    VectorPerson(int capacity = DEFAULT_SIZE);
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
    static Person* inputPerson(istream* in) {
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
    //Factory factory;

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
    	Person *copied = new Person(*s);
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
        Person* p = Factory::inputPerson(&cin); // 한 사람 정보 입력 받음
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
        "= 0.Exit 1.Display 2.Append 3.Clear 4.Login(CurrentUser, ch6)   =\n"
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
 * ch5_1: Reference and CopyConstructor
 ******************************************************************************/
class CopyConstructor
{
    Person u;
    Person backup;

public:
    // u는 일반 생성자, backup은 복사생성자를 이용하여 멤버를 초기화
    CopyConstructor(): u("u", 1, 70, true, "Gwangju"), backup(u) { }

    void explicitCopyConstructor() { // Menu item 1
            cout << "u: "; u.println();

            //TODO: Person형의 객체 p를 선언하되 복사생성자를 사용하여 u을 복사하여 초기화하라.
            Person p(u);
            //TODO: 실행결과를 참고하여 지역 객체 p의 멤버 값들을 변경하라.
            p.setName("p"); p.set(2); p.set(80); p.set(false), p.setAddress("Seoul");

            cout << "u: "; u.println();
            cout << "p: "; p.println();
            // 함수 리턴 시 객체 p의 소멸자가 실행됨
        }

    void referenceVariable() { // Menu item 2
           cout << "p: ";
           Person p(u);

           //TODO: 지역 변수 p와 객체를 공유하는 Person형의 참조 변수 r을 선언하고 초기화하라.
           Person &r = p;
           cout << "p: "; p.println();         // r은 참조이고 u와 동일한 객체를 공유하므로
           cout << "r: "; r.println();         // 두 출력물은 동일한 객체를 출력함

           //TODO: 실행결과를 참고하여 참조 변수 r의 멤버 값들을 변경하라.
           r.setName("r"); r.set(2); r.set(80); r.set(false), r.setAddress("Seoul");
           cout << "p: "; p.println();
           cout << "r: "; r.println();

           //TODO: 실행결과를 참고하여 지역 객체 p의 멤버 값들을 변경하라.
           p.setName("p"); p.set(1); p.set(70); p.set(true), p.setAddress("Gwangju");
           cout << "p: "; p.println();
           cout << "r: "; r.println();
           // r은 객체 p를 공유하는 참조이므로 함수 리턴 시 소멸자가 실행되지 않음
       }


    // 매개변수 a는 함수호출시 복사 생성자에 의해 복사되어 초기화됨; call by value로 매개변수를 선언하라.
    void function_argument( Person a /* TODO: 매개변수 a 선언; [교재 예제 5-12] 참조 */ ) { // Menu item 3-1
        cout << "a: "; a.println();
        cout << "a.setName(a)" << endl;
        a.setName("a");
        cout << "a: "; a.println();
    }

    Person return_member_object() { // Menu item 3-2
        //TODO: u 객체를 반환함 (여기서는 임식 객체에 복사됨); [교재 예제 5-12] 참조
    	return u;
    }

    void implicitCopyConstructor() { // Menu item 3
        cout << "u: "; u.println();
        cout << "\nimplicit copy constructor 1: operator =" << endl;
        cout << "Person p = u" << endl;
        cout << "p: ";

        //TODO: 새로운 Person 객체 p를 선언하고 객체 u를 p에 대입하여 초기화하라.(묵시적 복사생성자를 호출) [교재 예제 5-12] 참조
        Person p = u;

        cout << "p.setName(p)" << endl;
        p.setName("p");
        cout << "p: "; p.println();

        cout << "\nimplicit copy constructor 2: function argument" << endl;
        cout << "call function_argument(u)" << endl;

        // 묵시적으로 복사 생성자를 호출하여 함수 인자를 복사하여 함수의 매개변수 a에 넘김
        function_argument(u);
        cout << "function_argument(u) returned" << endl;

        cout << "\nimplicit copy constructor 3: return object" << endl;
        cout << "call return_member_object().println()" << endl;
        return_member_object().println();
          // return_member_object()의 리턴 값을 복사생성자에 의해 임시객체(이름 없는 지역변수)에 복사하고
          // 그 임시 객체의 println()을 호출하여 객체를 출력한다.
          // 출력 후 더 이상 그 임시 객체가 필요 없기 때문에 소멸자를 호출하여 바로 임시 객체를 소멸시킴
        cout << "return_member_object() returned" << endl;
    }

    Person return_local_object() { // Menu item 4-1
            Person pL("pL", 0, 70, true, "Gwangju Nam-gu");
            cout << "return_local_object() returns pL" << endl;
            return pL;
            // 정석대로 하면 리턴하기 전에 복사생성자를 호출하여 지역 객체 pL를 복사해 준 후
            // 리턴 직전에 pL를 소멸해야 한다.
            // 그러나 컴파일러는 효율적인 실행을 위해 복사생성자와 소멸자를 호출하지 않고
            // 함수가 리턴한 후 return_local_object().println() 호출 시
            // 함수의 지역 객체 pL의 메모리를 그대로 재사용한다. (소멸되지 않았으므로)
            // println() 실행 후 더 이상 객체가 필요 없으면 그제서야 소멸자를 호출하여 소멸시킴
        }

    Person return_temporary_object() { // Menu item 4-2
            cout << "return_temporary_object() returns Person(pT, 0, 70, true, Gwangju Nam-gu)" << endl;
            return Person("pT", 0, 70, true, "Gwangju Nam-gu"); // 임시 객체를 리턴함
            // return {"p", 0, 70, true, "Gwangju Nam-gu"}; // 위 문장과 동일한 기능임
            // 위 문장은 컴파일러가 함수의 리턴 타입을 참고하여
            //   Person("p", 0, 70, true, "Gwangju Nam-gu")으로 자동 처리함
        }

    void temporaryObject() { // Menu item 4
            cout << "temporary object 1: Person(p0, 10, 70, true, Gwangju)" << endl;
            // 임시객체 생성 방법: 클래스이름(생성자 인자들)
            // 생성자 호출된 후 아래 문장이 끝나면 바로 소멸자가 호출됨
            // 주요: 임시 객체가 포함된 해당 문장의 실행이 종료되면 임시 객체는 소멸됨
            Person("p0", 10, 70, true, "Gwangju");

            cout << "\ntemporary object 2: Person(p1, 11, 60, false, Seoul).println()" << endl;
            // 임시객체: 생성자 호출, println() 호출, 소멸자 호출
            Person("p1", 11, 60, false, "Seoul").println();

            cout << "\nreturn_member_object().println()" << endl;
            return_member_object().println(); // 복사 생성자 활용
            cout << "return_member_object() returned" << endl;

            cout << "\nreturn_local_object().println()" << endl;
            return_local_object().println(); // 복사 생성자 사용하지 않음
            cout << "return_local_object() returned" << endl;

            cout << "\nPerson pL = return_local_object()" << endl;
            Person pL = return_local_object(); // 복사 생성자 활용
            cout << "return_local_object() returned" << endl;
            pL.println();

            cout << "\nreturn_temporary_object().println()" << endl;
            return_temporary_object().println();  // 생성자 활용
            cout << "return_temporary_object() returned" << endl;

            cout << "\nPerson pT = return_temporary_object()" << endl;
            Person pT = return_temporary_object();  // 생성자 활용
            cout << "return_temporary_object() returned" << endl;
            pT.println();
        }

    // TODO: 아래 함수 매개변수를 call by value가 되게 Person의 객체 p로 선언하라.
    void call_by_value( /* TODO */ Person p ) { // Menu item 5-1: call by value: 복사생성자에 의해 복사됨
               cout << "p: "; p.println();
               cout << "u: "; u.println();
               cout << "p.set(p, 2, 80, false, Seoul)" << endl;
               p.set("p", 2, 80, false, "Seoul");
               cout << "p: "; p.println();      // p는 u과 다른 메모리를 사용하는 객체임
               cout << "u: "; u.println(); // u은 영향을 받지 않음
               // 함수 리턴 시 매개변수 p 객체의 소멸자 함수가 호출됨
       }

       // TODO: 아래 함수 매개변수를 call by reference가 되게 Person의 참조변수 p로 선언하라.
    void call_by_reference( /* TODO */ Person &p ) { // Menu item 5-2: call by reference
               cout << "p: "; p.println();   // p는 u의 참조이므로 u와 동일한 객체 메모리를 공유함
               cout << "u: "; u.println();
               cout << "p.set(p, 2, 80, false, Seoul)" << endl;
               p.set("p", 2, 80, false, "Seoul");
               cout << "p: "; p.println();   // p와 u은 동일한 객체 메모리를 공유하므로 항상 내용이 동일함
               cout << "u: "; u.println();
               u.assign(backup);       // u 값을 원래 값으로 복구
               // 매개변수 p는 참조이므로 함수 리턴 시 소멸자가 호출되지 않음
       }

       // TODO: 아래 함수 매개변수를 call by address가 되게 Person에 대한 포인터 변수 p로 선언하라.
    void call_by_address( /* TODO */ Person* p ) { // Menu item 5-3: call by address
               cout << "p: "; p->println();   // p는 u 메모리를 포인터하므로 동일한 내용이 출력됨
               cout << "u: "; u.println();
               cout << "p->set(p, 2, 80, false, Seoul)" << endl;
               p->set("p", 2, 80, false, "Seoul");
               cout << "p: "; p->println();   // p는 u 메모리를 포인터하므로 항상 동일한 내용이 출력됨
               cout << "u: "; u.println();
               u.assign(backup);       // u 값을 원래 값으로 복구
               // 매개변수 p는 포인터이므로 함수 리턴 시 소멸자가 호출되지 않음
       }

    void functionParameterType() { // Menu item 5
               cout << "call_by_value" << endl;
               // 함수 호출 시 복사생성자를 통해 u 객체를 매개변수 p에 복사
               //TODO: call_by_value()을 호출하되 멤버 객체 u을 인자로 넘겨 주어라.
               call_by_value(u);
               cout << endl;

               cout << "call_by_reference" << endl;
               //TODO: call_by_reference()을 호출하되 멤버 객체 u을 인자로 넘겨 주어라.
               call_by_reference(u);
               cout << endl; // 단순히 u의 참조만 전달됨

               cout << "call_by_address" << endl;
               //TODO: call_by_address()을 호출하되 멤버 객체 u의 주소를 인자로 넘겨 주어라.
               call_by_address(&u);
               cout << endl; // 단순히 u의 주소값, 즉 포인터가 전달됨
       }

       // TODO: 아래 주석(/* */)을 지우고 Person 객체를 리턴하도록 함수의 리턴 타입을 선언하라.
    Person return_value() {                   // return value
               return u;
           }

    void return_value_test() { // Menu item 6-1
               cout << "--- return_value_test() ---" << endl;
               cout << "call return_value()" << endl;
               cout << "p: ";

               //TODO: Person형 객체 변수 p를 선언하고 return_value()의 리턴 값으로 초기화하라.
               Person p = return_value();

               cout << "return_value() returned" << endl;
               cout << "p: "; p.println();
               cout << "p.set(p, 2, 80, false, Seoul)" << endl;
               p.set("p", 2, 80, false, "Seoul");
               cout << "p: "; p.println();        // 두 객체의 name은 다른 값임
               cout << "u: "; u.println();
               // 함수 리턴 시 지역변수 p 객체의 소멸자 함수가 호출됨
           }

       // TODO: 아래 주석(/* */)을 지우고
       //       Person 객체의 참조를 리턴하도록 함수의 리턴 데이타 타입을 선언하라.
    Person& return_reference() {                  // return reference
               return u;
           }

    void return_reference_test() { // Menu item 6-2
               cout << "--- return_reference_test() ---" << endl;
               cout << "call return_reference()" << endl;
               cout << "p: ";

               //TODO: Person형 객체 변수 p를 선언하고 return_reference()의 리턴 값으로 초기화하라.
               Person p = return_reference();

               cout << "return_reference() returned" << endl;
               cout << "p.set(p, 2, 80, false, Seoul)" << endl;
               p.set("p", 2, 80, false, "Seoul");
               cout << "p: "; p.println();
               cout << "u: "; u.println();
               cout << endl;

               cout << "call return_reference()" << endl;

               //TODO: Person형 참조 변수 r를 선언하고 return_reference()의 리턴 값으로 초기화하라.

               Person& r = return_reference(); // r는 리턴된 u의 참조를 저장하고 있는 참조변수임
               cout << "return_reference() returned" << endl;
               cout << "u: "; u.println();
               cout << "r.set(p, 2, 80, false, Seoul)" << endl;
               r.set("r", 2, 80, false, "Seoul");
               cout << "r: "; r.println();
               cout << "u: "; u.println();
               u.assign(backup);
               // 함수 리턴 시 지역 객체 p의 경우 소멸자 함수가 호출되지만
               //           참조 변수 r는 소멸자가 호출되지 않음
           }

       // TODO: 아래 주석을 지우고 함수의 리턴 값을 참조하여 함수의 리턴 데이타 타입을 적절히 선언하라.
    Person* return_address() {                  // return address
               return &u;
           }

    void return_address_test() { // Menu item 6-3
               cout << "--- return_address_test() ---" << endl;
               cout << "call return_address()" << endl;

               //TODO: Person형 포인터 변수 p를 선언하고 return_address()의 리턴 값으로 초기화하라.
               Person *p = return_address();

               cout << "return_address() returned" << endl;
               cout << "p: "; p->println();
               cout << "u: "; u.println();
               cout << "p.set(p, 2, 80, false, Seoul)" << endl;
               p->set("p", 2, 80, false, "Seoul"); // p는 u 객체를 포인터하므로 u과 동일한 메모리를 공유함
               cout << "p: "; p->println();
               cout << "u: "; u.println();
               u.assign(backup);
               // 함수 리턴 시 p는 포인터 변수이므로 소멸자가 호출되지 않음
           }

    void returnDataType() { // Menu item 6
               return_value_test();        cout << endl;
               return_reference_test();    cout << endl;
               return_address_test();
           }

    void inputPerson() { // Menu item 7
       	cout << "u: "; u.println();
           while (!UI::inputPerson(&u)) ;  // USER 11 88 false :DONG-GU, DAEGU:
           backup.assign(u);
           cout << "u: "; u.println();
       }

    void run() {
        using CC = CopyConstructor;
        // TODO 문제 [3]: func_t, func_arr[], menuCount 선언
    	using func_t = void (CC::*)();
    	    func_t func_arr[] = {
    	        nullptr, &CC::explicitCopyConstructor, &CC::referenceVariable,
				&CC::implicitCopyConstructor, &CC::temporaryObject, &CC::functionParameterType,
				&CC::returnDataType, &CC::inputPerson,
    	    };
    	    int menuCount = sizeof(func_arr) / sizeof(func_arr[0]); // func_arr[] 길이

        string menuStr =
            "+++++++++ Reference And Copy Constructor Menu ++++++++++\n"
            "+ 0.Exit 1.ExplicitCopyConstructor 2.ReferenceVariable +\n"
            "+ 3.ImplicitCopyConstructor 4.TemporaryObject          +\n"
            "+ 5.FunctionParameterType 6.FunctionReturnType         +\n"
            "+ 7.inputPerson                                        +\n"
            "++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";

        // TODO 문제 [3]: while 문장 삽입하여 선택된 메뉴항목 실행하는 함수를 호출하라.
        while (true) {
                int menuItem = UI::selectMenu(menuStr, menuCount);
                if (menuItem == 0) return;
                (this->*func_arr[menuItem])();
            }
  }
}; // ch5_1: Reference and CopyConstructor

/******************************************************************************
 * ch5_2: AllocatedMember, new을 이용한 동적 메모리 할당한 멤버 취급
 ******************************************************************************/
class AllocatedMember
{
    Person  u;
    Memo    memo;

    void set_print_address(Person& p, const char* address) {
           cout << "p.setAddress(" << (address? address : "") << ")" << endl;
           p.setAddress(address);
           p.println();
           cout << endl;
    }

    void changeAddress() { // Menu Item 1
           Person p("p", 1, 70, true, "Gwangju");
           set_print_address(p, "short address");
           set_print_address(p, "middle length Address, Seoul");
           set_print_address(p, "long length Address Seoul Mapo-gu Korea");
           set_print_address(p, u.getAddress());
    }

    void print_memo(Person& p) { // 객체 p의 메모 출력
           cout << "------ " << p.getName() << " memo ------" << endl;
           const char *pmemo = p.getMemo();
           cout << (pmemo? pmemo : ""); // 메모 출력; nullptr이면 "" 출력
           cout << "--------------------" << endl << endl;
    }

    void set_print_memo(Person& p, const char* memo) { // 객체 p에 메모 복사하고 출력
           cout << "p.setMemo(memo)" << endl;
           p.setMemo(memo);
           print_memo(p);
    }

    void changeMemo() { // Menu Item 2
           Person p("p", 1, 70, true, "Gwangju");
           set_print_memo(p, "short memo\n");
           set_print_memo(p, "middle long memo: The Last of the Mohicans\n");
           set_print_memo(p, u.getMemo());
    }

    void manageMemo() { // Menu item 3
            memo.c_str(u.getMemo());
            memo.run();
            cout << "\nmemo.run() returned" << endl;
            u.setMemo(memo.c_str());   // 메모 메뉴에서 새로 삽입한 메모를 u에 저장
            print_memo(u);
        }

    Person call_by_value_and_return_value(Person p) { // 복사생성자로 p 초기화
            cout << "p.setName(p)" << endl;
            p.setName("p");
            // 함수 리턴 전에 p를 p2에 복사해서 초기화(복사생성자)
            cout << "p2: ";
            return p; // 함수 리턴 시 p 소멸자 실행
        }

    void copyConstructor() { // Menu Item 4
            cout << "u: ";    u.println();
            print_memo(u);

            cout << "Person p1(u)" << endl;
            cout << "p1: ";
            Person p1(u); // 명시적 복사생성자로 p1 초기화
            p1.setName("p1");
            p1.println();
            print_memo(p1);

            cout << "Person p2 = call_by_value_and_return_value(p1)" << endl;
            // 묵시적으로 두번의 복사생성자 실행: 함수 인자를 넘길 때, 함수 리턴 값을 p2에 복사할 때
            cout << "p: ";
            Person p2 = call_by_value_and_return_value(p1);
            cout << "call_by_value_and_return_value(p1) returned\n" << endl;
            cout << "p2.setName(p2)" << endl;
            p2.setName("p2");
            p2.println();
            print_memo(p2);
            cout << "copyConstructor() returns" << endl;
            // 함수 리턴 시 지역객체 p1, p2 소멸자 실행됨
        }

    void nullptrMember() { // Menu Item 5
            u.println();
            print_memo(u);
            cout << "set address = memo_c_str = nullptr" << endl;
            u.setAddress(nullptr);
            u.setMemo(nullptr);
            u.println();
            print_memo(u);

            cout << "memo.c_str(u.getMemo())" << endl;
            memo.c_str(u.getMemo()); // u.getMemo() == nullptr이므로 memo의 mStr은 ""이다.
            memo.displayMemo();

            cout << endl << "u.setMemo(memo.c_str())" << endl;
            u.setMemo(memo.c_str()); // memo.get_c_str() == nullptr 이므로
                                          // u의 memo_c_str은 nullptr로 설정
            print_memo(u);
        }

    void inputPerson() { // Menu item 6
            cout << "u: "; u.println();
            while (!UI::inputPerson(&u)) ;  // USER 11 88 false :DONG-GU, DAEGU:
            cout << "u: "; u.println();
        }

public:
    AllocatedMember():
        u("u", 1, 70, true, "NAMDAEMUN-RO 123, JONGNO-GU, SEOUL, KOREA") {
        u.setMemo("It is believed that the Aborigines of the American continent\n");
    }

    void run() {
        using AM = AllocatedMember;

        // TODO 문제 [1]: func_t, func_arr[], menuCount 선언
    	using func_t = void (AM::*)();
    	    	    func_t func_arr[] = {
    	    	        nullptr, &AM::changeAddress, &AM::changeMemo, &AM::manageMemo,
						&AM::copyConstructor, &AM::nullptrMember, &AM::inputPerson,
    	    	    };
    	    	    int menuCount = sizeof(func_arr) / sizeof(func_arr[0]); // func_arr[] 길이


        string menuStr =
            "++++++++++++++++ Allocated Member Menu ++++++++++++++++\n"
            "+ 0.Exit 1.ChangeAddress 2.ChangeMemo 3.UsingMemoMenu +\n"
            "+ 4.CopyConstructor 5.NullptrMember 6.inputPerson     +\n"
            "+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";

        // TODO 문제 [1]: while 문장 삽입하여 선택된 메뉴항목 실행하는 함수 호출
        while (true) {
                        int menuItem = UI::selectMenu(menuStr, menuCount);
                        if (menuItem == 0) return;
                        (this->*func_arr[menuItem])();
                    }

  }
}; // ch5_2: AllocatedMember


/******************************************************************************
 * Main Menu
 ******************************************************************************/

class MainMenu
{
public:
    void run() {
        int menuCount = 5; // 상수 정의
        string menuStr =
        		"******************************* Main Menu *********************************\n"
        		"* 0.Exit 1.PersonManager(ch3_2, 4, 6)                                     *\n"
        		"* 2.Class:Object(ch3_1) 3.CopyConstructor(ch5_1) 4.AllocatedMember(ch5_2) *\n"
        		"***************************************************************************\n";

        while (true) {
            int menuItem = UI::selectMenu(menuStr, menuCount);
            if (menuItem == 0) break;

            switch(menuItem) {
            case 1: MultiManager().run();		break;
            case 2: ClassAndObject().run();		break;
            case 3: CopyConstructor().run();	break;
            case 4: AllocatedMember().run(); 	break;
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




