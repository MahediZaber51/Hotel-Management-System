/*
    * Project Name: Hotel Management System
    * Description : A complete solution to manage your hotel with multiple admins and a super admin.
                    Where only the super admin can add new admins. All admins can manage the rooms,
                    book a room for the customers, check all rooms, edit price of the rooms, add new
                    customers, and vacate a room when customers leave!
    * Developers  : Mahedi Zaman Zaber, Afsana Meem, Shanjida Afrin, Sumayia Akter, Sumiaya Afrin
    * University  : IUBAT - International University of Business Agriculture and Technology
    * Course      : Programming in C++ Lab (CSC 284)
*/

#include <iostream>
#include <vector>
#include <string>
using namespace std;

// ANSI Colors
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"


// Global Variable
bool loggedIn = false;
bool superAdmin = false;

class Room
{
    string roomNumber;
    bool isBooked;
    int customerID;
    double roomPrice;
    int roomFloor;
public:
    // Constructor
    Room(string number,double price,int floor)
    {
        roomNumber= number;
        isBooked = false;
        customerID = -1;
        roomPrice = price;
        roomFloor = floor;
    }

    string getRoomNumber()
    {
        return roomNumber;
    }

    int getCustomerID()
    {
        return customerID;
    }
    double getPrice()
    {
        return roomPrice;
    }
    void setPrice(double price)
    {
        roomPrice = price;
    }

    bool getIsBooked()
    {
        return isBooked;
    }

    void book(int customer)
    {
        isBooked = true;
        customerID = customer;
    }

    void vacate()
    {
        isBooked = false;
        customerID = -1;
    }

    void getDetails()
    {
        cout << "Room No: "<< roomNumber<<endl;
        cout << "Floor No: "<< roomFloor<<endl;
        cout << "Price ($): "<< roomPrice<<endl;
        cout << "Available: ";
        if(isBooked)
        {
            cout <<RED<< "No"<<RESET<<endl;
        }
        else
        {
            cout <<GREEN<< "Yes"<<RESET<<endl;
        }
    }
};



class Customer
{
    static int lastID;
    int customerID;
    string customerName;
    string customerAddress;
    string customerPhone;
    string customerEmail;
public:
    // Constructor
    Customer()
    {
        customerID = ++lastID;
        cout << "Customer ID: " << customerID <<endl;
        cin.ignore();
        cout << "Customer Name: ";
        getline(cin,customerName);
        cout << "Customer Address: ";
        getline(cin,customerAddress);
        cout << "Customer Phone: ";
        cin >> customerPhone;
        cout << "Customer Email: ";
        cin >> customerEmail;
    }

    string getName()
    {
        return customerName;
    }

    int getID()
    {
        return customerID;
    }
    void getDetails()
    {
        cout << "Customer ID: "<< customerID <<endl;
        cout << "Customer Name: "<< customerName<<endl;
        cout << "Customer Address: "<< customerAddress<<endl;
        cout << "Customer Phone: "<< customerPhone<<endl;
        cout << "Customer Email: "<< customerEmail<<endl;
    }

    // To Show as a List
    void getDetailsFlat()
    {
        cout <<CYAN<< "\n["<<(customerID < 10 ? "00" : (customerID < 100 ? "0" : ""))<<customerID<< "] "<<RESET;
        cout <<BOLD<<"NAME\t: "<<RESET << customerName <<"\n      "<<BOLD<<"ADDRESS\t: "<<RESET<< customerAddress ;
        cout <<"\n      "<<BOLD<<"PHONE\t: "<<RESET<< customerPhone<<"\n      "<<BOLD<<"EMAIL\t: "<<RESET<< customerEmail;
        cout <<endl;
    }
};

// Static Value Declaration (Auto Increment ID)
int Customer::lastID = 0;

// To get customer By ID
Customer* checkCustomer(vector<Customer>& customers, int id)
{
    for (int i =0;i < customers.size();i++)
    {
        if (customers[i].getID() == id)
        {
            customers[i].getDetails();
            return &customers[i];
        }
    }
    return nullptr;
}

// Get the list of Customers
void viewAllCustomers(vector<Customer>& customers)
{
    if(customers.size()==0){
        cout << "\nNo Registered Customers Yet!";
    } else {
        for (int i = 0; i < customers.size(); i++)
        {
            customers[i].getDetailsFlat();
        }
    }
}


class Hotel
{
    string hotelName;
    vector<Room> rooms;
    int totalFloors;
    int eachFloorRoomCount;
public:
    // Constructor
    Hotel(string name,int floors, int roomEach,double price)
    {
        hotelName = name;
        totalFloors = floors;
        eachFloorRoomCount = roomEach;

        char floorChar = 'A';
        int floorExt = 1;
        for (int floor = 1; floor <= totalFloors; ++floor)
        {
            for(int roomNum = 1; roomNum <= eachFloorRoomCount; ++roomNum)
            {
                // Room number format: Character Representing Floor + Floor Extension + 00 or 0 + Room Number
                string roomNumber = floorChar + to_string(floorExt)
                                    + (roomNum < 10 ? "00" : (roomNum < 100 ? "0" : ""))
                                    + to_string(roomNum);

                rooms.push_back(Room(roomNumber,price,floor));
            }
            // Reset To Character Representing Floor To 'A' if it Exceeds 'Z'
            // Increase Floor Extension If Character Representing Floor Exceeds 'Z'
            if (floorChar == 'Z')
            {
                floorChar = 'A';
                floorExt++;
            }
            else
            {
                floorChar++;
            }
        }
    }

    // Check if a room is available
    bool isRoomAvailable(string roomNumber)
    {
        for (int i =0;i < rooms.size();i++)
        {
            if (rooms[i].getRoomNumber() == roomNumber && !rooms[i].getIsBooked())
            {
                return true;
            }
        }
        return false;
    }

    // Book a room for a customer
    bool bookRoom(string roomNumber, Customer& customer)
    {
        for (int i =0;i < rooms.size();i++)
        {
            if (rooms[i].getRoomNumber() == roomNumber && !rooms[i].getIsBooked())
            {
                rooms[i].book(customer.getID());
                cout <<GREEN<< "Room " << roomNumber << " Booked For " << customer.getName()<<RESET << endl;
                return true;
            }
        }
        cout <<RED<< "Room " << roomNumber << " Is Not Available!"<<RESET << endl;
        return false;
    }

    // Vacate a room
    void vacateRoom(string roomNumber)
    {
        for (int i =0;i < rooms.size();i++)
        {
            if (rooms[i].getRoomNumber() == roomNumber)
            {
                if(rooms[i].getIsBooked()){
                    rooms[i].vacate();
                    cout <<GREEN<< "Room " << roomNumber << " Vacated!"<<RESET << endl;
                } else {
                    cout <<GREEN<< "Room " << roomNumber << " Is Available To Book!"<<RESET << endl;
                }

                return;
            }
        }
        cout <<RED<< "Room " << roomNumber << " Doesn\'t Exist!"<<RESET << endl;
    }

    // Get Room Price
    double getRoomPrice(string roomNumber)
    {
        for (int i =0;i < rooms.size();i++)
        {
            if (rooms[i].getRoomNumber() == roomNumber)
            {
                return rooms[i].getPrice();
            }
        }
        cout <<RED<< "Room " << roomNumber << " Doesn\'t Exist!"<<RESET << endl;
        return -1;
    }

    //get room info and return customer id if booked
    int checkRoom(string roomNumber)
    {
        for (int i =0;i < rooms.size();i++)
        {
            if (rooms[i].getRoomNumber() == roomNumber)
            {
                rooms[i].getDetails();
                return rooms[i].getCustomerID();
            }
        }
        cout <<RED<< "Room " << roomNumber << " Doesn\'t Exist!"<<RESET << endl;
        return -1;
    }

    // Set Room Price
    bool editRoom(string roomNumber)
    {
        for (int i =0;i < rooms.size();i++)
        {
            if (rooms[i].getRoomNumber() == roomNumber)
            {
                double price;
                cout << "New Price ($):";
                cin >> price;
                rooms[i].setPrice(price);
                cout <<GREEN<< "Room " << roomNumber << " Price Updated To " << price<<RESET << endl;
                return true;
            }
        }
        cout <<RED<< "Room " << roomNumber << " Doesn\'t Exist!"<<RESET << endl;
        return false;
    }


    // Get The List of All Rooms
    void viewAllRooms()
    {
        char floorChar = 'A';
        int floorExt = 1;
        for (int floor = 1; floor <= totalFloors; ++floor)
        {
            cout << "FLOOR "<<(floor < 10 ? "00" : (floor < 100 ? "0" : "")) <<floor;
            cout << ":\n----------\n";
            for(int roomNum = 1; roomNum <= eachFloorRoomCount; ++roomNum)
            {
                // Room number format: Character Representing Floor + Floor Extension + 00 or 0 + Room Number
                string roomNumber = floorChar + to_string(floorExt)
                                    + (roomNum < 10 ? "00" : (roomNum < 100 ? "0" : ""))+ to_string(roomNum);
                if (isRoomAvailable(roomNumber))
                {
                    cout <<"["<<GREEN<< roomNumber << RESET<< "] ";
                }
                else
                {
                    cout <<"["<< RED << roomNumber << RESET<< "] ";
                }
            }
            cout <<endl<< endl;
            // Reset To Character Representing Floor To 'A' if it Exceeds 'Z'
            // Increase Floor Extension If Character Representing Floor Exceeds 'Z'
            if (floorChar == 'Z')
            {
                floorChar = 'A';
                floorExt++;
            }
            else
            {
                floorChar++;
            }
        }
        cout << endl;
    }
};


// Admin Class
class Admin
{
    string username;
    string password;
    bool superAdmin;
public:
    // Constructor
    Admin(bool super = false)
    {
        cout << "Username: ";
        cin >> username;
        cout << "Password: ";
        cin >> password;
        superAdmin = super;
    }
    string getUsername()
    {
        return username;
    }
    string getPassword()
    {
        return password;
    }
    bool isSuper(){
        return superAdmin;
    }
};

// Check If Admin Credentials Match
bool checkAdmin(vector<Admin>& admins, string username, string password)
{
    for (int i =0;i < admins.size();i++)
    {
        if (admins[i].getUsername() == username && admins[i].getPassword() == password)
        {
            if(admins[i].isSuper()){
                ::superAdmin = true;
            } else {
                ::superAdmin = false;
            }
            return true;
        }
    }
    return false;
}


// Enter to Continue
void continueProcess()
{
    cin.ignore();
    cout << "\n\nPress Enter To Continue...";
    cin.get();
    system("cls");
}

// Login Screen
bool loginScreen(vector<Admin>& admins)
{
    string username,password;
    cout << "\n\nLogin To Panel\n=================\n";
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;
    if(checkAdmin(admins,username,password))
    {
        cin.get();
        system("cls");
        return true;
    }
    else
    {
        return false;
    }
}


// Menu Screen
void menuScreen(vector<Admin>& admins,vector<Customer>& customers,Hotel& hotel)
{
    // Temporary Variables
    int option;
    string id;
    int custType;
    int cust;
    Customer* tempCust;


    // Admin Panel UI
    cout << "Admin Panel\n=================\n";
    cout <<"Select An Option (1-6)\n\n";
    cout <<MAGENTA<< "[1] "<<RESET<< "All Rooms"<<endl;
    cout <<MAGENTA<< "[2] "<<RESET<< "All Customers"<<endl;
    cout <<MAGENTA<< "[3] "<<RESET<< "Room Details"<<endl;
    cout <<MAGENTA<< "[4] "<<RESET<< "Edit Room Price"<<endl;
    cout <<MAGENTA<< "[5] "<<RESET<< "Book A Room"<<endl;
    cout <<MAGENTA<< "[6] "<<RESET<< "Vacate A Room"<<endl;
    if(::superAdmin){
        cout <<MAGENTA<< "[7] "<<RESET<< "Add New Admin"<<endl;
        cout <<MAGENTA<< "[8] "<<RESET<< "Logout"<<endl;
    } else {
        cout <<MAGENTA<< "[7] "<<RESET<< "Logout"<<endl;
    }
    cout << "\n> ";
    cin >> option;
    system("cls");


    // After Menu Selection
    switch(option)
    {
    case 1:
        // Check All Rooms
        cout << "All Rooms\n=================\nList of all rooms in the hotel\n\n";
        hotel.viewAllRooms();
        continueProcess();
        break;
    case 2:
        // Check All Registered Customers
        cout << "All Customers\n=================\nList of all registered customers in the hotel\n\n";
        viewAllCustomers(customers);
        continueProcess();
        break;
    case 3:
        // Check Room Details
        cout << "Room Details\n=================\nEnter a Room Number below to get the room details\n\n";
        cout << "Room Number: ";
        cin >> id;
        cout << endl<< endl;
        cust = hotel.checkRoom(id);
        checkCustomer(customers,cust);
        continueProcess();
        break;

    case 4:
        // Edit Room Price
        cout << "Edit Room Price\n=================\nEnter a Room Number below to edit it\'s details\n\n";
        cout << "Room No: ";
        cin >> id;
        hotel.editRoom(id);
        continueProcess();
        break;

    case 5:
        // Book A Room
        cout << "Book A Room\n=================\nEnter a Room Number below to book a room\n\n";
        cout << "Room No:";
        cin >> id;
        if(!hotel.isRoomAvailable(id))
        {
            cout <<RED<< "Room " << id << " Is Not Available!"<<RESET << endl;
            continueProcess();
            break;
        }
        cout << "Room Price ($): " << hotel.getRoomPrice(id) <<endl;

        // Register A Customer or Use Existing
        cout << "\nCustomer Type:\n"<<MAGENTA<< "[1]"<<RESET<<" Existing\n"<<MAGENTA<<"[2]"<<RESET<<" New Customer\n\n> ";
        cin >> custType;
        if(custType == 1)
        {
            cout << "Customer ID:";
            cin >> cust;
            tempCust = checkCustomer(customers,cust);
            if(tempCust != nullptr)
            {
                hotel.bookRoom(id, *tempCust);
            }
            else
            {
                cout <<RED<< "\nInvalid Customer ID!"<<RESET;
            }
        }
        else if(custType == 2)
        {
            customers.push_back(Customer());
            hotel.bookRoom(id, customers.back());
        }
        else
        {
            cout <<RED<< "\nInvalid Choice!"<<RESET;
        }
        continueProcess();
        break;

    case 6:
        // Make A Room Available
        cout << "Vacant A Room\n=================\nEnter a Room Number below to make a room available\n\n";
        cout << "Room No:";
        cin >> id;
        hotel.vacateRoom(id);
        continueProcess();
        break;
    case 7:
        if(::superAdmin){
            cout << "Add An Admin\n=================\nEnter a login credentials of the new admin\n\n";
            admins.push_back(Admin());
            continueProcess();
        } else {
            ::loggedIn = false;
            system("cls");
            cout <<GREEN<< "Logged Out Successfully!...\n\n"<<RESET;
        }
        break;
    case 8:
        if(::superAdmin){
            ::loggedIn = false;
            system("cls");
            cout <<GREEN<< "Logged Out Successfully!...\n\n"<<RESET;
        } else {
            cout <<RED<< "Invalid Option! Try again...\n\n"<<RESET;
            continueProcess();
        }
        break;
    default:
        cout <<RED<< "Invalid Option! Try again...\n\n"<<RESET;
        continueProcess();
        break;
    }
}


int main()
{
    string tempStr1,tempStr2;
    int tempInt1,tempInt2;
    double tempDouble1;
    bool loggedIn = false;


    // Intro Screen
    cout << "======================================"<<endl;
    cout << "|| ZealTyro Hotel Management System ||"<<endl;
    cout << "======================================"<<endl<<endl;
    cout << "This Management System is developed as a project of the course - CSC 284\n";
    cout << "Given by our honerable faculty - ["<<YELLOW<<"ASM Shakil Ahamed"<<RESET<<"]\n\n";
    cout << BOLD << "DESCRIPTION:\n-----------\n" << RESET;
    cout << "A complete solution to manage your hotel with multiple admins and a super admin. Where only the super admin"
         " can add new admins. All admins can manage the rooms, book a room for the customers, check all rooms, "
         "edit price of the rooms, add new customers, and vacate a room when customers leave! \n\n";
    cout << BOLD << "DEVELOPERS:\n-----------\n" << RESET;
    cout << "Students of International University of Business Agriculture and Technology";
    cout << "\n[NAME:"<<BLUE<<" Afsana Meem"<<RESET<<", ID:"<<BLUE<<" 23203"<<RESET<<"]\t";
    cout << "[NAME:"<<BLUE<<" Shanjida Afrin"<<RESET<<", ID:"<<BLUE<<" 23103290"<<RESET<<"]\t";
    cout << "\n[NAME:"<<BLUE<<" Sumayia Akter"<<RESET<<", ID:"<<BLUE<<" 22303423"<<RESET<<"]\t";
    cout << "[NAME:"<<BLUE<<" Sumiaya Afrin"<<RESET<<", ID:"<<BLUE<<" 23203130"<<RESET<<"]\t";
    cout << "\n[NAME:"<<BLUE<<" Md. Mahedi Zaman Zaber"<<RESET<<", ID:"<<BLUE<<" 23203134"<<RESET<<"]\n"<<endl;
    cout << "\nYou are just one step away from having your hotel management system!";
    cout << "\nPress Enter To Continue...";
    cin.get();
    system("cls");


    // First Time Hotel Setup
    cout << "First Time Setup\n=================\n";
    cout << "Before we can continue, let\'s setup your hotel first. "
         "Please fill the form below:\n\n";
    cout << "Hotel Name:";
    getline(cin, tempStr1);
    cout << "Total Floors:";
    cin >> tempInt1;
    cout << "Rooms In Each Floors:";
    cin >> tempInt2;
    cout << "Default Room Cost ($):";
    cin >> tempDouble1;
    Hotel hotel(tempStr1,tempInt1,tempInt2,tempDouble1);
    vector<Customer> customers;
    cout <<GREEN<< "Hotel Information Saved!"<<RESET;


    // Super Admin Setup
    cout << "\n\nSuper Admin\n=================\n";
    cout << "Let\'s setup a super admin of your hotel. Only super admins have the power to"
         " add more admins and have all admin powers\n\n";
    vector<Admin> admins;
    admins.push_back(Admin(true));
    cout << "\nSuper Admin created! Now continue to the panel using your username "
         "and password to start managing your hotel";
    continueProcess();

    // Admin Panel
    while(true)
    {
        if(::loggedIn)
        {
            //Main Menu
            menuScreen(admins,customers,hotel);


        }
        else
        {
            // Login Screen
            if(loginScreen(admins))
            {
                ::loggedIn = true;
            }
            else
            {
                system("cls");
                cout <<RED<< "Invalid Iogin Credentials!"<<RESET;

            }
        }
    }
    return 0;
}
