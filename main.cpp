#include<bits/stdc++.h>
using namespace std;

const char *FILENAME = "accounts.dat";

struct Account {
    int accNo;
    char name[50];
    char type;
    double balance;

    Account() {
        accNo = 0;
        name[0] = '\0';
        type = 'S';
        balance = 0;
    }

    void createAccount(int nextAccNo) {
        accNo = nextAccNo;

        cout << "Enter Name: ";
        string temp;
        getline(cin >> ws, temp);

        strncpy(name, temp.c_str(), sizeof(name)-1);
        name[sizeof(name)-1] = '\0';

        cout << "Enter Account Type (S/C): ";
        cin >> type;
        type = toupper(type);

        cout << "Enter Initial Balance: ";
        cin >> balance;

        cout << "\nAccount Created Successfully!\n";
        cout << "Account Number: " << accNo << "\n";
    }

    void showAccount() const {
        cout << "\nAccount No : " << accNo << "\n";
        cout << "Name       : " << name << "\n";
        cout << "Type       : " << type << "\n";
        cout << "Balance    : " << balance << "\n";
    }
};

int getNextAccountNumber() {

    ifstream file(FILENAME, ios::binary);

    if(!file)
        return 1;

    Account acc;
    int maxID = 0;

    while(file.read((char*)&acc,sizeof(acc))) {

        if(acc.accNo > 0 && acc.accNo > maxID)
            maxID = acc.accNo;
    }

    file.close();

    return maxID + 1;
}

bool findAccount(int accNo, Account &acc, streampos &pos) {

    fstream file(FILENAME, ios::in | ios::binary);

    if(!file)
        return false;

    while(file.read((char*)&acc,sizeof(acc))) {

        streampos currentPos = file.tellg() - (streampos)sizeof(acc);

        if(acc.accNo <= 0)
            continue;

        if(acc.accNo == accNo) {
            pos = currentPos;
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}

void createAccount() {

    Account acc;
    int id = getNextAccountNumber();

    acc.createAccount(id);

    ofstream file(FILENAME, ios::binary | ios::app);

    file.write((char*)&acc,sizeof(acc));

    file.close();
}

void displayAccount() {

    cout << "Enter Account Number: ";
    int accNo;
    cin >> accNo;

    Account acc;
    streampos pos;

    if(findAccount(accNo, acc, pos))
        acc.showAccount();
    else
        cout << "Account Not Found\n";
}

void displayAllAccounts() {

    ifstream file(FILENAME, ios::binary);

    if(!file) {
        cout << "No Records Found\n";
        return;
    }

    Account acc;

    cout << "\n---- ALL ACCOUNTS ----\n";

    while(file.read((char*)&acc,sizeof(acc))) {

        if(acc.accNo <= 0) 
            continue;  

        acc.showAccount();
        cout << "----------------------\n";
    }

    file.close();
}

void modifyAccount() {

    cout << "Enter Account Number to Modify: ";
    int accNo;
    cin >> accNo;

    Account acc;
    streampos pos;

    if(!findAccount(accNo, acc, pos)) {
        cout << "Account Not Found\n";
        return;
    }

    cout << "\nExisting Details:";
    acc.showAccount();

    cout << "\nEnter New Name: ";
    string temp;
    getline(cin >> ws, temp);

    strncpy(acc.name,temp.c_str(),sizeof(acc.name)-1);
    acc.name[sizeof(acc.name)-1] = '\0';

    cout << "Enter New Type (S/C): ";
    cin >> acc.type;
    acc.type = toupper(acc.type);

    cout << "Enter New Balance: ";
    cin >> acc.balance;

    fstream file(FILENAME, ios::in | ios::out | ios::binary);

    file.seekp(pos);
    file.write((char*)&acc,sizeof(acc));

    file.close();

    cout << "Account Modified Successfully\n";
}

void deleteAccount() {

    cout << "Enter Account Number to Delete: ";
    int accNo;
    cin >> accNo;

    ifstream file(FILENAME, ios::binary);

    if(!file) {
        cout << "No Records Found\n";
        return;
    }

    ofstream temp("temp.dat", ios::binary);

    Account acc;
    bool found=false;

    while(file.read((char*)&acc,sizeof(acc))) {

        if(acc.accNo == accNo) {
            found=true;
            continue;
        }

        temp.write((char*)&acc,sizeof(acc));
    }

    file.close();
    temp.close();

    if(found) {
        remove(FILENAME);
        rename("temp.dat",FILENAME);
        cout << "Account Deleted Successfully\n";
    }
    else {
        remove("temp.dat");
        cout << "Account Not Found\n";
    }
}

void depositWithdraw(bool deposit) {

    cout << "Enter Account Number: ";
    int accNo;
    cin >> accNo;

    Account acc;
    streampos pos;

    if(!findAccount(accNo, acc, pos)) {
        cout << "Account Not Found\n";
        return;
    }

    cout << "Current Balance: " << acc.balance << "\n";

    double amount;

    if(deposit)
        cout << "Enter Amount to Deposit: ";
    else
        cout << "Enter Amount to Withdraw: ";

    cin >> amount;

    if(amount <= 0) {
        cout << "Invalid Amount\n";
        return;
    }

    if(!deposit && amount > acc.balance) {
        cout << "Insufficient Balance\n";
        return;
    }

    if(deposit)
        acc.balance += amount;
    else
        acc.balance -= amount;

    fstream file(FILENAME, ios::in | ios::out | ios::binary);

    file.seekp(pos);
    file.write((char*)&acc,sizeof(acc));

    file.close();

    cout << "Transaction Successful\n";
    cout << "New Balance: " << acc.balance << "\n";
}

int main() {
 ofstream clearFile(FILENAME, ios::trunc | ios::binary);
    clearFile.close();

    int choice;

    do {

        cout << "\n===== BANK MANAGEMENT SYSTEM =====\n";
        cout << "1. Create Account\n";
        cout << "2. Display Account\n";
        cout << "3. Display All Accounts\n";
        cout << "4. Deposit\n";
        cout << "5. Withdraw\n";
        cout << "6. Modify Account\n";
        cout << "7. Delete Account\n";
        cout << "8. Exit\n";

        cout << "Enter Choice: ";
        cin >> choice;

        switch(choice) {

            case 1: createAccount(); break;
            case 2: displayAccount(); break;
            case 3: displayAllAccounts(); break;
            case 4: depositWithdraw(true); break;
            case 5: depositWithdraw(false); break;
            case 6: modifyAccount(); break;
            case 7: deleteAccount(); break;
            case 8: cout << "Thank You\n"; break;
            default: cout << "Invalid Choice\n";
        }

    } while(choice != 8);

    return 0;
}
