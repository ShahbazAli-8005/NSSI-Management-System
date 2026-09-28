#include <iostream>
#include <windows.h>
#include <conio.h>
#include <math.h>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <ctime>
#include <stdlib.h>
using namespace std;

void gotoRowCol(int r, int c)
{
    COORD coord;
    coord.X = c;
    coord.Y = r;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void sleep(int m)
{
    for (int i = 0; i < m * 500000; i++)
    {
    }
}

class Validate
{
protected:
    bool validateString(string str)
    {
        for (int i = 0; i < str.length(); i++)
        {
            if (isdigit(str[i]))
                return false;
        }
        return true;
    }

    bool validateNumber(string num)
    {
        for (int i = 0; i < num.length(); i++)
        {
            if (isalpha(num[i]))
                return false;
        }
        return true;
    }

    bool limitNumber(string input, int n)
    {
        return input.length() == n;
    }
};

class person
{
public:
    string name;
    string contactNumber;
    string address;
    string regNo;

    void setName(string n)
    {
        name = n;
    }

    void setContact(string n)
    {
        contactNumber = n;
    }

    void setAddress(string a)
    {
        address = a;
    }

    void getName()
    {
        cout << "Name            |  " << name << endl;
    }
    void getContact()
    {
        cout << "Contact Number  |  " << contactNumber << endl;
    }
    void getAddress()
    {
        cout << "Address         |  " << address << endl;
    }

    void SetRegNo(string roll)
    {
        regNo = roll;
    }
    void getRegNo()
    {
        cout << "Registration No |  " << regNo << endl;
    }

    virtual void saveData(ofstream &outFile) const
    {
        outFile << regNo << ","
                << name << ","
                << contactNumber << ","
                << address << endl;
    }

    virtual void loadData(ifstream &inFile)
    {
        string line;
        cin.ignore();
        if (getline(inFile, line))
        {
            stringstream ss(line);
            string segment;

            getline(ss, segment, ',');
            regNo = segment;
            getline(ss, segment, ',');
            name = segment;
            getline(ss, segment, ',');
            contactNumber = segment;
            getline(ss, segment);
            address = segment;
        }
    }
};

class EducationWingPerson : public person, public Validate
{
public:
    EducationWingPerson() {}

    void EnterData()
    {
        string userInput;
        bool loopBreak = false;
        cin.ignore();
        while (!loopBreak)
        {
            cout << "Enter Registration No: ";
            getline(cin, userInput);
            if (validateNumber(userInput))
            {
                SetRegNo(userInput);
                loopBreak = true;
            }
            else
            {
                cout << "Wrong Input, Enter Again...." << endl;
            }
        }
        loopBreak = false;
        while (!loopBreak)
        {
            cout << "Enter Name: ";
            getline(cin, userInput);
            if (validateString(userInput))
            {
                setName(userInput);
                loopBreak = true;
            }
            else
            {
                cout << "Wrong Input, Enter Again...." << endl;
            }
        }
        loopBreak = false;
        while (!loopBreak)
        {
            cout << "Enter Contact Number(03XXXXXXXXX): ";
            getline(cin, userInput);
            if (validateNumber(userInput) && limitNumber(userInput, 11))
            {
                setContact(userInput);
                loopBreak = true;
            }
            else
            {
                cout << "Wrong Input, Enter Again...." << endl;
            }
        }
        cout << "Enter Address: ";
        getline(cin, userInput);
        setAddress(userInput);
    }

    void displayData()
    {
        getName();
        getRegNo();
        getContact();
        getAddress();
    }

    void saveData(ofstream &outFile) const override
    {
        person::saveData(outFile);
    }

    void loadData(ifstream &inFile) override
    {
        person::loadData(inFile);
    }
    ~EducationWingPerson() {}
};

class studentDynamic
{
public:
    vector<EducationWingPerson> cluster;
    int indx;

    studentDynamic()
    {
        indx = 0;
        loadFromFile("students.csv");
    }

    void setValues()
    {
        cout << "--------------------------------------------------" << endl;
        cout << endl
             << "========= Enter Data =========" << endl;
        cluster.emplace_back();
        cout << "====== Data of " << indx + 1 << " ======" << endl;
        cluster[indx].EnterData();
        indx++;
        saveToFile("students.csv");
        cout << endl;
    }

    void printData()
    {
        cout << "--------------------------------------------------" << endl;
        cout << endl
             << "========= Data Displayed =========" << endl;
        if (indx == 0)
        {
            cout << "-----------------------------" << endl;
            cout << "No student data to display." << endl;
            cout << "-----------------------------" << endl;
            return;
        }
        for (int i = 0; i < indx; i++)
        {
            cout << "======= Data of " << i + 1 << " ======" << endl;
            cluster[i].displayData();
            cout << endl;
        }
    }

    void saveToFile(const string &filename) const
    {
        ofstream outFile(filename);
        if (outFile.is_open())
        {
            outFile << indx << endl;
            for (int i = 0; i < indx; i++)
            {
                cluster[i].saveData(outFile);
            }
            outFile.close();
        }
        else
        {
            cout << "Unable to open file for saving: " << filename << endl;
        }
    }

    void loadFromFile(const string &filename)
    {
        ifstream inFile(filename);
        if (inFile.is_open())
        {
            string firstLine;
            if (getline(inFile, firstLine))
            {
                try
                {
                    indx = stoi(firstLine);
                }
                catch (const std::invalid_argument &e)
                {
                    cout << "Error: Invalid data in " << filename << ". Expected integer for count, got: '" << firstLine << "'. " << e.what() << endl;
                    indx = 0;
                }
                catch (const std::out_of_range &e)
                {
                    cout << "Error: Count out of range in " << filename << ". " << e.what() << endl;
                    indx = 0;
                }
            }
            else
            {
                indx = 0;
            }

            cluster.clear();
            cluster.resize(indx);

            for (int i = 0; i < indx; i++)
            {
                cluster[i].loadData(inFile);
            }
            inFile.close();
        }
        else
        {
            indx = 0;
        }
    }

    ~studentDynamic() {}
};

class Subject
{
public:
    string subject[5] = {"Urdu", "English", "Math", "Science", "Skills"};
    string day[5] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};
};

class Team : public EducationWingPerson, public Subject
{
    string role;
    int subjectIndx;

public:
    Team() : subjectIndx(-1) {}

    void setTeam()
    {
        EnterData();
        cout << "Enter Role of Team Member: ";
        getline(cin, role);
    }
    void displayTeam()
    {
        displayData();
        cout << "Role:           |  " << role << endl;
        if (subjectIndx != -1)
        {
            displayDuties();
        }
        else
        {
            cout << "--------------------------------------" << endl;
            cout << name << " has no assigned duties yet." << endl;
            cout << "--------------------------------------" << endl;
        }
    }
    void AssignDutyToMentors()
    {
        int n;
        cout << "--------------------------------------------------" << endl;
        cout << "You are Assigning Duty to " << name << " " << regNo << endl;
        for (int i = 0; i < 5; i++)
        {
            cout << i << ": " << subject[i] << endl;
        }
        while (true)
        {
            cout << "Enter Choice: ";
            cin >> n;
            if (n >= 0 && n < 5)
            {
                subjectIndx = n;
                return;
            }
            else
            {
                cout << "Wrong input....." << endl;
            }
        }
    }
    void displayDuties()
    {
        if (subjectIndx != -1)
        {
            cout << name << " Id: " << regNo << " is conducting " << subject[subjectIndx] << " on " << day[subjectIndx] << endl;
        }
        else
        {
            cout << "--------------------------------------" << endl;
            cout << name << " has no assigned duties yet." << endl;
            cout << "--------------------------------------" << endl;
        }
    }

    void saveData(ofstream &outFile) const override
    {
        outFile << regNo << ","
                << name << ","
                << contactNumber << ","
                << address << ","
                << role << ","
                << subjectIndx << endl;
    }

    void loadData(ifstream &inFile) override
    {
        string line;
        if (getline(inFile, line))
        {
            stringstream ss(line);
            string segment;

            getline(ss, segment, ',');
            regNo = segment;
            getline(ss, segment, ',');
            name = segment;
            getline(ss, segment, ',');
            contactNumber = segment;
            getline(ss, segment, ',');
            address = segment;
            getline(ss, segment, ',');
            role = segment;

            getline(ss, segment);
            try
            {
                subjectIndx = stoi(segment);
            }
            catch (const std::invalid_argument &e)
            {
                cout << "Error converting subject Indx to int: " << e.what() << " in line: " << line << endl;
                subjectIndx = -1;
            }
            catch (const std::out_of_range &e)
            {
                cout << "Subject Indx out of range: " << e.what() << " in line: " << line << endl;
                subjectIndx = -1;
            }
        }
    }
};

class teamDynamic
{
public:
    vector<Team> team;
    int indx;
    teamDynamic()
    {
        indx = 0;
        loadFromFile("team.csv");
    }
    void setValues()
    {
        cout << "--------------------------------------------------" << endl;
        cout << endl
             << "=========== Enter Data =========" << endl;
        team.emplace_back();
        cout << "===== Data of " << indx + 1 << " =====" << endl;
        team[indx].setTeam();
        indx++;
        saveToFile("team.csv");
        cout << endl;
    }

    void printData()
    {
        cout << "--------------------------------------------------" << endl;
        cout << endl
             << "========= Data Displayed ========" << endl;
        if (indx == 0)
        {
            cout << "----------------------------" << endl;
            cout << "No team data to display." << endl;
            cout << "----------------------------" << endl;
            return;
        }
        for (int i = 0; i < indx; i++)
        {
            cout << "====== Data of " << i + 1 << " ======" << endl;
            team[i].displayTeam();
            cout << endl;
        }
    }

    void saveToFile(const string &filename) const
    {
        ofstream outFile(filename);
        if (outFile.is_open())
        {
            outFile << indx << endl;
            for (int i = 0; i < indx; i++)
            {
                team[i].saveData(outFile);
            }
            outFile.close();
        }
        else
        {
            cout << "Unable to open file for saving: " << filename << endl;
        }
    }

    void loadFromFile(const string &filename)
    {
        ifstream inFile(filename);
        if (inFile.is_open())
        {
            string firstLine;
            if (getline(inFile, firstLine))
            {
                try
                {
                    indx = stoi(firstLine);
                }
                catch (const std::invalid_argument &e)
                {
                    cout << "Error: Invalid data in " << filename << ". Expected integer for count, got: '" << firstLine << "'. " << e.what() << endl;
                    indx = 0;
                }
                catch (const std::out_of_range &e)
                {
                    cout << "Error: Count out of range in " << filename << ". " << e.what() << endl;
                    indx = 0;
                }
            }
            else
            {
                indx = 0;
            }

            team.clear();
            team.resize(indx);

            for (int i = 0; i < indx; i++)
            {
                team[i].loadData(inFile);
            }
            inFile.close();
        }
        else
        {
            indx = 0;
        }
    }

    ~teamDynamic() {}
};

class DataBase
{
public:
    studentDynamic studentData;
    teamDynamic teamData;
    void executeProgram()
    {
        char ch;
        while (true)
        {
            cout << "--------------------------------------------------" << endl;
            cout << "Enter Choice: " << endl;
            cout << "1. Add Student \n2. Display Student \n3. Add Team \n4. Display Team \n5. Search \n6. Back.......: ";
            cin >> ch;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (ch)
            {
            case '1':
                studentData.setValues();
                break;
            case '2':
                studentData.printData();
                break;
            case '3':
                teamData.setValues();
                break;
            case '4':
                teamData.printData();
                break;
            case '5':
                search();
                break;
            case '6':
                return;
            default:
                cout << "Invalid Choice: " << endl;
                break;
            }
        }
    }
    void search() {
        char ch;
        string regNo;
        cout << "Enter your Choice: " << endl;
        cout << "1.Search Student \n2.Search Team: ";
        cin >> ch;
        
        cout << "Enter Reg NO: ";
        cin.ignore();
        getline(cin, regNo);

        if (ch == '1') {
            bool found = false;
            for (int i = 0; i < studentData.indx; i++) {
                if (studentData.cluster[i].regNo == regNo) {
                    studentData.cluster[i].displayData();
                    found = true;
                    break;
                }
            }
            if (!found) {
                cout << "Student with Registration No. " << regNo << " not found." << endl;
            }
        }
        else if (ch == '2') {
            bool found = false;
            for (int i = 0; i < teamData.indx; i++) {
                if (teamData.team[i].regNo == regNo) {
                    teamData.team[i].displayTeam();
                    found = true;
                    break;
                }
            }
            if (!found) {
                cout << "Team member with Registration No. " << regNo << " not found." << endl;
            }
        }
        else {
            cout << "Invalid search choice." << endl;
        }
    }
    ~DataBase() {}
};

class Attendence
{
    int capacity = 2;
    int indx = 0;

public:
    char *attendence;
    Attendence()
    {
        attendence = new char[capacity];
    }

    Attendence(const Attendence &other) : capacity(other.capacity), indx(other.indx)
    {
        attendence = new char[capacity];
        for (int i = 0; i < indx; ++i)
        {
            attendence[i] = other.attendence[i];
        }
    }

    Attendence &operator=(const Attendence &other)
    {
        if (this != &other)
        {
            delete[] attendence;
            capacity = other.capacity;
            indx = other.indx;
            attendence = new char[capacity];
            for (int i = 0; i < indx; ++i)
            {
                attendence[i] = other.attendence[i];
            }
        }
        return *this;
    }

    void setAttendence(char p)
    {
        if (indx >= capacity)
        {
            int newCapacity = capacity * 2;
            char *newArray = new char[newCapacity];
            for (int i = 0; i < indx; i++)
            {
                newArray[i] = attendence[i];
            }
            delete[] attendence;
            attendence = newArray;
            capacity = newCapacity;
        }
        attendence[indx] = p;
        indx++;
    }

    int getIndx() const
    {
        return indx;
    }

    void saveAttendence(ofstream &outFile) const
    {
        for (int i = 0; i < indx; ++i)
        {
            outFile << attendence[i];
            if (i < indx - 1)
            {
                outFile << ",";
            }
        }
        outFile << endl;
    }

    void loadAttendence(ifstream &inFile)
    {
        string line;
        if (getline(inFile, line))
        {
            stringstream ss(line);
            string segment;
            indx = 0;

            if (attendence)
            {
                delete[] attendence;
                attendence = nullptr;
            }
            capacity = 2;
            attendence = new char[capacity];

            while (getline(ss, segment, ','))
            {
                if (segment.length() > 0)
                {
                    setAttendence(segment[0]);
                }
            }
        }
    }

    ~Attendence()
    {
        delete[] attendence;
    }
};

class dailyAttendence
{
    Attendence *dailyPresence;
    string *date;
    int capacity = 2;
    int indx = 0;

public:
    dailyAttendence()
    {
        dailyPresence = new Attendence[capacity];
        date = new string[capacity];
        loadFromFile("attendance.csv");
    }

    void markAttendence(const DataBase &B)
    {
        if (B.studentData.indx == 0)
        {
            cout << "-------------------------------------------" << endl;
            cout << "No students available to mark attendance." << endl;
            cout << "-------------------------------------------" << endl;
            return;
        }

        string dat;
        cout << "--------------------------------------------------" << endl;
        cout << "Enter date (dd/mm/yyyy): ";
        cin.ignore();
        getline(cin, dat);

        if (indx >= capacity)
        {
            int newCapacity = capacity * 2;
            string *newDate = new string[newCapacity];
            Attendence *newArray = new Attendence[newCapacity];
            for (int i = 0; i < indx; i++)
            {
                newDate[i] = date[i];
                newArray[i] = dailyPresence[i];
            }
            delete[] dailyPresence;
            delete[] date;
            dailyPresence = newArray;
            date = newDate;
            capacity = newCapacity;
        }

        Attendence currentDayAttendance;
        for (int i = 0; i < B.studentData.indx; i++)
        {
            char attendenceChar;
            cout << B.studentData.cluster[i].regNo << ": " << B.studentData.cluster[i].name << " (P/A): ";
            cin >> attendenceChar;
            currentDayAttendance.setAttendence(attendenceChar);
        }

        date[indx] = dat;
        dailyPresence[indx] = currentDayAttendance;
        indx++;
        saveToFile("attendance.csv");
    }

    void displayAttendence(const DataBase &B)
    {
        if (indx == 0)
        {
            cout << "-----------------------------------" << endl;
            cout << "No attendance records to display." << endl;
            cout << "-----------------------------------" << endl;
            return;
        }
        for (int j = 0; j < indx; j++)
        {
            cout << "--------------------------------------------------" << endl;
            cout << "========== Attendence of " << date[j] << " =======" << endl;
            for (int i = 0; i < B.studentData.indx; i++)
            {
                if (i < dailyPresence[j].getIndx())
                {
                    cout << B.studentData.cluster[i].regNo << ": " << B.studentData.cluster[i].name << ": " << dailyPresence[j].attendence[i] << endl;
                }
                else
                {
                    cout << B.studentData.cluster[i].regNo << ": " << B.studentData.cluster[i].name << ": N/A" << endl;
                }
            }
        }
    }

    void saveToFile(const string &filename) const
    {
        ofstream outFile(filename);
        if (outFile.is_open())
        {
            outFile << indx << endl;
            for (int i = 0; i < indx; ++i)
            {
                outFile << date[i] << endl;
                dailyPresence[i].saveAttendence(outFile);
            }
            outFile.close();
        }
        else
        {
            cout << "Unable to open file for saving: " << filename << endl;
        }
    }

    void loadFromFile(const string &filename)
    {
        ifstream inFile(filename);
        if (inFile.is_open())
        {
            string firstLine;
            if (getline(inFile, firstLine))
            {
                try
                {
                    indx = stoi(firstLine);
                }
                catch (const std::invalid_argument &e)
                {
                    cout << "Error: Invalid data in " << filename << ". Expected integer for count, got: '" << firstLine << "'. " << e.what() << endl;
                    indx = 0;
                }
                catch (const std::out_of_range &e)
                {
                    cout << "Error: Count out of range in " << filename << ". " << e.what() << endl;
                    indx = 0;
                }
            }
            else
            {
                indx = 0;
            }

            if (indx > capacity)
            {
                delete[] dailyPresence;
                delete[] date;
                capacity = indx;
                dailyPresence = new Attendence[capacity];
                date = new string[capacity];
            }
            else if (indx == 0)
            {
                delete[] dailyPresence;
                delete[] date;
                capacity = 2;
                dailyPresence = new Attendence[capacity];
                date = new string[capacity];
            }

            for (int i = 0; i < indx; ++i)
            {
                getline(inFile, date[i]);
                dailyPresence[i].loadAttendence(inFile);
            }
            inFile.close();
        }
        else
        {
            indx = 0;
        }
    }

    ~dailyAttendence()
    {
        delete[] dailyPresence;
        delete[] date;
    }
};

class mainClass
{
public:
    DataBase database;
    dailyAttendence daily_attendence;

    void manageAttendece()
    {
        char ch;
        while (true)
        {
            cout << "--------------------------------------------------" << endl;
            cout << "Enter Choice: " << endl;
            cout << "1.Mark Attendence \n2.Show Attendence \n3.Back......";
            cin >> ch;

            if (ch == '1')
            {
                daily_attendence.markAttendence(database);
                cout << "Attendence is Marked" << endl;
            }
            else if (ch == '2')
            {
                daily_attendence.displayAttendence(database);
                cout << endl;
            }
            else if (ch == '3')
            {
                return;
            }
            else
            {
                cout << "Invalid Choice: " << endl;
            }
        }
    }
    void manageDuties()
    {
        char ch;
        cout << endl;
        if (database.teamData.indx != 0)
        {
            while (true)
            {
                cout << "--------------------------------------------------" << endl;
                cout << "Enter Choice: " << endl;
                cout << "1.Assign Duties \n2.Display Duties \n3.Back....";
                cin >> ch;

                if (ch == '1')
                {
                    for (int i = 0; i < database.teamData.indx; i++)
                    {
                        database.teamData.team[i].AssignDutyToMentors();
                    }
                    database.teamData.saveToFile("team.csv");
                }
                else if (ch == '2')
                {
                    cout << "--------------------------------------------------" << endl;
                    cout << "====== Display Duty =====" << endl;
                    for (int i = 0; i < database.teamData.indx; i++)
                    {
                        database.teamData.team[i].displayDuties();
                    }
                }
                else if (ch == '3')
                {
                    return;
                }
                else
                {
                    cout << "Wrong Input......." << endl;
                }
            }
        }
        else
        {
            cout << "First Enter Team Data: " << endl;
        }
        cout << endl;
    }
};
void Heading(string heading, int time)
{
    system("cls");
    cout << "\n\n\n\t\t\t\t";
    for (int i = 0; i < heading.length(); i++)
    {
        cout << heading[i];
        sleep(time);
    }
}

void EducationWingManager()
{
    mainClass M;
    char ch;
    cout << "======== Main Menu =======" << endl;
    while (true)
    {
        cout << "Enter Your Choice" << endl;
        cout << "1. Manage Data \n2. Manage Attendence \n3. Manage Duties and Shedule \n4. Exit: ";
        cin >> ch;

        cout << endl;

        if (ch == '1')
        {
            M.database.executeProgram();
        }
        else if (ch == '2')
        {
            if (M.database.studentData.indx == 0)
            {
                cout << "------------------------------------------" << endl;
                cout << "You have no students to mark attendance." << endl;
                cout << "------------------------------------------" << endl;
            }
            else
            {
                M.manageAttendece();
            }
        }
        else if (ch == '3')
        {
            M.manageDuties();
        }
        else if (ch == '4')
        {
            return;
        }
        else
        {
            cout << "Wrong input........." << endl;
        }
        cout << endl;
    }
}

// 2nd Person
class FundManagement
{
public:
    static double central_fund;

    struct FundAllocation
    {
        string projectName;
        double allocatedAmount;
        string allocationDate;
    };

    FundAllocation *fund_allocation_records = nullptr;
    int fund_record_count = 0;
    int fund_record_capacity = 0;

    FundManagement() : fund_allocation_records(nullptr), fund_record_count(0), fund_record_capacity(0) {}

    void initialize_fund_records()
    {
        if (fund_allocation_records == nullptr)
        {
            fund_record_capacity = 5;
            fund_allocation_records = new FundAllocation[fund_record_capacity];
        }
    }

    void cleanup_fund_records()
    {
        if (fund_allocation_records != nullptr)
        {
            delete[] fund_allocation_records;
            fund_allocation_records = nullptr;
        }
        fund_record_count = 0;
        fund_record_capacity = 0;
    }

    void setFund(double initial)
    {
        central_fund = initial;
    }

    void getFund()
    {
        cout << "Central Fund: " << central_fund << endl;
    }

    void AddFund(double amount)
    {
        central_fund += amount;
        cout << "Amount added to Central Fund Successfully." << endl;
        saveFundDataToFile(); // Save after adding funds
    }

    void AddFundSilent(double amount)
    {
        central_fund += amount;
        saveFundDataToFile(); // Save after silent addition
    }

    bool AllocateFund(double amount, string &projectName, string &allocationDate)
    {
        if (amount > central_fund)
            return false;
        central_fund -= amount;

        if (fund_record_count >= fund_record_capacity)
        {
            fund_record_capacity += 5;
            FundAllocation *newRecords = new FundAllocation[fund_record_capacity];
            for (int i = 0; i < fund_record_count; ++i)
            {
                newRecords[i] = fund_allocation_records[i];
            }
            delete[] fund_allocation_records;
            fund_allocation_records = newRecords;
        }
        fund_allocation_records[fund_record_count].projectName = projectName;
        fund_allocation_records[fund_record_count].allocatedAmount = amount;
        fund_allocation_records[fund_record_count].allocationDate = allocationDate;
        fund_record_count++;
        saveFundDataToFile(); // Save after allocation
        return true;
    }

    void ViewFundAllocationRecords()
    {
        if (fund_record_count == 0)
        {
            cout << "\nNo fund allocation records found." << endl;
            return;
        }

        cout << "\n===== FUND ALLOCATION DETAILS =====" << endl;
        cout << "Total Records: " << fund_record_count << endl;
        cout << "===================================" << endl;

        double totalAllocated = 0;
        for (int i = 0; i < fund_record_count; ++i)
        {
            cout << "Record: #" << i + 1 << endl;
            cout << "Project Name: " << fund_allocation_records[i].projectName << endl;
            cout << "Allocated Amount: Rs." << fund_allocation_records[i].allocatedAmount << endl;
            cout << "Allocation Date: " << fund_allocation_records[i].allocationDate << endl;
            cout << "===================================" << endl;
            totalAllocated += fund_allocation_records[i].allocatedAmount;
        }

        cout << "\nSUMMARY:" << endl;
        cout << "Total Allocated: Rs." << totalAllocated << endl;
        cout << "Current Central Fund: Rs." << central_fund << endl;
        cout << "===================================" << endl;
    }

    // CSV File Handling for Fund Management
    void saveFundDataToFile()
    {
        ofstream file("funds.csv");
        if (!file.is_open())
        {
            cout << "Error: Could not open funds.csv for writing." << endl;
            return;
        }

        // Write central fund
        file << "Central Fund," << central_fund << endl;

        // Write fund allocation records header
        file << "Project Name,Allocated Amount,Allocation Date" << endl;

        // Write fund allocation records
        for (int i = 0; i < fund_record_count; ++i)
        {
            file << fund_allocation_records[i].projectName << ","
                 << fund_allocation_records[i].allocatedAmount << ","
                 << fund_allocation_records[i].allocationDate << endl;
        }

        file.close();
    }

    void loadFundDataFromFile()
    {
        ifstream file("funds.csv");
        if (!file.is_open())
        {
            // File doesn't exist, use default values
            central_fund = 500000;
            return;
        }

        string line;

        // Read central fund
        if (getline(file, line))
        {
            size_t commaPos = line.find(',');
            if (commaPos != string::npos)
            {
                central_fund = stod(line.substr(commaPos + 1));
            }
        }

        // Skip header line for allocation records
        if (getline(file, line))
        {
        } // Skip header

        // Clear existing records
        fund_record_count = 0;

        // Read allocation records
        while (getline(file, line))
        {
            if (line.empty())
                continue;

            if (fund_record_count >= fund_record_capacity)
            {
                fund_record_capacity += 5;
                FundAllocation *newRecords = new FundAllocation[fund_record_capacity];
                for (int i = 0; i < fund_record_count; ++i)
                {
                    newRecords[i] = fund_allocation_records[i];
                }
                delete[] fund_allocation_records;
                fund_allocation_records = newRecords;
            }

            stringstream ss(line);
            string projectName, amountStr, date;

            getline(ss, projectName, ',');
            getline(ss, amountStr, ',');
            getline(ss, date, ',');

            fund_allocation_records[fund_record_count].projectName = projectName;
            fund_allocation_records[fund_record_count].allocatedAmount = stod(amountStr);
            fund_allocation_records[fund_record_count].allocationDate = date;
            fund_record_count++;
        }

        file.close();
    }
};

double FundManagement::central_fund = 0;

class Verify
{
protected:
    bool verifyString(string st)
    {
        if (st.empty())
            return false;
        for (char c : st)
            if (isdigit(c))
                return false;
        return true;
    }

    bool verifyNumber(string num)
    {
        if (num.empty())
            return false;
        for (char c : num)
            if (isalpha(c))
                return false;
        return true;
    }

    bool isValidDouble(string s)
    {
        if (s.empty())
            return false;
        int dotCount = 0;
        for (char c : s)
        {
            if (c == '.')
            {
                if (++dotCount > 1)
                    return false;
            }
            else if (!isdigit(c))
                return false;
        }
        return true;
    }

    bool isValidInt(string s)
    {
        if (s.empty())
            return false;
        for (char c : s)
            if (!isdigit(c))
                return false;
        return true;
    }
};

class MemberData
{
public:
    string name;
    string role;
    string phNo;

    void setName(string n)
    {
        name = n;
    }
    void setRole(string r)
    {
        role = r;
    }
    void setPhone(string p)
    {
        phNo = p;
    }
    void getName()
    {
        cout << "Name: " << name << endl;
    }
    void getRole()
    {
        cout << "Role: " << role << endl;
    }
    void getPhone()
    {
        cout << "Phone No: " << phNo << endl;
    }
};

class TeamMember : public MemberData, public Verify
{
public:
    TeamMember() {}

    void EnterData()
    {
        string input;
        cout << "Enter Member Name: ";
        getline(cin, input);
        while (!verifyString(input))
        {
            cout << "Wrong Entry, Try Again!" << endl;
            getline(cin, input);
        }
        setName(input);

        cout << "Enter Member Role: ";
        getline(cin, input);
        while (!verifyString(input))
        {
            cout << "Wrong Entry, Try Again!" << endl;
            getline(cin, input);
        }
        setRole(input);

        cout << "Enter Member Phone No(03XXXXXXXXX): ";
        getline(cin, input);
        while (!verifyNumber(input) || input.length() != 11)
        {
            cout << "Wrong Entry, Try Again!" << endl;
            getline(cin, input);
        }
        setPhone(input);
    }

    void display()
    {
        getName();
        getRole();
        getPhone();
    }
};

class MemberDynamic
{
public:
    TeamMember *member;
    int memberCount;
    int capacity;

    MemberDynamic() : memberCount(0), capacity(5)
    {
        member = new TeamMember[capacity];
        loadMembersFromFile(); // Load members on initialization
    }

    ~MemberDynamic()
    {
        delete[] member;
    }

    void AddMember()
    {
        if (memberCount >= capacity)
        {
            capacity += 5;
            TeamMember *newMember = new TeamMember[capacity];
            for (int i = 0; i < memberCount; i++)
            {
                newMember[i] = member[i];
            }
            delete[] member;
            member = newMember;
        }
        member[memberCount].EnterData();
        memberCount++;
        saveMembersToFile(); // Save after adding member
    }

    void display()
    {
        cout << "\n---- Team Members ----" << endl;
        for (int i = 0; i < memberCount; i++)
        {
            cout << "Member " << i + 1 << ":" << endl;
            member[i].display();
            cout << endl;
        }
        cout << "\n===================================" << endl;
    }

    void ModifyMemberData()
    {
        if (memberCount == 0)
        {
            cout << "No members to modify." << endl;
            return;
        }
        string searchName;
        cout << "Enter the name of the member to modify: ";
        getline(cin, searchName);

        bool found = false;
        for (int i = 0; i < memberCount; ++i)
        {
            if (member[i].name == searchName)
            {
                cout << "Member found. Enter new data for " << member[i].name << ":" << endl;
                member[i].EnterData();
                cout << "Member data modified successfully!" << endl;
                cout << "\n===================================" << endl;
                saveMembersToFile(); // Save after modification
                found = true;
                break;
            }
        }
        if (!found)
        {
            cout << "Member with name '" << searchName << "' not found." << endl;
        }
    }

    void SearchMemberByName()
    {
        if (memberCount == 0)
        {
            cout << "No members to search." << endl;
            return;
        }
        string searchName;
        cout << "Enter the name of the member to search: ";
        getline(cin, searchName);

        bool found = false;
        for (int i = 0; i < memberCount; ++i)
        {
            if (member[i].name == searchName)
            {
                cout << "\n----- Member Found -----" << endl;
                member[i].display();
                cout << "\n===================================" << endl;
                found = true;
                break;
            }
        }
        if (!found)
        {
            cout << "Member with name '" << searchName << "' not found." << endl;
        }
    }

    // CSV File Handling for Members
    void saveMembersToFile()
    {
        ofstream file("members.csv");
        if (!file.is_open())
        {
            cout << "Error: Could not open members.csv for writing." << endl;
            return;
        }

        // Write CSV header
        file << "Name,Role,Phone Number" << endl;

        // Write member data
        for (int i = 0; i < memberCount; ++i)
        {
            file << member[i].name << ","
                 << member[i].role << ","
                 << member[i].phNo << endl;
        }

        file.close();
    }

    void loadMembersFromFile()
    {
        ifstream file("members.csv");
        if (!file.is_open())
        {
            // File doesn't exist, start with empty member list
            return;
        }

        string line;

        // Skip header line
        if (getline(file, line))
        {
        } // Skip header

        // Clear existing members
        memberCount = 0;

        // Read member data
        while (getline(file, line))
        {
            if (line.empty())
                continue;

            if (memberCount >= capacity)
            {
                capacity += 5;
                TeamMember *newMember = new TeamMember[capacity];
                for (int i = 0; i < memberCount; i++)
                {
                    newMember[i] = member[i];
                }
                delete[] member;
                member = newMember;
            }

            stringstream ss(line);
            string name, role, phone;

            getline(ss, name, ',');
            getline(ss, role, ',');
            getline(ss, phone, ',');

            member[memberCount].setName(name);
            member[memberCount].setRole(role);
            member[memberCount].setPhone(phone);
            memberCount++;
        }

        file.close();
    }
};

class ProjectsData
{
public:
    string projectName, location, date;
    double allocateFund = 0;
    int peopleServed = 0;
    double totalExpenses = 0;
    string projectType;

    void setProjectName(string n)
    {
        projectName = n;
    }
    void setLocation(string l)
    {
        location = l;
    }
    void setDate(string d)
    {
        date = d;
    }
    void setAllocateFund(double amount)
    {
        allocateFund = amount;
    }
    void setPeopleServed(int people)
    {
        peopleServed = people;
    }
    void setExpenses(double expenses)
    {
        totalExpenses = expenses;
    }
    void setProjectType(string type)
    {
        projectType = type;
    }

    void getProjectName()
    {
        cout << "Project Name: " << projectName << endl;
    }
    void getLocation()
    {
        cout << "Project Location: " << location << endl;
    }
    void getDate()
    {
        cout << "Project Date: " << date << endl;
    }
    void getAllocatefund()
    {
        cout << "Project Allocated Fund Amount: " << allocateFund << endl;
    }
    void getPeopleServed()
    {
        cout << "People Served: " << peopleServed << endl;
    }
    void getExpenses()
    {
        cout << "Project Total Expenses: " << totalExpenses << endl;
    }
    void getProjectType()
    {
        cout << "Project Type: " << projectType << endl;
    }

    void Expenses(double amount)
    {
        if (amount > allocateFund - totalExpenses)
        {
            cout << "Insufficient Fund to Allocate for Project Expenses." << endl;
        }
        else
        {
            totalExpenses += amount;
            cout << "Spent Rs." << amount << " on " << projectName << endl;
            cout << "Remaining Budget: Rs." << (allocateFund - totalExpenses) << endl;
        }
    }
};

class WelfareProjects : public ProjectsData, public Verify
{
    FundManagement *fm;

public:
    WelfareProjects() : fm(nullptr) {}

    WelfareProjects(FundManagement &fundManager) : fm(&fundManager) {}

    double getValidatedFundInput(string prompt)
    {
        string input;
        double fund;
        cout << prompt;
        getline(cin, input);
        while (!isValidDouble(input))
        {
            cout << "Wrong Entry, Try Again!" << endl;
            cout << prompt;
            getline(cin, input);
        }
        fund = stod(input);
        return fund;
    }

    void EnterData()
    {
        string input;
        cout << "Enter Project Type (DisasterRelief/FoodDrive/HealthCamp): ";
        getline(cin, input);
        setProjectType(input);

        cout << "Enter Project Name: ";
        getline(cin, input);
        while (!verifyString(input))
        {
            cout << "Wrong Entry, Try Again!" << endl;
            getline(cin, input);
        }
        setProjectName(input);

        cout << "Enter Project Date (DD/MM/YYYY): ";
        getline(cin, input);
        while (!verifyNumber(input))
        {
            cout << "Wrong Entry, Try Again!" << endl;
            getline(cin, input);
        }
        setDate(input);

        cout << "Enter Project Location: ";
        getline(cin, input);
        while (!verifyString(input))
        {
            cout << "Wrong Entry, Try Again!" << endl;
            getline(cin, input);
        }
        setLocation(input);

        double fund;
        bool allocated = false;
        while (!allocated)
        {
            if (fm->central_fund == 0)
            {
                cout << "\nCentral fund is ZERO. You must add more funds to continue." << endl;
                cout << "1. Add Funds\n2. Exit Project Entry\nChoose: ";
                getline(cin, input);

                if (input == "1")
                {
                    cout << "Enter amount to add to central fund: ";
                    getline(cin, input);
                    while (!isValidDouble(input))
                    {
                        cout << "Invalid amount. Try again: ";
                        getline(cin, input);
                    }
                    fm->AddFund(stod(input));
                }
                else
                {
                    cout << "Project entry cancelled due to insufficient funds." << endl;
                    return;
                }
            }

            fund = getValidatedFundInput("Enter Project Allocation Fund: ");
            if (!fm->AllocateFund(fund, projectName, date))
            {
                cout << "Fund allocation failed due to insufficient Central Funds." << endl;
            }
            else
            {
                setAllocateFund(fund);
                allocated = true;
                cout << "Allocated " << fund << " to " << projectName << endl;
            }
        }

        int served;
        cout << "Enter Number of People Served: ";
        getline(cin, input);
        while (!isValidInt(input))
        {
            cout << "Wrong Entry, Try Again!" << endl;
            getline(cin, input);
        }
        served = stoi(input);
        setPeopleServed(served);

        double expense;
        cout << "Enter Project Expenses: ";
        getline(cin, input);
        while (!isValidDouble(input) || stod(input) > allocateFund)
        {
            cout << "Wrong Entry, Try Again!" << endl;
            getline(cin, input);
        }
        expense = stod(input);
        setExpenses(expense);

        double remaining = fund - expense;
        fm->AddFundSilent(remaining);
    }

    void display()
    {
        cout << "\n===== Project Details =====" << endl;
        getProjectType();
        getProjectName();
        getDate();
        getLocation();
        getAllocatefund();
        getPeopleServed();
        getExpenses();

        double remaining = allocateFund - totalExpenses;
        cout << "Returned Fund to Central: Rs." << remaining << endl;
        cout << "Current Central Fund: Rs." << fm->central_fund << endl;
    }

    virtual void execute()
    {
        cout << "Project executed: " << projectName << endl;
    }
};

class DisasterRelief : public WelfareProjects
{
    string disasterType;

public:
    DisasterRelief(FundManagement &fm) : WelfareProjects(fm) {}

    void disaster()
    {
        cout << "Enter Disaster Type: ";
        getline(cin, disasterType);
    }

    void execute()
    {
        cout << "Executing Disaster Relief for " << disasterType << endl;
    }

    void displayDetails(MemberDynamic &team)
    {
        display();
        cout << "Disaster Type: " << disasterType << endl;
        if (team.memberCount > 0)
        {
            cout << "\n----- Team Members Involved -----" << endl;
            team.display();
        }
        else
        {
            cout << "\nNo team members assigned yet." << endl;
        }
    }
};

class FoodDrive : public WelfareProjects
{
public:
    FoodDrive(FundManagement &fm) : WelfareProjects(fm) {}

    void execute()
    {
        cout << "Executing Food Drive: Distributing meals..." << endl;
    }

    void displayDetails(MemberDynamic &team)
    {
        display();
        if (team.memberCount > 0)
        {
            cout << "\n----- Team Members Involved -----" << endl;
            team.display();
        }
        else
        {
            cout << "\nNo team members assigned yet." << endl;
        }
    }
};

class HealthCamp : public WelfareProjects
{
public:
    HealthCamp(FundManagement &fm) : WelfareProjects(fm) {}

    void execute()
    {
        cout << "Executing Health Camp: Providing medical services..." << endl;
    }

    void displayDetails(MemberDynamic &team)
    {
        display();
        if (team.memberCount > 0)
        {
            cout << "\n----- Team Members Involved -----" << endl;
            team.display();
        }
        else
        {
            cout << "\nNo team members assigned yet." << endl;
        }
    }
};

class WelfareProgramRunner
{
    WelfareProjects *projects;
    int projectCount;
    int projectCapacity;

    FundManagement fm;
    MemberDynamic team;

public:
    WelfareProgramRunner() : projectCount(0), projectCapacity(5)
    {
        projects = new WelfareProjects[projectCapacity];
        fm.initialize_fund_records();
        fm.loadFundDataFromFile(); // Load fund data on startup
        loadProjectsFromFile();    // Load projects on startup
    }

    ~WelfareProgramRunner()
    {
        delete[] projects;
        fm.cleanup_fund_records();
    }

    void resizeProjectsArray()
    {
        projectCapacity += 5;
        WelfareProjects *newProjects = new WelfareProjects[projectCapacity];
        for (int i = 0; i < projectCount; ++i)
        {
            newProjects[i] = projects[i];
        }
        delete[] projects;
        projects = newProjects;
    }

    // CSV File Handling for Projects
    void saveProjectsToFile()
    {
        ofstream file("projects.csv");
        if (!file.is_open())
        {
            cout << "Error: Could not open projects.csv for writing." << endl;
            return;
        }

        // Write CSV header
        file << "Project Type,Project Name,Date,Location,Allocated Fund,People Served,Total Expenses" << endl;

        // Write project data
        for (int i = 0; i < projectCount; ++i)
        {
            file << projects[i].projectType << ","
                 << projects[i].projectName << ","
                 << projects[i].date << ","
                 << projects[i].location << ","
                 << projects[i].allocateFund << ","
                 << projects[i].peopleServed << ","
                 << projects[i].totalExpenses << endl;
        }

        file.close();
    }

    void loadProjectsFromFile()
    {
        ifstream file("projects.csv");
        if (!file.is_open())
        {
            // File doesn't exist, start with empty project list
            return;
        }

        string line;

        // Skip header line
        if (getline(file, line))
        {
        } // Skip header

        // Clear existing projects
        projectCount = 0;

        // Read project data
        while (getline(file, line))
        {
            if (line.empty())
                continue;

            if (projectCount >= projectCapacity)
            {
                resizeProjectsArray();
            }

            stringstream ss(line);
            string projectType, projectName, date, location, allocatedFundStr, peopleServedStr, expensesStr;

            getline(ss, projectType, ',');
            getline(ss, projectName, ',');
            getline(ss, date, ',');
            getline(ss, location, ',');
            getline(ss, allocatedFundStr, ',');
            getline(ss, peopleServedStr, ',');
            getline(ss, expensesStr, ',');

            projects[projectCount].setProjectType(projectType);
            projects[projectCount].setProjectName(projectName);
            projects[projectCount].setDate(date);
            projects[projectCount].setLocation(location);
            projects[projectCount].setAllocateFund(stod(allocatedFundStr));
            projects[projectCount].setPeopleServed(stoi(peopleServedStr));
            projects[projectCount].setExpenses(stod(expensesStr));

            projectCount++;
        }

        file.close();
    }

    void Run()
    {
        if (fm.central_fund == 0)
            fm.setFund(500000); // Set default only if not loaded from file
        int MenuChoice;
        do
        {
            cout << "\n========== Welfare Society Menu ==========" << endl;
            cout << "1. Society Members" << endl;
            cout << "2. Welfare Projects" << endl;
            cout << "3. Funds Management" << endl;
            cout << "4. Exit" << endl;
            cout << "Enter your choice: ";
            cin >> MenuChoice;
            cin.ignore();

            switch (MenuChoice)
            {
            case 1:
                memberMenu();
                break;
            case 2:
                projectMenu();
                break;
            case 3:
                fundMenu();
                break;
            case 4:
                cout << "Exiting program...\n";
                break;
            default:
                cout << "Invalid Choice!" << endl;
            }
        } while (MenuChoice != 4);
    }

    void memberMenu()
    {
        int MemberChoice;
        do
        {
            cout << "\n======= Society Members =======" << endl;
            cout << "1. Display All Members" << endl;
            cout << "2. Add New Member" << endl;
            cout << "3. Modify Member Data" << endl;
            cout << "4. Search Member by Name" << endl;
            cout << "5. Back" << endl;
            cout << "Enter your choice: ";
            cin >> MemberChoice;
            cin.ignore();

            switch (MemberChoice)
            {
            case 1:
                if (team.memberCount == 0)
                    cout << "No Team Member found." << endl;
                else
                    team.display();
                break;
            case 2:
                team.AddMember();
                cout << "Member added successfully.\n";
                break;
            case 3:
                team.ModifyMemberData();
                break;
            case 4:
                team.SearchMemberByName();
                break;
            case 5:
                break;
            default:
                cout << "Invalid Choice!" << endl;
            }
        } while (MemberChoice != 5);
    }

    void projectMenu()
    {
        int ProjectChoice;
        do
        {
            cout << "\n======= Welfare Projects Menu =======" << endl;
            cout << "1. Add New Project" << endl;
            cout << "2. Display All Projects" << endl;
            cout << "3. Modify Project Data" << endl;
            cout << "4. Search Project by Allocation Date" << endl;
            cout << "5. Back" << endl;
            cout << "Enter your choice: ";
            cin >> ProjectChoice;
            cin.ignore();

            switch (ProjectChoice)
            {
            case 1:
                addProject();
                break;
            case 2:
                displayProjects();
                break;
            case 3:
                modifyProject();
                break;
            case 4:
                searchProjectByDate();
                break;
            case 5:
                break;
            default:
                cout << "Invalid Choice!" << endl;
            }
        } while (ProjectChoice != 5);
    }

    void addProject()
    {
        if (projectCount >= projectCapacity)
            resizeProjectsArray();

        cout << "\n===== Add New Project =====" << endl;
        cout << "1. Disaster Relief" << endl;
        cout << "2. Food Drive" << endl;
        cout << "3. Health Camp" << endl;
        cout << "Enter project type: ";
        int projTypeChoice;
        cin >> projTypeChoice;
        cin.ignore();

        switch (projTypeChoice)
        {
        case 1:
        {
            DisasterRelief dr(fm);
            dr.EnterData();
            dr.disaster();
            projects[projectCount++] = dr;
            dr.execute();
            saveProjectsToFile(); // Save after adding project
            break;
        }
        case 2:
        {
            FoodDrive fd(fm);
            fd.EnterData();
            projects[projectCount++] = fd;
            fd.execute();
            saveProjectsToFile(); // Save after adding project
            break;
        }
        case 3:
        {
            HealthCamp hc(fm);
            hc.EnterData();
            projects[projectCount++] = hc;
            hc.execute();
            saveProjectsToFile(); // Save after adding project
            break;
        }
        default:
            cout << "Invalid project type!" << endl;
        }
    }

    void displayProjects()
    {
        if (projectCount == 0)
        {
            cout << "No projects to display." << endl;
            return;
        }
        cout << "\n===== All Welfare Projects =====" << endl;
        for (int i = 0; i < projectCount; ++i)
        {
            cout << "\n--- Project " << i + 1 << " ---" << endl;
            projects[i].display();
        }
    }

    void modifyProject()
    {
        if (projectCount == 0)
        {
            cout << "No projects to modify." << endl;
            return;
        }

        cout << "\n----- All Projects -----" << endl;
        for (int i = 0; i < projectCount; ++i)
        {
            cout << i + 1 << ". " << projects[i].projectName << endl;
        }

        string searchProjectName;
        cout << "Enter the name of the project to modify: ";
        getline(cin, searchProjectName);

        bool found = false;
        for (int i = 0; i < projectCount; ++i)
        {
            if (projects[i].projectName == searchProjectName)
            {
                cout << "Project found. Enter new data for " << projects[i].projectName << ":" << endl;

                double old_allocated_fund = projects[i].allocateFund;
                double old_total_expenses = projects[i].totalExpenses;
                string old_projectName = projects[i].projectName;
                string old_date = projects[i].date;

                projects[i].EnterData();
                fm.AddFundSilent(old_allocated_fund);

                double unspent = old_allocated_fund - old_total_expenses;
                if (unspent > 0)
                    fm.AddFundSilent(-unspent);

                for (int j = 0; j < fm.fund_record_count; j++)
                {
                    if (fm.fund_allocation_records[j].projectName == old_projectName && fm.fund_allocation_records[j].allocationDate == old_date)
                    {
                        for (int k = j; k < fm.fund_record_count - 1; k++)
                        {
                            fm.fund_allocation_records[k] = fm.fund_allocation_records[k + 1];
                        }
                        fm.fund_record_count--;
                        break;
                    }
                }

                cout << "Project data modified successfully!" << endl;
                saveProjectsToFile(); // Save after modification
                found = true;
                break;
            }
        }
        if (!found)
            cout << "Project with name '" << searchProjectName << "' not found." << endl;
    }

    void searchProjectByDate()
    {
        if (projectCount == 0)
        {
            cout << "No projects to search." << endl;
            return;
        }
        string searchDate;
        cout << "Enter the allocation date (DD/MM/YYYY) to search for Project: ";
        getline(cin, searchDate);

        bool found = false;
        cout << "\n----- Projects on " << searchDate << " -----" << endl;
        for (int i = 0; i < projectCount; ++i)
        {
            if (projects[i].date == searchDate)
            {
                projects[i].display();
                found = true;
            }
        }
        if (!found)
            cout << "No projects found for the date '" << searchDate << "'." << endl;
    }

    void fundMenu()
    {
        int FundChoice;
        do
        {
            cout << "\n======= Fund Management =======" << endl;
            cout << "1. View Balance" << endl;
            cout << "2. Add Funds" << endl;
            cout << "3. View Fund Allocation Records" << endl;
            cout << "4. Back" << endl;
            cout << "Enter your choice: ";
            cin >> FundChoice;
            cin.ignore();

            switch (FundChoice)
            {
            case 1:
                cout << "Central Fund: " << fm.central_fund << endl;
                break;
            case 2:
            {
                double amount;
                cout << "Enter amount to add: ";
                cin >> amount;
                fm.AddFund(amount);
                cin.ignore();
                break;
            }
            case 3:
                cout << "Fund Allocation Records: (" << fm.fund_record_count << ")" << endl;
                fm.ViewFundAllocationRecords();
                break;
            case 4:
                break;
            default:
                cout << "Invalid Choice!" << endl;
            }
        } while (FundChoice != 4);
    }
};
void box(int a, int b)
{
    for (int i = 0; i < 65; i++)
    {
        gotoRowCol(a, b);
        cout << "_";
        b++;
    }
    a++;
    for (int i = 0; i < 10; i++)
    {
        gotoRowCol(a, b);
        cout << "|";
        a++;
    }
    b--;
    a--;
    for (int i = 0; i < 65; i++)
    {
        gotoRowCol(a, b);
        cout << "_";
        b--;
    }

    for (int i = 0; i < 10; i++)
    {
        gotoRowCol(a, b);
        cout << "|";
        a--;
    }
}

void validateAlpha(string &input)
{
    while (true)
    {
        char ch;
        input = "";
        bool valid = true;

        while (true)
        {
            ch = cin.get();
            if (ch == '\n')
                break;
            input += ch;
        }

        for (int i = 0; input[i] != '\0'; i++)
        {
            char c = input[i];
            if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == ' '))
            {
                valid = false;
                break;
            }
        }

        if (valid && !input.empty())
            break;

        cout << "Invalid! Enter letters only: ";
    }
}

void validatePhone(string &phone)
{
    while (true)
    {
        phone = "";
        bool valid = true;
        char ch;

        while (true)
        {
            ch = cin.get();
            if (ch == '\n')
                break;
            if (!isdigit(ch))
                valid = false;
            phone += ch;
        }

        if (valid && phone.length() == 11 && phone[0] == '0')
            break;

        cout << "Invalid! Must be 11 digits starting with 0.\n";
        cout << "Enter Phone Number (11 digits, starts with 0): ";
    }
}

int validYear(string input)
{
    while (true)
    {
        int Digit = 1;
        for (int i = 0; input[i] != '\0'; i++)
        {
            if (input[i] < '0' || input[i] > '9')
            {
                Digit = 0;
                break;
            }
        }

        if (Digit == 1)
        {
            int year = 0;
            for (int i = 0; input[i] != '\0'; i++)
            {
                year = year * 10 + (input[i] - '0');
            }

            if (year > 0 && year < 5)
                return year;
        }

        cout << "Invalid! Enter a valid year (1-4): ";
        cin >> input;
    }
}

void validBG(string &BG)
{
    string sample[8] = {"A+", "A-", "B+", "B-", "O+", "O-", "AB+", "AB-"};

    while (true)
    {
        for (int i = 0; i < 8; i++)
        {
            if (BG == sample[i])
                return;
        }
        cout << "Invalid Blood Group, enter again (A+, A-, B+, B-, O+, O-, AB+, AB-): ";
        cin >> BG;
    }
}

void validDepartment(string &dep)
{
    string sample[4] = {"BSCS", "BSEE", "BBA", "BSMath"};

    while (true)
    {
        for (int i = 0; i < 4; i++)
        {
            if (dep == sample[i])
                return;
        }
        cout << "Invalid Department name, enter again (BSCS, BSEE, BBA, BSMath): ";
        cin >> dep;
    }
}

int stringToInt(const string &str)
{
    int num = 0;
    for (int i = 0; i < str.length(); i++)
    {
        num = num * 10 + (str[i] - '0');
    }
    return num;
}

// Updated age validator without using atoi/stoi
void validateAge(string &age)
{
    while (true)
    {
        age = "";
        bool valid = true;
        char ch;

        while (true)
        {
            ch = cin.get();
            if (ch == '\n')
                break;
            if (!isdigit(ch))
                valid = false;
            age += ch;
        }

        if (valid && !age.empty())
        {
            int num = stringToInt(age);
            if (num >= 12 && num <= 100)
                break;
        }

        cout << "Invalid! Enter a valid age: ";
    }
}

void writeToFile(const string &data)
{
    ofstream outFile("Patient file.txt", ios::app);
    if (outFile.is_open())
    {
        outFile << data << "\n";
        outFile.close();
    }
}

void loadDataFromFile()
{
    ifstream inFile("Patient file.txt");
    string line;
    cout << "\n--- Previous Records ---\n";
    while (getline(inFile, line))
    {
        cout << line << endl;
    }
    cout << "------------------------\n";
    inFile.close();
}

string generateNextID()
{
    string lastID;
    ifstream inFile("LastID.txt");
    if (inFile.is_open())
    {
        getline(inFile, lastID);
        inFile.close();
    }

    char letter = 'A';
    int number = 1;

    if (!lastID.empty())
    {
        letter = lastID[0];
        number = stoi(lastID.substr(1));
        number++;
        if (number > 9)
        {
            number = 1;
            letter++;
        }
    }

    string newID = string(1, letter) + to_string(number);

    ofstream outFile("LastID.txt");
    outFile << newID;
    outFile.close();

    return newID;
}

class Patient
{
public:
    string id;
    string name;
    int age;
    string issue;
    string bloodGroup;
    string department;
    int year;
    string personalEmail;
    string uniEmail;
    string phone;
    string emergencyType;
    string profession;
    string location;

    void assignID()
    {
        id = generateNextID();
    }

    void inputBasic()
    {
        assignID(); // Assign a new ID on input
        cout << "Enter name: ";
        cin.ignore();
        validateAlpha(name);

        string ageInput;
        cout << "Enter age: ";
        validateAge(ageInput);
        age = atoi(ageInput.c_str());

        cout << "Enter issue: ";
        validateAlpha(issue);

        cout << "Enter Blood Group: ";
        cin >> bloodGroup;
        validBG(bloodGroup);

        cout << "Enter Phone Number: ";
        cin.ignore();
        validatePhone(phone);

        cout << "Enter emergency type: ";
        validateAlpha(emergencyType);

        cout << "Enter Location: ";
        validateAlpha(location);
    }

    string getBasicDisplay() const
    {
        string data = "ID               |     " + id + "\n" +
                      "Name             |     " + name + "\n" +
                      "Age              |     " + to_string(age) + "\n" +
                      "Issue            |     " + issue + "\n" +
                      "Blood Group      |     " + bloodGroup + "\n" +
                      "Phone Number     |     " + phone + "\n" +
                      "Emergency Type   |     " + emergencyType + "\n" +
                      "Location         |     " + location + "\n";
        return data;
    }
};

class Student : public Patient
{
public:
    void input()
    {
        inputBasic();
        cout << "Enter department: ";
        cin >> department;
        validDepartment(department);

        string yearInput;
        cout << "Enter Year: ";
        cin >> yearInput;
        year = validYear(yearInput);

        cout << "Enter Personal Email: ";
        cin >> personalEmail;
        cout << "Enter University Email: ";
        cin >> uniEmail;

        writeToFile(display(true));
    }

    string display(bool toString = false)
    {
        string data = getBasicDisplay();
        data += "Department       |     " + department + "\n" +
                "Year             |     " + to_string(year) + "\n" +
                "Personal Email   |     " + personalEmail + "\n" +
                "University Email |     " + uniEmail + "\n";
        if (!toString)
            cout << data;
        return data;
    }
};

class Faculty : public Patient
{
public:
    void input()
    {
        inputBasic();
        cout << "Enter Department: ";
        cin >> department;
        validDepartment(department);

        cout << "Enter Personal Email: ";
        cin >> personalEmail;
        cout << "Enter University Email: ";
        cin >> uniEmail;

        cout << "Enter Profession: ";
        cin.ignore();
        validateAlpha(profession);

        writeToFile(display(true));
    }

    string display(bool toString = false)
    {
        string data = getBasicDisplay();
        data += "Department       |     " + department + "\n" +
                "Profession       |     " + profession + "\n" +
                "Personal Email   |     " + personalEmail + "\n" +
                "University Email |     " + uniEmail + "\n";
        if (!toString)
            cout << data;
        return data;
    }
};

class NonFaculty : public Patient
{
public:
    void input()
    {
        inputBasic();

        cout << "Enter Profession: ";
        validateAlpha(profession);

        cout << "Enter Personal Email: ";
        cin >> personalEmail;

        writeToFile(display(true));
    }

    string display(bool toString = false)
    {
        string data = getBasicDisplay();
        data += "Profession       |     " + profession + "\n" +
                "Personal Email   |     " + personalEmail + "\n";
        if (!toString)
            cout << data;
        return data;
    }
};

void searchByID(const string &idToFind)
{
    ifstream inFile("Patient file.txt");
    if (!inFile)
    {
        cout << "No data file found.\n";
        return;
    }

    string line, block = "";
    bool found = false;

    while (getline(inFile, line))
    {
        if (line.find("ID") == 0)
            block = ""; // new record block
        block += line + "\n";
        if (line.find("ID") != string::npos && line.find(idToFind) != string::npos)
        {
            found = true;
        }
        if (line.empty() && found)
        {
            cout << "\n--- Record Found ---\n"
                 << block;
            return;
        }
    }

    if (!found)
        cout << "No record found with ID: " << idToFind << "\n";
}

void deleteByID(const string &idToDelete)
{
    ifstream inFile("Patient file.txt");
    if (!inFile)
    {
        cout << "No data file found.\n";
        return;
    }

    string line, block = "", allData = "";
    bool deleting = false, deleted = false;

    while (getline(inFile, line))
    {
        if (line.find("ID") == 0)
        {
            if (!deleting)
                allData += block; // keep previous block
            block = "";           // reset block
            deleting = false;
        }

        block += line + "\n";
        if (line.find("ID") != string::npos && line.find(idToDelete) != string::npos)
        {
            deleting = true;
            deleted = true;
        }
    }
    if (!deleting)
        allData += block; // don't forget the last block

    inFile.close();

    ofstream outFile("Patient file.txt");
    outFile << allData;
    outFile.close();

    if (deleted)
        cout << "Record with ID " << idToDelete << " deleted successfully.\n";
    else
        cout << "No record found with ID: " << idToDelete << "\n";
}

void Student_managing()
{
    Student *students = nullptr;
    Faculty *faculty = nullptr;
    NonFaculty *nonFaculty = nullptr;

    int studentCount = 0, facultyCount = 0, nonFacultyCount = 0;

    while (true)
    {
        cout << "\nEmergency Response and Assistance\n";
        cout << "1. Add Student\n";
        cout << "2. Display All Students\n";
        cout << "3. Add Faculty\n";
        cout << "4. Display All Faculty\n";
        cout << "5. Add Non-Faculty\n";
        cout << "6. Display All Non-Faculty\n";
        cout << "7. Show Previously Saved Records\n";
        cout << "8. Exit\n";
        cout << "9. Search Record by ID\n";
        cout << "10. Delete Record by ID\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        while (true)
        {
            if (choice == 1 || choice == 2 || choice == 3 || choice == 4 || choice == 5 || choice == 6 || choice == 7 || choice == 8 || choice == 9 || choice == 10)
            {
                break;
            }
            else
            {
                cout << "Enter your choice: ";
                cin >> choice;
            }
        }

        switch (choice)
        {
        case 1:
        {
            Student *temp = new Student[studentCount + 1];
            for (int i = 0; i < studentCount; i++)
                temp[i] = students[i];
            temp[studentCount].input();
            delete[] students;
            students = temp;
            studentCount++;
            break;
        }
        case 2:
            if (studentCount == 0)
                cout << "No student records available.\n";
            else
                for (int i = 0; i < studentCount; i++)
                {
                    cout << "\nStudent #" << i + 1 << ":\n";
                    students[i].display();
                }
            break;
        case 3:
        {
            Faculty *temp = new Faculty[facultyCount + 1];
            for (int i = 0; i < facultyCount; i++)
                temp[i] = faculty[i];
            temp[facultyCount].input();
            delete[] faculty;
            faculty = temp;
            facultyCount++;
            break;
        }
        case 4:
            if (facultyCount == 0)
                cout << "No faculty records available.\n";
            else
                for (int i = 0; i < facultyCount; i++)
                {
                    cout << "\nFaculty #" << i + 1 << ":\n";
                    faculty[i].display();
                }
            break;
        case 5:
        {
            NonFaculty *temp = new NonFaculty[nonFacultyCount + 1];
            for (int i = 0; i < nonFacultyCount; i++)
                temp[i] = nonFaculty[i];
            temp[nonFacultyCount].input();
            delete[] nonFaculty;
            nonFaculty = temp;
            nonFacultyCount++;
            break;
        }
        case 6:
            if (nonFacultyCount == 0)
                cout << "No non-faculty records available.\n";
            else
                for (int i = 0; i < nonFacultyCount; i++)
                {
                    cout << "\nNon-Faculty #" << i + 1 << ":\n";
                    nonFaculty[i].display();
                }
            break;
        case 7:
            loadDataFromFile();
            break;
        case 8:
            delete[] students;
            delete[] faculty;
            delete[] nonFaculty;
            cout << "Exiting...\n";
            return;
        case 9:
        {
            cout << "Enter ID to search: ";
            string id;
            cin >> id;
            searchByID(id);
            break;
        }
        case 10:
        {
            cout << "Enter ID to delete: ";
            string id;
            cin >> id;
            deleteByID(id);
            break;
        }
        default:
            cout << "Invalid choice! Please try again.\n";
        }
    }
}
/////////////////////////////////////////////------------Student data---------/////////////////////////////////////////////////////////////

class Person;
class Responder;
class Supervisor;

// Global variables
int recordCount = 0;
int maxCapacity = 100;
Person **personnel = new Person *[maxCapacity];

int currentNumber = 1;
char currentLetter = 'A';

// Utility functions
int stringToInt(const char *str)
{
    int result = 0;
    int i = 0;
    while (str[i] != '\0')
    {
        if (str[i] >= '0' && str[i] <= '9')
        {
            result = result * 10 + (str[i] - '0');
        }
        else
        {
            return -1;
        }
        i++;
    }
    return result;
}

bool isNumber(const char *str)
{
    int i = 0;
    if (str[0] == '\0')
        return false;
    while (str[i] != '\0')
    {
        if (str[i] < '0' || str[i] > '9')
            return false;
        i++;
    }
    return true;
}

char *generateID()
{
    static char id[10];
    id[0] = '\0';

    int num = currentNumber;
    int len = 0;

    // Convert number to string manually
    char temp[10];
    int tempIndex = 0;
    while (num > 0)
    {
        temp[tempIndex++] = (num % 10) + '0';
        num = num / 10;
    }

    for (int i = tempIndex - 1; i >= 0; i--)
    {
        id[len++] = temp[i];
    }

    id[len++] = currentLetter;
    id[len] = '\0';

    currentLetter++;
    if (currentLetter > 'Z')
    {
        currentLetter = 'A';
        currentNumber++;
    }

    return id;
}

void resizeArray()
{
    int newCapacity = maxCapacity + 50;
    Person **newArray = new Person *[newCapacity];
    for (int i = 0; i < recordCount; i++)
    {
        newArray[i] = personnel[i];
    }
    delete[] personnel;
    personnel = newArray;
    maxCapacity = newCapacity;
    cout << "Array resized to " << maxCapacity << " entries." << endl;
}

// Base abstract class
class Person
{
public:
    char id[10];
    char name[50];
    char phone[15];
    char experience[5];
    char availability[20];
    char department[20];
    char location[50];

    virtual void input() = 0;
    virtual void display() = 0;
    virtual void writeToFile(ofstream &out) = 0;
    virtual const char *getType() = 0;
    virtual Responder *asResponder() { return 0; }
    virtual Supervisor *asSupervisor() { return 0; }

    virtual ~Person() {}
};

int strLength(const char *str)
{
    int len = 0;
    while (str[len] != '\0')
    {
        len++;
    }
    return len;
}

int compareStrings(const char *s1, const char *s2)
{
    int i = 0;
    while (s1[i] != '\0' && s2[i] != '\0')
    {
        if (s1[i] != s2[i])
        {
            return s1[i] - s2[i];
        }
        i++;
    }
    return s1[i] - s2[i];
}

void copyString(char *dest, const char *src)
{
    int i = 0;
    while (src[i] != '\0')
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

int getLength(const char *str)
{
    int length = 0;
    while (str[length] != '\0')
    {
        length++;
    }
    return length;
}

class Responder : public Person
{
public:
    void input()
    {
        cout << "---- Enter Responder Info ----\n";

        cout << "Name: ";
        cin.getline(name, 50);

        do
        {
            cout << "Phone (11 digits starting with 0): ";
            cin.getline(phone, 15);
        } while (phone[0] != '0' || strLength(phone) != 11);

        do
        {
            cout << "Experience (0-50): ";
            cin.getline(experience, 5);
        } while (!isNumber(experience) || stringToInt(experience) > 50);

        do
        {
            cout << "Availability (Available/Not Available): ";
            cin.getline(availability, 30);
        } while (compareStrings(availability, "Available") != 0 &&
                 compareStrings(availability, "Not Available") != 0);

        do
        {
            cout << "Department (Medical/Security/Fire/General): ";
            cin.getline(department, 20);
        } while (
            compareStrings(department, "Medical") != 0 &&
            compareStrings(department, "Security") != 0 &&
            compareStrings(department, "Fire") != 0 &&
            compareStrings(department, "General") != 0);

        cout << "Location: ";
        cin.getline(location, 50);

        copyString(id, generateID());
    }

    void display()
    {
        cout << "------------------------------\n";
        cout << "Responder ID: " << id << "\n";
        cout << "Name: " << name << "\n";
        cout << "Phone: " << phone << "\n";
        cout << "Experience: " << experience << " years\n";
        cout << "Availability: " << availability << "\n";
        cout << "Department: " << department << "\n";
        cout << "Location: " << location << "\n";
    }

    void writeToFile(ofstream &out)
    {
        out << id << "\n"
            << name << "\n"
            << phone << "\n";
        out << experience << "\n"
            << availability << "\n";
        out << department << "\n"
            << location << "\n";
    }

    const char *getType() { return "RESPONDER"; }
    Responder *asResponder() { return this; }
};

class Supervisor : public Person
{
public:
    char teamSize[5];

    void input()
    {
        cout << "---- Enter Supervisor Info ----\n";

        cout << "Name: ";
        cin.getline(name, 50);

        do
        {
            cout << "Phone (11 digits starting with 0): ";
            cin.getline(phone, 15);
        } while (phone[0] != '0' || getLength(phone) != 11);

        do
        {
            cout << "Experience (0-50): ";
            cin.getline(experience, 5);
        } while (!isNumber(experience) || stringToInt(experience) > 50);

        do
        {
            cout << "Availability (Available/Not Available): ";
            cin.getline(availability, 20);
        } while (compareStrings(availability, "Available") != 0 && compareStrings(availability, "Not Available") != 0);

        do
        {
            cout << "Department (Medical/Security/Fire/General): ";
            cin.getline(department, 20);
        } while (
            compareStrings(department, "Medical") != 0 &&
            compareStrings(department, "Security") != 0 &&
            compareStrings(department, "Fire") != 0 &&
            compareStrings(department, "General") != 0);

        cout << "Location: ";
        cin.getline(location, 50);

        do
        {
            cout << "Team Size: ";
            cin.getline(teamSize, 5);
        } while (!isNumber(teamSize));

        copyString(id, generateID());
    }

    void display()
    {
        cout << "------------------------------\n";
        cout << "Supervisor ID: " << id << "\n";
        cout << "Name: " << name << "\n";
        cout << "Phone: " << phone << "\n";
        cout << "Experience: " << experience << " years\n";
        cout << "Availability: " << availability << "\n";
        cout << "Department: " << department << "\n";
        cout << "Location: " << location << "\n";
        cout << "Team Size: " << teamSize << "\n";
    }

    void writeToFile(ofstream &out)
    {
        out << id << "\n"
            << name << "\n"
            << phone << "\n";
        out << experience << "\n"
            << availability << "\n";
        out << department << "\n"
            << location << "\n"
            << teamSize << "\n";
    }

    const char *getType() { return "SUPERVISOR"; }
    Supervisor *asSupervisor() { return this; }
};

void saveResponders()
{
    ofstream out("responders.txt");
    if (!out)
    {
        cout << "Error writing responders.\n";
        return;
    }

    for (int i = 0; i < recordCount; i++)
    {
        Responder *r = personnel[i]->asResponder();
        if (r)
        {
            out << "RESPONDER\n";
            r->writeToFile(out);
        }
    }

    out.close();
    cout << "Responders saved.\n";
}

void saveSupervisors()
{
    ofstream out("supervisors.txt");
    if (!out)
    {
        cout << "Error writing supervisors.\n";
        return;
    }

    for (int i = 0; i < recordCount; i++)
    {
        Supervisor *s = personnel[i]->asSupervisor();
        if (s)
        {
            out << "SUPERVISOR\n";
            s->writeToFile(out);
        }
    }

    out.close();
    cout << "Supervisors saved.\n";
}

void loadData()
{
    ifstream in;
    char buffer[100];

    // Load Responders
    in.open("responders.txt");
    if (in)
    {
        while (in.getline(buffer, 100))
        {
            if (compareStrings(buffer, "RESPONDER") == 0)
            {
                if (recordCount >= maxCapacity)
                    resizeArray();
                Responder *r = new Responder();

                in.getline(r->id, 10);
                in.getline(r->name, 50);
                in.getline(r->phone, 15);
                in.getline(r->experience, 5);
                in.getline(r->availability, 30);
                in.getline(r->department, 20);
                in.getline(r->location, 50);

                personnel[recordCount++] = r;
            }
        }
        in.close();
    }

    // Load Supervisors
    in.open("supervisors.txt");
    if (in)
    {
        while (in.getline(buffer, 100))
        {
            if (compareStrings(buffer, "SUPERVISOR") == 0)
            {
                if (recordCount >= maxCapacity)
                    resizeArray();
                Supervisor *s = new Supervisor();

                in.getline(s->id, 10);
                in.getline(s->name, 50);
                in.getline(s->phone, 15);
                in.getline(s->experience, 5);
                in.getline(s->availability, 20);
                in.getline(s->department, 20);
                in.getline(s->location, 50);
                in.getline(s->teamSize, 5);

                personnel[recordCount++] = s;
            }
        }
        in.close();
    }

    cout << "Data loaded. Total records: " << recordCount << endl;
}

void initializeID()
{
    ifstream in1("responders.txt");
    ifstream in2("supervisors.txt");

    char line[100];
    char lastID[10] = "";

    auto readLastID = [&](ifstream &in)
    {
        char tempID[10];
        while (in.getline(line, 100))
        {
            if (compareStrings(line, "RESPONDER") == 0 || compareStrings(line, "SUPERVISOR") == 0)
            {
                if (in.getline(tempID, 10))
                {
                    copyString(lastID, tempID); // keep overwriting
                }
            }
        }
    };

    if (in1)
        readLastID(in1);
    if (in2)
        readLastID(in2);

    in1.close();
    in2.close();

    if (getLength(lastID) == 0)
        return;

    int i = 0;
    int num = 0;
    while (lastID[i] >= '0' && lastID[i] <= '9')
    {
        num = num * 10 + (lastID[i] - '0');
        i++;
    }

    char letter = lastID[i];

    currentNumber = num;
    currentLetter = letter;

    currentLetter++;
    if (currentLetter > 'Z')
    {
        currentLetter = 'A';
        currentNumber++;
    }
}

int findPersonByID(const char *searchID)
{
    for (int i = 0; i < recordCount; i++)
    {
        if (compareStrings(personnel[i]->id, searchID) == 0)
        {
            return i;
        }
    }
    return -1;
}

void deletePersonByID()
{
    char delID[10];
    cout << "Enter ID to delete: ";
    cin.getline(delID, 10);

    int index = findPersonByID(delID);
    if (index == -1)
    {
        cout << "ID not found.\n";
        return;
    }

    delete personnel[index];

    for (int i = index; i < recordCount - 1; i++)
    {
        personnel[i] = personnel[i + 1];
    }
    recordCount--;

    cout << "Record with ID " << delID << " deleted successfully.\n";
}

void responder_managment()
{

    int choice;
    loadData();
    initializeID();
    while (1)
    {
        cout << "\n=== EMERGENCY PERSONNEL SYSTEM ===\n";
        cout << "1. Add Responder\n";
        cout << "2. Add Supervisor\n";
        cout << "3. View All Responders\n";
        cout << "4. View All Supervisors\n";
        cout << "5. Save Responders to File\n";
        cout << "6. Save Supervisors to File\n";
        cout << "7. Delete Personnel by ID\n";
        cout << "8. Search Personnel by ID\n";
        cout << "9. Exit\n";

        cout << endl
             << "Notice: Press enter twice while deleting an entity." << endl;
        cout << "Choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1)
        {
            if (recordCount >= maxCapacity)
                resizeArray();
            Responder *r = new Responder();
            r->input();
            personnel[recordCount++] = r;
        }

        else if (choice == 2)
        {
            if (recordCount >= maxCapacity)
                resizeArray();
            Supervisor *s = new Supervisor();
            s->input();
            personnel[recordCount++] = s;
        }

        else if (choice == 3)
        {
            for (int i = 0; i < recordCount; i++)
            {
                Responder *r = personnel[i]->asResponder();
                if (r)
                    r->display();
            }
        }

        else if (choice == 4)
        {
            for (int i = 0; i < recordCount; i++)
            {
                Supervisor *s = personnel[i]->asSupervisor();
                if (s)
                    s->display();
            }
        }

        else if (choice == 5)
        {
            saveResponders();
        }

        else if (choice == 6)
        {
            saveSupervisors();
        }

        else if (choice == 7)
        {
            cin.ignore(); // Clear input buffer
            deletePersonByID();
        }

        else if (choice == 8)
        {
            char searchID[10];
            cout << "Enter ID to search: ";
            cin.ignore();
            cin.getline(searchID, 10);
            int idx = findPersonByID(searchID);
            if (idx == -1)
            {
                cout << "ID not found.\n";
            }
            else
            {
                personnel[idx]->display();
            }
        }

        else if (choice == 9)
        {
            saveResponders();
            saveSupervisors();
            for (int i = 0; i < recordCount; i++)
            {
                delete personnel[i];
            }
            delete[] personnel;
            cout << "Exiting...\n";
            break;
        }
        else
        {
            cout << "Invalid input!\n";
        }
    }

    return;
}

//////////////////////////////////----------responder---------//////////////////////////////

char globalBuffer[100];

int myStrLen(const char *str)
{
    int length = 0;
    while (str[length] != '\0')
    {
        length++;
    }
    return length;
}

void myStrCpy(char *dest, const char *src)
{
    int i = 0;
    while (src[i] != '\0')
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

int myStrCmp(const char *s1, const char *s2)
{
    int i = 0;
    while (s1[i] != '\0' && s2[i] != '\0')
    {
        if (s1[i] != s2[i])
            return s1[i] - s2[i];
        i++;
    }
    return s1[i] - s2[i];
}

void getLine(char *buffer, int size)
{
    int i = 0;
    char ch;
    while (i < size - 1 && cin.get(ch) && ch != '\n')
    {
        buffer[i++] = ch;
    }
    buffer[i] = '\0';
}

bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}

bool validateDate(const char *date)
{
    if (myStrLen(date) != 10)
        return false;
    if (date[2] != '-' || date[5] != '-')
        return false;
    for (int i = 0; i < 10; i++)
    {
        if (i == 2 || i == 5)
            continue;
        if (!isDigit(date[i]))
            return false;
    }
    return true;
}

bool isAlphabetic(const char *str)
{
    int len = myStrLen(str);
    if (len == 0)
        return false;
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (!(str[i] >= 'A' && str[i] <= 'Z') &&
            !(str[i] >= 'a' && str[i] <= 'z') &&
            str[i] != ' ')
        {
            return false;
        }
    }
    return true;
}

bool isNumeric(const char *str)
{
    int len = myStrLen(str);
    if (len == 0)
        return false;
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (!isDigit(str[i]))
            return false;
    }
    return true;
}

void getValidatedAlphabeticInput(char *destination, int maxLength, const char *prompt)
{
    while (true)
    {
        cout << prompt;
        getLine(globalBuffer, 100);
        if (myStrLen(globalBuffer) > maxLength || !isAlphabetic(globalBuffer))
        {
            cout << "Invalid input! Only letters and spaces allowed.\n";
        }
        else
        {
            myStrCpy(destination, globalBuffer);
            break;
        }
    }
}

void getValidatedNumericInput(int &value, const char *prompt)
{
    while (true)
    {
        cout << prompt;
        getLine(globalBuffer, 100);
        if (!isNumeric(globalBuffer))
        {
            cout << "Invalid input! Only digits allowed.\n";
        }
        else
        {
            value = 0;
            for (int i = 0; globalBuffer[i] != '\0'; i++)
            {
                value = value * 10 + (globalBuffer[i] - '0');
            }
            if (value <= 0)
            {
                cout << "Value must be positive.\n";
            }
            else
            {
                break;
            }
        }
    }
}

void getValidatedDateInput(char *destination, const char *prompt)
{
    while (true)
    {
        cout << prompt;
        getLine(globalBuffer, 100);
        if (validateDate(globalBuffer))
        {
            myStrCpy(destination, globalBuffer);
            break;
        }
        else
        {
            cout << "Invalid date format! Use dd-mm-yyyy.\n";
        }
    }
}

void getValidatedID(char *destination, int maxLength, const char *prompt)
{
    while (true)
    {
        cout << prompt;
        getLine(globalBuffer, 100);
        int len = myStrLen(globalBuffer);
        if (len == 0 || len > maxLength)
        {
            cout << "Invalid ID! Cannot be empty and must be within length limit.\n";
        }
        else
        {
            myStrCpy(destination, globalBuffer);
            break;
        }
    }
}

bool isValidCategory(const char *str)
{
    const char *validCategories[] = {
        "tablet",
        "syrup",
        "capsule",
        "ointment",
        "injection",
        "cream"};
    const int validCount = sizeof(validCategories) / sizeof(validCategories[0]);

    for (int i = 0; i < validCount; i++)
    {
        if (myStrCmp(str, validCategories[i]) == 0)
        {
            return true;
        }
    }
    return false;
}

void getValidatedCategoryInput(char *destination, int maxLength, const char *prompt)
{
    while (true)
    {
        cout << prompt;
        getLine(globalBuffer, 100);

        // Convert input to lowercase for case-insensitive comparison
        // (Optional, but recommended for user convenience)
        for (int i = 0; globalBuffer[i] != '\0'; i++)
        {
            if (globalBuffer[i] >= 'A' && globalBuffer[i] <= 'Z')
            {
                globalBuffer[i] = globalBuffer[i] - 'A' + 'a';
            }
        }

        if (myStrLen(globalBuffer) > maxLength || !isValidCategory(globalBuffer))
        {
            cout << "Invalid category! Allowed categories are: tablet, syrup, capsule, ointment, injection, cream.\n";
        }
        else
        {
            myStrCpy(destination, globalBuffer);
            break;
        }
    }
}

const int MAX_MEDICINES = 100;
const int ID_LEN = 20;
const int NAME_LEN = 50;
const int CATEGORY_LEN = 20;
const int DATE_LEN = 11;

class Medicine
{
public:
    char id[ID_LEN];
    char name[NAME_LEN];
    char category[CATEGORY_LEN];
    int quantity;
    char purchaseDate[DATE_LEN];
    char expiryDate[DATE_LEN];

    Medicine()
    {
        id[0] = '\0';
        name[0] = '\0';
        category[0] = '\0';
        quantity = 0;
        purchaseDate[0] = '\0';
        expiryDate[0] = '\0';
    }

    void inputMedicineDetails()
    {
        getValidatedID(id, ID_LEN - 1, "Enter Medicine ID: ");
        getValidatedAlphabeticInput(name, NAME_LEN - 1, "Enter Medicine Name: ");
        getValidatedCategoryInput(category, CATEGORY_LEN - 1, "Enter Category (tablet, syrup, capsule, ointment, injection, cream): ");
        getValidatedNumericInput(quantity, "Enter Quantity: ");
        getValidatedDateInput(purchaseDate, "Enter Purchase Date (dd-mm-yyyy): ");
        getValidatedDateInput(expiryDate, "Enter Expiry Date (dd-mm-yyyy): ");
    }

    const char *getID() const
    {
        return id;
    }

    int getQuantity() const
    {
        return quantity;
    }

    void updateStock(int change)
    {
        quantity += change;
        if (quantity < 0)
            quantity = 0;
    }

    bool isLowStock(int threshold) const
    {
        return quantity <= threshold;
    }

    const char *getName() const
    {
        return name;
    }

    const char *getCategory() const
    {
        return category;
    }

    const char *getPurchaseDate() const
    {
        return purchaseDate;
    }

    const char *getExpiryDate() const
    {
        return expiryDate;
    }

    void display() const
    {
        cout << "------------------------------------\n";
        cout << "Medicine ID: " << id << "\n";
        cout << "Name       : " << name << "\n";
        cout << "Category   : " << category << "\n";
        cout << "Stock      : " << quantity << "\n";
        cout << "Expiry Date: " << expiryDate << "\n";
        cout << "------------------------------------\n";
    }
};

class MedicineInventory
{
public:
    Medicine *medicines;
    int capacity;
    int count;

    void resizeIfNeeded()
    {
        if (count >= capacity - 1)
        {
            int newCapacity = capacity * 2;
            Medicine *newArray = new Medicine[newCapacity];

            for (int i = 0; i < count; i++)
            {
                newArray[i] = medicines[i];
            }

            delete[] medicines;
            medicines = newArray;
            capacity = newCapacity;

            cout << "Inventory resized to " << capacity << " medicines.\n";
        }
    }

    MedicineInventory(int initialSize = 10)
    {
        capacity = initialSize;
        count = 0;
        medicines = new Medicine[capacity];
    }

    ~MedicineInventory()
    {
        delete[] medicines;
    }

    void addMedicine()
    {
        resizeIfNeeded();
        Medicine newMed;
        newMed.inputMedicineDetails();

        // Check for duplicate ID
        for (int i = 0; i < count; i++)
        {
            if (myStrCmp(medicines[i].getID(), newMed.getID()) == 0)
            {
                cout << "Error: Medicine with this ID already exists!\n";
                return;
            }
        }

        medicines[count++] = newMed;
        cout << "Medicine added successfully.\n";
    }

    void displayAll() const
    {
        if (count == 0)
        {
            cout << "No medicines in inventory.\n";
            return;
        }
        for (int i = 0; i < count; i++)
        {
            medicines[i].display();
        }
    }

    int findByID(const char *id) const
    {
        for (int i = 0; i < count; i++)
        {
            if (myStrCmp(medicines[i].getID(), id) == 0)
            {
                return i;
            }
        }
        return -1;
    }

    void searchMedicineByID() const
    {
        cout << "Enter Medicine ID to search: ";
        getLine(globalBuffer, 100);
        int index = findByID(globalBuffer); // ✅ Corrected
        if (index == -1)
        {
            cout << "Medicine not found!\n";
        }
        else
        {
            medicines[index].display();
        }
    }

    void deleteMedicineByID()
    {
        cout << "Enter Medicine ID to delete: ";
        getLine(globalBuffer, 100);
        int index = findByID(globalBuffer); // ✅ Corrected
        if (index == -1)
        {
            cout << "Medicine not found!\n";
        }
        else
        {
            for (int i = index; i < count - 1; ++i)
            {
                medicines[i] = medicines[i + 1];
            }
            count--;
            cout << "Medicine deleted successfully!\n";
        }
    }

    void deleteMedicine()
    {
        char id[ID_LEN];
        getValidatedID(id, ID_LEN - 1, "Enter ID to delete: ");

        int index = findByID(id);
        if (index == -1)
        {
            cout << "Medicine not found.\n";
            return;
        }

        for (int i = index; i < count - 1; i++)
        {
            medicines[i] = medicines[i + 1];
        }
        count--;
        cout << "Medicine deleted successfully.\n";
    }

    void issueMedicine()
    {
        char id[ID_LEN];
        getValidatedID(id, ID_LEN - 1, "Enter ID to issue: ");
        int index = findByID(id);

        if (index == -1)
        {
            cout << "Medicine not found.\n";
            return;
        }

        int qty;
        getValidatedNumericInput(qty, "Enter quantity to issue: ");

        if (qty > medicines[index].getQuantity())
        {
            cout << "Not enough stock available.\n";
        }
        else
        {
            medicines[index].updateStock(-qty);
            cout << "Medicine issued successfully.\n";
        }
    }

    void checkLowStock(int threshold = 5) const
    {
        bool found = false;
        for (int i = 0; i < count; i++)
        {
            if (medicines[i].isLowStock(threshold))
            {
                medicines[i].display();
                found = true;
            }
        }
        if (!found)
        {
            cout << "No medicines with low stock.\n";
        }
    }

    void saveToFile() const
    {
        ofstream fout("Medicine.txt");
        if (!fout)
        {
            cout << "Error opening file for writing.\n";
            return;
        }

        for (int i = 0; i < count; i++)
        {
            fout << medicines[i].getID() << '|'
                 << medicines[i].getName() << '|'
                 << medicines[i].getCategory() << '|'
                 << medicines[i].getQuantity() << '|'
                 << medicines[i].getPurchaseDate() << '|'
                 << medicines[i].getExpiryDate() << '\n';
        }

        fout.close();
        cout << "Data saved to file successfully.\n";
    }

    void loadFromFile()
    {
        ifstream fin("Medicine.txt");
        if (!fin)
        {
            cout << "No existing file found. Starting with empty inventory.\n";
            return;
        }

        char line[256];
        while (fin.getline(line, 256))
        {
            Medicine med;
            int field = 0;
            char temp[6][100];

            for (int i = 0, k = 0;; i++)
            {
                if (line[i] == '|' || line[i] == '\0')
                {
                    temp[field][k] = '\0';
                    field++;
                    k = 0;
                    if (line[i] == '\0')
                        break;
                }
                else
                {
                    temp[field][k++] = line[i];
                }
            }

            myStrCpy(med.id, temp[0]);
            myStrCpy(med.name, temp[1]);
            myStrCpy(med.category, temp[2]);

            med.quantity = 0;
            for (int i = 0; temp[3][i] != '\0'; i++)
            {
                med.quantity = med.quantity * 10 + (temp[3][i] - '0');
            }

            myStrCpy(med.purchaseDate, temp[4]);
            myStrCpy(med.expiryDate, temp[5]);

            resizeIfNeeded();
            medicines[count++] = med;
        }

        fin.close();
        cout << "Data loaded from file successfully.\n";
    }
};

void medicine_inventory()
{
    MedicineInventory inventory;
    inventory.loadFromFile();
    int choice;

    while (true)
    {
        cout << "\n====== NSSI Dispensary Inventory Menu ======\n";
        cout << "1. Add New Medicine\n";
        cout << "2. Display All Medicines\n";
        cout << "3. Search Medicine by ID\n";
        cout << "4. Delete Medicine by ID\n";
        cout << "5. Issue Medicine (Reduce Stock)\n";
        cout << "6. Show Low Stock Alerts\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";

        getLine(globalBuffer, 100);
        if (!isNumeric(globalBuffer))
        {
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }
        choice = 0;
        for (int i = 0; globalBuffer[i] != '\0'; i++)
        {
            choice = choice * 10 + (globalBuffer[i] - '0');
        }

        switch (choice)
        {
        case 1:
            inventory.addMedicine();
            inventory.saveToFile();
            break;
        case 2:
            inventory.displayAll();
            break;
        case 3:
            inventory.searchMedicineByID();
            break;
        case 4:
            inventory.deleteMedicineByID();
            inventory.saveToFile();
            break;
        case 5:
            inventory.issueMedicine();
            inventory.saveToFile();
            break;
        case 6:
            inventory.checkLowStock();
            break;
        case 7:
            cout << "Exiting program. Thank you!\n";
            return;
        default:
            cout << "Invalid choice! Please select a valid option (1-7).\n";
        }
    }
}

//////////////////////////////////----------medicine inventory---------//////////////////////////////
void Emergency_Response_And_Management_Wing()
{

    int choice = 0;
    int running = 1;

    while (running == 1)
    {
        cout << "==========================================================" << endl;
        cout << "----------Emergency Response And Management Wing----------" << endl;
        cout << "==========================================================" << endl;
        cout << "1. Emergency Student and Faculty Management System" << endl;
        cout << "2. Emergency Responder System." << endl;
        cout << "3. NSSI Dispensary Inventory Menu." << endl;
        cout << "4. Exit Program" << endl;
        cout << "Enter your choice: ";

        cin >> choice;

        switch (choice)
        {
        case 1:
            Student_managing();
            break;
        case 2:
            responder_managment();
            break;
        case 3:
            medicine_inventory();
            break;
        case 4:
            running = 0;
            cout << "Exiting program. Goodbye!\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }

    return;
}
class Donor
{
public:
    string name;
    int age;
    string bloodgroup;
    string contactnumber;
    string gmail;
    string address;
    string healthstatus;
    string lastdonationdate;
    void addData()
    { // function to add the data of one donor
        cout << "Enter Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter Age: ";
        cin >> age;
        cout << "Enter Blood Group: ";
        cin >> bloodgroup;
        cout << "Enter Contact Number: ";
        cin >> contactnumber;
        cout << "Enter Gmail: ";
        cin >> gmail;
        cin.ignore();
        cout << "Enter Address: ";
        getline(cin, address);
        cout << "Enter Health Status (Rating from 1 to 10 base on health): ";
        cin >> healthstatus;
        cout << "Enter Last Donation Date (dd-mm-yyyy): ";
        cin >> lastdonationdate;

        ofstream out("donors.txt", ios::app);
        if (!out.is_open())
        {
            cout << "Error opening file to write.\n";
            return;
        }

        out << name << "," << age << "," << bloodgroup << "," << contactnumber << ","
            << gmail << "," << address << "," << healthstatus << "," << lastdonationdate << "\n";

        out.close();
        cout << "Donor data added successfully!\n";
    }

    time_t changetotime(string &date)
    {
        struct tm t = {};
        sscanf(date.c_str(), "%d-%d-%d", &t.tm_mday, &t.tm_mon, &t.tm_year);
        t.tm_mon -= 1;
        t.tm_year -= 1900;
        return mktime(&t);
    }
    void showdata();
    void showdata(const string &name);
    void changedata(const string &changename);
    void removeData(const string &nameToRemove);
    vector<string> requestsubmission(const string &blood, string &today_date);
    void gmailtodonors(const vector<string> &selected);
};
class DonorSubmission
{
private:
    string patient;
    string bloodgroup;
    string address;
    int age;
    string contact;
    int healthcondition;
    string todaydate;

public:
    Donor obj;
    void addPatient()
    {
        cout << "Enter Patient Name: ";
        cin.ignore();
        getline(cin, patient);
        cout << "Enter Blood Group: ";
        cin >> bloodgroup;
        cin.ignore();
        cout << "Enter Address: ";
        getline(cin, address);

        cout << "Enter Age: ";
        cin >> age;
        cout << "Enter Contact: ";
        cin >> contact;
        cout << "Enter Health Condition (1 to 10): ";
        cin >> healthcondition;
        cout << "Enter Today’s Date (dd-mm-yyyy): ";
        cin >> todaydate;
        cin.ignore();
        obj.requestsubmission(bloodgroup, todaydate);
    }
};

void Donor::showdata()
{ // function to show the data of all the donors
    ifstream in("donors.txt");
    if (!in.is_open())
    {
        cout << "Error opening file.\n";
        return;
    }

    string name, bloodGroup, contactNumber, gmail, address, healthStatus, lastDonationDate;
    int age;

    cout << "\n-----------------------------------------------------------------------------------------------------------\n";
    cout << "| Name           | Age | Blood Group | Contact       | Gmail               | Health | Last Donation Date |\n";
    cout << "-----------------------------------------------------------------------------------------------------------\n";

    string line;
    while (getline(in, line))
    {
        stringstream ss(line);
        getline(ss, name, ',');
        ss >> age;
        ss.ignore();
        getline(ss, bloodGroup, ',');
        getline(ss, contactNumber, ',');
        getline(ss, gmail, ',');
        getline(ss, address, ',');
        getline(ss, healthStatus, ',');
        getline(ss, lastDonationDate, ',');

        cout << "| " << setw(14) << left << name
             << "| " << setw(3) << age
             << " | " << setw(11) << bloodGroup
             << "| " << setw(13) << contactNumber
             << "| " << setw(28) << gmail
             << "| " << setw(7) << healthStatus
             << "| " << setw(19) << lastDonationDate << "|\n";
    }

    cout << "-----------------------------------------------------------------------------------------------------------\n";
    in.close();
}
void Donor::showdata(const string &searchName)
{ // function to show the donor data by its name
    ifstream in("donors.txt");
    if (!in.is_open())
    {
        cout << "Error opening file.\n";
        return;
    }

    string name, bloodGroup, contactNumber, gmail, address, healthStatus, lastDonationDate;
    string ageStr;
    int age;
    bool found = false;
    static int a = 1;

    string line;
    while (getline(in, line))
    {
        stringstream ss(line);
        getline(ss, name, ',');
        getline(ss, ageStr, ',');
        age = stoi(ageStr);
        getline(ss, bloodGroup, ',');
        getline(ss, contactNumber, ',');
        getline(ss, gmail, ',');
        getline(ss, address, ',');
        getline(ss, healthStatus, ',');
        getline(ss, lastDonationDate, ',');

        if (name == searchName)
        {
            found = true;
            if (a == 1)
            {
                cout << "Donor data found.\n";
                cout << "\n-----------------------------------------------------------------------------------------------------------\n";
                cout << "| Name           | Age | Blood Group | Contact       | Gmail               | Health | Last Donation Date |\n";
                cout << "-----------------------------------------------------------------------------------------------------------\n";
                a = 2;
            }

            cout << "| " << setw(14) << left << name
                 << "| " << setw(3) << age
                 << "| " << setw(11) << bloodGroup
                 << "| " << setw(13) << contactNumber
                 << "| " << setw(28) << gmail
                 << "| " << setw(7) << healthStatus
                 << "| " << setw(19) << lastDonationDate << "|\n";
        }
    }

    if (!found)
        cout << "No donor found with name: " << searchName << endl;

    in.close();
}

void Donor::changedata(const string &changename)
{ // function to update the donor data
    ifstream out("donors.txt");
    if (!out.is_open())
    {
        cout << "Can not open the file .\n";
        return;
    }
    string name, bloodgroup, contactnumber, gmail, address, healthstatus, lastdonationdate;
    int age;
    bool found = false;
    string line, sage;
    vector<string> l;
    while (getline(out, line))
    {
        stringstream ss(line);
        getline(ss, name, ',');
        getline(ss, sage, ',');
        age = stoi(sage);
        getline(ss, bloodgroup, ',');
        getline(ss, contactnumber, ',');
        getline(ss, gmail, ',');
        getline(ss, address, ',');
        getline(ss, healthstatus, ',');
        getline(ss, lastdonationdate, ',');
        if (changename == name)
        {
            found = true;
            cout << "Donor data found .\n";
            cout << "\n-----------------------------------------------------------------------------------------------------------\n";
            cout << "| Name           | Age | Blood Group | Contact       | Gmail               | Health | Last Donation Date |\n";
            cout << "-----------------------------------------------------------------------------------------------------------\n";
            cout << "| " << setw(14) << left << name
                 << "| " << setw(3) << age
                 << "| " << setw(11) << bloodgroup
                 << "| " << setw(13) << contactnumber
                 << "| " << setw(28) << gmail
                 << "| " << setw(7) << healthstatus
                 << "| " << setw(19) << lastdonationdate << "|\n";

            cout << "\n-------------------------------------------------------------------------------------------------------------\n";
            cout << " Enter its information again which you wanted to change " << endl;
            cout << "Enter Name: ";
            getline(cin, name);
            cout << "Enter Age: ";
            cin >> age;
            cout << "Enter Blood Group: ";
            cin >> bloodgroup;
            cout << "Enter Contact Number: ";
            cin >> contactnumber;
            cout << "Enter Gmail: ";
            cin >> gmail;
            cin.ignore();
            cout << "Enter Address: ";
            getline(cin, address);
            cout << "Enter Health Status (Rating from 1 to 10 base on health): ";
            cin >> healthstatus;
            cout << "Enter Last Donation Date (dd-mm-yyyy): ";
            cin >> lastdonationdate;
        }

        string updatedLine = name + "," + to_string(age) + "," + bloodgroup + "," + contactnumber + "," +
                             gmail + "," + address + "," + healthstatus + "," + lastdonationdate;
        l.push_back(updatedLine);
    }

    out.close();

    if (!found)
    {
        cout << "Donor not found.\n";
        return;
    }

    ofstream in("donors.txt");
    for (const string &m : l)
    {
        in << m << "\n";
    }
    in.close();

    cout << "Data updated successfully.\n";
}

void Donor::removeData(const string &nametoremove)
{ //// function to remove the donor data
    ifstream in("donors.txt");
    if (!in.is_open())
    {
        cout << "Unable to open file for reading.\n";
        return;
    }

    vector<string> updateddata;
    string line;
    bool found = false;

    while (getline(in, line))
    {
        stringstream ss(line);
        string name;
        getline(ss, name, ',');

        if (name != nametoremove)
        {
            updateddata.push_back(line);
        }
        else
        {
            found = true;
        }
    }

    in.close();

    if (!found)
    {
        cout << "No donor found with the name: " << nametoremove << endl;
        return;
    }

    ofstream out("donors.txt");
    if (!out.is_open())
    {
        cout << "Unable to open file for writing.\n";
        return;
    }

    for (const string &l : updateddata)
    {
        out << l << "\n";
    }

    out.close();
    cout << "All donors named \"" << nametoremove << "\" have been removed successfully.\n";
}
vector<string> Donor::requestsubmission(const string &group, string &date)
{ // function to show the donor data by its name
    ifstream in("donors.txt");
    if (!in.is_open())
    {
        cout << "Error opening file.\n";
        return {};
    }
    string name, bloodGroup, contactNumber, gmail, address, healthStatus, lastDonationDate;
    string ageStr;
    vector<string> match_donors;
    vector<string> gmailofdonors;
    int age;
    string line;
    while (getline(in, line))
    {
        stringstream ss(line);
        getline(ss, name, ',');
        getline(ss, ageStr, ',');
        age = stoi(ageStr);
        getline(ss, bloodGroup, ',');
        getline(ss, contactNumber, ',');
        getline(ss, gmail, ',');
        getline(ss, address, ',');
        getline(ss, healthStatus, ',');
        getline(ss, lastDonationDate, ',');

        if (group == bloodGroup && stoi(healthStatus) > 6)
        {
            time_t today = changetotime(date);
            time_t date_dona = changetotime(lastDonationDate);
            double seconds = difftime(today, date_dona);
            double days = seconds / (24 * 60 * 60);
            if (days >= 90)
            {
                match_donors.push_back(line);
            }
        }
    }
    if (match_donors.empty())
    {
        cout << "No donor found with that blood group: " << endl;
        return {};
    }
    in.close();
    vector<string> selected;
    vector<string> remaining;
    for (const string &line : match_donors)
    {
        stringstream ss(line);
        string name, ageStr, bloodGroup, contact, gmail, address, health, donationDate;
        getline(ss, name, ',');
        getline(ss, ageStr, ',');
        int age = stoi(ageStr);
        getline(ss, bloodGroup, ',');
        getline(ss, contact, ',');
        getline(ss, gmail, ',');
        getline(ss, address, ',');
        getline(ss, health, ',');
        getline(ss, donationDate, ',');

        if (age >= 19 && age <= 25 && selected.size() < 5)
        {
            selected.push_back(line);
            gmailofdonors.push_back(gmail);
        }
        else
        {
            remaining.push_back(line);
        }
    }

    for (const string &line : remaining)
    {
        if (selected.size() >= 5)
            break;
        selected.push_back(line);
        stringstream ss(line);
        string name, ageStr, bloodGroup, contact, gmail, address, health, donationDate;
        getline(ss, name, ',');
        getline(ss, ageStr, ',');
        getline(ss, bloodGroup, ',');
        getline(ss, contact, ',');
        getline(ss, gmail, ',');
        gmailofdonors.push_back(gmail);
    }

    cout << "\n---------------------- Selected Donors ----------------------\n";
    cout << left << setw(15) << "Name"
         << setw(6) << "Age"
         << setw(8) << "Group"
         << setw(15) << "Contact"
         << setw(25) << "Gmail"
         << setw(15) << "Address"
         << setw(10) << "Health"
         << "Last Donation" << endl;
    cout << "-------------------------------------------------------------\n";

    for (const string &donor : selected)
    {
        stringstream ss(donor);
        string name, ageStr, bloodGroup, contact, gmail, address, health, donationDate;

        getline(ss, name, ',');
        getline(ss, ageStr, ',');
        getline(ss, bloodGroup, ',');
        getline(ss, contact, ',');
        getline(ss, gmail, ',');
        getline(ss, address, ',');
        getline(ss, health, ',');
        getline(ss, donationDate, ',');

        cout << left << setw(15) << name
             << setw(6) << ageStr
             << setw(8) << bloodGroup
             << setw(15) << contact
             << setw(25) << gmail
             << setw(15) << address
             << setw(10) << health
             << donationDate << endl;
    }
    cout << "If YOU WANTED TO SEND GMAIL TO THE FOLLOWING DONORS THAN PRESS 1 ELSE 2 : " << endl;
    int a;
    cin >> a;
    if (a == 1)
    {
        gmailtodonors(gmailofdonors);
    }
    else
    {
        return selected;
    }
}
void Donor::gmailtodonors(const vector<string> &gmails)
{
    if (gmails.empty())
    {
        cout << "No emails to save.\n";
        return;
    }

    ofstream file("selectedemails.txt");
    if (!file.is_open())
    {
        cout << "Error opening file.\n";
        return;
    }

    for (const string &email : gmails)
    {
        file << email << endl;
        cout << "Saved: " << email << endl;
    }

    file.close();
}
void bloodwing()
{
    Donor donor;
    DonorSubmission patient;
    int choice;
    string name;

    do
    {
        cout << "\nDonor and Patient Management System\n";
        cout << "1. Add Donor\n";
        cout << "2. Show All Donors\n";
        cout << "3. Search Donor by Name\n";
        cout << "4. Update Donor Data\n";
        cout << "5. Remove Donor\n";
        cout << "6. Add Patient\n"; // New option to add patient
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            donor.addData();
            break;
        case 2:
            donor.showdata();
            break;
        case 3:
            cout << "Enter donor name to search: ";
            cin.ignore();
            getline(cin, name);
            donor.showdata(name);
            break;
        case 4:
            cout << "Enter donor name to update: ";
            cin.ignore();
            getline(cin, name);
            donor.changedata(name);
            break;
        case 5:
            cout << "Enter donor name to remove: ";
            cin.ignore();
            getline(cin, name);
            donor.removeData(name);
            break;
        case 6:
            patient.addPatient(); // Call function to add patient data
            break;
        case 7:
            cout << "Exiting program.\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
            break;
        }

    } while (choice != 7);
}

// Some instructions will be displayed first before the execution of the program
void instructions()
{
    string instruction = "DISPLAYING INSTRUCTIONS: \n1.    This software is only for the use of NSSI society. \n2.    It can only be accessed by NSSI. \n3.    Data is the private access of the society.";
    string wings = "There are four main wings.... \n1.    Education Wing \n2.    Social Welfare Wing \n3.    Blood Donation Wing \n4.    Emergency Response and Management Wing";
    Heading(instruction + "\n\n" + wings, 20);
    cout << endl
         << "Press any key to Proceed Next";
    getch();
    system("cls");
}

void addNSSI()
{
    char ch = 219;
    int a = 18, b = 50;
    box(15, 45);

    gotoRowCol(a, b);
    cout << ch << ch << "    " << ch << " " << ch << ch << ch << ch << " " << ch << ch << ch << ch << " " << ch << "    " << ch << ch << ch << ch << " " << ch << ch << ch << ch << " " << ch << ch << ch << ch << " " << ch << " " << ch << ch << ch << ch << " ";
    cout << ch << ch << ch << ch << ch << " " << ch << "   " << ch << " " << endl;
    a++;
    gotoRowCol(a, b);
    cout << ch << " " << ch << "   " << ch << " " << ch << "   " << " " << ch << "   " << " " << ch << "    " << ch << "   " << " " << ch << "  " << ch << " " << ch << "   " << " " << ch << " " << ch << "   " << " ";
    cout << "  " << ch << "  " << "  " << ch << " " << ch << " " << endl;
    a++;
    gotoRowCol(a, b);
    cout << ch << "  " << ch << "  " << ch << " " << ch << ch << ch << ch << " " << ch << ch << ch << ch << " " << ch << "    " << ch << ch << ch << ch << " " << ch << "  " << ch << " " << ch << "   " << " " << ch << " " << ch << ch << ch << ch << " ";
    cout << "  " << ch << "  " << "   " << ch << endl;
    a++;
    gotoRowCol(a, b);
    cout << ch << "   " << ch << " " << ch << " " << "   " << ch << " " << "   " << ch << " " << ch << "    " << "   " << ch << " " << ch << "  " << ch << " " << ch << "   " << " " << ch << " " << ch << "   " << " ";
    cout << "  " << ch << "  " << "   " << ch << endl;
    a++;
    gotoRowCol(a, b);
    cout << ch << "    " << ch << ch << " " << ch << ch << ch << ch << " " << ch << ch << ch << ch << " " << ch << "    " << ch << ch << ch << ch << " " << ch << ch << ch << ch << " " << ch << ch << ch << ch << " " << ch << " " << ch << ch << ch << ch << " ";
    cout << "  " << ch << "  " << "   " << ch << endl;
    a++;

    cout << endl
         << endl;

    getch();
    system("cls");
}

int main()
{
    // loade4();
    int wing_number;
    addNSSI();
    Heading("WElcome to Namal Society for Social Impact", 70);
    instructions();
    WelfareProgramRunner program;
    while (true)
    {
        cout << endl
             << ".....MAIN MENU....." << endl;
        cout << "Please enter the digit corresponding to the wing..." << endl;
        cout << "1-->EDUCATION WING\n2-->SOCIAL WELFARE WING\n3-->BLOOD DONATION WING\n4-->EMERGENCY WING\n5-->EXIT..." << endl;
        cin >> wing_number;
        switch (wing_number)
        {
        case 1:
        {
            Heading("Welcome To NSSI Education Wing", 50);
            EducationWingManager();
            sleep(1000);
            system("cls");
        }
        break;
        case 2:
        {
            Heading("Welcome To Social Welfare Wing", 50);
            program.Run();
            sleep(1000);
            system("cls");
        }
        break;
        case 3:
        {
            Heading("Welcome To Blood Donation Wing", 50);
            bloodwing();
        }
        break;
        case 4:
        {
            Heading("Welcome To Emergency Response and Management Wing", 70);
            cout << endl;
            Emergency_Response_And_Management_Wing();
            sleep(1000);
            system("cls");
        }
        break;
        case 5:
        {
            cout << " ...GOOD BYE...:)" << endl;
        }
        break;
        default:
            cout << "invalid digit....try again..:)" << endl;
            sleep(1000);
            system("cls");
            break;
        }
        if (wing_number == 5)
        {
            break;
        }
        system("cls");
    }
    return 0;
}