#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;
#define MAX 100
#define MAXTRANS 200

// ================= ACCOUNT CLASS =================

class Account
{
    int id;
    char name[40];
    double balance;

public:
    Account()
    {
        id = 0;
        strcpy(name, "");
        balance = 0;
    }

    Account(int i, char n[], double b)
    {
        id = i;
        strcpy(name, n);
        balance = b;
    }

    int getId()
    {
        return id;
    }

    char *getName()
    {
        return name;
    }

    double getBalance()
    {
        return balance;
    }

    void setName(char n[])
{
        strcpy(name, n);
    }

    int deposit(double amount)
    {
        if (amount <= 0)
            return 0;

        balance += amount;
        return 1;
    }
 int withdraw(double amount)
    {
        if (amount <= 0 || amount > balance)
            return 0;

        balance -= amount;
        return 1;
    }
};

// ================= CARD CLASS =================

class Card
{
    char number[20];
    int accountId;
    char pin[10];

public:
    Card()
    {
        strcpy(number, "");
        accountId = 0;
        strcpy(pin, "1234");
    }

    Card(char no[], int id, char p[])
    {
        strcpy(number, no);
        accountId = id;
        strcpy(pin, p);
    }

    int getAccountId()
    {
        return accountId;
    }
char *getNumber()
    {
        return number;
    }

    int validatePin(char p[])
    {
        return strcmp(pin, p) == 0;
    }
};

// ================= TRANSACTION CLASS =================

class Transaction
{
protected:
    char type[20];
    int fromId;
    int toId;
    double amount;

public:
    Transaction()
    {
        strcpy(type, "GENERAL");
        fromId = 0;
        toId = 0;
        amount = 0;
    }

    Transaction(const char *t, int f, int to, double a)
    {
        strcpy(type, t);
        fromId = f;
 toId = to;
        amount = a;
    }

    virtual void display()
    {
        cout << type << " Amount: Rs. " << amount << "\n";
    }

    char *getType()
    {
        return type;
    }
 int getFrom()
    {
        return fromId;
    }

    int getTo()
    {
        return toId;
    }

    double getAmount()
    {
        return amount;
    }

    virtual ~Transaction() { }
};

// ================= DEPOSIT TRANSACTION =================

class DepositTransaction : public Transaction
{
public:
    DepositTransaction(int id, double a)
        : Transaction("DEPOSIT", id, id, a)
    {
    }

    void display()
    {
        cout << "Deposit into account " << fromId
             << ", Amount: Rs. " << amount << "\n";
    }
};

// ================= WITHDRAWAL TRANSACTION =================

class WithdrawalTransaction : public Transaction
{
public:
    WithdrawalTransaction(int id, double a)
        : Transaction("WITHDRAWAL", id, id, a)
    {
    }

    void display()
    {
cout << "Withdrawal from account " << fromId
             << ", Amount: Rs. " << amount << "\n";
    }
};

// ================= TRANSFER TRANSACTION =================

class TransferTransaction : public Transaction
{
public:
    TransferTransaction(int f, int t, double a)
        : Transaction("TRANSFER", f, t, a)
    {
    }

    void display()
    {
        cout << "Transfer from account " << fromId
             << " to account " << toId
             << ", Amount: Rs. " << amount << "\n";
    }
};

// ================= ATM CLASS =================

class ATM
{
public:
    int depositMoney(Account &a, double amount)
    {
        return a.deposit(amount);
    }

    int withdrawMoney(Account &a, double amount)
    {
        return a.withdraw(amount);
    }

    void checkBalance(Account &a)
    {
        cout << "Current Balance: Rs. "
             << a.getBalance() << "\n";
    }
};

// ================= FILE MANAGER CLASS =================

class FileManager
{
public:
    void saveAccounts(Account a[], int n)
    {
        ofstream out("ACCOUNTS.TXT");

        for (int i = 0; i < n; i++)
        {
            out << a[i].getId() << "|"
                << a[i].getName() << "|"
                << a[i].getBalance() << "\n";
        }

        out.close();
    }

    int loadAccounts(Account a[])
    {
        ifstream in("ACCOUNTS.TXT");

        int n = 0;
        int id;
        char name[40];
        char separator;
        double balance;

        if (!in)
            return 0;

        while (n < MAX && in >> id)
        {
            in.get(separator);
            in.getline(name, 40, '|');
            in >> balance;
            in.ignore(1000, '\n');

            a[n] = Account(id, name, balance);
            n++;
        }

        in.close();
        return n;
    }

    void saveCards(Card c[], int n)
    {
        ofstream out("CARDS.TXT");

        for (int i = 0; i < n; i++)
        {
            out << c[i].getNumber() << "|"
                << c[i].getAccountId() << "\n";
        }

        out.close();
    }

    int loadCards(Card c[])
    {
        ifstream in("CARDS.TXT");

        int n = 0;
        int id;
        char number[20];
    char pin[10];

        if (!in)
            return 0;

        while (n < MAX && in.getline(number, 20, '|'))
        {
            if (!(in >> id))
                break;

            in.ignore(1000, '\n');

            strcpy(pin, "1234");
            c[n] = Card(number, id, pin);
            n++;
        }

        in.close();
        return n;
    }

    void saveTransactions(Transaction *t[], int n)
    {
        ofstream out("TRANS.TXT");

        for (int i = 0; i < n; i++)
        {
            out << t[i]->getType() << " "
                << t[i]->getFrom() << " "
                << t[i]->getTo() << " "
                << t[i]->getAmount() << "\n";
        }

        out.close();
    }

    void saveSnapshot(Account a[], int accountCount,
                      Card c[], int cardCount,
                      Transaction *t[], int transactionCount)
    {
        ofstream out("ATM_DATA.TXT");

        out << "ACCOUNTS " << accountCount << "\n";
        for (int i = 0; i < accountCount; i++)
        {
            out << a[i].getId() << "|"
                << a[i].getName() << "|"
                << a[i].getBalance() << "\n";
        }

        out << "CARDS " << cardCount << "\n";
        for (int i = 0; i < cardCount; i++)
        {
            out << c[i].getNumber() << "|"
                << c[i].getAccountId() << "\n";
        }

        out << "TRANSACTIONS " << transactionCount << "\n";
        for (int i = 0; i < transactionCount; i++)
        {
            out << t[i]->getType() << " "
                << t[i]->getFrom() << " "
                << t[i]->getTo() << " "
                << t[i]->getAmount() << "\n";
        }
    }

    void displaySavedFiles()
    {
        const char *fileNames[] = {"ACCOUNTS.TXT", "CARDS.TXT", "TRANS.TXT"};

        for (int i = 0; i < 3; i++)
        {
            ifstream in(fileNames[i]);
            cout << "\n--- " << fileNames[i] << " ---\n";

            if (!in)
            {
                cout << "Could not open file.\n";
                continue;
            }

            char line[256];
            bool hasLine = false;
            while (in.getline(line, sizeof(line)))
            {
                cout << line << "\n";
                hasLine = true;
            }

            if (!hasLine)
                cout << "(empty)\n";
        }
    }

    int loadTransactions(Transaction *t[])
    {
        ifstream in("TRANS.TXT");

        char type[20];
        int from, to, n = 0;
        double amount;

        if (!in)
            return 0;

        while (n < MAXTRANS &&
               in >> type >> from >> to >> amount)
        {
            if (strcmp(type, "DEPOSIT") == 0)
                t[n++] = new DepositTransaction(from, amount);

            else if (strcmp(type, "WITHDRAWAL") == 0)
                t[n++] = new WithdrawalTransaction(from, amount);

            else if (strcmp(type, "TRANSFER") == 0)
                t[n++] = new TransferTransaction(from, to, amount);

            else
                t[n++] = new Transaction(type, from, to, amount);
        }

        in.close();
        return n;
    }
};

// ================= ATM SYSTEM CLASS =================

class ATMSystem
{
    Account *accounts;
    Card *cards;
    Transaction *transactions[MAXTRANS];

    int accountCount;
    int cardCount;
    int transactionCount;

    ATM atm;
    FileManager fm;

    int findAccount(int id)
    {
        for (int i = 0; i < accountCount; i++)
        {
            if (accounts[i].getId() == id)
                return i;
        }

        return -1;
    }

    int nextId()
    {
        int largest = 1000;

        for (int i = 0; i < accountCount; i++)
        {
            if (accounts[i].getId() > largest)
                largest = accounts[i].getId();
        }

        return largest + 1;
    }

    void addTransaction(Transaction *t)
    {
        if (transactionCount < MAXTRANS)
            transactions[transactionCount++] = t;
        else
            delete t;
    }

public:
    ATMSystem()
    {
 accounts = new Account[MAX];
        cards = new Card[MAX];

        accountCount = 0;
        cardCount = 0;
        transactionCount = 0;
    }

    ~ATMSystem()
    {
        for (int i = 0; i < transactionCount; i++)
            delete transactions[i];

        delete[] accounts;
        delete[] cards;
    }

    void loadData()
    {
        accountCount = fm.loadAccounts(accounts);
        cardCount = fm.loadCards(cards);
        transactionCount = fm.loadTransactions(transactions);
    }

    void saveData()
    {
        fm.saveAccounts(accounts, accountCount);
        fm.saveCards(cards, cardCount);
        fm.saveTransactions(transactions, transactionCount);
        fm.saveSnapshot(accounts, accountCount, cards, cardCount,
                        transactions, transactionCount);

        cout << "\nData saved successfully to ATM_DATA.TXT.\n";
        fm.displaySavedFiles();
    }

    // OPTION 1: ADD RECORD

    void addRecord()
    {
        char name[40], cardNo[20], pin[10];
        double opening;

        if (accountCount >= MAX || cardCount >= MAX)
        {
            cout << "Storage is full.\n";
            return;
        }

        cout << "Enter customer name: ";
        cin.getline(name, 40);

        if (strlen(name) == 0 || strchr(name, '|') != NULL)
        {
            cout << "Invalid name.\n";
            return;
        }

        cout << "Enter opening balance: ";
        cin >> opening;
        cin.ignore(1000, '\n');

        if (cin.fail() || opening < 0)
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid balance.\n";
            return;
        }

        int id = nextId();

        accounts[accountCount++] = Account(id, name, opening);

        sprintf(cardNo, "CARD%d", id);
        strcpy(pin, "1234");

        cards[cardCount++] = Card(cardNo, id, pin);

        cout << "\nAccount created successfully!\n";
        cout << "Account ID: " << id << "\n";
        cout << "Card Number: " << cardNo << "\n";
        cout << "Demo PIN: 1234\n";

        if (opening > 0)
            addTransaction(new DepositTransaction(id, opening));

        saveData();
    }

    // OPTION 2: DISPLAY RECORDS

    void displayRecords()
    {
        if (accountCount == 0)
        {
            cout << "No records found.\n";
            return;
        }

        cout << "\nACCOUNT RECORDS\n";
        cout << "ID\tName\t\tBalance\n";

        for (int i = 0; i < accountCount; i++)
        {
            cout << accounts[i].getId() << "\t"
                 << accounts[i].getName() << "\t\tRs. "
                 << accounts[i].getBalance() << "\n";
        }
    }

    // OPTION 3: SEARCH RECORD

    void searchRecord()
    {
        int id;

        cout << "Enter account ID: ";
        cin >> id;
        cin.ignore(1000, '\n');

        int i = findAccount(id);

        if (i == -1)
        {
            cout << "Account not found.\n";
            return;
        }

        cout << "\nAccount ID: " << accounts[i].getId();
        cout << "\nName: " << accounts[i].getName();
        cout << "\nBalance: Rs. " << accounts[i].getBalance() << "\n";
    }

    // OPTION 4: UPDATE RECORD

    void updateRecord()
    {
        int id;
        char name[40];

        cout << "Enter account ID: ";
        cin >> id;
        cin.ignore(1000, '\n');

        int i = findAccount(id);

        if (i == -1)
        {
            cout << "Account not found.\n";
            return;
        }

        cout << "Enter new customer name: ";
        cin.getline(name, 40);

        if (strlen(name) == 0 || strchr(name, '|') != NULL)
 {
            cout << "Invalid name.\n";
            return;
        }

        accounts[i].setName(name);

        saveData();
        cout << "Record updated successfully.\n";
    }

    // OPTION 5: DELETE RECORD

    void deleteRecord()
    {
        int id;

        cout << "Enter account ID: ";
        cin >> id;
        cin.ignore(1000, '\n');

        int i = findAccount(id);

        if (i == -1)
        {
            cout << "Account not found.\n";
            return;
        }

        if (accounts[i].getBalance() != 0)
        {
            cout << "Withdraw all money before deletion.\n";
            return;
        }

        for (int j = i; j < accountCount - 1; j++)
            accounts[j] = accounts[j + 1];

        accountCount--;

        for (i = 0; i < cardCount; i++)
        {
            if (cards[i].getAccountId() == id)
            {
                for (int j = i; j < cardCount - 1; j++)
                    cards[j] = cards[j + 1];

                cardCount--;
break;
            }
        }

        saveData();
        cout << "Account deleted successfully.\n";
    }

    // OPTION 6: ATM TRANSACTIONS

    void transactionMenu()
    {
        int id, idx, valid = 0, choice;
        int toId, toIdx;
        char pin[10];
        double amount;

        cout << "Enter account ID: ";
        cin >> id;

        idx = findAccount(id);

        if (idx == -1)
        {
            cout << "Account not found.\n";
            cin.ignore(1000, '\n');
            return;
        }

        cout << "Enter PIN (1234): ";
        cin >> pin;

        for (int i = 0; i < cardCount; i++)
        {
            if (cards[i].getAccountId() == id &&
                cards[i].validatePin(pin))
                valid = 1;
        }

        if (!valid)
        {
            cout << "Invalid PIN.\n";
            cin.ignore(1000, '\n');
            return;
        }

        cout << "\n1. Check Balance";
        cout << "\n2. Deposit";
        cout << "\n3. Withdraw";
        cout << "\n4. Transfer";
        cout << "\nEnter choice: ";
        cin >> choice;

        if (choice == 1)
        {
            atm.checkBalance(accounts[idx]);
        }
        else if (choice == 2 || choice == 3)
        {
            cout << "Enter amount: ";
            cin >> amount;

            if (cin.fail() || amount <= 0)
            {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid amount.\n";
                return;
            }

            if (choice == 2 &&
                atm.depositMoney(accounts[idx], amount))
            {
                addTransaction(new DepositTransaction(id, amount));
                cout << "Deposit successful.\n";
                saveData();
            }
            else if (choice == 3 &&
                     atm.withdrawMoney(accounts[idx], amount))
            {
                addTransaction(new WithdrawalTransaction(id, amount));
                cout << "Withdrawal successful.\n";
                saveData();
            }
            else
            {
                cout << "Transaction failed. Check balance.\n";
            }
        }
        else if (choice == 4)
        {
            cout << "Enter destination account ID: ";
            cin >> toId;

            toIdx = findAccount(toId);

            cout << "Enter amount: ";
            cin >> amount;

            if (toIdx == -1 || toIdx == idx)
            {
                cout << "Invalid destination account.\n";
            }
            else if (amount <= 0)
            {
                cout << "Amount must be positive.\n";
            }
            else if (atm.withdrawMoney(accounts[idx], amount))
            {
                accounts[toIdx].deposit(amount);

                addTransaction(new TransferTransaction(id, toId, amount));

                cout << "Transfer successful.\n";
                saveData();
            }
            else
            {
                cout << "Insufficient balance.\n";
            }
        }
        else
        {
            cout << "Invalid choice.\n";
        }

        cin.ignore(1000, '\n');
    }

    // OPTION 7: REPORT

    void report()
    {
        double total = 0;

        cout << "\n========== ATM REPORT ==========\n";
        cout << "Total accounts: " << accountCount << "\n";

        for (int i = 0; i < accountCount; i++)
            total += accounts[i].getBalance();

        cout << "Total balance: Rs. " << total << "\n";

        cout << "\nTRANSACTION HISTORY\n";

        for (int i = 0; i < transactionCount; i++)
        {
            cout << i + 1 << ". ";
            transactions[i]->display();
        }
    }

    // MAIN MENU

    void menu()
    {
        int choice, sub;

        loadData();

        do
        {
            

            cout << "====================================\n";
            cout << "      ATM MANAGEMENT SYSTEM\n";
            cout << "====================================\n";
            cout << "1. Add Record\n";
            cout << "2. Display Records\n";
            cout << "3. Search Record\n";
            cout << "4. Update Record\n";
            cout << "5. Delete Record\n";
            cout << "6. Main Transaction/Process\n";
            cout << "7. Report\n";
            cout << "8. Save/Load\n";
            cout << "9. Exit\n";
            cout << "====================================\n";
            cout << "Enter your choice: ";

            cin >> choice;

            if (cin.fail())
            {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Enter a valid number.\n";
                choice = 0;
                continue;
            }

            cin.ignore(1000, '\n');

            switch (choice)
            {
                case 1:
                    addRecord();
                    break;

                case 2:
                    displayRecords();
                    break;

                case 3:
                    searchRecord();
                    break;

                case 4:
                    updateRecord();
                    break;

                case 5:
                    deleteRecord();
                    break;

                case 6:
                    transactionMenu();
                    break;

                case 7:
                    report();
                    break;

                case 8:
                    cout << "1. Save Data\n";
                    cout << "2. Reload Data\n";
                    cout << "Enter choice: ";
                    cin >> sub;
                    cin.ignore(1000, '\n');

                    if (sub == 1)
                        saveData();
                    else if (sub == 2)
                    {
                        loadData();
                        cout << "Data loaded successfully.\n";
                    }
                    else
                        cout << "Invalid choice.\n";
                    break;

                case 9:
                    saveData();
                    cout << "Thank you for using the ATM.\n";
                    break;

                default:
                    cout << "Invalid choice. Select 1 to 9.\n";
            }

            if (choice != 9)
            {
                cout << "\nPress any key to return to menu...";
            }

        } while (choice != 9);
    }
};

// ================= MAIN FUNCTION =================

int main()
{

    ATMSystem system;

    system.menu();

    cout << "\nPress any key to exit...";

    return 0;
}

