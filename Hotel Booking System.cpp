/***
//// Last Build : 27_11_2020
//// Compiler   : VS2019
//// Programmer : Chong Bin Yong
//// Capability : Implementing a variable size record.
//// Function   : add, search, update, delete, display records
***/

/////////////     Includes    /////////////

#include <cstdlib> // system()
#include <conio.h> // getch()
#include <fstream>
#include <sys\stat.h> // stat(status of a file)
#include <iostream>
#include <iomanip>
using namespace std;

/////////////     Data types    /////////////

enum {SINGLE = '1', DOUBLE = '2', SUITE = '3'};

struct Reservation_Data // Reservation data
{
    char ROOM_TYPE; // Tag_Field : "SINGLE = 1, DOUBLE = 2, SUITE = 3"

    union
    {
        struct //// SINGLE
        {
            char CheckInDate[15];
            int Days;
        };

        struct //// DOUBLE
        {
            char CheckInDate[15];
            int Days;
        };

        struct ////SUITE
        {
            char CheckInDate[15];
            int Days;
        };
    };

    int ID;
    char FirstName[15];
    char LastName[15];
    char Gender[15];
    int Age;
    char PhoneNumber[15];
    char IdentityCardNumber[15];
    bool deleted;
};


/////////////     Variable Declarations & Constants /////////////

#define ORDER_FILE_NAME "Reservation data.txt" // name of the database file to store reservation order informations

Reservation_Data Order;
char     choice; // for choice in menu
fstream* fs = NULL, * fs1 = NULL;// file streams for files : fs -> 'Order', fs1 -> 'temp'
bool     deletion = false; // if any record has been deleted


/////////////     Function Prototypes    /////////////

void closeFile(fstream*); // closes a file with its pointer, then deletes the file pointer
bool isFileExist(const char*); // check if a file exists


/////////////     Main    /////////////
int main()
{
    while (true)
    {
        do ////// Menu //////
        {
            system("cls"); // clear screen

            cout << "\n --Welcome to Hotel Booking System-- \n\n";
            cout << " 1. Add a new Record \n";
            cout << " 2. Search an existing Record \n";
            cout << " 3. Update an existing Record \n";
            cout << " 4. Delete an existing Record \n";
            cout << " 5. Display Records \n";
            cout << " 6. Exit \n\n";
            cout << " Please enter your choice (1-6) : " << flush;
            choice = _getch();
        } 
        while (choice < '1' || choice > '6'); // while we have no good(between 1 and 6), show menu again

        system("cls");
        _getch();

        // to search, update, delete or display records, database file should exist, then we have some records           
        if (choice == '2' || choice == '3' || choice == '4' || choice == '5')
        {
            if (!isFileExist(ORDER_FILE_NAME)) // if database file doesn't exist
            {
                cout << "\n Database file ('" << ORDER_FILE_NAME << "') doesn't exist, then there are no records." << endl;
                system("pause");
                continue; // show the menu again
            }
        }

        switch (choice)
        {
            int recs_num; // number of records before the record for modifying(deletion)
            int id;

        case '1': ////// Add Record //////
            
            cout << "\n < Entering a new record > \n";
            cout << "\n Enter the following informations for the new record : ";
            cout << "\n\n Room Type ( SINGLE : 1, DOUBLE : 2, SUITE : 3 ) : ";
            cin >> Order.ROOM_TYPE;

            cout << "\n Order ID : ";
            cin >> Order.ID;
            cout << "\n First Name : ";
            cin.ignore();
            cin.getline(Order.FirstName, 15);
            cout << "\n Last Name : ";
            cin.getline(Order.LastName, 15);
            cout << "\n Gender (Male or Female) : ";
            cin >> Order.Gender;
            cout << "\n Age : ";
            cin >> Order.Age;
            cout << "\n Phone Number (E.g : 012-3456789) : ";
            cin >> Order.PhoneNumber;
            cout << "\n Identity Card Number (E.g : YYMMDD-PB-###G) : ";
            cin.ignore();
            cin.getline(Order.IdentityCardNumber, 15);

            if (Order.ROOM_TYPE == SINGLE)
            {
                int total;
                int single = 300;
                int service = 30;

                cout << "\n Check-In Date (E.g : DD-MM-YYYY) : ";
                cin >> Order.CheckInDate;
                cout << "\n Days : ";
                cin >> Order.Days;
                total = (single + service) * Order.Days;
                cout << "\n Total Bill : " << total << "\n";
            }

            if (Order.ROOM_TYPE == DOUBLE)
            {
                int total;
                int doubl = 450;
                int service = 45;

                cout << "\n Check-In Date (E.g : DD-MM-YYYY) : ";
                cin >> Order.CheckInDate;
                cout << "\n Days : ";
                cin >> Order.Days;
                total = (doubl + service) * Order.Days;
                cout << "\n Total Bill : " << total << "\n";
            }

            if (Order.ROOM_TYPE == SUITE)
            {
                int total;
                int suite = 600;
                int service = 60;

                cout << "\n Check-In Date (E.g : DD-MM-YYYY) : ";
                cin >> Order.CheckInDate;
                cout << "\n Days : ";
                cin >> Order.Days;
                total = (suite + service) * Order.Days;
                cout << "\n Total Bill : " << total << "\n";
            }

            Order.deleted = 0;

            fs = new fstream(ORDER_FILE_NAME, ios::out | ios::app | ios::binary);

            if (!fs)
            {
                cout << "\n Can't open or create '" << ORDER_FILE_NAME << "' file" << endl;
                system("pause");
                
                break;
            }

            fs->write((char*)&Order, sizeof(Order));

            closeFile(fs);

            cout << "\n Record added." << endl;
            system("pause");
            
            break;

        case '2': ////// Search Record //////

            cout << "\n < Search existing record > \n";
            cout << "\n Enter the order ID that you want to search its information : ";
            cin >> id;

            fs = new fstream(ORDER_FILE_NAME, ios::in | ios::binary);

            if (!fs)
            {
                cout << "\n Can't open or create '" << ORDER_FILE_NAME << "' file." << endl;
                system("pause");
                
                break;
            }

            cout << "\n\t\t\t\t\t\t\t\t     < RECORD >\n\n";
            cout << "Order ID\tFirst Name\tLast Name\tGender\t\tAge\tPhone Number\tIdentity Card Number\tCheck-In Date\tDays\tRoom Type\n"
                << "------------------------------------------------------------------------------------------------------------------------------------------------- \n";

            while (fs->read((char*)&Order, sizeof(Reservation_Data)))
            {
                if (Order.ID == id && !Order.deleted)
                {
                    cout << Order.ID << setw(5) << "\t\t";
                    cout << Order.FirstName << setw(5) << "\t";
                    cout << Order.LastName << setw(5) << "\t";
                    cout << Order.Gender << setw(5) << "\t";
                    cout << Order.Age << setw(5) << "\t";
                    cout << Order.PhoneNumber << setw(5) << "\t";
                    cout << Order.IdentityCardNumber << setw(5) << "\t";
                    cout << Order.CheckInDate << setw(5) << "\t";
                    cout << Order.Days << setw(5) << "\t";
                    cout << Order.ROOM_TYPE << setw(5) << "\n";
                }
            }

            cout << "\n\n - Room Type ( SINGLE : 1, DOUBLE : 2, SUITE : 3 )";

            cout << "\n\n To see menu, ";
            system("pause");

            closeFile(fs);

            break;


        case '3': ////// Update Record //////

            int a;
            int b;

            cout << "\n < Update existing record > \n";
            cout << "\n Enter the order ID that you want to update its information : ";
            cin >> id;

            fs = new fstream(ORDER_FILE_NAME, ios::in | ios::out | ios::binary);

            if (!fs)
            {
                cout << "\n Can't open or create '" << ORDER_FILE_NAME << "' file" << endl;
                system("pause");

                break;
            }

            recs_num = -1;

            while (fs->read((char*)&Order, sizeof(Order)))
            {
                recs_num++;

                if (Order.ID == id && !Order.deleted)
                break;
            }

            if (fs->eof()) // if (the record is not in the file || it's there but it's deleted)
            {
                cout << "\n Your specified order doesn't exist in file." << endl;
                closeFile(fs);
                system("pause");
                break;
            }

            cout << "\n\n 1. Update specific information";
            cout << "\n 2. Update all informations";
            cout << "\n\n Please enter your choice (1 or 2) : ";
            cin >> a;

            if (a == 1)
            {
                cout << "\n\n 1. Room Type";
                cout << "\n 2. Order ID";
                cout << "\n 3. First Name";
                cout << "\n 4. Last Name";
                cout << "\n 5. Gender";
                cout << "\n 6. Age";
                cout << "\n 7. Phone Number";
                cout << "\n 8. Identity Card Number";
                cout << "\n 9. Check-In Date";
                cout << "\n 10. Days";
                cout << "\n\n Please enter your choice (1-10) : ";
                cin >> b;
                                
                switch (b)
                {
                    case 1:
                    {
                        cout << "\n\n Enter new information for this record : ";
                        cout << "\n\n Room Type ( SINGLE : 1, DOUBLE : 2, SUITE : 3 ) : ";
                        cin >> Order.ROOM_TYPE;

                        if (Order.ROOM_TYPE == SINGLE)
                        {
                            int total;
                            int single = 300;
                            int service = 30;

                            total = (single + service) * Order.Days;
                            cout << "\n Total Bill : " << total;
                        }

                        if (Order.ROOM_TYPE == DOUBLE)
                        {
                            int total;
                            int doubl = 450;
                            int service = 45;

                            total = (doubl + service) * Order.Days;
                            cout << "\n Total Bill : " << total;
                        }

                        if (Order.ROOM_TYPE == SUITE)
                        {
                            int total;
                            int suite = 600;
                            int service = 60;

                            total = (suite + service) * Order.Days;
                            cout << "\n Total Bill : " << total;
                        }
                        break;
                    }

                    case 2:
                    {
                        cout << "\n\n Enter new information for this record : ";
                        cout << "\n\n Order ID : ";
                        cin >> Order.ID;
                        break;
                    }

                    case 3:
                    {
                        cout << "\n\n Enter new information for this record : ";
                        cout << "\n\n First Name : ";
                        cin.ignore();
                        cin.getline(Order.FirstName, 15);
                        break;
                    }

                    case 4:
                    {
                        cout << "\n\n Enter new information for this record : ";
                        cout << "\n\n Last Name : ";
                        cin.ignore();
                        cin.getline(Order.LastName, 15);
                        break;
                    }

                    case 5:
                    {
                        cout << "\n\n Enter new information for this record : ";
                        cout << "\n\n Gender (Male or Female) : ";
                        cin >> Order.Gender;
                        break;
                    }

                    case 6:
                    {
                        cout << "\n\n Enter new information for this record : ";
                        cout << "\n\n Age : ";
                        cin >> Order.Age;
                        break;
                    }

                    case 7:
                    {
                        cout << "\n\n Enter new information for this record : ";
                        cout << "\n\n Phone Number (E.g : 012-3456789) : ";
                        cin >> Order.PhoneNumber;
                        break;
                    }

                    case 8:
                    {
                        cout << "\n\n Enter new information for this record : ";
                        cout << "\n\n Identity Card Number (E.g : YYMMDD-PB-###G) : ";
                        cin.ignore();
                        cin.getline(Order.IdentityCardNumber, 15);
                        break;
                    }

                    case 9:
                    {
                        cout << "\n\n Enter new information for this record : ";
                        cout << "\n\n Check-In Date (E.g : DD-MM-YYYY) : ";
                        cin >> Order.CheckInDate;
                        break;
                    }

                    case 10:
                    {
                        cout << "\n\n Enter new information for this record : ";

                        if (Order.ROOM_TYPE == SINGLE)
                        {
                            int total;
                            int single = 300;
                            int service = 30;
                                                        
                            cout << "\n\n Days : ";
                            cin >> Order.Days;
                            total = (single + service) * Order.Days;
                            cout << "\n Total Bill : " << total;
                        }

                        if (Order.ROOM_TYPE == DOUBLE)
                        {
                            int total;
                            int doubl = 450;
                            int service = 45;

                            cout << "\n\n Days : ";
                            cin >> Order.Days;
                            total = (doubl + service) * Order.Days;
                            cout << "\n Total Bill : " << total;
                        }

                        if (Order.ROOM_TYPE == SUITE)
                        {
                            int total;
                            int suite = 600;
                            int service = 60;

                            cout << "\n\n Days : ";
                            cin >> Order.Days;
                            total = (suite + service) * Order.Days;
                            cout << "\n Total Bill : " << total;
                        }
                        break;
                    }

                    default:
                    {
                        cout << "\n\n The choice is not valid, please try again. \n";
                        break;
                    }
                }
            }
            else if (a == 2)
            {
                cout << "\n\n Enter new informations for this record : ";
                cout << "\n\n Room Type ( SINGLE : 1, DOUBLE : 2, SUITE : 3 ) : ";
                cin >> Order.ROOM_TYPE;

                cout << "\n Order ID : ";
                cin >> Order.ID;
                cout << "\n First Name : ";
                cin.ignore();
                cin.getline(Order.FirstName, 15);
                cout << "\n Last Name : ";
                cin.getline(Order.LastName, 15);
                cout << "\n Gender (Male or Female) : ";
                cin >> Order.Gender;
                cout << "\n Age : ";
                cin >> Order.Age;
                cout << "\n Phone Number (E.g : 012-3456789) : ";
                cin >> Order.PhoneNumber;
                cout << "\n Identity Card Number (E.g : YYMMDD-PB-###G) : ";
                cin.ignore();
                cin.getline(Order.IdentityCardNumber, 15);

                if (Order.ROOM_TYPE == SINGLE)
                {
                    int total;
                    int single = 300;
                    int service = 30;

                    cout << "\n Check-In Date (E.g : DD-MM-YYYY) : ";
                    cin >> Order.CheckInDate;
                    cout << "\n Days : ";
                    cin >> Order.Days;
                    total = (single + service) * Order.Days;
                    cout << "\n Total Bill : " << total;
                }

                if (Order.ROOM_TYPE == DOUBLE)
                {
                    int total;
                    int doubl = 450;
                    int service = 45;

                    cout << "\n Check-In Date (E.g : DD-MM-YYYY) : ";
                    cin >> Order.CheckInDate;
                    cout << "\n Days : ";
                    cin >> Order.Days;
                    total = (doubl + service) * Order.Days;
                    cout << "\n Total Bill : " << total;
                }

                if (Order.ROOM_TYPE == SUITE)
                {
                    int total;
                    int suite = 600;
                    int service = 60;

                    cout << "\n Check-In Date (E.g : DD-MM-YYYY) : ";
                    cin >> Order.CheckInDate;
                    cout << "\n Days : ";
                    cin >> Order.Days;
                    total = (suite + service) * Order.Days;
                    cout << "\n Total Bill : " << total;
                }
            }
            else
            {
                cout << "\n\n The choice is not valid, please try again. \n";
            }
            

            fs->seekp(sizeof(Order) * recs_num, ios::beg); // go to the first of the record to be modified
            fs->write((char*)&Order, sizeof(Order));

            closeFile(fs);

            cout << "\n\n Record is updated." << endl;
            system("pause");

            break;


        case '4': ////// Delete Record //////

            cout << "\n < Delete existing record > \n";
            cout << "\n Enter the order ID for deletion : ";
            cin >> id;

            fs = new fstream(ORDER_FILE_NAME, ios::in | ios::out | ios::binary);

            if (!fs)
            {
                cout << "\n Can't open or create '" << ORDER_FILE_NAME << "' file." << endl;
                system("pause");
                
                break;
            }

            recs_num = -1;

            while (fs->read((char*)&Order, sizeof(Order)))
            {
                recs_num++;

                if (Order.ID == id && !Order.deleted) // if user deleted a order then added another one with the same ID in the same instance of program runs, deleted employee is still there, then we should go through all the file
                break;
            }

            if (fs->eof()) // if (the record is not in the file || it's there but it's deleted)
            {
                cout << "\n Your specified order doesn't exist in database file." << endl;
                closeFile(fs);
                system("pause");
                break;
            }

            Order.deleted = 1;

            fs->seekp(sizeof(Order) * recs_num, ios::beg);
            fs->write((char*)&Order, sizeof(Order));

            closeFile(fs);

            deletion = true; // we have some deleted records

            cout << "\n Record is deleted." << endl;
            system("pause");

            break;


        case '5': // Display Records

              ////// Print single room records...
            fs = new fstream(ORDER_FILE_NAME, ios::in | ios::binary);

            if (!fs)
            {
                cout << "\n Can't open or create '" << ORDER_FILE_NAME << "' file." << endl;
                system("pause");
                
                break;
            }

            // display column titles
            cout << "\n\t\t\t\t\t\t\t     < SINGLE >\n\n";
            cout << "Order ID\tFirst Name\tLast Name\tGender\t\tAge\tPhone Number\tIdentity Card Number\tCheck-In Date\tDays\n"
                << "------------------------------------------------------------------------------------------------------------------------------------ \n";

            while (fs->read((char*)&Order, sizeof(Order))) // display records
            {
                if (Order.ROOM_TYPE == SINGLE && !Order.deleted)
                {
                    cout << Order.ID << setw(5) << "\t\t";
                    cout << Order.FirstName << setw(5) << "\t";
                    cout << Order.LastName << setw(5) << "\t";
                    cout << Order.Gender << setw(5) << "\t";
                    cout << Order.Age << setw(5) << "\t";
                    cout << Order.PhoneNumber << setw(5) << "\t";
                    cout << Order.IdentityCardNumber << setw(5) << "\t";
                    cout << Order.CheckInDate << setw(5) << "\t";
                    cout << Order.Days << setw(5) << "\n";
                }
            }

            cout << "\n To see double room records, ";
            system("pause");

            closeFile(fs);

            ////// Print double room records...                 
            system("cls");

            fs = new fstream(ORDER_FILE_NAME, ios::in | ios::binary);

            if (!fs)
            {
                cout << "\n Can't open or create '" << ORDER_FILE_NAME << "' file." << endl;
                system("pause");
                
                break;
            }

            cout << "\n\t\t\t\t\t\t\t     < DOUBLE >\n\n";
            cout << "Order ID\tFirst Name\tLast Name\tGender\t\tAge\tPhone Number\tIdentity Card Number\tCheck-In Date\tDays\n"
                << "------------------------------------------------------------------------------------------------------------------------------------ \n";

            while (fs->read((char*)&Order, sizeof(Reservation_Data)))
            {
                if (Order.ROOM_TYPE == DOUBLE && !Order.deleted)
                {
                    cout << Order.ID << setw(5) << "\t\t";
                    cout << Order.FirstName << setw(5) << "\t";
                    cout << Order.LastName << setw(5) << "\t";
                    cout << Order.Gender << setw(5) << "\t";
                    cout << Order.Age << setw(5) << "\t";
                    cout << Order.PhoneNumber << setw(5) << "\t";
                    cout << Order.IdentityCardNumber << setw(5) << "\t";
                    cout << Order.CheckInDate << setw(5) << "\t";
                    cout << Order.Days << setw(5) << "\n";
                }
            }

            cout << "\n To see suite room records, ";
            system("pause");

            closeFile(fs);

            ////// Print suite room records...                 
            system("cls");

            fs = new fstream(ORDER_FILE_NAME, ios::in | ios::binary);

            if (!fs)
            {
                cout << "\n Can't open or create '" << ORDER_FILE_NAME << "' file." << endl;
                system("pause");
                break;
            }

            cout << "\n\t\t\t\t\t\t\t     < SUITE >\n\n";
            cout << "Order ID\tFirst Name\tLast Name\tGender\t\tAge\tPhone Number\tIdentity Card Number\tCheck-In Date\tDays\n"
                << "------------------------------------------------------------------------------------------------------------------------------------ \n";

            while (fs->read((char*)&Order, sizeof(Reservation_Data)))
            {
                if (Order.ROOM_TYPE == SUITE && !Order.deleted)
                {
                    cout << Order.ID << setw(5) << "\t\t";
                    cout << Order.FirstName << setw(5) << "\t";
                    cout << Order.LastName << setw(5) << "\t";
                    cout << Order.Gender << setw(5) << "\t";
                    cout << Order.Age << setw(5) << "\t";
                    cout << Order.PhoneNumber << setw(5) << "\t";
                    cout << Order.IdentityCardNumber << setw(5) << "\t";
                    cout << Order.CheckInDate << setw(5) << "\t";
                    cout << Order.Days << setw(5) << "\n";
                }
            }

            cout << "\n To see menu, ";
            system("pause");

            closeFile(fs);

            break;


        case '6': // Exit

            if (deletion) // if there is any deletion, then update database file (create a new temp file that doesn't have deleted records, then remove the old database file and rename temp file to database file name)
            {
                cout << "\n Updating '" << ORDER_FILE_NAME << "' File..." << endl;

                fs = new fstream(ORDER_FILE_NAME, ios::in | ios::binary);
                if (!fs)
                {
                    cout << "\n Can't open '" << ORDER_FILE_NAME << "' file, then Updating is incomplete." << endl;
                    system("pause");

                    system("cls");
                    return 1;
                }

                fs1 = new fstream("temp", ios::out | ios::binary);
                if (!fs1)
                {
                    cout << "\n Can't create temp file, then Updating is incomplete." << endl;
                    system("pause");

                    closeFile(fs);
                    system("cls");
                    return 1;
                }

                // write nondeleted records to the temp file
                while (fs->read((char*)&Order, sizeof(Order)))
                    if (!Order.deleted)
                        fs1->write((char*)&Order, sizeof(Order));

                closeFile(fs);
                closeFile(fs1);

                if (remove(ORDER_FILE_NAME) == -1) // if there is an error
                {
                    cout << "\n Can't delete '" << ORDER_FILE_NAME << "' file, then Updating is incomplete." << endl;
                    system("pause");

                    system("cls");
                    return 1;
                }


                struct stat st; // to check size of the temp file
                int res = stat("temp", &st);

                if (st.st_size == 0) // if all of records are deleted then the temp file size is zero                        
                    remove("temp"); // we have no records, then no database file is needed, just delete the temp file
                else
                    if (rename("temp", ORDER_FILE_NAME))
                    {
                        cout << "\n Can't rename temp file, then Updating is incomplete." << endl;
                        system("pause");

                        system("cls");
                        return 1;
                    }

                cout << "\n Updating database file completed." << endl;
                system("pause");
            }

            system("cls");
            return 0;

            break;
        } // end 'switch'
    } // end 'while'

    return 0;
} // end 'main()'


/////////////     Function Definitions    /////////////

void closeFile(fstream* fs)
{
    fs->close(); // close the file
    delete fs;
    fs = NULL;
}

bool isFileExist(const char* file_name)
{
    struct stat st; // to check status of file
    int res = stat(file_name, &st);
    return (res == 0); // if file exists     
}
